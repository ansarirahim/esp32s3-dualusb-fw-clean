#include "esp_log.h"
static const char *TAG = "led";

void led_set_status(int state) {
    // TODO: drive RGB LED per state
    ESP_LOGI(TAG, "LED state=%d", state);
}
