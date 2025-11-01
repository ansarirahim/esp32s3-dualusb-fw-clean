/**
 * @file main.c
 * @brief ESP32-S3 Dual USB Firmware - Main Application Entry Point
 *
 * @author A.R. Ansari <ansarirahim1@gmail.com>
 * @date 2025-10-21
 * @version 1.0.0
 *
 * @section description Description
 * Main application entry point for ESP32-S3 Dual USB Firmware.
 * Initializes all subsystems including:
 * - LED status indicators
 * - Internal FATFS filesystem
 * - USB Device Mode (MSC - Mass Storage Class)
 * - USB Host Mode (MSC - Mass Storage Class)
 *
 * @section features Features
 * - USB Device Mode (MSC) with internal FATFS
 * - USB Host Mode (MSC) for external USB drives
 * - LED status indication (IDLE, ACTIVE, ERROR)
 * - FreeRTOS task management
 * - Error handling and recovery
 *
 * @section milestone Milestone
 * Milestone 1: USB Device Mode (MSC) with internal FATFS
 * Milestone 2: USB Host Mode (MSC) for external USB drives
 *
 * @section contact Contact
 * - Email: ansarirahim1@gmail.com
 * - Phone: +91 9024304883
 * - GitHub: https://github.com/ansarirahim
 * - LinkedIn: https://www.linkedin.com/in/abdul-raheem-ansari-a6871320/
 *
 * @section license License
 * Copyright (c) 2025 A.R. Ansari. All rights reserved.
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "led_control.h"
#include "led_autoprobe.h"
#include "filesystem.h"
#include "usb_device.h"
#include "usb_host.h"
#include "usb_mode.h"

/* External USB descriptors initialization function */
extern void usb_descriptors_init(void);

static const char *TAG = "app";  /**< Log tag for application messages */

/**
 * @brief Main application entry point
 *
 * Initializes all subsystems in the following order:
 * 1. LED control system for status indication
 * 2. Internal FATFS filesystem
 * 3. USB Device Mode (MSC)
 *
 * If any initialization fails, the LED is set to ERROR state and the function returns.
 * Otherwise, the application enters an infinite loop with periodic task yields.
 *
 * @note This function is called by the ESP-IDF framework after system initialization.
 * @note The function never returns under normal operation.
 *
 * @return void (never returns)
 *
 * @see led_init()
 * @see fs_init_internal()
 * @see usb_device_init()
 */
void app_main(void) {
    ESP_LOGI(TAG, "ESP32-S3 Dual USB FW boot - vPT-A4.2");

    /* Initialize USB descriptors (custom VID/PID and strings) */
    usb_descriptors_init();

    /* Initialize LED autoprobe (disabled - using fixed GPIO 6) */
    led_autoprobe_init();
    ESP_LOGI(TAG, "LED GPIO fixed to 6");
    vTaskDelay(pdMS_TO_TICKS(500));  /* Brief delay for initialization */

    /* Initialize LED control */
    led_init();
    led_set_state(LED_STATE_IDLE);

    /* Initialize internal FATFS (non-blocking - continue even if fails) */
    if (!fs_init_internal()) {
        ESP_LOGW(TAG, "Failed to initialize filesystem - continuing without storage");
        /* Don't return - continue with USB initialization */
    }

    /* Initialize USB Device Mode (MSC) */
    if (!usb_device_init()) {
        ESP_LOGE(TAG, "Failed to initialize USB device");
        led_set_state(LED_STATE_ERROR);
        return;
    }

    /* Initialize USB Host Mode (MSC) */
    if (!usb_host_init()) {
        ESP_LOGE(TAG, "Failed to initialize USB host");
        led_set_state(LED_STATE_ERROR);
        return;
    }

    /* Initialize USB Mode Control */
    if (!usb_mode_init()) {
        ESP_LOGE(TAG, "Failed to initialize USB mode control");
        led_set_state(LED_STATE_ERROR);
        return;
    }

    /* Set to dual mode with automatic switching */
    usb_mode_set(USB_MODE_DUAL_AUTO);

    ESP_LOGI(TAG, "ESP32-S3 Dual USB FW ready - Dual Mode (Device + Host)");

    /* Keep app running */
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
