# How to Get esp_tinyusb.h - Complete Installation Guide

## Problem
```
fatal error: esp_tinyusb.h: No such file or directory
```

The `esp_tinyusb` component is not installed in your project.

---

## Solution: 3 Methods to Install esp_tinyusb

### Method 1: Using idf.py add-dependency (RECOMMENDED)

**On Linux/macOS:**
```bash
cd /path/to/esp32s3-dualusb-fw
idf.py add-dependency espressif/esp_tinyusb
```

**On Windows (PowerShell):**
```powershell
cd C:\path\to\esp32s3-dualusb-fw
python -m idf add-dependency espressif/esp_tinyusb
```

**On Windows (Command Prompt):**
```cmd
cd C:\path\to\esp32s3-dualusb-fw
python -m idf add-dependency espressif/esp_tinyusb
```

---

### Method 2: Manual Component Installation

1. **Create managed_components directory:**
   ```bash
   mkdir -p managed_components
   ```

2. **Clone esp_tinyusb:**
   ```bash
   cd managed_components
   git clone https://github.com/espressif/esp_tinyusb.git espressif__esp_tinyusb
   cd ..
   ```

3. **Verify installation:**
   ```bash
   ls managed_components/espressif__esp_tinyusb/esp_tinyusb.h
   ```

---

### Method 3: Using idf_component.yml (ALREADY DONE)

The `idf_component.yml` file has been created in your project root with:

```yaml
version: "1.0"
description: "ESP32-S3 Dual USB Firmware with MSC (Mass Storage Class) support"

dependencies:
  espressif/esp_tinyusb:
    version: "^1.4.0"
    git: "https://github.com/espressif/esp_tinyusb.git"
```

**To use this:**
```bash
# Run from project root
idf.py reconfigure
idf.py build
```

---

## Step-by-Step Instructions for Michael

### On Linux/macOS:

```bash
# 1. Navigate to project
cd /mnt/d/github/esp32s3-dualusb-fw/esp32s3-dualusb-fw

# 2. Add dependency
idf.py add-dependency espressif/esp_tinyusb

# 3. Reconfigure
idf.py reconfigure

# 4. Clean build
idf.py fullclean
idf.py build

# 5. Flash
idf.py -p /dev/ttyUSB0 flash
```

### On Windows (PowerShell):

```powershell
# 1. Navigate to project
cd 'C:\path\to\esp32s3-dualusb-fw'

# 2. Add dependency (using Python)
python -m idf add-dependency espressif/esp_tinyusb

# 3. Reconfigure
idf.py reconfigure

# 4. Clean build
idf.py fullclean
idf.py build

# 5. Flash
idf.py -p COM3 flash
```

### On Windows (Command Prompt):

```cmd
# 1. Navigate to project
cd C:\path\to\esp32s3-dualusb-fw

# 2. Add dependency
python -m idf add-dependency espressif/esp_tinyusb

# 3. Reconfigure
idf.py reconfigure

# 4. Clean build
idf.py fullclean
idf.py build

# 5. Flash
idf.py -p COM3 flash
```

---

## What Each Step Does

| Step | Command | Purpose |
|------|---------|---------|
| 1 | `idf.py add-dependency` | Downloads and registers esp_tinyusb component |
| 2 | `idf.py reconfigure` | Updates CMake configuration with new component |
| 3 | `idf.py fullclean` | Removes old build artifacts |
| 4 | `idf.py build` | Compiles with esp_tinyusb available |
| 5 | `idf.py flash` | Flashes firmware to device |

---

## Verification

After installation, verify the component is available:

```bash
# Check if managed_components directory exists
ls managed_components/

# Check if esp_tinyusb is there
ls managed_components/espressif__esp_tinyusb/

# Check if header file exists
ls managed_components/espressif__esp_tinyusb/esp_tinyusb/esp_tinyusb.h
```

---

## Troubleshooting

### Issue: "idf.py: command not found"
**Solution**: Make sure ESP-IDF is properly installed and activated:
```bash
source ~/esp/esp-idf/export.sh  # On Linux/macOS
# or
%USERPROFILE%\esp\esp-idf\export.bat  # On Windows
```

### Issue: "Component not found"
**Solution**: Try manual installation (Method 2) or check internet connection

### Issue: "Permission denied"
**Solution**: Use `sudo` on Linux/macOS or run PowerShell as Administrator on Windows

### Issue: Build still fails after installation
**Solution**: 
```bash
# Clean everything and rebuild
idf.py fullclean
rm -rf build managed_components dependencies.lock
idf.py add-dependency espressif/esp_tinyusb
idf.py build
```

---

## Files Created/Modified

✅ **idf_component.yml** - Component dependency configuration
✅ **main/CMakeLists.txt** - Already has `REQUIRES esp_tinyusb`
✅ **main/usb_device.c** - Already includes `esp_tinyusb.h`

---

## Next Steps

1. Run the appropriate command for your OS (see Step-by-Step Instructions)
2. Wait for component to download (first time only)
3. Build should now succeed
4. Flash to device

---

## Support

If you encounter issues:
1. Check that ESP-IDF is properly installed
2. Verify internet connection (component needs to download)
3. Try the manual installation method (Method 2)
4. Check the build logs for specific errors

---

**Status**: ✅ Ready to install
**Component**: espressif/esp_tinyusb
**Version**: ^1.4.0
**Repository**: https://github.com/espressif/esp_tinyusb

