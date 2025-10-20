#include "led_control.h"
#include "board_pins.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static const char *TAG = "led";

/* LED blink timing (in ms) */
#define LED_IDLE_ON_MS      500
#define LED_IDLE_OFF_MS     1500
#define LED_BUSY_ON_MS      200
#define LED_BUSY_OFF_MS     200
#define LED_ERROR_SOLID_MS  3000

/* Global state */
static led_state_t g_led_state = LED_STATE_IDLE;
static TaskHandle_t g_led_task_handle = NULL;

/**
 * @brief LED blink task
 */
static void led_blink_task(void *arg) {
    while (1) {
        switch (g_led_state) {
            case LED_STATE_IDLE:
                /* Slow blink: 500ms ON / 1500ms OFF */
                gpio_set_level(PIN_LED_R, 1);
                vTaskDelay(pdMS_TO_TICKS(LED_IDLE_ON_MS));
                gpio_set_level(PIN_LED_R, 0);
                vTaskDelay(pdMS_TO_TICKS(LED_IDLE_OFF_MS));
                break;

            case LED_STATE_BUSY:
                /* Fast blink: 200ms ON / 200ms OFF */
                gpio_set_level(PIN_LED_R, 1);
                vTaskDelay(pdMS_TO_TICKS(LED_BUSY_ON_MS));
                gpio_set_level(PIN_LED_R, 0);
                vTaskDelay(pdMS_TO_TICKS(LED_BUSY_OFF_MS));
                break;

            case LED_STATE_ERROR:
                /* Error: solid 3s, then slow blink */
                gpio_set_level(PIN_LED_R, 1);
                vTaskDelay(pdMS_TO_TICKS(LED_ERROR_SOLID_MS));
                gpio_set_level(PIN_LED_R, 0);
                vTaskDelay(pdMS_TO_TICKS(LED_IDLE_OFF_MS));
                break;

            default:
                vTaskDelay(pdMS_TO_TICKS(100));
                break;
        }
    }
}

void led_init(void) {
    ESP_LOGI(TAG, "Initializing LED on GPIO %d", PIN_LED_R);

    /* Configure GPIO */
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << PIN_LED_R),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&io_conf);
    gpio_set_level(PIN_LED_R, 0);

    /* Create LED blink task */
    xTaskCreate(led_blink_task, "led_blink", 2048, NULL, 5, &g_led_task_handle);
    ESP_LOGI(TAG, "LED initialized");
}

void led_set_state(led_state_t state) {
    g_led_state = state;
    ESP_LOGI(TAG, "LED state changed to %d", state);
}

led_state_t led_get_state(void) {
    return g_led_state;
}
