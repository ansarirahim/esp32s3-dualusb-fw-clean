/**
 * @file board_pins.h
 * @brief Hardware Pin Definitions for ESP32-S3 DevKitC-1
 *
 * @author A.R. Ansari <ansarirahim1@gmail.com>
 * @date 2025-10-21
 * @version 1.0.0
 *
 * @section description Description
 * Hardware pin definitions and GPIO mappings for ESP32-S3 DevKitC-1 board.
 * Defines USB PHY pins, LED pins, and boot/mode selection pins.
 *
 * @section board Board Information
 * - **Board**: ESP32-S3 DevKitC-1 (N8R8 / N16R8)
 * - **Microcontroller**: ESP32-S3 (Dual-core, 240 MHz)
 * - **USB PHY**: Native (GPIO20=D+, GPIO19=D-)
 * - **Flash**: 8MB or 16MB (depending on variant)
 * - **RAM**: 8MB PSRAM
 *
 * @section pinout Pin Assignments
 * - **USB D+**: GPIO20
 * - **USB D-**: GPIO19
 * - **LED Red**: GPIO6
 * - **BOOT1**: GPIO0 (read-only in M1, for future mode select)
 *
 * @section usb_phy USB PHY Configuration
 * The ESP32-S3 has a native USB PHY that supports:
 * - USB Device Mode (MSC, CDC, HID, etc.)
 * - USB Host Mode (future milestone)
 * - Full-speed USB (12 Mbps)
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

/** @defgroup usb_pins USB PHY Pins
 * @brief USB PHY pin definitions (Native PHY)
 * @{
 */
/** @brief USB Data+ pin (GPIO20) */
#define PIN_USB_DP        20
/** @brief USB Data- pin (GPIO19) */
#define PIN_USB_DM        19
/** @} */

/** @defgroup led_pins LED Pins
 * @brief LED control pin definitions
 * @{
 */
/** @brief Red LED pin for Device Mode status indication (GPIO6) */
#define PIN_LED_R         6
/** @} */

/** @defgroup boot_pins Boot and Mode Selection Pins
 * @brief Boot and mode selection pin definitions
 * @{
 */
/** @brief GPIO0 - BOOT1 (read-only in M1, for future mode select) */
#define PIN_BOOT1         0
/** @} */
