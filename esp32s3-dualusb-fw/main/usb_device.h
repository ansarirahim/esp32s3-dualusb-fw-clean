/**
 * @file usb_device.h
 * @brief USB Device Mode (MSC) Interface
 *
 * @author A.R. Ansari <ansarirahim1@gmail.com>
 * @date 2025-10-21
 * @version 1.0.0
 *
 * @section description Description
 * Provides interface for USB Device Mode (MSC - Mass Storage Class) functionality.
 * Exposes internal FATFS volume as a USB Mass Storage Device to host computers.
 *
 * @section features Features
 * - TinyUSB MSC device stack
 * - Block device backed by internal FATFS
 * - Sector-level read/write operations
 * - SCSI START/STOP UNIT handling for safe eject
 * - I/O activity monitoring and LED state updates
 * - Write synchronization for data safety
 *
 * @section usage Usage
 * @code
 * // Initialize USB Device Mode
 * if (usb_device_init()) {
 *     // Check connection status
 *     if (usb_device_is_connected()) {
 *         // Device is connected to host
 *     }
 *     if (usb_device_is_mounted()) {
 *         // Device is mounted on host
 *     }
 * }
 * @endcode
 *
 * @section contact Contact
 * - Email: ansarirahim1@gmail.com
 * - Phone: +91 9024304883
 * - GitHub: https://github.com/ansarirahim
 *
 * @section license License
 * Copyright (c) 2025 A.R. Ansari. All rights reserved.
 */

#pragma once

#include <stdbool.h>

/**
 * @brief Initialize USB Device Mode (MSC)
 *
 * Initializes the TinyUSB device stack with MSC (Mass Storage Class) support.
 * Configures the USB peripheral for device mode and exposes the internal FATFS
 * volume as a USB Mass Storage Device to host computers.
 *
 * @details
 * This function performs the following operations:
 * - Configures TinyUSB driver with appropriate PHY settings
 * - Installs MSC driver with FATFS backend
 * - Sets up I/O activity monitoring
 * - Initializes internal state variables
 *
 * @return true if initialization successful, false otherwise
 * @retval true USB Device Mode initialized and ready
 * @retval false Initialization failed (check logs for details)
 *
 * @note Must be called after filesystem initialization
 * @note Should be called from app_main() during system startup
 *
 * @see usb_device_is_connected()
 * @see usb_device_is_mounted()
 */
bool usb_device_init(void);

/**
 * @brief Check if USB device is connected to host
 *
 * Queries the current USB connection status. Returns true if the device
 * is physically connected to a host computer via USB cable.
 *
 * @return true if USB device is connected to host, false otherwise
 * @retval true Device is connected
 * @retval false Device is not connected
 *
 * @note This indicates physical connection, not necessarily mounted status
 * @see usb_device_is_mounted()
 */
bool usb_device_is_connected(void);

/**
 * @brief Check if USB device is mounted on host
 *
 * Queries the current USB mount status. Returns true if the host computer
 * has successfully mounted the USB Mass Storage Device and can access files.
 *
 * @return true if USB device is mounted on host, false otherwise
 * @retval true Device is mounted and accessible
 * @retval false Device is not mounted
 *
 * @note Device must be connected before it can be mounted
 * @see usb_device_is_connected()
 */
bool usb_device_is_mounted(void);

/**
 * @brief Notify USB device of I/O activity start
 *
 * Called by MSC read/write callbacks to signal the start of I/O operations.
 * Updates LED state to indicate active data transfer.
 *
 * @note Internal function, called automatically by MSC callbacks
 * @see usb_device_notify_io_end()
 */
void usb_device_notify_io_start(void);

/**
 * @brief Notify USB device of I/O activity end
 *
 * Called by MSC read/write callbacks to signal the end of I/O operations.
 * Updates LED state based on device status.
 *
 * @note Internal function, called automatically by MSC callbacks
 * @see usb_device_notify_io_start()
 */
void usb_device_notify_io_end(void);
