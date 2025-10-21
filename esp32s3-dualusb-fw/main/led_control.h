/**
 * @file led_control.h
 * @brief LED Control Interface for ESP32-S3 Dual USB Firmware
 *
 * @author A.R. Ansari <ansarirahim1@gmail.com>
 * @date 2025-10-21
 * @version 1.0.0
 *
 * @section description Description
 * Provides LED control interface for status indication on ESP32-S3.
 * Implements state machine with different blinking patterns for various device states.
 *
 * @section features Features
 * - Multiple LED states with distinct blinking patterns
 * - GPIO-based LED control
 * - FreeRTOS task-based LED management
 * - Thread-safe state transitions
 * - Comprehensive error indication
 *
 * @section led_patterns LED Blinking Patterns
 * - **IDLE**: Slow blink RED (500ms ON / 1500ms OFF) - Device ready, no activity
 * - **BUSY**: Fast blink RED (200ms ON / 200ms OFF) - Active I/O operation
 * - **ERROR**: RED solid 3s, then slow blink - Error condition detected
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

/**
 * @enum led_state_t
 * @brief LED State Enumeration
 *
 * Defines possible LED states with associated blinking patterns.
 */
typedef enum {
    LED_STATE_IDLE,      /**< Device idle: slow blink RED (500ms ON / 1500ms OFF) */
    LED_STATE_BUSY,      /**< Device active I/O: fast blink RED (200ms ON / 200ms OFF) */
    LED_STATE_ERROR,     /**< Error: RED solid 3s, then slow blink */
} led_state_t;

/**
 * @brief Initialize LED Control System
 *
 * Sets up GPIO pin and FreeRTOS task for LED blinking patterns.
 * Initializes the LED to IDLE state.
 *
 * @details
 * - Configures GPIO pin for LED output
 * - Creates FreeRTOS task for LED management
 * - Initializes state machine
 * - Sets initial state to IDLE
 *
 * @note Must be called before any other LED functions
 * @note Should be called from app_main() during system startup
 *
 * @see led_set_state()
 * @see led_get_state()
 */
void led_init(void);

/**
 * @brief Set LED State
 *
 * Changes the LED state, which triggers the corresponding blinking pattern.
 * Thread-safe operation with state machine protection.
 *
 * @param[in] state LED state to set (LED_STATE_IDLE, LED_STATE_BUSY, or LED_STATE_ERROR)
 *
 * @details
 * - Updates internal state machine
 * - Triggers appropriate blinking pattern
 * - Thread-safe with semaphore protection
 * - Can be called from any context (task or ISR)
 *
 * @note Thread-safe operation
 * @see led_state_t
 * @see led_get_state()
 */
void led_set_state(led_state_t state);

/**
 * @brief Get Current LED State
 *
 * Retrieves the current LED state.
 *
 * @return Current LED state
 * @retval LED_STATE_IDLE Device is idle
 * @retval LED_STATE_BUSY Device is performing I/O
 * @retval LED_STATE_ERROR Error condition detected
 *
 * @note Thread-safe operation
 * @see led_state_t
 * @see led_set_state()
 */
led_state_t led_get_state(void);
