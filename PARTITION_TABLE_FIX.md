# ESP32-S3 Partition Table & FATFS Mount Fix

## Problem
**Error:** `Failed to find FATFS partition (type='data', subtype='fat', partition_label='storage')`

**Root Cause:** Partition table mismatch or missing wear levelling configuration.

---

## Solution

### 1. Partition Table Configuration
**File:** `partitions.csv`

```csv
# Name,   Type, SubType, Offset,  Size, Flags
nvs,      data, nvs,     0x9000,  0x6000,
phy_init, data, phy,     0xf000,  0x1000,
factory,  app,  factory, 0x10000, 1M,
storage,  data, fat,     ,        1M,
```

**Key Points:**
- Partition name: `storage` (must match mount label in code)
- Type: `data`
- SubType: `fat`
- Size: 1MB (auto-calculated offset)

### 2. SDK Configuration
**File:** `sdkconfig.defaults`

```
# Partition Table Configuration
CONFIG_PARTITION_TABLE_CUSTOM=y
CONFIG_PARTITION_TABLE_CUSTOM_FILENAME="partitions.csv"

# Flash Size Configuration
CONFIG_ESPTOOLPY_FLASHSIZE_16MB=y

# Wear Levelling Configuration
CONFIG_WL_SECTOR_SIZE=4096

# FATFS Configuration
CONFIG_FATFS_LFN_STACK=1
CONFIG_FATFS_CODEPAGE_437=y
CONFIG_FATFS_FS_LOCK=4
CONFIG_FATFS_IMMEDIATE_FSYNC=y
```

**Key Points:**
- `CONFIG_WL_SECTOR_SIZE=4096` - Wear levelling sector size
- `CONFIG_FATFS_IMMEDIATE_FSYNC=y` - Sync writes immediately
- `CONFIG_ESPTOOLPY_FLASHSIZE_16MB=y` - Match actual flash size

### 3. Firmware Code
**File:** `main/filesystem.c`

```c
// Mount configuration with format_if_mount_failed
esp_vfs_fat_sdmmc_mount_config_t mount_config = {
    .format_if_mount_failed = true,  // Auto-format on first boot
    .max_files = 5,
    .allocation_unit_size = 4096,
};

// Mount with partition label "storage"
esp_err_t ret = esp_vfs_fat_spiflash_mount_rw_wl(
    MOUNT_POINT,
    "storage",  // Must match partition name in partitions.csv
    &mount_config,
    &g_wl_handle
);
```

**Key Points:**
- Partition label: `"storage"` (must match partitions.csv)
- `format_if_mount_failed=true` - Auto-format on first boot
- Uses wear levelling (`_rw_wl` suffix)

---

## Flash Instructions

### Option 1: Automated Script (Recommended)

**Linux/Mac:**
```bash
chmod +x flash_and_verify.sh
./flash_and_verify.sh /dev/ttyUSB0
```

**Windows:**
```cmd
flash_and_verify.bat COM3
```

### Option 2: Manual Steps

**Step 1: Build partition table**
```bash
idf.py partition-table
```

**Step 2: Flash partition table**
```bash
idf.py -p COM3 -b 460800 partition-table-flash
```

**Step 3: Flash application**
```bash
idf.py -p COM3 -b 460800 flash
```

**Step 4: Verify partition table**
```bash
parttool.py -p COM3 get_partition_info --partition-type=data --partition-subtype=fat --info=name,size,offset
```

---

## Verification

### Expected Output

**Serial Monitor:**
```
[fs] Initializing internal FATFS at /storage
[fs] FATFS mounted successfully at /storage
[fs] Created README.txt
```

**Partition Table Verification:**
```
Partition name: storage
Partition type: data
Partition subtype: fat
Partition offset: 0x110000
Partition size: 0x100000 (1MB)
```

### Troubleshooting

| Error | Cause | Solution |
|-------|-------|----------|
| `Failed to find FATFS partition` | Partition table not flashed | Run `idf.py partition-table-flash` |
| `Failed to mount FATFS` | Wear levelling not configured | Add `CONFIG_WL_SECTOR_SIZE=4096` |
| `Detected size larger than binary` | Flash size mismatch | Set `CONFIG_ESPTOOLPY_FLASHSIZE_16MB=y` |
| `Mount failed, formatting...` | First boot or corrupted FS | Normal - auto-formats on first boot |

---

## Technical Details

### Partition Table Layout (16MB Flash)

```
0x0000_0000 - 0x0000_8FFF: Bootloader (36 KB)
0x0000_9000 - 0x0000_EFFF: NVS (24 KB)
0x0000_F000 - 0x0000_FFFF: PHY Init (4 KB)
0x0001_0000 - 0x0010_FFFF: Factory App (1 MB)
0x0011_0000 - 0x011F_FFFF: Storage/FATFS (1 MB)
0x0120_0000 - 0xFFFF_FFFF: Free (14 MB)
```

### Wear Levelling

- **Purpose:** Distribute flash writes evenly to prevent wear
- **Sector Size:** 4096 bytes (CONFIG_WL_SECTOR_SIZE)
- **Overhead:** ~1 sector per partition
- **Benefit:** Extends flash lifespan from ~10K to ~100K+ write cycles

### FATFS Mount Options

| Option | Value | Purpose |
|--------|-------|---------|
| `format_if_mount_failed` | true | Auto-format on first boot or corruption |
| `max_files` | 5 | Maximum open files simultaneously |
| `allocation_unit_size` | 4096 | Cluster size (must match WL_SECTOR_SIZE) |

---

## References

- [ESP-IDF FATFS Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/api-reference/storage/fatfs.html)
- [Partition Table Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/api-reference/storage/partition_table.html)
- [Wear Levelling Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/api-reference/storage/wear_levelling.html)

---

## Status

✅ **Partition table:** Configured correctly  
✅ **SDK configuration:** Wear levelling enabled  
✅ **Firmware code:** Mount with format_if_mount_failed=true  
✅ **Flash scripts:** Provided for automated flashing  

**Ready for testing!**

