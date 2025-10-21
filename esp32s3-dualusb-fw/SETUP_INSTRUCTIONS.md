# Setup Instructions - ESP32-S3 Dual USB Firmware

## Quick Start (Recommended)

### On Windows
```bash
cd esp32s3-dualusb-fw
setup-dependencies.bat
```

### On Linux/macOS
```bash
cd esp32s3-dualusb-fw
chmod +x setup-dependencies.sh
./setup-dependencies.sh
```

---

## Manual Setup (If Script Doesn't Work)

### Step 0: Clean Everything (CRITICAL!)
```bash
cd esp32s3-dualusb-fw
rmdir /s /q build
rmdir /s /q managed_components
del dependencies.lock
```

### Step 1: Add Dependency (MUST BE BEFORE set-target!)
```bash
idf.py add-dependency espressif/esp_tinyusb
# Note: If it says "already exists", that's OK - just continue
```

### Step 2: Set Target (IMPORTANT!)
```bash
idf.py set-target esp32s3
```

### Step 3: Reconfigure
```bash
idf.py reconfigure
```

### Step 4: Build
```bash
idf.py build
```

### Step 5: Flash (Optional)
```bash
idf.py -p COM3 flash
```

---

## What These Commands Do

| Command | Purpose |
|---------|---------|
| `rmdir /s /q build` | Deletes corrupted build directory |
| `rmdir /s /q managed_components` | Deletes old component cache |
| `del dependencies.lock` | Deletes old dependency lock file |
| `idf.py add-dependency espressif/esp_tinyusb` | Downloads esp_tinyusb component and creates dependencies.lock (MUST BE FIRST!) |
| `idf.py set-target esp32s3` | Sets the build target to ESP32-S3 (CRITICAL!) |
| `idf.py reconfigure` | Updates CMake configuration with the new target and component |
| `idf.py build` | Compiles the firmware |
| `idf.py -p COM3 flash` | Flashes firmware to device (replace COM3 with your port) |

---

## Expected Result

After running the setup:

✅ `managed_components/` directory created  
✅ `dependencies.lock` file created  
✅ `esp_tinyusb` component installed  
✅ Build completes successfully  
✅ Binary: `build/esp32s3-dualusb-fw.bin`  

---

## Troubleshooting

### Issue: "idf.py: command not found"
**Solution**: Activate ESP-IDF environment
```bash
# Windows
C:\Espressif\esp-idf\export.bat

# Linux/macOS
source ~/esp/esp-idf/export.sh
```

### Issue: "Failed to add dependency"
**Solution**: Check internet connection and try again
```bash
idf.py add-dependency espressif/esp_tinyusb
```

### Issue: Build still fails
**Solution**: Complete clean rebuild
```bash
idf.py fullclean
rm -rf build managed_components dependencies.lock
idf.py add-dependency espressif/esp_tinyusb
idf.py build
```

---

## Next Steps

After successful build:

1. **Flash to device**:
   ```bash
   idf.py -p COM3 flash
   ```

2. **Monitor output**:
   ```bash
   idf.py -p COM3 monitor
   ```

3. **Connect USB**:
   - Connect ESP32-S3 to PC via USB-C
   - Device should enumerate as USB Mass Storage
   - LED should blink (idle state)

---

## Files Created During Setup

- `managed_components/` - Downloaded components
- `dependencies.lock` - Dependency lock file
- `build/` - Build artifacts
- `build/esp32s3-dualusb-fw.bin` - Firmware binary

---

## Support

If you encounter any issues:
1. Check the troubleshooting section above
2. Verify ESP-IDF v5.5.1 is installed
3. Ensure internet connection is available
4. Try the manual setup steps

