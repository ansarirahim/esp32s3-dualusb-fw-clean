# Unit Tests for ESP32-S3 Dual USB Firmware
#
# This CMakeLists.txt configures the unit test build for the firmware.
# Tests use the Unity testing framework provided by ESP-IDF.

cmake_minimum_required(VERSION 3.16)

project(esp32s3_dualusb_fw_tests)

# Include ESP-IDF components
set(EXTRA_COMPONENT_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/../components")

# Register test components
idf_component_register(
    SRC_DIRS 
        "unit"
    INCLUDE_DIRS 
        "unit"
        "../main"
    REQUIRES 
        unity
        esp_tinyusb
        fatfs
        wear_levelling
        esp_partition
)

# Add test executable
add_executable(${PROJECT_NAME}_test
    unit/test_led_control.c
    unit/test_filesystem.c
    unit/test_usb_device.c
    unit/test_usb_host.c
    unit/test_usb_mode.c
    unit/test_main.c
)

# Link with main component sources (excluding main.c entry point)
target_sources(${PROJECT_NAME}_test PRIVATE
    ../main/led_control.c
    ../main/filesystem.c
    ../main/usb_device.c
    ../main/usb_host.c
    ../main/usb_mode.c
)

# Link libraries
target_link_libraries(${PROJECT_NAME}_test PRIVATE
    unity
    esp_tinyusb
    fatfs
    wear_levelling
    esp_partition
)

# Enable testing
enable_testing()

# Add test
add_test(
    NAME ${PROJECT_NAME}_test
    COMMAND ${PROJECT_NAME}_test
)

