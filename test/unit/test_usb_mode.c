/**
 * @file test_usb_mode.c
 * @brief Unit Tests for USB Mode Control
 *
 * @author A.R. Ansari <ansarirahim1@gmail.com>
 * @date 2025-10-22
 * @version 1.0.0
 *
 * Tests for USB Mode control functionality including:
 * - Initialization and deinitialization
 * - Mode setting and getting
 * - Mode state management
 * - Connection notifications
 * - Status retrieval
 */

#include "unity.h"
#include "usb_mode.h"
#include "esp_log.h"
#include <string.h>

static const char *TAG = "test_usb_mode";

/**
 * @brief Setup function - called before each test
 */
void setUp(void) {
    ESP_LOGI(TAG, "Setting up USB Mode test");
}

/**
 * @brief Teardown function - called after each test
 */
void tearDown(void) {
    ESP_LOGI(TAG, "Tearing down USB Mode test");
    usb_mode_deinit();
}

/**
 * @test USB Mode initialization
 */
void test_usb_mode_init(void) {
    ESP_LOGI(TAG, "Test: USB Mode initialization");
    
    bool result = usb_mode_init();
    TEST_ASSERT_TRUE(result);
}

/**
 * @test USB Mode double initialization
 */
void test_usb_mode_double_init(void) {
    ESP_LOGI(TAG, "Test: USB Mode double initialization");
    
    bool result1 = usb_mode_init();
    TEST_ASSERT_TRUE(result1);
    
    bool result2 = usb_mode_init();
    TEST_ASSERT_TRUE(result2);
}

/**
 * @test USB Mode deinitialization
 */
void test_usb_mode_deinit(void) {
    ESP_LOGI(TAG, "Test: USB Mode deinitialization");
    
    bool init_result = usb_mode_init();
    TEST_ASSERT_TRUE(init_result);
    
    bool deinit_result = usb_mode_deinit();
    TEST_ASSERT_TRUE(deinit_result);
}

/**
 * @test USB Mode set and get
 */
void test_usb_mode_set_get(void) {
    ESP_LOGI(TAG, "Test: USB Mode set and get");
    
    usb_mode_init();
    
    usb_mode_set(USB_MODE_DEVICE_ONLY);
    usb_mode_t mode = usb_mode_get();
    TEST_ASSERT_EQUAL(USB_MODE_DEVICE_ONLY, mode);
    
    usb_mode_set(USB_MODE_HOST_ONLY);
    mode = usb_mode_get();
    TEST_ASSERT_EQUAL(USB_MODE_HOST_ONLY, mode);
    
    usb_mode_set(USB_MODE_DUAL_AUTO);
    mode = usb_mode_get();
    TEST_ASSERT_EQUAL(USB_MODE_DUAL_AUTO, mode);
}

/**
 * @test USB Mode get state
 */
void test_usb_mode_get_state(void) {
    ESP_LOGI(TAG, "Test: USB Mode get state");
    
    usb_mode_init();
    
    usb_mode_state_t state = usb_mode_get_state();
    TEST_ASSERT_NOT_EQUAL(USB_MODE_STATE_ERROR, state);
}

/**
 * @test USB Mode is switching
 */
void test_usb_mode_is_switching(void) {
    ESP_LOGI(TAG, "Test: USB Mode is switching");
    
    usb_mode_init();
    
    bool switching = usb_mode_is_switching();
    TEST_ASSERT_FALSE(switching);
}

/**
 * @test USB Mode wait ready
 */
void test_usb_mode_wait_ready(void) {
    ESP_LOGI(TAG, "Test: USB Mode wait ready");
    
    usb_mode_init();
    
    bool ready = usb_mode_wait_ready(1000);
    TEST_ASSERT_TRUE(ready);
}

/**
 * @test USB Mode is device active
 */
void test_usb_mode_is_device_active(void) {
    ESP_LOGI(TAG, "Test: USB Mode is device active");
    
    usb_mode_init();
    usb_mode_set(USB_MODE_DEVICE_ONLY);
    
    bool active = usb_mode_is_device_active();
    TEST_ASSERT_TRUE(active);
}

/**
 * @test USB Mode is host active
 */
void test_usb_mode_is_host_active(void) {
    ESP_LOGI(TAG, "Test: USB Mode is host active");
    
    usb_mode_init();
    usb_mode_set(USB_MODE_HOST_ONLY);
    
    bool active = usb_mode_is_host_active();
    TEST_ASSERT_TRUE(active);
}

/**
 * @test USB Mode get status
 */
void test_usb_mode_get_status(void) {
    ESP_LOGI(TAG, "Test: USB Mode get status");
    
    usb_mode_init();
    
    usb_mode_status_t status;
    bool result = usb_mode_get_status(&status);
    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_NOT_EQUAL(USB_MODE_STATE_ERROR, status.state);
}

/**
 * @test USB Mode get status with NULL pointer
 */
void test_usb_mode_get_status_null(void) {
    ESP_LOGI(TAG, "Test: USB Mode get status (NULL pointer)");
    
    usb_mode_init();
    
    bool result = usb_mode_get_status(NULL);
    TEST_ASSERT_FALSE(result);
}

/**
 * @test USB Mode get status string
 */
void test_usb_mode_get_status_string(void) {
    ESP_LOGI(TAG, "Test: USB Mode get status string");
    
    usb_mode_init();
    
    const char *status_str = usb_mode_get_status_string();
    TEST_ASSERT_NOT_NULL(status_str);
    TEST_ASSERT_NOT_EQUAL(0, strlen(status_str));
}

/**
 * @test USB Mode notify device connected
 */
void test_usb_mode_notify_device_connected(void) {
    ESP_LOGI(TAG, "Test: USB Mode notify device connected");
    
    usb_mode_init();
    
    usb_mode_notify_device_connected();
    
    usb_mode_status_t status;
    usb_mode_get_status(&status);
    TEST_ASSERT_TRUE(status.device_connected);
}

/**
 * @test USB Mode notify device disconnected
 */
void test_usb_mode_notify_device_disconnected(void) {
    ESP_LOGI(TAG, "Test: USB Mode notify device disconnected");
    
    usb_mode_init();
    
    usb_mode_notify_device_connected();
    usb_mode_notify_device_disconnected();
    
    usb_mode_status_t status;
    usb_mode_get_status(&status);
    TEST_ASSERT_FALSE(status.device_connected);
}

/**
 * @test USB Mode notify host device connected
 */
void test_usb_mode_notify_host_device_connected(void) {
    ESP_LOGI(TAG, "Test: USB Mode notify host device connected");
    
    usb_mode_init();
    
    usb_mode_notify_host_device_connected();
    
    usb_mode_status_t status;
    usb_mode_get_status(&status);
    TEST_ASSERT_TRUE(status.host_connected);
}

/**
 * @test USB Mode notify host device disconnected
 */
void test_usb_mode_notify_host_device_disconnected(void) {
    ESP_LOGI(TAG, "Test: USB Mode notify host device disconnected");
    
    usb_mode_init();
    
    usb_mode_notify_host_device_connected();
    usb_mode_notify_host_device_disconnected();
    
    usb_mode_status_t status;
    usb_mode_get_status(&status);
    TEST_ASSERT_FALSE(status.host_connected);
}

/**
 * @test USB Mode multiple mode switches
 */
void test_usb_mode_multiple_switches(void) {
    ESP_LOGI(TAG, "Test: USB Mode multiple mode switches");
    
    usb_mode_init();
    
    for (int i = 0; i < 3; i++) {
        usb_mode_set(USB_MODE_DEVICE_ONLY);
        usb_mode_t mode = usb_mode_get();
        TEST_ASSERT_EQUAL(USB_MODE_DEVICE_ONLY, mode);
        
        usb_mode_set(USB_MODE_HOST_ONLY);
        mode = usb_mode_get();
        TEST_ASSERT_EQUAL(USB_MODE_HOST_ONLY, mode);
    }
}

/**
 * @test USB Mode status structure
 */
void test_usb_mode_status_structure(void) {
    ESP_LOGI(TAG, "Test: USB Mode status structure");
    
    usb_mode_status_t status;
    memset(&status, 0, sizeof(status));
    
    // Verify structure members
    TEST_ASSERT_EQUAL(sizeof(usb_mode_t), sizeof(status.mode));
    TEST_ASSERT_EQUAL(sizeof(usb_mode_state_t), sizeof(status.state));
    TEST_ASSERT_EQUAL(sizeof(bool), sizeof(status.device_connected));
    TEST_ASSERT_EQUAL(sizeof(bool), sizeof(status.host_connected));
}

