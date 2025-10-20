#include "esp_log.h"
static const char *TAG = "fs";

void fs_init_internal(void) {
    // TODO: mount/format internal FATFS
    ESP_LOGI(TAG, "Internal FATFS mounted");
}
