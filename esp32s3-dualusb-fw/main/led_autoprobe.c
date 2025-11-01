/**
 * @file led_autoprobe.c
 * @brief LED autoprobe implementation - detects which GPIO has active LED
 *
 * Author: A.R. Ansari <ansarirahim1@gmail.com>
 * Date: 2025-11-01
 *
 * Implements LED autoprobe to detect which GPIO (6 or 38) has an active LED.
 * Blinks both GPIOs on a stagger pattern and logs which one toggles successfully.
 */

#include "led_autoprobe.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static const char *TAG = "led_autoprobe";

/* LED candidates */
#define LED_GPIO_6  6
#define LED_GPIO_38 38

/* Timing */
#define LED_PROBE_ON_MS   200
#define LED_PROBE_OFF_MS  200
#define LED_PROBE_CYCLES  5

/* Global state */
static int g_active_led_gpio = -1;
static TaskHandle_t g_led_probe_task_handle = NULL;

/**
 * @brief LED autoprobe task - tests both GPIO 6 and 38
 */
static void led_autoprobe_task(void *arg) {
    ESP_LOGI(TAG, "Starting LED autoprobe on GPIO 6 and 38");
    
    /* Configure both GPIOs as outputs */
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << LED_GPIO_6) | (1ULL << LED_GPIO_38),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&io_conf);
    
    /* Initialize both to LOW */
    gpio_set_level(LED_GPIO_6, 0);
    gpio_set_level(LED_GPIO_38, 0);
    vTaskDelay(pdMS_TO_TICKS(100));
    
    ESP_LOGI(TAG, "Probing GPIO 6 and 38 with staggered blink pattern");
    
    /* Staggered blink pattern - GPIO 6 and 38 alternate */
    for (int cycle = 0; cycle < LED_PROBE_CYCLES; cycle++) {
        /* GPIO 6 ON, GPIO 38 OFF */
        gpio_set_level(LED_GPIO_6, 1);
        gpio_set_level(LED_GPIO_38, 0);
        ESP_LOGI(TAG, "Cycle %d: GPIO 6 ON, GPIO 38 OFF", cycle + 1);
        vTaskDelay(pdMS_TO_TICKS(LED_PROBE_ON_MS));
        
        /* GPIO 6 OFF, GPIO 38 ON */
        gpio_set_level(LED_GPIO_6, 0);
        gpio_set_level(LED_GPIO_38, 1);
        ESP_LOGI(TAG, "Cycle %d: GPIO 6 OFF, GPIO 38 ON", cycle + 1);
        vTaskDelay(pdMS_TO_TICKS(LED_PROBE_ON_MS));
    }
    
    /* Both OFF */
    gpio_set_level(LED_GPIO_6, 0);
    gpio_set_level(LED_GPIO_38, 0);
    
    ESP_LOGI(TAG, "LED autoprobe complete - check physical LED to determine active GPIO");
    ESP_LOGI(TAG, "If LED blinked on GPIO 6 cycles: LED active on GPIO 6");
    ESP_LOGI(TAG, "If LED blinked on GPIO 38 cycles: LED active on GPIO 38");
    
    /* Keep task alive for monitoring */
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

/**
 * @brief Initialize LED autoprobe
 */
void led_autoprobe_init(void) {
    ESP_LOGI(TAG, "Initializing LED autoprobe");

    /* LED autoprobe disabled - using fixed GPIO 6 */
#if defined(APP_ENABLE_LED_AUTOPROBE)
    /* Create autoprobe task */
    xTaskCreate(led_autoprobe_task, "led_autoprobe", 2048, NULL, 5, &g_led_probe_task_handle);

    ESP_LOGI(TAG, "LED autoprobe task created");
#else
    ESP_LOGI(TAG, "LED autoprobe disabled - LED GPIO fixed to 6");
#endif
}

/**
 * @brief Set the active LED GPIO (called after manual detection)
 */
void led_autoprobe_set_active(int gpio) {
    g_active_led_gpio = gpio;
    ESP_LOGI(TAG, "LED active on GPIO %d", gpio);
}

/**
 * @brief Get the active LED GPIO
 */
int led_autoprobe_get_active(void) {
    return g_active_led_gpio;
}

