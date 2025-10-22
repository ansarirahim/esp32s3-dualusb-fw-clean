/**
 * @file test_usb_host.c
 * @brief Unit Tests for USB Host Mode (MSC)
 *
 * @author A.R. Ansari <ansarirahim1@gmail.com>
 * @date 2025-10-22
 * @version 1.0.0
 *
 * Tests for USB Host Mode functionality including:
 * - Initialization and deinitialization
 * - Device connection status
 * - State management
 * - Device information retrieval
 * - File operations (read/write)
 * - Error handling
 */

#include "unity.h"
#include "usb_host.h"
#include "esp_log.h"
#include <string.h>

static const char *TAG = "test_usb_host";

/**
 * @brief Setup function - called before each test
 */
void setUp(void) {
    ESP_LOGI(TAG, "Setting up USB Host test");
}

/**
 * @brief Teardown function - called after each test
 */
void tearDown(void) {
    ESP_LOGI(TAG, "Tearing down USB Host test");
    usb_host_deinit();
}

/**
 * @test USB Host initialization
 */
void test_usb_host_init(void) {
    ESP_LOGI(TAG, "Test: USB Host initialization");
    
    bool result = usb_host_init();
    TEST_ASSERT_TRUE(result);
    
    // Verify state is IDLE after initialization
    usb_host_state_t state = usb_host_get_state();
    TEST_ASSERT_EQUAL(USB_HOST_STATE_IDLE, state);
}

/**
 * @test USB Host double initialization
 */
void test_usb_host_double_init(void) {
    ESP_LOGI(TAG, "Test: USB Host double initialization");
    
    bool result1 = usb_host_init();
    TEST_ASSERT_TRUE(result1);
    
    // Second initialization should also succeed
    bool result2 = usb_host_init();
    TEST_ASSERT_TRUE(result2);
}

/**
 * @test USB Host deinitialization
 */
void test_usb_host_deinit(void) {
    ESP_LOGI(TAG, "Test: USB Host deinitialization");
    
    bool init_result = usb_host_init();
    TEST_ASSERT_TRUE(init_result);
    
    bool deinit_result = usb_host_deinit();
    TEST_ASSERT_TRUE(deinit_result);
}

/**
 * @test USB Host device not connected initially
 */
void test_usb_host_device_not_connected_initially(void) {
    ESP_LOGI(TAG, "Test: USB Host device not connected initially");
    
    usb_host_init();
    
    bool connected = usb_host_is_device_connected();
    TEST_ASSERT_FALSE(connected);
}

/**
 * @test USB Host get state
 */
void test_usb_host_get_state(void) {
    ESP_LOGI(TAG, "Test: USB Host get state");
    
    usb_host_init();
    
    usb_host_state_t state = usb_host_get_state();
    TEST_ASSERT_EQUAL(USB_HOST_STATE_IDLE, state);
}

/**
 * @test USB Host get device info when no device connected
 */
void test_usb_host_get_device_info_no_device(void) {
    ESP_LOGI(TAG, "Test: USB Host get device info (no device)");
    
    usb_host_init();
    
    usb_host_device_info_t info;
    bool result = usb_host_get_device_info(&info);
    TEST_ASSERT_FALSE(result);
}

/**
 * @test USB Host get device info with NULL pointer
 */
void test_usb_host_get_device_info_null_pointer(void) {
    ESP_LOGI(TAG, "Test: USB Host get device info (NULL pointer)");
    
    usb_host_init();
    
    bool result = usb_host_get_device_info(NULL);
    TEST_ASSERT_FALSE(result);
}

/**
 * @test USB Host read file when no device connected
 */
void test_usb_host_read_file_no_device(void) {
    ESP_LOGI(TAG, "Test: USB Host read file (no device)");
    
    usb_host_init();
    
    uint8_t buffer[256];
    int result = usb_host_read_file("/usb/test.txt", buffer, sizeof(buffer));
    TEST_ASSERT_EQUAL(-1, result);
}

/**
 * @test USB Host read file with invalid parameters
 */
void test_usb_host_read_file_invalid_params(void) {
    ESP_LOGI(TAG, "Test: USB Host read file (invalid params)");
    
    usb_host_init();
    
    uint8_t buffer[256];
    
    // NULL path
    int result1 = usb_host_read_file(NULL, buffer, sizeof(buffer));
    TEST_ASSERT_EQUAL(-1, result1);
    
    // NULL buffer
    int result2 = usb_host_read_file("/usb/test.txt", NULL, sizeof(buffer));
    TEST_ASSERT_EQUAL(-1, result2);
    
    // Zero size
    int result3 = usb_host_read_file("/usb/test.txt", buffer, 0);
    TEST_ASSERT_EQUAL(-1, result3);
}

/**
 * @test USB Host write file when no device connected
 */
void test_usb_host_write_file_no_device(void) {
    ESP_LOGI(TAG, "Test: USB Host write file (no device)");
    
    usb_host_init();
    
    uint8_t buffer[256] = {0};
    int result = usb_host_write_file("/usb/test.txt", buffer, sizeof(buffer));
    TEST_ASSERT_EQUAL(-1, result);
}

/**
 * @test USB Host write file with invalid parameters
 */
void test_usb_host_write_file_invalid_params(void) {
    ESP_LOGI(TAG, "Test: USB Host write file (invalid params)");
    
    usb_host_init();
    
    uint8_t buffer[256] = {0};
    
    // NULL path
    int result1 = usb_host_write_file(NULL, buffer, sizeof(buffer));
    TEST_ASSERT_EQUAL(-1, result1);
    
    // NULL buffer
    int result2 = usb_host_write_file("/usb/test.txt", NULL, sizeof(buffer));
    TEST_ASSERT_EQUAL(-1, result2);
    
    // Zero size
    int result3 = usb_host_write_file("/usb/test.txt", buffer, 0);
    TEST_ASSERT_EQUAL(-1, result3);
}

/**
 * @test USB Host list files when no device connected
 */
void test_usb_host_list_files_no_device(void) {
    ESP_LOGI(TAG, "Test: USB Host list files (no device)");
    
    usb_host_init();
    
    char *files[10];
    int result = usb_host_list_files("/usb/", files, 10);
    TEST_ASSERT_EQUAL(-1, result);
}

/**
 * @test USB Host list files with invalid parameters
 */
void test_usb_host_list_files_invalid_params(void) {
    ESP_LOGI(TAG, "Test: USB Host list files (invalid params)");
    
    usb_host_init();
    
    char *files[10];
    
    // NULL path
    int result1 = usb_host_list_files(NULL, files, 10);
    TEST_ASSERT_EQUAL(-1, result1);
    
    // NULL files array
    int result2 = usb_host_list_files("/usb/", NULL, 10);
    TEST_ASSERT_EQUAL(-1, result2);
    
    // Zero max files
    int result3 = usb_host_list_files("/usb/", files, 0);
    TEST_ASSERT_EQUAL(-1, result3);
}

/**
 * @test USB Host eject device
 */
void test_usb_host_eject_device(void) {
    ESP_LOGI(TAG, "Test: USB Host eject device");
    
    usb_host_init();
    
    bool result = usb_host_eject_device();
    TEST_ASSERT_TRUE(result);
    
    // After eject, device should not be connected
    bool connected = usb_host_is_device_connected();
    TEST_ASSERT_FALSE(connected);
}

/**
 * @test USB Host state after eject
 */
void test_usb_host_state_after_eject(void) {
    ESP_LOGI(TAG, "Test: USB Host state after eject");
    
    usb_host_init();
    usb_host_eject_device();
    
    usb_host_state_t state = usb_host_get_state();
    TEST_ASSERT_EQUAL(USB_HOST_STATE_IDLE, state);
}

/**
 * @test USB Host multiple init/deinit cycles
 */
void test_usb_host_multiple_cycles(void) {
    ESP_LOGI(TAG, "Test: USB Host multiple init/deinit cycles");
    
    for (int i = 0; i < 3; i++) {
        bool init_result = usb_host_init();
        TEST_ASSERT_TRUE(init_result);
        
        usb_host_state_t state = usb_host_get_state();
        TEST_ASSERT_EQUAL(USB_HOST_STATE_IDLE, state);
        
        bool deinit_result = usb_host_deinit();
        TEST_ASSERT_TRUE(deinit_result);
    }
}

/**
 * @test USB Host device info structure
 */
void test_usb_host_device_info_structure(void) {
    ESP_LOGI(TAG, "Test: USB Host device info structure");
    
    usb_host_device_info_t info;
    memset(&info, 0, sizeof(info));
    
    // Verify structure size
    TEST_ASSERT_EQUAL(sizeof(uint16_t), sizeof(info.vendor_id));
    TEST_ASSERT_EQUAL(sizeof(uint16_t), sizeof(info.product_id));
    TEST_ASSERT_EQUAL(sizeof(uint32_t), sizeof(info.total_sectors));
    TEST_ASSERT_EQUAL(sizeof(uint32_t), sizeof(info.sector_size));
}

