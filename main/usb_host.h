/**
 * @file usb_host.h
 * @brief USB Host Mode (MSC) Interface
 *
 * @author A.R. Ansari <ansarirahim1@gmail.com>
 * @date 2025-10-22
 * @version 1.0.0
 *
 * @section description Description
 * Provides interface for USB Host Mode (MSC - Mass Storage Class) functionality.
 * Allows ESP32-S3 to act as a USB host and read/write files from external USB drives.
 *
 * @section features Features
 * - USB Host Mode (MSC) support
 * - External USB drive detection
 * - File read/write operations
 * - Mount/unmount handling
 * - Error handling and recovery
 * - LED status indicators for host mode
 * - Thread-safe operations
 *
 * @section usage Usage
 * @code
 * // Initialize USB Host Mode
 * if (usb_host_init()) {
 *     // Check if device is connected
 *     if (usb_host_is_device_connected()) {
 *         // Read file from external drive
 *         usb_host_read_file("/usb/test.txt", buffer, size);
 *     }
 * }
 * @endcode
 */

#ifndef USB_HOST_H
#define USB_HOST_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup usb_host_types USB Host Types
 * @brief Type definitions for USB Host Mode
 * @{
 */

/**
 * @enum usb_host_state_t
 * @brief USB Host State Enumeration
 *
 * Defines possible USB Host states.
 */
typedef enum {
    USB_HOST_STATE_IDLE,           /**< Host idle, no device connected */
    USB_HOST_STATE_DEVICE_ATTACHED, /**< Device attached, initializing */
    USB_HOST_STATE_DEVICE_READY,   /**< Device ready for file operations */
    USB_HOST_STATE_ERROR,          /**< Error state */
} usb_host_state_t;

/**
 * @struct usb_host_device_info_t
 * @brief USB Host Device Information
 *
 * Contains information about connected USB device.
 */
typedef struct {
    uint16_t vendor_id;            /**< USB Vendor ID */
    uint16_t product_id;           /**< USB Product ID */
    uint32_t total_sectors;        /**< Total sectors on device */
    uint32_t sector_size;          /**< Sector size in bytes */
    char manufacturer[64];         /**< Manufacturer name */
    char product[64];              /**< Product name */
    char serial[64];               /**< Serial number */
} usb_host_device_info_t;

/** @} */

/**
 * @defgroup usb_host_functions USB Host Functions
 * @brief USB Host Mode functions
 * @{
 */

/**
 * @brief Initialize USB Host Mode (MSC)
 *
 * Initializes the USB Host stack with MSC (Mass Storage Class) support.
 * Configures the USB peripheral for host mode and sets up device detection.
 *
 * @details
 * This function performs the following operations:
 * - Initializes USB Host stack
 * - Installs MSC driver
 * - Sets up device detection task
 * - Initializes internal state variables
 *
 * @return true if initialization successful, false otherwise
 * @retval true USB Host Mode initialized and ready
 * @retval false Initialization failed (check logs for details)
 *
 * @note Must be called after filesystem initialization
 * @note Should be called before usb_host_is_device_connected()
 *
 * @see usb_host_deinit()
 */
bool usb_host_init(void);

/**
 * @brief Deinitialize USB Host Mode
 *
 * Cleans up USB Host Mode resources and stops device detection.
 *
 * @return true if deinitialization successful, false otherwise
 *
 * @see usb_host_init()
 */
bool usb_host_deinit(void);

/**
 * @brief Check if USB device is connected
 *
 * Checks if a USB device is currently connected and ready for operations.
 *
 * @return true if device is connected and ready, false otherwise
 *
 * @see usb_host_get_device_info()
 */
bool usb_host_is_device_connected(void);

/**
 * @brief Get USB Host current state
 *
 * Returns the current state of the USB Host.
 *
 * @return Current USB Host state
 *
 * @see usb_host_state_t
 */
usb_host_state_t usb_host_get_state(void);

/**
 * @brief Get connected device information
 *
 * Retrieves information about the connected USB device.
 *
 * @param[out] info Pointer to device info structure to fill
 *
 * @return true if device info retrieved successfully, false otherwise
 *
 * @note Only valid if usb_host_is_device_connected() returns true
 *
 * @see usb_host_device_info_t
 */
bool usb_host_get_device_info(usb_host_device_info_t *info);

/**
 * @brief Read file from external USB drive
 *
 * Reads a file from the external USB drive.
 *
 * @param[in] path File path (e.g., "/usb/test.txt")
 * @param[out] buffer Buffer to store file contents
 * @param[in] max_size Maximum bytes to read
 *
 * @return Number of bytes read, or -1 on error
 *
 * @note Device must be connected (usb_host_is_device_connected() == true)
 * @note Buffer must be large enough to hold max_size bytes
 *
 * @see usb_host_write_file()
 */
int usb_host_read_file(const char *path, uint8_t *buffer, size_t max_size);

/**
 * @brief Write file to external USB drive
 *
 * Writes data to a file on the external USB drive.
 *
 * @param[in] path File path (e.g., "/usb/test.txt")
 * @param[in] buffer Buffer containing data to write
 * @param[in] size Number of bytes to write
 *
 * @return Number of bytes written, or -1 on error
 *
 * @note Device must be connected (usb_host_is_device_connected() == true)
 * @note File will be created if it doesn't exist
 *
 * @see usb_host_read_file()
 */
int usb_host_write_file(const char *path, const uint8_t *buffer, size_t size);

/**
 * @brief List files in directory on external USB drive
 *
 * Lists files in a directory on the external USB drive.
 *
 * @param[in] path Directory path (e.g., "/usb/")
 * @param[out] files Array of file names
 * @param[in] max_files Maximum number of files to list
 *
 * @return Number of files listed, or -1 on error
 *
 * @note Device must be connected (usb_host_is_device_connected() == true)
 *
 * @see usb_host_read_file()
 */
int usb_host_list_files(const char *path, char **files, int max_files);

/**
 * @brief Safely eject USB device
 *
 * Safely ejects the USB device by unmounting and stopping host mode.
 *
 * @return true if ejection successful, false otherwise
 *
 * @note After ejection, device can be physically removed
 *
 * @see usb_host_init()
 */
bool usb_host_eject_device(void);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* USB_HOST_H */

