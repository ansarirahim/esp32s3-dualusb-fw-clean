/**
 * @file test_main.c
 * @brief Main Test Runner for ESP32-S3 Dual USB Firmware
 *
 * @author A.R. Ansari <ansarirahim1@gmail.com>
 * @date 2025-10-21
 * @version 1.0.0
 *
 * @section description Description
 * Main test runner that initializes the Unity testing framework
 * and runs all unit tests for the ESP32-S3 Dual USB Firmware.
 *
 * @section test_suites Test Suites
 * - LED Control Tests (test_led_control.c)
 * - Filesystem Tests (test_filesystem.c)
 * - USB Device Tests (test_usb_device.c)
 *
 * @section contact Contact
 * - Email: ansarirahim1@gmail.com
 * - GitHub: https://github.com/ansarirahim
 *
 * @section license License
 * Copyright (c) 2025 A.R. Ansari. All rights reserved.
 */

#include <stdio.h>
#include <string.h>
#include "unity.h"
#include "esp_log.h"

static const char *TAG = "test";

/**
 * @brief Print test banner
 *
 * Displays a welcome banner for the test suite.
 */
static void print_banner(void) {
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║   ESP32-S3 Dual USB Firmware - Unit Test Suite             ║\n");
    printf("║   Author: A.R. Ansari <ansarirahim1@gmail.com>            ║\n");
    printf("║   Version: 1.0.0                                           ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    printf("\n");
}

/**
 * @brief Print test summary
 *
 * Displays a summary of test results.
 */
static void print_summary(void) {
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║   Test Suite Complete                                      ║\n");
    printf("║   All unit tests have been executed                        ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    printf("\n");
}

/**
 * @brief Main test entry point
 *
 * Initializes the Unity testing framework and runs all tests.
 * This function is called by the ESP-IDF framework.
 */
void app_main(void) {
    print_banner();

    ESP_LOGI(TAG, "Starting unit tests...");

    /* Initialize Unity */
    UNITY_BEGIN();

    /* Run all tests */
    unity_run_menu();

    /* Print summary */
    print_summary();

    ESP_LOGI(TAG, "Unit tests completed");
}

/**
 * @brief Setup function called before each test
 *
 * Initializes test environment.
 */
void setUp(void) {
    /* Common setup for all tests */
}

/**
 * @brief Teardown function called after each test
 *
 * Cleans up after each test.
 */
void tearDown(void) {
    /* Common teardown for all tests */
}

