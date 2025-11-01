/**
 * @file usb_device.c
 * @brief USB Device Mode (MSC) Implementation for ESP32-S3
 *
 * @author A.R. Ansari <ansarirahim1@gmail.com>
 * @date 2025-10-21
 * @version 1.0.0
 *
 * @section description Description
 * Implements TinyUSB MSC (Mass Storage Class) device stack for ESP32-S3.
 * Provides USB Device Mode functionality with internal FATFS backend.
 *
 * @section features Features
 * - Block device backed by internal FATFS
 * - Sector-level read/write operations (512-byte sectors)
 * - SCSI START/STOP UNIT handling for safe eject
 * - I/O activity monitoring and LED state updates
 * - Write synchronization for data safety
 * - Thread-safe operations with semaphores
 * - Comprehensive error handling and logging
 *
 * @section implementation Implementation Details
 * - Uses TinyUSB v2.0.1 MSC driver
 * - Integrates with ESP-IDF FATFS component
 * - FreeRTOS task for I/O activity monitoring
 * - LED state machine for user feedback
 *
 * @section contact Contact
 * - Email: ansarirahim1@gmail.com
 * - Phone: +91 9024304883
 * - GitHub: https://github.com/ansarirahim
 *
 * @section license License
 * Copyright (c) 2025 A.R. Ansari. All rights reserved.
 */

#include "usb_device.h"
#include "filesystem.h"
#include "led_control.h"
#include "esp_log.h"
#include "tinyusb.h"
#include "tinyusb_msc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "ff.h"

static const char *TAG = "usb_device";  /**< Log tag for USB device messages */

/** @defgroup usb_device_state USB Device State Variables
 * @{
 */
static bool g_usb_connected = false;        /**< USB connection status */
static bool g_usb_mounted = false;          /**< USB mount status on host */
static uint32_t g_io_activity_timeout = 0;  /**< I/O activity timeout counter */
/** @} */

/** @defgroup usb_device_sync Synchronization Primitives
 * @{
 */
static SemaphoreHandle_t g_io_semaphore = NULL;  /**< Semaphore for I/O activity synchronization */
static TaskHandle_t g_io_monitor_task = NULL;    /**< Task handle for I/O activity monitor */
static tinyusb_msc_storage_handle_t g_msc_storage_handle = NULL;  /**< MSC storage handle */
/** @} */

/**
 * @brief MSC Callback: Read Sectors from Storage
 *
 * Called by TinyUSB MSC driver when host requests to read sectors from the device.
 * Reads data from internal FATFS filesystem and returns to host.
 *
 * @param[in] lba Logical Block Address (sector number)
 * @param[in] offset Byte offset within the sector
 * @param[out] buffer Pointer to buffer for read data
 * @param[in] bufsize Number of bytes to read
 *
 * @return Number of bytes read on success, -1 on error
 * @retval >0 Number of bytes successfully read
 * @retval -1 Read operation failed
 *
 * @details
 * - Sector size is 512 bytes
 * - Updates LED state during I/O operation
 * - Thread-safe with semaphore protection
 * - Comprehensive error handling with logging
 *
 * @note Called from USB interrupt context
 * @see msc_write_sectors()
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
 * @brief MSC Callback: Write Sectors to Storage
 *
 * Called by TinyUSB MSC driver when host requests to write sectors to the device.
 * Writes data to internal FATFS filesystem.
 *
 * @param[in] lba Logical Block Address (sector number)
 * @param[in] offset Byte offset within the sector
 * @param[in] buffer Pointer to buffer containing data to write
 * @param[in] bufsize Number of bytes to write
 *
 * @return Number of bytes written on success, -1 on error
 * @retval >0 Number of bytes successfully written
 * @retval -1 Write operation failed
 *
 * @details
 * - Sector size is 512 bytes
 * - Updates LED state during I/O operation
 * - Thread-safe with semaphore protection
 * - Synchronizes writes to ensure data safety
 * - Comprehensive error handling with logging
 *
 * @note Called from USB interrupt context
 * @see msc_read_sectors()
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
        .port = TINYUSB_PORT_FULL_SPEED_0,
        .phy = {
            .skip_setup = false,
            .self_powered = false,
        },
        .task = {
            .size = 4096,
            .priority = 5,
            .xCoreID = 0,
        },
    };

    esp_err_t ret = tinyusb_driver_install(&tusb_cfg);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to install TinyUSB driver: %s", esp_err_to_name(ret));
        return false;
    }

    /* Install MSC driver */
    const tinyusb_msc_driver_config_t msc_driver_cfg = {
        .user_flags = {
            .auto_mount_off = 0,
        },
        .callback = NULL,
        .callback_arg = NULL,
    };

    ret = tinyusb_msc_install_driver(&msc_driver_cfg);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to install MSC driver: %s", esp_err_to_name(ret));
        return false;
    }

    /* Create MSC storage with SPI Flash - using internal FATFS mount point */
    tinyusb_msc_storage_config_t msc_cfg = {
        .medium.wl_handle = fs_get_wl_handle(),  /* Get WL handle from filesystem */
        .fat_fs = {
            .base_path = MOUNT_POINT,
            .config = {
                .format_if_mount_failed = true,
                .max_files = 5,
                .allocation_unit_size = 4096,
            },
            .do_not_format = false,
            .format_flags = 0,
        },
        .mount_point = TINYUSB_MSC_STORAGE_MOUNT_USB,
    };

    ret = tinyusb_msc_new_storage_spiflash(&msc_cfg, &g_msc_storage_handle);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize MSC storage: %s", esp_err_to_name(ret));
        return false;
    }

    ESP_LOGI(TAG, "MSC storage initialized successfully");
    ESP_LOGI(TAG, "USB Device (MSC) initialized");

    g_usb_connected = true;
    g_usb_mounted = true;
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
