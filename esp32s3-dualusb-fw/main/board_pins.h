/*
 * @file board_pins.h
 * @brief Hardware pin definitions for ESP32-S3 DevKitC-1
 *
 * Author: A.R. Ansari <ansarirahim1@gmail.com>
 * Date: 2025-10-20
 *
 * Board: ESP32-S3 DevKitC-1 (N8R8 / N16R8)
 * USB PHY: Native (GPIO20=D+, GPIO19=D-)
 */

#pragma once

/* USB Pins (Native PHY) */
#define PIN_USB_DP        20
#define PIN_USB_DM        19

/* LED Pins */
#define PIN_LED_R         6    /* Red LED for Device Mode */

/* Boot/Mode Selection */
#define PIN_BOOT1         0    /* GPIO0 - BOOT1 (read-only in M1, for future mode select) */
