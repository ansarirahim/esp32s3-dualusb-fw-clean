/**
 * @file filesystem.h
 * @brief Internal FATFS Filesystem Interface
 *
 * @author A.R. Ansari <ansarirahim1@gmail.com>
 * @date 2025-10-21
 * @version 1.0.0
 *
 * @section description Description
 * Provides interface for internal FATFS filesystem on SPI flash.
 * Manages filesystem initialization, mounting, and basic file operations.
 *
 * @section features Features
 * - SPI flash FATFS mount with automatic format on first boot
 * - Mutex-protected concurrent access
 * - Write synchronization for data safety
 * - README.txt creation on first boot
 * - Filesystem statistics (total/free space)
 * - File existence checking
 * - Mount/unmount/remount operations
 *
 * @section usage Usage
 * @code
 * // Initialize filesystem
 * if (fs_init_internal()) {
 *     // Check if file exists
 *     if (fs_exists("/storage/test.txt")) {
 *         // File exists
 *     }
 *
 *     // Get filesystem statistics
 *     uint64_t total, free;
 *     if (fs_get_stats(&total, &free)) {
 *         printf("Total: %llu, Free: %llu\n", total, free);
 *     }
 * }
 * @endcode
 *
 * @section contact Contact
 * - Email: ansarirahim1@gmail.com
 * - Phone: +91 9024304883
 * - GitHub: https://github.com/ansarirahim
 *
 * @section license License
 * Copyright (c) 2025 A.R. Ansari. All rights reserved.
 */

#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stdbool.h>
#include <stdint.h>

/** @defgroup filesystem_config Filesystem Configuration
 * @{
 */
/** @brief Mount point for internal FATFS volume */
#define MOUNT_POINT "/storage"
/** @} */

/**
 * @brief Initialize Internal FATFS Volume
 *
 * Mounts SPI flash at /storage with automatic format on first boot.
 * Creates README.txt file on first boot to indicate successful initialization.
 * Sets up mutex for thread-safe concurrent access.
 *
 * @return true if initialization successful, false otherwise
 * @retval true Filesystem initialized and ready
 * @retval false Initialization failed (check logs for details)
 *
 * @details
 * - Automatically formats volume on first boot
 * - Creates README.txt with initialization timestamp
 * - Initializes synchronization primitives
 * - Mounts at /storage mount point
 *
 * @note Must be called before any filesystem operations
 * @note Should be called from app_main() during system startup
 *
 * @see fs_unmount()
 * @see fs_remount()
 */
bool fs_init_internal(void);

/**
 * @brief Check if File Exists
 *
 * Checks whether a file exists at the specified path in the filesystem.
 *
 * @param[in] path File path (e.g., "/storage/test.txt")
 *
 * @return true if file exists, false otherwise
 * @retval true File exists at specified path
 * @retval false File does not exist
 *
 * @note Thread-safe operation
 * @see fs_write_test_file()
 */
bool fs_exists(const char *path);

/**
 * @brief Write Test File to Filesystem
 *
 * Creates a test file in the filesystem to verify write operations.
 * Useful for testing filesystem functionality.
 *
 * @return true if successful, false otherwise
 * @retval true Test file written successfully
 * @retval false Write operation failed
 *
 * @note Thread-safe operation
 * @see fs_exists()
 */
bool fs_write_test_file(void);

/**
 * @brief Get Filesystem Statistics
 *
 * Retrieves total and free space information from the filesystem.
 *
 * @param[out] total_bytes Pointer to store total filesystem size in bytes
 * @param[out] free_bytes Pointer to store free space in bytes
 *
 * @return true if successful, false otherwise
 * @retval true Statistics retrieved successfully
 * @retval false Failed to retrieve statistics
 *
 * @note Thread-safe operation
 * @note Both pointers must be valid
 */
bool fs_get_stats(uint64_t *total_bytes, uint64_t *free_bytes);

/**
 * @brief Unmount Filesystem
 *
 * Safely unmounts the filesystem, flushing all pending writes.
 * Useful before power-down or system reset.
 *
 * @return true if successful, false otherwise
 * @retval true Filesystem unmounted successfully
 * @retval false Unmount operation failed
 *
 * @note Thread-safe operation
 * @see fs_remount()
 */
bool fs_unmount(void);

/**
 * @brief Remount Filesystem
 *
 * Remounts the filesystem after unmounting.
 * Useful for recovery or reinitialization.
 *
 * @return true if successful, false otherwise
 * @retval true Filesystem remounted successfully
 * @retval false Remount operation failed
 *
 * @note Thread-safe operation
 * @see fs_unmount()
 */
bool fs_remount(void);

#endif /* FILESYSTEM_H */
