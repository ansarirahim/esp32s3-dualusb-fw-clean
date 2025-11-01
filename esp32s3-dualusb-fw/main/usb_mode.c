/**
 * @file usb_mode.c
 * @brief USB Mode Control Implementation for Dual-Mode Operation
 *
 * @author A.R. Ansari <ansarirahim1@gmail.com>
 * @date 2025-10-22
 * @version 1.0.0
 *
 * Implements USB mode control and switching for dual-mode operation.
 */

#include "usb_mode.h"
#include "usb_device.h"
#include "usb_host.h"
#include "led_control.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include <string.h>

static const char *TAG = "usb_mode";

/**
 * @defgroup usb_mode_internal USB Mode Internal
 * @brief Internal USB Mode implementation
 * @{
 */

/**
 * @struct usb_mode_context_t
 * @brief USB Mode context structure
 */
typedef struct {
    bool initialized;                   /**< Initialization flag */
    usb_mode_t mode;                   /**< Current mode setting */
    usb_mode_state_t state;            /**< Current operational state */
    bool device_connected;             /**< Device mode: host connected */
    bool host_connected;               /**< Host mode: external device connected */
    uint32_t mode_switch_count;        /**< Number of mode switches */
    uint32_t last_switch_time_ms;      /**< Time of last mode switch */
    SemaphoreHandle_t state_mutex;     /**< State protection mutex */
    SemaphoreHandle_t ready_semaphore; /**< Mode ready semaphore */
    TaskHandle_t mode_task;            /**< Mode control task handle */
} usb_mode_context_t;

/** Global USB Mode context */
static usb_mode_context_t g_usb_mode_ctx = {
    .initialized = false,
    .mode = USB_MODE_DEVICE_ONLY,
    .state = USB_MODE_STATE_IDLE,
    .device_connected = false,
    .host_connected = false,
    .mode_switch_count = 0,
    .last_switch_time_ms = 0,
    .state_mutex = NULL,
    .ready_semaphore = NULL,
    .mode_task = NULL,
};

/** @} */

/**
 * @brief Update LED based on mode state
 */
static void usb_mode_update_led(void) {
    if (g_usb_mode_ctx.state == USB_MODE_STATE_ERROR) {
        led_set_state(LED_STATE_ERROR);
    } else if (g_usb_mode_ctx.device_connected || g_usb_mode_ctx.host_connected) {
        led_set_state(LED_STATE_BUSY);
    } else {
        led_set_state(LED_STATE_IDLE);
    }
}

/**
 * @brief Mode control task
 */
static void usb_mode_task(void *arg) {
    ESP_LOGI(TAG, "USB Mode control task started");
    
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(500));
        
        if (xSemaphoreTake(g_usb_mode_ctx.state_mutex, pdMS_TO_TICKS(100))) {
            // Handle automatic mode switching
            if (g_usb_mode_ctx.mode == USB_MODE_DUAL_AUTO) {
                // Auto-switch logic would go here
                // For now, just maintain state
            }
            
            usb_mode_update_led();
            xSemaphoreGive(g_usb_mode_ctx.state_mutex);
        }
    }
}

bool usb_mode_init(void) {
    ESP_LOGI(TAG, "Initializing USB Mode control");
    
    if (g_usb_mode_ctx.initialized) {
        ESP_LOGW(TAG, "USB Mode already initialized");
        return true;
    }
    
    // Create state protection mutex
    g_usb_mode_ctx.state_mutex = xSemaphoreCreateMutex();
    if (!g_usb_mode_ctx.state_mutex) {
        ESP_LOGE(TAG, "Failed to create state mutex");
        return false;
    }
    
    // Create ready semaphore
    g_usb_mode_ctx.ready_semaphore = xSemaphoreCreateBinary();
    if (!g_usb_mode_ctx.ready_semaphore) {
        ESP_LOGE(TAG, "Failed to create ready semaphore");
        vSemaphoreDelete(g_usb_mode_ctx.state_mutex);
        return false;
    }
    
    // Initialize state
    if (xSemaphoreTake(g_usb_mode_ctx.state_mutex, pdMS_TO_TICKS(100))) {
        g_usb_mode_ctx.state = USB_MODE_STATE_DEVICE;
        g_usb_mode_ctx.mode = USB_MODE_DEVICE_ONLY;
        g_usb_mode_ctx.device_connected = false;
        g_usb_mode_ctx.host_connected = false;
        xSemaphoreGive(g_usb_mode_ctx.state_mutex);
    }
    
    // Create mode control task
    if (xTaskCreate(usb_mode_task, "usb_mode", 4096, NULL, 3, &g_usb_mode_ctx.mode_task) != pdPASS) {
        ESP_LOGE(TAG, "Failed to create USB Mode task");
        vSemaphoreDelete(g_usb_mode_ctx.state_mutex);
        vSemaphoreDelete(g_usb_mode_ctx.ready_semaphore);
        return false;
    }
    
    g_usb_mode_ctx.initialized = true;
    xSemaphoreGive(g_usb_mode_ctx.ready_semaphore);
    
    ESP_LOGI(TAG, "USB Mode control initialized");
    return true;
}

bool usb_mode_deinit(void) {
    ESP_LOGI(TAG, "Deinitializing USB Mode control");
    
    if (!g_usb_mode_ctx.initialized) {
        return true;
    }
    
    // Delete mode task
    if (g_usb_mode_ctx.mode_task) {
        vTaskDelete(g_usb_mode_ctx.mode_task);
        g_usb_mode_ctx.mode_task = NULL;
    }
    
    // Delete semaphores
    if (g_usb_mode_ctx.state_mutex) {
        vSemaphoreDelete(g_usb_mode_ctx.state_mutex);
        g_usb_mode_ctx.state_mutex = NULL;
    }
    
    if (g_usb_mode_ctx.ready_semaphore) {
        vSemaphoreDelete(g_usb_mode_ctx.ready_semaphore);
        g_usb_mode_ctx.ready_semaphore = NULL;
    }
    
    g_usb_mode_ctx.initialized = false;
    
    ESP_LOGI(TAG, "USB Mode control deinitialized");
    return true;
}

bool usb_mode_set(usb_mode_t mode) {
    ESP_LOGI(TAG, "Setting USB mode to %d", mode);
    
    if (xSemaphoreTake(g_usb_mode_ctx.state_mutex, pdMS_TO_TICKS(100))) {
        g_usb_mode_ctx.mode = mode;
        xSemaphoreGive(g_usb_mode_ctx.state_mutex);
    }
    
    return true;
}

usb_mode_t usb_mode_get(void) {
    usb_mode_t mode = USB_MODE_DEVICE_ONLY;
    
    if (xSemaphoreTake(g_usb_mode_ctx.state_mutex, pdMS_TO_TICKS(100))) {
        mode = g_usb_mode_ctx.mode;
        xSemaphoreGive(g_usb_mode_ctx.state_mutex);
    }
    
    return mode;
}

usb_mode_state_t usb_mode_get_state(void) {
    usb_mode_state_t state = USB_MODE_STATE_IDLE;
    
    if (xSemaphoreTake(g_usb_mode_ctx.state_mutex, pdMS_TO_TICKS(100))) {
        state = g_usb_mode_ctx.state;
        xSemaphoreGive(g_usb_mode_ctx.state_mutex);
    }
    
    return state;
}

bool usb_mode_is_switching(void) {
    bool switching = false;
    
    if (xSemaphoreTake(g_usb_mode_ctx.state_mutex, pdMS_TO_TICKS(100))) {
        switching = (g_usb_mode_ctx.state == USB_MODE_STATE_SWITCHING);
        xSemaphoreGive(g_usb_mode_ctx.state_mutex);
    }
    
    return switching;
}

bool usb_mode_wait_ready(uint32_t timeout_ms) {
    if (!g_usb_mode_ctx.ready_semaphore) {
        return false;
    }
    
    TickType_t ticks = (timeout_ms == 0) ? portMAX_DELAY : pdMS_TO_TICKS(timeout_ms);
    return xSemaphoreTake(g_usb_mode_ctx.ready_semaphore, ticks) == pdTRUE;
}

bool usb_mode_is_device_active(void) {
    bool active = false;
    
    if (xSemaphoreTake(g_usb_mode_ctx.state_mutex, pdMS_TO_TICKS(100))) {
        active = (g_usb_mode_ctx.state == USB_MODE_STATE_DEVICE);
        xSemaphoreGive(g_usb_mode_ctx.state_mutex);
    }
    
    return active;
}

bool usb_mode_is_host_active(void) {
    bool active = false;
    
    if (xSemaphoreTake(g_usb_mode_ctx.state_mutex, pdMS_TO_TICKS(100))) {
        active = (g_usb_mode_ctx.state == USB_MODE_STATE_HOST);
        xSemaphoreGive(g_usb_mode_ctx.state_mutex);
    }
    
    return active;
}

bool usb_mode_get_status(usb_mode_status_t *status) {
    if (!status) {
        return false;
    }
    
    if (xSemaphoreTake(g_usb_mode_ctx.state_mutex, pdMS_TO_TICKS(100))) {
        status->mode = g_usb_mode_ctx.mode;
        status->state = g_usb_mode_ctx.state;
        status->device_connected = g_usb_mode_ctx.device_connected;
        status->host_connected = g_usb_mode_ctx.host_connected;
        status->mode_switch_count = g_usb_mode_ctx.mode_switch_count;
        status->last_switch_time_ms = g_usb_mode_ctx.last_switch_time_ms;
        xSemaphoreGive(g_usb_mode_ctx.state_mutex);
        return true;
    }
    
    return false;
}

const char* usb_mode_get_status_string(void) {
    usb_mode_state_t state = usb_mode_get_state();
    
    switch (state) {
        case USB_MODE_STATE_IDLE:
            return "Idle";
        case USB_MODE_STATE_DEVICE:
            return "Device Mode";
        case USB_MODE_STATE_HOST:
            return "Host Mode";
        case USB_MODE_STATE_SWITCHING:
            return "Switching...";
        case USB_MODE_STATE_ERROR:
            return "Error";
        default:
            return "Unknown";
    }
}

void usb_mode_notify_device_connected(void) {
    if (xSemaphoreTake(g_usb_mode_ctx.state_mutex, pdMS_TO_TICKS(100))) {
        g_usb_mode_ctx.device_connected = true;
        usb_mode_update_led();
        xSemaphoreGive(g_usb_mode_ctx.state_mutex);
    }
}

void usb_mode_notify_device_disconnected(void) {
    if (xSemaphoreTake(g_usb_mode_ctx.state_mutex, pdMS_TO_TICKS(100))) {
        g_usb_mode_ctx.device_connected = false;
        usb_mode_update_led();
        xSemaphoreGive(g_usb_mode_ctx.state_mutex);
    }
}

void usb_mode_notify_host_device_connected(void) {
    if (xSemaphoreTake(g_usb_mode_ctx.state_mutex, pdMS_TO_TICKS(100))) {
        g_usb_mode_ctx.host_connected = true;
        usb_mode_update_led();
        xSemaphoreGive(g_usb_mode_ctx.state_mutex);
    }
}

void usb_mode_notify_host_device_disconnected(void) {
    if (xSemaphoreTake(g_usb_mode_ctx.state_mutex, pdMS_TO_TICKS(100))) {
        g_usb_mode_ctx.host_connected = false;
        usb_mode_update_led();
        xSemaphoreGive(g_usb_mode_ctx.state_mutex);
    }
}

