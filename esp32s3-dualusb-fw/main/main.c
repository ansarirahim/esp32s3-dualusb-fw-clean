/*
 * @file main.c
 * @brief ESP32-S3 Dual USB Firmware - Main Application
 *
 * Author: A.R. Ansari <ansarirahim1@gmail.com>
 * Date: 2025-10-20
 *
 * Milestone 1: USB Device Mode (MSC) with internal FATFS
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "led_control.h"
#include "filesystem.h"
#include "usb_device.h"

static const char *TAG = "app";

void app_main(void) {
    ESP_LOGI(TAG, "ESP32-S3 Dual USB FW boot");

    /* Initialize LED */
    led_init();
    led_set_state(LED_STATE_IDLE);

    /* Initialize internal FATFS */
    if (!fs_init_internal()) {
        ESP_LOGE(TAG, "Failed to initialize filesystem");
        led_set_state(LED_STATE_ERROR);
        return;
    }

    /* Initialize USB Device Mode (MSC) */
    if (!usb_device_init()) {
        ESP_LOGE(TAG, "Failed to initialize USB device");
        led_set_state(LED_STATE_ERROR);
        return;
    }

    ESP_LOGI(TAG, "ESP32-S3 Dual USB FW ready - Device Mode (MSC)");

    /* Keep app running */
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
