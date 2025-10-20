#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static const char *TAG = "app";

void app_main(void) {
    ESP_LOGI(TAG, "ESP32-S3 Dual USB FW boot");
    // TODO: read BOOT1 GPIO and init device or host mode accordingly
}
