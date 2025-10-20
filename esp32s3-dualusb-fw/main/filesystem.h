/*
 * @file filesystem.h
 * @brief Internal FATFS filesystem interface
 *
 * Author: A.R. Ansari <ansarirahim1@gmail.com>
 * Date: 2025-10-20
 */

#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stdbool.h>
#include <stdint.h>

/* Mount point for internal FATFS volume */
#define MOUNT_POINT "/storage"

/**
 * @brief Initialize internal FATFS volume
 *
 * Mounts SPI flash at /storage with automatic format on first boot.
 * Creates README.txt on first boot.
 *
 * @return true if successful, false otherwise
 */
bool fs_init_internal(void);

/**
 * @brief Check if file exists
 *
 * @param path File path (e.g., "/storage/test.txt")
 * @return true if exists, false otherwise
 */
bool fs_exists(const char *path);

/**
 * @brief Write test file to filesystem
 *
 * @return true if successful, false otherwise
 */
bool fs_write_test_file(void);

/**
 * @brief Get filesystem statistics
 *
 * @param total_bytes Pointer to store total size
 * @param free_bytes Pointer to store free size
 * @return true if successful, false otherwise
 */
bool fs_get_stats(uint64_t *total_bytes, uint64_t *free_bytes);

/**
 * @brief Unmount filesystem
 *
 * @return true if successful, false otherwise
 */
bool fs_unmount(void);

/**
 * @brief Remount filesystem
 *
 * @return true if successful, false otherwise
 */
bool fs_remount(void);

#endif /* FILESYSTEM_H */
