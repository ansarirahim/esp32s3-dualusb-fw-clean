/*
 * @file led_control.h
 * @brief LED control interface for ESP32-S3 Dual USB Firmware
 *
 * Author: A.R. Ansari <ansarirahim1@gmail.com>
 * Date: 2025-10-20
 */

#pragma once

typedef enum {
    LED_STATE_IDLE,      /* Device idle: slow blink RED (500ms ON / 1500ms OFF) */
    LED_STATE_BUSY,      /* Device active I/O: fast blink RED (200ms ON / 200ms OFF) */
    LED_STATE_ERROR,     /* Error: RED solid 3s, then slow blink */
} led_state_t;

/**
 * @brief Initialize LED control
 *
 * Sets up GPIO and timer/task for LED blinking patterns.
 */
void led_init(void);

/**
 * @brief Set LED state
 *
 * @param state LED state (idle, busy, error)
 */
void led_set_state(led_state_t state);

/**
 * @brief Get current LED state
 *
 * @return Current LED state
 */
led_state_t led_get_state(void);

#endif /* LED_CONTROL_H */
