/**
 * @file test_led_control.c
 * @brief Unit Tests for LED Control Module
 *
 * @author A.R. Ansari <ansarirahim1@gmail.com>
 * @date 2025-10-21
 * @version 1.0.0
 *
 * @section description Description
 * Comprehensive unit tests for LED control functionality.
 * Tests LED state management, state transitions, and state queries.
 *
 * @section test_cases Test Cases
 * - LED initialization
 * - LED state setting
 * - LED state getting
 * - LED state transitions
 * - Invalid state handling
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
#include "unity.h"
#include "led_control.h"

/**
 * @brief Setup function called before each test
 *
 * Initializes the LED control system for testing.
 */
void setUp(void) {
    /* Initialize LED for each test */
    led_init();
}

/**
 * @brief Teardown function called after each test
 *
 * Cleans up after each test.
 */
void tearDown(void) {
    /* No cleanup needed for LED tests */
}

/**
 * @test LED Initialization
 *
 * Verifies that LED is properly initialized and starts in IDLE state.
 */
TEST_CASE("LED: Initialization", "[led]") {
    /* After initialization, LED should be in IDLE state */
    led_state_t state = led_get_state();
    TEST_ASSERT_EQUAL(LED_STATE_IDLE, state);
}

/**
 * @test LED State Setting - IDLE
 *
 * Verifies that LED state can be set to IDLE.
 */
TEST_CASE("LED: Set State to IDLE", "[led]") {
    led_set_state(LED_STATE_IDLE);
    led_state_t state = led_get_state();
    TEST_ASSERT_EQUAL(LED_STATE_IDLE, state);
}

/**
 * @test LED State Setting - BUSY
 *
 * Verifies that LED state can be set to BUSY.
 */
TEST_CASE("LED: Set State to BUSY", "[led]") {
    led_set_state(LED_STATE_BUSY);
    led_state_t state = led_get_state();
    TEST_ASSERT_EQUAL(LED_STATE_BUSY, state);
}

/**
 * @test LED State Setting - ERROR
 *
 * Verifies that LED state can be set to ERROR.
 */
TEST_CASE("LED: Set State to ERROR", "[led]") {
    led_set_state(LED_STATE_ERROR);
    led_state_t state = led_get_state();
    TEST_ASSERT_EQUAL(LED_STATE_ERROR, state);
}

/**
 * @test LED State Transitions
 *
 * Verifies that LED can transition between different states.
 */
TEST_CASE("LED: State Transitions", "[led]") {
    /* Start in IDLE */
    led_set_state(LED_STATE_IDLE);
    TEST_ASSERT_EQUAL(LED_STATE_IDLE, led_get_state());

    /* Transition to BUSY */
    led_set_state(LED_STATE_BUSY);
    TEST_ASSERT_EQUAL(LED_STATE_BUSY, led_get_state());

    /* Transition to ERROR */
    led_set_state(LED_STATE_ERROR);
    TEST_ASSERT_EQUAL(LED_STATE_ERROR, led_get_state());

    /* Transition back to IDLE */
    led_set_state(LED_STATE_IDLE);
    TEST_ASSERT_EQUAL(LED_STATE_IDLE, led_get_state());
}

/**
 * @test LED Multiple State Changes
 *
 * Verifies that LED can handle rapid state changes.
 */
TEST_CASE("LED: Multiple State Changes", "[led]") {
    for (int i = 0; i < 10; i++) {
        led_set_state(LED_STATE_IDLE);
        TEST_ASSERT_EQUAL(LED_STATE_IDLE, led_get_state());

        led_set_state(LED_STATE_BUSY);
        TEST_ASSERT_EQUAL(LED_STATE_BUSY, led_get_state());

        led_set_state(LED_STATE_ERROR);
        TEST_ASSERT_EQUAL(LED_STATE_ERROR, led_get_state());
    }
}

/**
 * @test LED State Persistence
 *
 * Verifies that LED state persists after being set.
 */
TEST_CASE("LED: State Persistence", "[led]") {
    led_set_state(LED_STATE_BUSY);
    
    /* Check state multiple times */
    for (int i = 0; i < 5; i++) {
        TEST_ASSERT_EQUAL(LED_STATE_BUSY, led_get_state());
    }
}

/**
 * @test LED Enumeration Values
 *
 * Verifies that LED state enumeration values are correct.
 */
TEST_CASE("LED: Enumeration Values", "[led]") {
    /* Verify enumeration values are distinct */
    TEST_ASSERT_NOT_EQUAL(LED_STATE_IDLE, LED_STATE_BUSY);
    TEST_ASSERT_NOT_EQUAL(LED_STATE_IDLE, LED_STATE_ERROR);
    TEST_ASSERT_NOT_EQUAL(LED_STATE_BUSY, LED_STATE_ERROR);
}

/**
 * @test LED State After Multiple Initializations
 *
 * Verifies that LED state is correct after re-initialization.
 */
TEST_CASE("LED: State After Re-initialization", "[led]") {
    /* Set to BUSY */
    led_set_state(LED_STATE_BUSY);
    TEST_ASSERT_EQUAL(LED_STATE_BUSY, led_get_state());

    /* Re-initialize */
    led_init();

    /* Should be back to IDLE */
    TEST_ASSERT_EQUAL(LED_STATE_IDLE, led_get_state());
}

