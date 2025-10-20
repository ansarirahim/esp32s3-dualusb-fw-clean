#include "filesystem.h"
#include "esp_log.h"
#include "esp_vfs_fat.h"
#include "wear_levelling.h"
#include "ff.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include <time.h>
#include <sys/stat.h>

static const char *TAG = "fs";

/* Filesystem mutex for thread-safe access */
static SemaphoreHandle_t g_fs_mutex = NULL;

/* Wear levelling handle */
static wl_handle_t g_wl_handle = WL_INVALID_HANDLE;

/* Mount state */
static bool g_fs_mounted = false;

/**
 * @brief Acquire filesystem lock
 */
static void fs_lock(void) {
    if (g_fs_mutex) {
        xSemaphoreTake(g_fs_mutex, portMAX_DELAY);
    }
}

/**
 * @brief Release filesystem lock
 */
static void fs_unlock(void) {
    if (g_fs_mutex) {
        xSemaphoreGive(g_fs_mutex);
    }
}

bool fs_init_internal(void) {
    ESP_LOGI(TAG, "Initializing internal FATFS at %s", MOUNT_POINT);

    /* Create mutex for FS access */
    if (!g_fs_mutex) {
        g_fs_mutex = xSemaphoreCreateMutex();
        if (!g_fs_mutex) {
            ESP_LOGE(TAG, "Failed to create FS mutex");
            return false;
        }
    }

    /* Mount FATFS with format_if_mount_failed */
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = true,
        .max_files = 5,
        .allocation_unit_size = 4096,
    };

    esp_err_t ret = esp_vfs_fat_spiflash_mount_rw_wl(
        MOUNT_POINT,
        "storage",
        &mount_config,
        &g_wl_handle
    );

    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to mount FATFS: %s", esp_err_to_name(ret));
        return false;
    }

    g_fs_mounted = true;
    ESP_LOGI(TAG, "FATFS mounted successfully at %s", MOUNT_POINT);

    /* Create README.txt on first boot */
    fs_lock();
    FILE *f = fopen(MOUNT_POINT "/README.txt", "r");
    if (!f) {
        /* File doesn't exist, create it */
        f = fopen(MOUNT_POINT "/README.txt", "w");
        if (f) {
            fprintf(f, "ESP32-S3 Dual USB Firmware\n");
            fprintf(f, "Device Mode: Mass Storage Device (MSC)\n");
            fprintf(f, "Internal FATFS Volume\n");
            fprintf(f, "\nThis volume is managed by the ESP32-S3 firmware.\n");
            fprintf(f, "Safe eject before power-off to prevent data loss.\n");
            fclose(f);
            ESP_LOGI(TAG, "Created README.txt");
        }
    } else {
        fclose(f);
    }
    fs_unlock();

    return true;
}

bool fs_exists(const char *path) {
    if (!g_fs_mounted || !path) {
        return false;
    }

    fs_lock();
    struct stat st;
    bool exists = (stat(path, &st) == 0);
    fs_unlock();

    return exists;
}

bool fs_write_test_file(void) {
    if (!g_fs_mounted) {
        ESP_LOGE(TAG, "Filesystem not mounted");
        return false;
    }

    fs_lock();

    FILE *f = fopen(MOUNT_POINT "/test_write.txt", "w");
    if (!f) {
        ESP_LOGE(TAG, "Failed to open test file for writing");
        fs_unlock();
        return false;
    }

    time_t now = time(NULL);
    fprintf(f, "Test write at %s", ctime(&now));
    fprintf(f, "Timestamp: %ld\n", now);

    int ret = fclose(f);
    fs_unlock();

    if (ret != 0) {
        ESP_LOGE(TAG, "Failed to close test file");
        return false;
    }

    ESP_LOGI(TAG, "Test file written successfully");
    return true;
}

bool fs_get_stats(uint64_t *total_bytes, uint64_t *free_bytes) {
    if (!g_fs_mounted || !total_bytes || !free_bytes) {
        return false;
    }

    fs_lock();

    FATFS *fs;
    DWORD fre_clust, fre_sect, tot_sect;

    FRESULT res = f_getfree(MOUNT_POINT, &fre_clust, &fs);
    if (res != FR_OK) {
        ESP_LOGE(TAG, "f_getfree failed: %d", res);
        fs_unlock();
        return false;
    }

    tot_sect = (fs->n_fatent - 2) * fs->csize;
    fre_sect = fre_clust * fs->csize;

    *total_bytes = tot_sect * 512;
    *free_bytes = fre_sect * 512;

    fs_unlock();

    ESP_LOGI(TAG, "FS stats: total=%llu bytes, free=%llu bytes", *total_bytes, *free_bytes);
    return true;
}

bool fs_unmount(void) {
    if (!g_fs_mounted) {
        return true;
    }

    fs_lock();

    esp_err_t ret = esp_vfs_fat_spiflash_unmount_rw_wl(MOUNT_POINT, g_wl_handle);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to unmount FATFS: %s", esp_err_to_name(ret));
        fs_unlock();
        return false;
    }

    g_fs_mounted = false;
    fs_unlock();

    ESP_LOGI(TAG, "FATFS unmounted");
    return true;
}

bool fs_remount(void) {
    if (g_fs_mounted) {
        return true;
    }

    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = false,
        .max_files = 5,
        .allocation_unit_size = 4096,
    };

    esp_err_t ret = esp_vfs_fat_spiflash_mount_rw_wl(
        MOUNT_POINT,
        "storage",
        &mount_config,
        &g_wl_handle
    );

    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to remount FATFS: %s", esp_err_to_name(ret));
        return false;
    }

    g_fs_mounted = true;
    ESP_LOGI(TAG, "FATFS remounted");
    return true;
}
