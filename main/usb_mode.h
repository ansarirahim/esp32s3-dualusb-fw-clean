/**
 * @file usb_mode.h
 * @brief USB Mode Control Interface for Dual-Mode Operation
 *
 * @author A.R. Ansari <ansarirahim1@gmail.com>
 * @date 2025-10-22
 * @version 1.0.0
 *
 * @section description Description
 * Provides interface for controlling USB mode operation on ESP32-S3.
 * Supports Device Mode, Host Mode, and Dual Mode with automatic switching.
 *
 * @section features Features
 * - Multiple USB operation modes
 * - Automatic mode detection and switching
 * - Mode status monitoring
 * - LED priority management
 * - Safe resource cleanup during mode switches
 *
 * @section usage Usage
 * @code
 * // Initialize USB mode system
 * if (usb_mode_init()) {
 *     // Set to dual mode with automatic switching
 *     usb_mode_set(USB_MODE_DUAL_AUTO);
 *     
 *     // Check current mode
 *     usb_mode_t current = usb_mode_get();
 *     
 *     // Wait for mode to be ready
 *     usb_mode_wait_ready(5000);
 * }
 * @endcode
 */

#ifndef USB_MODE_H
#define USB_MODE_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup usb_mode USB Mode Control
 * @brief USB mode control and switching functionality
 * @{
 */

/**
 * @enum usb_mode_t
 * @brief USB operation modes
 */
typedef enum {
    USB_MODE_DEVICE_ONLY = 0,  /**< Device mode only (MSC) */
    USB_MODE_HOST_ONLY = 1,    /**< Host mode only (MSC) */
    USB_MODE_DUAL_AUTO = 2,    /**< Dual mode with automatic switching */
    USB_MODE_DUAL_MANUAL = 3,  /**< Dual mode with manual switching */
} usb_mode_t;

/**
 * @enum usb_mode_state_t
 * @brief USB mode operational states
 */
typedef enum {
    USB_MODE_STATE_IDLE = 0,        /**< Idle, no active mode */
    USB_MODE_STATE_DEVICE = 1,      /**< Device mode active */
    USB_MODE_STATE_HOST = 2,        /**< Host mode active */
    USB_MODE_STATE_SWITCHING = 3,   /**< Mode switching in progress */
    USB_MODE_STATE_ERROR = 4,       /**< Error state */
} usb_mode_state_t;

/**
 * @struct usb_mode_status_t
 * @brief USB mode status information
 */
typedef struct {
    usb_mode_t mode;                /**< Current mode setting */
    usb_mode_state_t state;         /**< Current operational state */
    bool device_connected;          /**< Device mode: host connected */
    bool host_connected;            /**< Host mode: external device connected */
    uint32_t mode_switch_count;     /**< Number of mode switches */
    uint32_t last_switch_time_ms;   /**< Time of last mode switch */
} usb_mode_status_t;

/**
 * @brief Initialize USB mode control system
 *
 * Initializes the USB mode control system and prepares for mode switching.
 * Must be called after USB Device and Host modes are initialized.
 *
 * @return true if initialization successful, false otherwise
 *
 * @note Must be called after usb_device_init() and usb_host_init()
 */
bool usb_mode_init(void);

/**
 * @brief Deinitialize USB mode control system
 *
 * Cleans up USB mode control resources.
 *
 * @return true if successful, false otherwise
 */
bool usb_mode_deinit(void);

/**
 * @brief Set USB operation mode
 *
 * Sets the USB operation mode and performs necessary mode switching.
 * If switching between modes, performs cleanup and initialization.
 *
 * @param[in] mode USB mode to set
 *
 * @return true if mode set successfully, false otherwise
 *
 * @note Mode switching may take up to 1 second
 * @see usb_mode_wait_ready()
 */
bool usb_mode_set(usb_mode_t mode);

/**
 * @brief Get current USB operation mode
 *
 * Returns the currently configured USB operation mode.
 *
 * @return Current USB mode
 */
usb_mode_t usb_mode_get(void);

/**
 * @brief Get current USB mode state
 *
 * Returns the current operational state of the USB mode system.
 *
 * @return Current operational state
 */
usb_mode_state_t usb_mode_get_state(void);

/**
 * @brief Check if mode switching is in progress
 *
 * Returns true if a mode switch is currently in progress.
 *
 * @return true if switching, false otherwise
 */
bool usb_mode_is_switching(void);

/**
 * @brief Wait for mode to be ready
 *
 * Blocks until the USB mode is ready or timeout expires.
 *
 * @param[in] timeout_ms Timeout in milliseconds (0 = no timeout)
 *
 * @return true if mode is ready, false if timeout
 */
bool usb_mode_wait_ready(uint32_t timeout_ms);

/**
 * @brief Check if device mode is active
 *
 * Returns true if device mode is currently active.
 *
 * @return true if device mode active, false otherwise
 */
bool usb_mode_is_device_active(void);

/**
 * @brief Check if host mode is active
 *
 * Returns true if host mode is currently active.
 *
 * @return true if host mode active, false otherwise
 */
bool usb_mode_is_host_active(void);

/**
 * @brief Get mode status information
 *
 * Retrieves detailed status information about the current mode.
 *
 * @param[out] status Pointer to status structure
 *
 * @return true if successful, false otherwise
 */
bool usb_mode_get_status(usb_mode_status_t *status);

/**
 * @brief Get mode status string
 *
 * Returns a human-readable string describing the current mode state.
 *
 * @return Status string (e.g., "Device Mode", "Host Mode", "Switching...")
 */
const char* usb_mode_get_status_string(void);

/**
 * @brief Notify device connection
 *
 * Called when a host connects to device mode.
 * Used for automatic mode switching.
 *
 * @note Called internally by USB Device Mode
 */
void usb_mode_notify_device_connected(void);

/**
 * @brief Notify device disconnection
 *
 * Called when a host disconnects from device mode.
 * Used for automatic mode switching.
 *
 * @note Called internally by USB Device Mode
 */
void usb_mode_notify_device_disconnected(void);

/**
 * @brief Notify host device connection
 *
 * Called when an external USB device connects in host mode.
 * Used for automatic mode switching.
 *
 * @note Called internally by USB Host Mode
 */
void usb_mode_notify_host_device_connected(void);

/**
 * @brief Notify host device disconnection
 *
 * Called when an external USB device disconnects in host mode.
 * Used for automatic mode switching.
 *
 * @note Called internally by USB Host Mode
 */
void usb_mode_notify_host_device_disconnected(void);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* USB_MODE_H */

