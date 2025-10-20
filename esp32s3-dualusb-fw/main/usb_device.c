/*
 * @file usb_device.c
 * @brief USB Device Mode (MSC) implementation for ESP32-S3
 *
 * Author: A.R. Ansari <ansarirahim1@gmail.com>
 * Date: 2025-10-20
 *
 * Implements TinyUSB MSC device stack with:
 * - Block device backed by internal FATFS
 * - Sector-level read/write operations
 * - SCSI START/STOP UNIT handling for safe eject
 * - I/O activity monitoring and LED state updates
 * - Write synchronization for data safety
 */

#include "usb_device.h"
#include "filesystem.h"
#include "led_control.h"
#include "esp_log.h"
#include "esp_tinyusb.h"
#include "tusb_msc_storage.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "ff.h"

static const char *TAG = "usb_device";

/* USB device state */
static bool g_usb_connected = false;
static bool g_usb_mounted = false;
static uint32_t g_io_activity_timeout = 0;

/* I/O activity tracking */
static SemaphoreHandle_t g_io_semaphore = NULL;
static TaskHandle_t g_io_monitor_task = NULL;

/**
 * @brief MSC callback: read sectors
 */
static int32_t msc_read_sectors(uint32_t lba, uint32_t offset, uint8_t *buffer, uint32_t bufsize) {
    usb_device_notify_io_start();

    /* Read from FATFS */
    FIL file;
    FRESULT res = f_open(&file, MOUNT_POINT, FA_READ);
    if (res != FR_OK) {
        ESP_LOGE(TAG, "Failed to open storage for read: %d", res);
        led_set_state(LED_STATE_ERROR);
        usb_device_notify_io_end();
        return -1;
    }

    /* Seek to LBA offset */
    uint32_t seek_offset = lba * 512 + offset;
    res = f_lseek(&file, seek_offset);
    if (res != FR_OK) {
        ESP_LOGE(TAG, "Failed to seek: %d", res);
        f_close(&file);
        led_set_state(LED_STATE_ERROR);
        usb_device_notify_io_end();
        return -1;
    }

    /* Read data */
    UINT bytes_read = 0;
    res = f_read(&file, buffer, bufsize, &bytes_read);
    f_close(&file);

    if (res != FR_OK) {
        ESP_LOGE(TAG, "Failed to read: %d", res);
        led_set_state(LED_STATE_ERROR);
        usb_device_notify_io_end();
        return -1;
    }

    usb_device_notify_io_end();
    return bytes_read;
}

/**
 * @brief MSC callback: write sectors
 */
static int32_t msc_write_sectors(uint32_t lba, uint32_t offset, uint8_t *buffer, uint32_t bufsize) {
    usb_device_notify_io_start();

    /* Write to FATFS */
    FIL file;
    FRESULT res = f_open(&file, MOUNT_POINT, FA_WRITE);
    if (res != FR_OK) {
        ESP_LOGE(TAG, "Failed to open storage for write: %d", res);
        led_set_state(LED_STATE_ERROR);
        usb_device_notify_io_end();
        return -1;
    }

    /* Seek to LBA offset */
    uint32_t seek_offset = lba * 512 + offset;
    res = f_lseek(&file, seek_offset);
    if (res != FR_OK) {
        ESP_LOGE(TAG, "Failed to seek: %d", res);
        f_close(&file);
        led_set_state(LED_STATE_ERROR);
        usb_device_notify_io_end();
        return -1;
    }

    /* Write data */
    UINT bytes_written = 0;
    res = f_write(&file, buffer, bufsize, &bytes_written);

    /* Force sync after write */
    if (res == FR_OK) {
        res = f_sync(&file);
    }

    f_close(&file);

    if (res != FR_OK) {
        ESP_LOGE(TAG, "Failed to write: %d", res);
        led_set_state(LED_STATE_ERROR);
        usb_device_notify_io_end();
        return -1;
    }

    usb_device_notify_io_end();
    return bytes_written;
}

/**
 * @brief MSC callback: get capacity
 */
static bool msc_capacity_cb(uint32_t *block_count, uint16_t *block_size) {
    uint64_t total_bytes, free_bytes;
    if (!fs_get_stats(&total_bytes, &free_bytes)) {
        ESP_LOGE(TAG, "Failed to get FS stats");
        return false;
    }

    *block_count = total_bytes / 512;
    *block_size = 512;

    ESP_LOGI(TAG, "MSC capacity: %u blocks of %u bytes", *block_count, *block_size);
    return true;
}

/**
 * @brief MSC callback: start/stop unit
 */
static bool msc_start_stop_cb(uint8_t power_condition, bool start, bool load_eject) {
    ESP_LOGI(TAG, "MSC start_stop: power=%d, start=%d, eject=%d", power_condition, start, load_eject);

    if (load_eject) {
        if (start) {
            /* Load media */
            g_usb_mounted = true;
            led_set_state(LED_STATE_IDLE);
        } else {
            /* Eject media */
            g_usb_mounted = false;
            led_set_state(LED_STATE_IDLE);
        }
    }

    return true;
}

/**
 * @brief I/O activity monitor task
 */
static void io_monitor_task(void *arg) {
    while (1) {
        if (xSemaphoreTake(g_io_semaphore, pdMS_TO_TICKS(100)) == pdTRUE) {
            /* I/O activity detected, set busy state */
            led_set_state(LED_STATE_BUSY);
            g_io_activity_timeout = 500; /* 500ms timeout */
        }

        if (g_io_activity_timeout > 0) {
            g_io_activity_timeout -= 100;
            if (g_io_activity_timeout == 0) {
                /* Return to idle */
                led_set_state(LED_STATE_IDLE);
            }
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

bool usb_device_init(void) {
    ESP_LOGI(TAG, "Initializing USB Device (MSC)");

    /* Create I/O semaphore */
    g_io_semaphore = xSemaphoreCreateBinary();
    if (!g_io_semaphore) {
        ESP_LOGE(TAG, "Failed to create I/O semaphore");
        return false;
    }

    /* Create I/O monitor task */
    xTaskCreate(io_monitor_task, "io_monitor", 2048, NULL, 4, &g_io_monitor_task);

    /* Initialize TinyUSB */
    const tinyusb_config_t tusb_cfg = {
        .device_descriptor = NULL,
        .string_descriptor = NULL,
        .external_phy = false,
        .configuration_descriptor = NULL,
    };

    esp_err_t ret = tinyusb_driver_install(&tusb_cfg);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to install TinyUSB driver: %s", esp_err_to_name(ret));
        return false;
    }

    /* Register MSC callbacks */
    tusb_msc_storage_init_spiflash(
        MOUNT_POINT,
        msc_capacity_cb,
        msc_start_stop_cb,
        msc_read_sectors,
        msc_write_sectors
    );

    g_usb_connected = true;
    ESP_LOGI(TAG, "USB Device (MSC) initialized");
    return true;
}

bool usb_device_is_connected(void) {
    return g_usb_connected;
}

bool usb_device_is_mounted(void) {
    return g_usb_mounted;
}

void usb_device_notify_io_start(void) {
    if (g_io_semaphore) {
        xSemaphoreGive(g_io_semaphore);
    }
}

void usb_device_notify_io_end(void) {
    /* Handled by monitor task */
}
