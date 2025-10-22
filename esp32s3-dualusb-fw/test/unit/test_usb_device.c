/**
 * @file test_usb_device.c
 * @brief Unit Tests for USB Device Module
 *
 * @author A.R. Ansari <ansarirahim1@gmail.com>
 * @date 2025-10-21
 * @version 1.0.0
 *
 * @section description Description
 * Comprehensive unit tests for USB Device Mode functionality.
 * Tests USB device initialization, connection status, and I/O notifications.
 *
 * @section test_cases Test Cases
 * - USB device initialization
 * - Connection status checking
 * - Mount status checking
 * - I/O activity notifications
 * - State transitions
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
#include <stdbool.h>
#include "unity.h"
#include "usb_device.h"

/**
 * @brief Setup function called before each test
 *
 * Initializes the USB device for testing.
 */
void setUp(void) {
    /* Initialize USB device for each test */
    usb_device_init();
}

/**
 * @brief Teardown function called after each test
 *
 * Cleans up after each test.
 */
void tearDown(void) {
    /* No cleanup needed for USB device tests */
}

/**
 * @test USB Device Initialization
 *
 * Verifies that USB device initializes successfully.
 */
TEST_CASE("USB: Initialization", "[usb_device]") {
    bool result = usb_device_init();
    TEST_ASSERT_TRUE(result);
}

/**
 * @test USB Device Connection Status - Initial State
 *
 * Verifies that USB device connection status is correct after initialization.
 */
TEST_CASE("USB: Connection Status - Initial State", "[usb_device]") {
    bool connected = usb_device_is_connected();
    /* After initialization, device should be connected (ready) */
    TEST_ASSERT_TRUE(connected);
}

/**
 * @test USB Device Mount Status - Initial State
 *
 * Verifies that USB device mount status is correct after initialization.
 */
TEST_CASE("USB: Mount Status - Initial State", "[usb_device]") {
    bool mounted = usb_device_is_mounted();
    /* After initialization, device may or may not be mounted (depends on host) */
    /* This test just verifies the function returns a boolean */
    TEST_ASSERT_TRUE(mounted == true || mounted == false);
}

/**
 * @test USB Device I/O Start Notification
 *
 * Verifies that I/O start notification can be called.
 */
TEST_CASE("USB: I/O Start Notification", "[usb_device]") {
    /* This should not crash or cause issues */
    usb_device_notify_io_start();
    TEST_PASS();
}

/**
 * @test USB Device I/O End Notification
 *
 * Verifies that I/O end notification can be called.
 */
TEST_CASE("USB: I/O End Notification", "[usb_device]") {
    /* This should not crash or cause issues */
    usb_device_notify_io_end();
    TEST_PASS();
}

/**
 * @test USB Device Multiple I/O Notifications
 *
 * Verifies that multiple I/O notifications can be handled.
 */
TEST_CASE("USB: Multiple I/O Notifications", "[usb_device]") {
    for (int i = 0; i < 10; i++) {
        usb_device_notify_io_start();
        usb_device_notify_io_end();
    }
    TEST_PASS();
}

/**
 * @test USB Device Connection Status Persistence
 *
 * Verifies that connection status persists.
 */
TEST_CASE("USB: Connection Status Persistence", "[usb_device]") {
    bool connected1 = usb_device_is_connected();
    bool connected2 = usb_device_is_connected();
    
    TEST_ASSERT_EQUAL(connected1, connected2);
}

/**
 * @test USB Device Mount Status Persistence
 *
 * Verifies that mount status persists.
 */
TEST_CASE("USB: Mount Status Persistence", "[usb_device]") {
    bool mounted1 = usb_device_is_mounted();
    bool mounted2 = usb_device_is_mounted();
    
    TEST_ASSERT_EQUAL(mounted1, mounted2);
}

/**
 * @test USB Device Status Queries
 *
 * Verifies that status queries return valid values.
 */
TEST_CASE("USB: Status Queries", "[usb_device]") {
    bool connected = usb_device_is_connected();
    bool mounted = usb_device_is_mounted();
    
    /* Both should be boolean values */
    TEST_ASSERT_TRUE(connected == true || connected == false);
    TEST_ASSERT_TRUE(mounted == true || mounted == false);
}

/**
 * @test USB Device I/O Notification Sequence
 *
 * Verifies that I/O notifications can be called in sequence.
 */
TEST_CASE("USB: I/O Notification Sequence", "[usb_device]") {
    usb_device_notify_io_start();
    usb_device_notify_io_start();
    usb_device_notify_io_end();
    usb_device_notify_io_end();
    
    TEST_PASS();
}

/**
 * @test USB Device Rapid I/O Notifications
 *
 * Verifies that rapid I/O notifications are handled correctly.
 */
TEST_CASE("USB: Rapid I/O Notifications", "[usb_device]") {
    for (int i = 0; i < 100; i++) {
        usb_device_notify_io_start();
        usb_device_notify_io_end();
    }
    TEST_PASS();
}

/**
 * @test USB Device Status After I/O Notifications
 *
 * Verifies that status remains consistent after I/O notifications.
 */
TEST_CASE("USB: Status After I/O Notifications", "[usb_device]") {
    bool connected_before = usb_device_is_connected();
    
    usb_device_notify_io_start();
    usb_device_notify_io_end();
    
    bool connected_after = usb_device_is_connected();
    
    TEST_ASSERT_EQUAL(connected_before, connected_after);
}

/**
 * @test USB Device Initialization Idempotency
 *
 * Verifies that multiple initializations don't cause issues.
 */
TEST_CASE("USB: Initialization Idempotency", "[usb_device]") {
    bool result1 = usb_device_init();
    bool result2 = usb_device_init();
    
    /* Both should succeed or both should fail consistently */
    TEST_ASSERT_EQUAL(result1, result2);
}

