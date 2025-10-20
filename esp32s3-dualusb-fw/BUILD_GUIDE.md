# Build Guide - ESP32-S3 Dual USB Firmware

**Author**: A.R. Ansari <ansarirahim1@gmail.com>  
**Date**: 2025-10-20  
**Project**: ESP32-S3 Dual USB Firmware - Milestone 1

---

## 📋 Build System Overview

### Build Tools & Framework
- **Build System**: CMake 3.16+
- **Framework**: ESP-IDF 5.0+
- **Compiler**: GCC (xtensa-esp32s3-elf)
- **Target**: ESP32-S3 (Xtensa dual-core processor)
- **Language**: C (C99 standard)

### Core Components
- **FreeRTOS**: Real-time operating system
- **TinyUSB**: USB stack (MSC - Mass Storage Class)
- **FATFS**: FAT filesystem
- **ESP-IDF Components**: USB Device, GPIO, SPI Flash

---

## 🔧 Prerequisites

### Required Software
1. **ESP-IDF 5.0 or later**
   - Download: https://github.com/espressif/esp-idf
   - Version tested: ESP-IDF 5.0+

2. **Python 3.7+**
   - Required for ESP-IDF tools

3. **Git**
   - For version control

4. **USB Driver** (Windows only)
   - CH340 or CP2102 driver for ESP32-S3 DevKitC-1

### Hardware
- **Board**: ESP32-S3 DevKitC-1 (N8R8 or N16R8)
- **USB Cable**: USB-C (for flashing and power)
- **PC**: Windows/Mac/Linux

---

## 📦 Installation Steps

### Step 1: Install ESP-IDF

**Windows**:
```bash
# Clone ESP-IDF repository
git clone https://github.com/espressif/esp-idf.git
cd esp-idf
git checkout v5.0

# Run installer
.\install.bat esp32s3

# Set up environment
.\export.bat
```

**macOS/Linux**:
```bash
# Clone ESP-IDF repository
git clone https://github.com/espressif/esp-idf.git
cd esp-idf
git checkout v5.0

# Run installer
./install.sh esp32s3

# Set up environment
source ./export.sh
```

### Step 2: Verify Installation

```bash
idf.py --version
# Should output: ESP-IDF v5.0 or later

idf.py info
# Should show ESP32-S3 target information
```

### Step 3: Clone Project Repository

```bash
git clone https://github.com/ansarirahim/esp32s3-dualusb-fw.git
cd esp32s3-dualusb-fw
```

---

## 🏗️ Build Configuration

### Project Structure
```
esp32s3-dualusb-fw/
├── CMakeLists.txt              # Root CMake configuration
├── sdkconfig.defaults          # Default SDK configuration
├── main/
│   ├── CMakeLists.txt          # Component CMake configuration
│   ├── main.c                  # Application entry point
│   ├── usb_device.c/.h         # USB Device (MSC) implementation
│   ├── usb_host.c              # USB Host (placeholder)
│   ├── filesystem.c/.h         # FATFS filesystem
│   ├── led_control.c/.h        # LED control
│   └── board_pins.h            # Hardware pin definitions
├── docs/
│   ├── wiring.md               # Hardware wiring guide
│   └── test-notes.md           # Test procedures
└── .github/workflows/          # CI/CD workflows
```

### SDK Configuration (sdkconfig.defaults)

**FreeRTOS**:
```
CONFIG_FREERTOS_HZ=1000         # 1000 Hz tick rate
```

**FATFS**:
```
CONFIG_FATFS_LFN_STACK=1        # Long filename support
CONFIG_FATFS_CODEPAGE_437=y     # Code page 437 (US/English)
CONFIG_FATFS_FS_LOCK=4          # File system lock count
CONFIG_FATFS_IMMEDIATE_FSYNC=y  # Immediate sync on write
```

**TinyUSB**:
```
CONFIG_TINYUSB_MSC_ENABLED=y    # Enable MSC (Mass Storage Class)
CONFIG_TINYUSB_DEBUG_LEVEL=0    # Debug level (0=off)
```

**USB Device**:
```
CONFIG_USB_DEVICE_STACK=y       # Enable USB device stack
```

**Logging**:
```
CONFIG_LOG_DEFAULT_LEVEL=3      # Info level logging
```

---

## 🔨 Build Commands

### Set Target
```bash
idf.py set-target esp32s3
```

### Configure Project
```bash
idf.py menuconfig
# Opens interactive configuration menu
# Default configuration is in sdkconfig.defaults
```

### Build Firmware
```bash
idf.py build
# Compiles all source files
# Output: build/esp32s3_dualusb_fw.bin
```

### Clean Build
```bash
idf.py fullclean
idf.py build
```

### Build with Verbose Output
```bash
idf.py build -v
# Shows detailed compilation information
```

---

## 📥 Flashing

### Flash to Device
```bash
idf.py -p <COM_PORT> flash
# Windows: COM3, COM4, etc.
# macOS: /dev/tty.usbserial-*
# Linux: /dev/ttyUSB0, /dev/ttyUSB1, etc.
```

### Flash and Monitor
```bash
idf.py -p <COM_PORT> flash monitor
# Flashes and opens serial monitor
```

### Erase Flash
```bash
idf.py -p <COM_PORT> erase-flash
# Erases entire flash memory
```

---

## 📊 Build Output

### Successful Build Output
```
Compiling C source files...
[100%] Built target esp32s3_dualusb_fw.elf
[100%] Built target esp32s3_dualusb_fw.bin

Project build complete. The binary you can use with esptool.py is:
  build/esp32s3_dualusb_fw.bin
```

### Build Artifacts
```
build/
├── esp32s3_dualusb_fw.elf      # ELF executable
├── esp32s3_dualusb_fw.bin      # Binary firmware
├── esp32s3_dualusb_fw.map      # Memory map
├── bootloader.bin              # Bootloader
├── partition_table.bin         # Partition table
└── ota_data_initial.bin        # OTA data
```

---

## 🔍 Compiler Information

### Compiler Toolchain
- **Compiler**: xtensa-esp32s3-elf-gcc
- **Version**: GCC 11.2.0 (or later)
- **Architecture**: Xtensa (32-bit)
- **Cores**: Dual-core (Pro + App)

### Compilation Flags
```
-std=c99                        # C99 standard
-Wall                           # All warnings
-Wextra                         # Extra warnings
-Werror=all                     # Treat warnings as errors
-O2                             # Optimization level 2
-ffunction-sections             # Function sections
-fdata-sections                 # Data sections
```

### Linker Flags
```
-Wl,--gc-sections               # Garbage collection
-Wl,--print-memory-usage        # Print memory usage
```

---

## 💾 Memory Layout

### Flash Memory
```
0x0000_0000 - 0x0000_7FFF       Bootloader (32 KB)
0x0000_8000 - 0x0000_FFFF       Partition table (32 KB)
0x0001_0000 - 0x00FF_FFFF       Application (16 MB)
0x0100_0000 - 0x01FF_FFFF       FATFS (16 MB)
```

### RAM Memory
```
0x4037_0000 - 0x4037_FFFF       IRAM (64 KB)
0x3FC8_0000 - 0x3FCE_FFFF       DRAM (448 KB)
```

### FATFS Storage
- **Location**: SPI Flash (partition: storage)
- **Size**: 16 MB
- **Filesystem**: FAT32
- **Mount Point**: /storage

---

## 🧪 Build Verification

### Check Build Size
```bash
idf.py size
# Shows memory usage breakdown
```

### Check Build Dependencies
```bash
idf.py size-components
# Shows component sizes
```

### Verify Binary
```bash
esptool.py image_info build/esp32s3_dualusb_fw.bin
# Shows binary information
```

---

## 🐛 Troubleshooting

### Issue: "idf.py: command not found"
**Solution**: Set up ESP-IDF environment
```bash
# Windows
cd esp-idf
.\export.bat

# macOS/Linux
cd esp-idf
source ./export.sh
```

### Issue: "Target not set"
**Solution**: Set target to esp32s3
```bash
idf.py set-target esp32s3
```

### Issue: "Build fails with compilation errors"
**Solution**: Clean and rebuild
```bash
idf.py fullclean
idf.py build
```

### Issue: "Flash fails - device not found"
**Solution**: Check USB connection and drivers
```bash
# List available ports
idf.py monitor -p <PORT>

# Windows: Check Device Manager for COM ports
# macOS/Linux: ls /dev/tty*
```

### Issue: "FATFS mount fails"
**Solution**: Erase flash and rebuild
```bash
idf.py -p <COM_PORT> erase-flash
idf.py -p <COM_PORT> flash
```

---

## 📈 Build Performance

### Build Time
- **Clean Build**: ~30-60 seconds
- **Incremental Build**: ~5-10 seconds
- **Full Build with Optimization**: ~60-90 seconds

### Binary Size
- **Bootloader**: ~32 KB
- **Application**: ~200-300 KB
- **Total Flash Used**: ~500 KB (excluding FATFS)

### Memory Usage
- **IRAM**: ~40 KB
- **DRAM**: ~100-150 KB
- **Available**: ~300 KB

---

## 🔄 CI/CD Build Pipeline

### GitHub Actions Workflow
**File**: `.github/workflows/build.yml`

**Triggers**:
- Push to master branch
- Push to develop branch
- Push to feat/* branches

**Build Steps**:
1. Checkout code
2. Set up ESP-IDF v5.0
3. Set target to esp32s3
4. Build firmware
5. Upload build artifacts

**Artifacts**:
- esp32s3_dualusb_fw.bin
- esp32s3_dualusb_fw.elf
- esp32s3_dualusb_fw.map

---

## 📝 Build Log Example

```
Executing action: all (aliases: build)
Running cmake in directory: /path/to/build
Defining config variables from /path/to/sdkconfig.defaults
Compiling C source files...
[  1%] Building C object esp-idf/main/CMakeFiles/__idf_main.dir/main.c.obj
[  2%] Building C object esp-idf/main/CMakeFiles/__idf_main.dir/usb_device.c.obj
[  3%] Building C object esp-idf/main/CMakeFiles/__idf_main.dir/filesystem.c.obj
[  4%] Building C object esp-idf/main/CMakeFiles/__idf_main.dir/led_control.c.obj
[ 50%] Linking CXX executable esp32s3_dualusb_fw.elf
[100%] Built target esp32s3_dualusb_fw.bin

Project build complete. The binary you can use with esptool.py is:
  build/esp32s3_dualusb_fw.bin
```

---

## ✅ Build Checklist

- [ ] ESP-IDF 5.0+ installed
- [ ] Python 3.7+ installed
- [ ] Git installed
- [ ] USB driver installed (Windows)
- [ ] Repository cloned
- [ ] Target set to esp32s3
- [ ] Build successful
- [ ] Binary generated
- [ ] Device connected
- [ ] Flash successful
- [ ] Serial monitor shows output

---

## 📞 Support

**Developer**: A.R. Ansari <ansarirahim1@gmail.com>  
**Date**: 2025-10-20

For build issues:
1. Check ESP-IDF installation
2. Review build output for errors
3. Check hardware connections
4. Verify USB drivers
5. Contact support

---

## 🔗 References

- **ESP-IDF Documentation**: https://docs.espressif.com/projects/esp-idf/
- **ESP32-S3 Datasheet**: https://www.espressif.com/sites/default/files/documentation/esp32-s3_datasheet_en.pdf
- **TinyUSB Documentation**: https://docs.tinyusb.org/
- **FATFS Documentation**: http://elm-chan.org/fsw/ff/

---

**Build Guide Complete**  
**Author**: A.R. Ansari <ansarirahim1@gmail.com>  
**Date**: 2025-10-20

