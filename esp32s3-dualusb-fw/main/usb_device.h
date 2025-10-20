/*
 * @file usb_device.h
 * @brief USB Device Mode (MSC) interface
 *
 * Author: A.R. Ansari <ansarirahim1@gmail.com>
 * Date: 2025-10-20
 */

#pragma once

#include <stdbool.h>

/**
 * @brief Initialize USB Device Mode (MSC)
 *
 * Initializes TinyUSB device stack with MSC class.
 * Exposes internal FATFS volume as a USB Mass Storage Device.
 *
 * @return true if successful, false otherwise
 */
bool usb_device_init(void);

/**
 * @brief Check if USB device is connected
 *
 * @return true if connected, false otherwise
 */
bool usb_device_is_connected(void);

/**
 * @brief Check if USB device is mounted on host
 *
 * @return true if mounted, false otherwise
 */
bool usb_device_is_mounted(void);

/**
 * @brief Notify USB device of I/O activity
 *
 * Called by MSC callbacks to update LED state.
 */
void usb_device_notify_io_start(void);
void usb_device_notify_io_end(void);
