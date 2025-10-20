# Build Log Example - Complete Reference

**Author**: A.R. Ansari <ansarirahim1@gmail.com>  
**Date**: 2025-10-20  
**Project**: ESP32-S3 Dual USB Firmware

---

## 📋 Complete Build Log

### Build Command
```bash
idf.py build
```

### Full Build Output

```
Executing action: all (aliases: build)
Running cmake in directory: /path/to/esp32s3-dualusb-fw/build
Defining config variables from /path/to/esp32s3-dualusb-fw/sdkconfig.defaults
Executing "cmake.exe" with arguments:
  --no-warn-unused-cli
  -DCMAKE_TOOLCHAIN_FILE=/path/to/esp-idf/tools/cmake/toolchain-esp32s3.cmake
  -DCMAKE_BUILD_TYPE=Release
  -G Ninja
  -DPYTHON_DEPS_CHECKED=1
  -DESP_PLATFORM=1
  /path/to/esp32s3-dualusb-fw

-- Project is configured and ready to build
-- Building ESP32-S3 application with ESP-IDF v5.0

Compiling C source files...
[  1%] Building C object esp-idf/main/CMakeFiles/__idf_main.dir/main.c.obj
[  2%] Building C object esp-idf/main/CMakeFiles/__idf_main.dir/usb_device.c.obj
[  3%] Building C object esp-idf/main/CMakeFiles/__idf_main.dir/usb_host.c.obj
[  4%] Building C object esp-idf/main/CMakeFiles/__idf_main.dir/filesystem.c.obj
[  5%] Building C object esp-idf/main/CMakeFiles/__idf_main.dir/led_control.c.obj

Linking C executable esp32s3_dualusb_fw.elf
[100%] Built target esp32s3_dualusb_fw.elf

Generating binary image from built executable...
esptool.py v4.4
Wrote 0x2a000 (172032 bytes) at offset 0x10000 in 1.23 seconds

Project build complete. The binary you can use with esptool.py is:
  build/esp32s3_dualusb_fw.bin

Build artifacts:
  - build/esp32s3_dualusb_fw.bin (firmware binary)
  - build/esp32s3_dualusb_fw.elf (ELF executable)
  - build/esp32s3_dualusb_fw.map (memory map)
  - build/bootloader.bin (bootloader)
  - build/partition_table.bin (partition table)
```

---

## 📊 Memory Usage Output

### Build Size Report
```
Memory Usage:
IRAM:   40 KB (used) / 64 KB (total)
DRAM:   120 KB (used) / 448 KB (total)
Flash:  250 KB (used) / 16 MB (total)

Component Sizes:
  main:           45 KB
  esp_system:     30 KB
  freertos:       25 KB
  tinyusb:        35 KB
  fatfs:          20 KB
  esp_common:     15 KB
  hal:            20 KB
  soc:            15 KB
  others:         15 KB
```

---

## 🔍 Detailed Compilation Output

### Main Component Compilation
```
Compiling main.c:
  xtensa-esp32s3-elf-gcc -c main.c -o main.c.obj
  -std=c99 -Wall -Wextra -O2 -ffunction-sections -fdata-sections
  -mlongcalls -mtext-section-literals
  -I. -I/esp-idf/components/freertos/include
  -I/esp-idf/components/tinyusb/include
  -I/esp-idf/components/fatfs/include
  -DESP32S3 -DESP_PLATFORM

Compiling usb_device.c:
  xtensa-esp32s3-elf-gcc -c usb_device.c -o usb_device.c.obj
  [same flags as above]

Compiling filesystem.c:
  xtensa-esp32s3-elf-gcc -c filesystem.c -o filesystem.c.obj
  [same flags as above]

Compiling led_control.c:
  xtensa-esp32s3-elf-gcc -c led_control.c -o led_control.c.obj
  [same flags as above]
```

---

## 🔗 Linking Output

### Linker Command
```
Linking C executable esp32s3_dualusb_fw.elf:
  xtensa-esp32s3-elf-gcc -o esp32s3_dualusb_fw.elf
  main.c.obj
  usb_device.c.obj
  usb_host.c.obj
  filesystem.c.obj
  led_control.c.obj
  -L/esp-idf/components/freertos/lib
  -L/esp-idf/components/tinyusb/lib
  -L/esp-idf/components/fatfs/lib
  -lfreertos -ltinyusb -lfatfs -lesp_system -lesp_common
  -Wl,--gc-sections
  -Wl,--print-memory-usage
  -Wl,--Map=esp32s3_dualusb_fw.map
  -Tesp32s3.ld
```

### Linker Output
```
Memory region         Used Size  Region Size  %age Used
           IRAM:       40 KB        64 KB     62.50%
           DRAM:      120 KB       448 KB     26.79%
          Flash:      250 KB        16 MB      1.53%
```

---

## 📦 Binary Generation

### esptool.py Output
```
esptool.py v4.4
Generating binary image from built executable...
Wrote 0x2a000 (172032 bytes) at offset 0x10000 in 1.23 seconds

Binary image generated:
  - Bootloader: 0x0000 (32 KB)
  - Partition table: 0x8000 (32 KB)
  - Application: 0x10000 (172 KB)
  - FATFS: 0x100000 (16 MB)
```

---

## 📈 Build Artifacts

### Generated Files
```
build/
├── esp32s3_dualusb_fw.bin          # Main firmware binary (172 KB)
├── esp32s3_dualusb_fw.elf          # ELF executable (250 KB)
├── esp32s3_dualusb_fw.map          # Memory map (50 KB)
├── bootloader.bin                  # Bootloader (32 KB)
├── partition_table.bin             # Partition table (32 KB)
├── ota_data_initial.bin            # OTA data (8 KB)
├── CMakeFiles/                     # CMake generated files
└── esp-idf/                        # Component build files
    ├── main/
    │   ├── CMakeFiles/
    │   ├── main.c.obj
    │   ├── usb_device.c.obj
    │   ├── filesystem.c.obj
    │   └── led_control.c.obj
    ├── freertos/
    ├── tinyusb/
    ├── fatfs/
    └── [other components]
```

---

## 🔄 Incremental Build Log

### Second Build (After Small Change)
```
Executing action: all (aliases: build)
Running cmake in directory: /path/to/build
Defining config variables from /path/to/sdkconfig.defaults

[  1%] Building C object esp-idf/main/CMakeFiles/__idf_main.dir/main.c.obj
[ 50%] Linking C executable esp32s3_dualusb_fw.elf
[100%] Built target esp32s3_dualusb_fw.elf

Generating binary image from built executable...
esptool.py v4.4
Wrote 0x2a000 (172032 bytes) at offset 0x10000 in 0.85 seconds

Project build complete.
```

**Note**: Only main.c was recompiled (1% of build time)

---

## ⚠️ Build with Warnings

### Example Warning Output
```
main.c:45:12: warning: unused variable 'temp' [-Wunused-variable]
    int temp = 0;
        ^
usb_device.c:120:5: warning: implicit function declaration [-Wimplicit-function-declaration]
    usb_init();
    ^
```

**Note**: Warnings are treated as errors in this build system

---

## ❌ Build Failure Example

### Compilation Error
```
main.c:50:5: error: 'usb_device_init' undeclared (first use in this function)
    usb_device_init();
    ^
main.c:50:5: note: 'usb_device_init' is defined in header 'usb_device.h'; did you forget '#include "usb_device.h"'?
main.c:1:1:
 +#include "usb_device.h"
 #include <stdio.h>
main.c:50:5:
    usb_device_init();
    ^

compilation terminated.
```

**Solution**: Add missing include or check function name

---

## 🔧 Build with Verbose Output

### Command
```bash
idf.py build -v
```

### Output
```
Executing action: all (aliases: build)
Running cmake in directory: /path/to/build
Defining config variables from /path/to/sdkconfig.defaults
Executing "cmake.exe" with arguments:
  --no-warn-unused-cli
  -DCMAKE_TOOLCHAIN_FILE=/path/to/esp-idf/tools/cmake/toolchain-esp32s3.cmake
  -DCMAKE_BUILD_TYPE=Release
  -G Ninja
  -DPYTHON_DEPS_CHECKED=1
  -DESP_PLATFORM=1
  /path/to/esp32s3-dualusb-fw

-- Project is configured and ready to build
-- Building ESP32-S3 application with ESP-IDF v5.0

[Detailed compilation commands...]
```

---

## 📊 Build Statistics

### Build Time Breakdown
```
CMake configuration:    2 seconds
Compilation:           35 seconds
Linking:                5 seconds
Binary generation:      3 seconds
Total:                 45 seconds
```

### File Sizes
```
main.c:                 ~300 lines
usb_device.c:           ~400 lines
filesystem.c:           ~250 lines
led_control.c:          ~150 lines
Total source:           ~1,100 lines

Compiled binary:        ~172 KB
ELF executable:         ~250 KB
```

---

## ✅ Successful Build Indicators

1. **No errors** - All files compiled successfully
2. **No warnings** - Code quality verified
3. **Binary generated** - esp32s3_dualusb_fw.bin created
4. **Memory within limits** - IRAM, DRAM, Flash usage acceptable
5. **Build complete message** - "Project build complete"

---

## 🔍 Checking Build Output

### View Memory Map
```bash
cat build/esp32s3_dualusb_fw.map
```

### Check Binary Info
```bash
esptool.py image_info build/esp32s3_dualusb_fw.bin
```

### View Component Sizes
```bash
idf.py size-components
```

---

## 📞 Support

**Developer**: A.R. Ansari <ansarirahim1@gmail.com>  
**Date**: 2025-10-20

For build issues:
1. Review build log output
2. Check for compilation errors
3. Verify component dependencies
4. Check memory usage
5. Contact support

---

**Build Log Reference Complete**  
**Author**: A.R. Ansari <ansarirahim1@gmail.com>  
**Date**: 2025-10-20

