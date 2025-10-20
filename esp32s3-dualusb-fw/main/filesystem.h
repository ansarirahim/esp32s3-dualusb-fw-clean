#pragma once

/**
 * @file filesystem.h
 * @brief Internal FATFS filesystem interface
 */

#include <stdbool.h>

#define MOUNT_POINT "/storage"

/**
 * @brief Initialize and mount internal FATFS
 * 
 * Mounts FATFS from SPI flash at /storage.
 * If mount fails, formats the partition and retries.
 * Creates README.txt on first boot.
 * 
 * @return true if successful, false otherwise
 */
bool fs_init_internal(void);

/**
 * @brief Check if a file exists
 * 
 * @param path File path (e.g., "/storage/test.txt")
 * @return true if file exists, false otherwise
 */
bool fs_exists(const char *path);

/**
 * @brief Write a test file to verify filesystem
 * 
 * Creates /storage/test_write.txt with timestamp.
 * 
 * @return true if successful, false otherwise
 */
bool fs_write_test_file(void);

/**
 * @brief Get filesystem statistics
 * 
 * @param total_bytes Pointer to store total size (bytes)
 * @param free_bytes Pointer to store free space (bytes)
 * @return true if successful, false otherwise
 */
bool fs_get_stats(uint64_t *total_bytes, uint64_t *free_bytes);

/**
 * @brief Unmount filesystem (for safe eject)
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

