# ESP32-S3 Dual USB Firmware

**Author**: A.R. Ansari <ansarirahim1@gmail.com>
**Version**: 1.0.0
**Date**: 2025-10-22

Complete firmware for ESP32-S3 supporting:
- **USB Device Mode (MSC)**: Expose internal FATFS as USB drive
- **USB Host Mode (MSC)**: Read/write files from external USB drives
- **Dual-Mode Operation**: Automatic switching between modes

## Features

- **USB Device Mode (MSC)**: Enumerates as a USB Mass Storage Device on PC
- **Internal FATFS**: SPI flash-based filesystem with automatic formatting
- **LED Status Indicators**:
  - Slow blink (500ms ON / 1500ms OFF): Device idle
  - Fast blink (200ms ON / 200ms OFF): Device active I/O
  - Solid 3s then slow blink: Error state
- **Safe Eject**: Proper handling of host-side eject commands
- **Robust Error Handling**: Mutex-protected filesystem access, write synchronization

## Prerequisites

### Windows
- **ESP-IDF 5.5.1** or later
- **Python 3.8+**
- **Git**
- **USB-C cable**

### Linux / WSL
- **ESP-IDF 5.5.1** or later
- **Python 3.8+**
- **Git**
- **Build tools**: `build-essential`, `cmake`, `ninja-build`
- **USB-C cable**

### Docker (Recommended for Consistency)
- **Docker** installed
- **Docker Compose** (optional)
- **USB-C cable**

## Quick Start

### Option 1: Use Build Scripts (RECOMMENDED)

**Windows** (requires Docker Desktop installed):
```powershell
# Clone the repository
git clone https://github.com/ansarirahim/esp32s3-dualusb-fw-clean.git
cd esp32s3-dualusb-fw-clean

# Run as Administrator (important!)
# Right-click PowerShell → Run as administrator, then:

.\build.bat build      # Build firmware
.\build.bat flash      # Flash to device
.\build.bat monitor    # Monitor serial output
.\build.bat full       # Build and flash
```

**Linux/Mac**:
```bash
# Clone the repository
git clone https://github.com/ansarirahim/esp32s3-dualusb-fw-clean.git
cd esp32s3-dualusb-fw-clean

bash build.sh build    # Build firmware
bash build.sh flash    # Flash to device
bash build.sh monitor  # Monitor serial output
bash build.sh full     # Build and flash
```

### Option 2: Docker Build (Manual)

```bash
# Build using Docker
docker compose build --no-cache
docker compose run --rm esp32 idf.py build

# Flash (requires USB passthrough)
docker compose run --rm esp32 idf.py -p /dev/ttyUSB0 flash

# Monitor
docker compose run --rm esp32 idf.py monitor
```

### Option 3: Native Build (Windows/Linux/WSL)

```bash
# Clone the repository
git clone https://github.com/ansarirahim/esp32s3-dualusb-fw-clean.git
cd esp32s3-dualusb-fw-clean

# Set target and build
idf.py set-target esp32s3
idf.py build

# Flash to device
idf.py -p <COM_PORT> flash

# Monitor output
idf.py monitor
```

Replace `<COM_PORT>` with:
- Windows: `COM3`, `COM4`, etc.
- Linux/WSL: `/dev/ttyUSB0`, `/dev/ttyACM0`, etc.

### Configuration

The `sdkconfig.defaults` file contains pre-configured settings:
- FreeRTOS tick rate (1000 Hz)
- FATFS with long filename support
- TinyUSB MSC enabled
- Immediate FSYNC for data safety

To customize:
```bash
idf.py menuconfig
```

Expected output:
```
I (XXX) app: ESP32-S3 Dual USB FW boot
I (XXX) led: Initializing LED on GPIO 6
I (XXX) fs: Initializing internal FATFS at /storage
I (XXX) usb_device: Initializing USB Device (MSC)
I (XXX) app: ESP32-S3 Dual USB FW ready - Device Mode (MSC)
```

## Usage

### On Windows

1. Connect ESP32-S3 to PC via USB-C cable
2. Open File Explorer → "This PC"
3. New drive should appear (e.g., "ESP32-S3 MSC")
4. Copy files to/from the drive as normal
5. Right-click → "Eject" before disconnecting

### On Linux

1. Connect ESP32-S3 to PC via USB-C cable
2. Device should auto-mount or use:
   ```bash
   lsblk  # Find device (e.g., /dev/sdb1)
   mount /dev/sdb1 /mnt/esp32
   ```
3. Copy files as normal
4. Unmount before disconnecting:
   ```bash
   umount /mnt/esp32
   ```

## Hardware Configuration

See [docs/wiring.md](docs/wiring.md) for detailed pin assignments:

| Function | GPIO | Notes |
|----------|------|-------|
| USB D+ | 20 | Native PHY |
| USB D- | 19 | Native PHY |
| LED Red | 6 | Status indicator |
| BOOT1 | 0 | Mode select (future) |

## Testing

See [docs/test-notes.md](docs/test-notes.md) for comprehensive test cases and procedures.

### Quick Test

1. **Enumeration**: Device appears in File Explorer / `lsblk`
2. **Copy File**: Copy a 20 MB file to device (LED should fast-blink)
3. **Eject**: Safely eject device (LED returns to slow blink)
4. **Power Cycle**: Reset device and verify file still exists

## Known Issues

1. **Windows Write Cache**: Use "Safely Remove Hardware" to ensure data is flushed
2. **Concurrent Access**: Do not access filesystem from both MSC and internal tasks
3. **Large Files**: Files > 100 MB may take time; be patient

## Performance

- **Read Speed**: ~1-2 MB/s (SPI flash limited)
- **Write Speed**: ~500 KB/s (SPI flash + FATFS overhead)
- **Latency**: ~10-50 ms per operation

## Project Structure

```
esp32s3-dualusb-fw/
├── main/
│   ├── main.c              # Application entry point
│   ├── usb_device.c/.h     # USB Device (MSC) implementation
│   ├── filesystem.c/.h     # FATFS mount and file operations
│   ├── led_control.c/.h    # LED blink patterns
│   ├── board_pins.h        # Hardware pin definitions
│   └── CMakeLists.txt
├── docs/
│   ├── wiring.md           # Hardware wiring guide
│   └── test-notes.md       # Test procedures and results
├── CMakeLists.txt
├── sdkconfig.defaults      # Default SDK configuration
└── README.md               # This file
```

## Troubleshooting

### Build Script Issues

#### "docker: The term 'docker' is not recognized" (Windows)
**Cause**: Docker not in PATH or not running
**Solution**:
1. Make sure Docker Desktop is installed and running
2. Run PowerShell as Administrator
3. Use `.\build-admin.bat` instead of `.\build.bat`

#### "error during connect: docker client must be run with elevated privileges" (Windows)
**Cause**: Docker requires administrator privileges
**Solution**: Run PowerShell as Administrator before running build commands

#### "docker-compose.yml not found"
**Cause**: Not in the project directory
**Solution**: Make sure you're in the project directory: `cd esp32s3-dualusb-fw-clean`

### "Invalid image block, can't boot" Error

**Cause**: Binary flashed without bootloader and partition table

**Solution**: Use build scripts (recommended) or flash all 3 components:
```bash
# With build script
.\build.bat flash

# With Docker
docker compose run --rm esp32 idf.py flash

# With esptool.py
esptool.py -p COM3 write_flash @build/flash_args
```

**Flash Addresses (CRITICAL)**:
- Bootloader: `0x0` → `bootloader/bootloader.bin`
- Partition Table: `0x8000` → `partition_table/partition-table.bin`
- Application: `0x10000` → `esp32s3_dualusb_fw.bin`

### Device doesn't enumerate
- **Check USB cable**: Use data cable, not power-only
- **Check power**: Verify 5V on VBUS pin
- **Check GPIO20/GPIO19**: Ensure not used by other peripherals
- **Try different USB port**: Some ports have issues
- **Check serial logs**: Look for initialization errors

### LED doesn't blink
- **Verify GPIO48**: Check pin is available
- **Check LED polarity**: Anode to GPIO48, cathode to GND
- **Check resistor**: 220Ω typical value
- **Check power**: Verify 3.3V supply

### USB connection drops
- **Check USB cable quality**: Use shielded cable
- **Check power supply**: Ensure stable 5V
- **Check for interference**: Move away from RF sources
- **Try different USB port**: Some ports are unstable

### Slow file transfer
- **Normal for SPI flash**: ~500 KB/s write speed is expected
- **Larger files take time**: 1 MB file ≈ 2 seconds
- **Don't interrupt**: Let transfer complete
- **Check host system**: Close other applications

### Host mode not detecting external USB device
- **Check USB device**: Try different USB drive
- **Check USB cable**: Use quality cable
- **Check power**: External devices may need more power
- **Check device format**: FAT32 recommended

### Files lost after power cycle
- **Always safely eject**: Use "Safely Remove Hardware"
- **Check filesystem logs**: Look for errors
- **Verify power supply**: Ensure stable power during writes

## Flashing with Docker (Recommended)

Docker ensures consistent build environment and correct flashing:

```bash
# Build and flash
docker-compose up --build
docker-compose run --rm esp32 idf.py flash

# Monitor output
docker-compose run --rm esp32 idf.py monitor

# Run tests
docker-compose run --rm esp32 idf.py test
```

### Docker Build Issues

If you get authentication error: `no basic auth credentials`

**Solution**: Clean Docker cache and rebuild
```bash
# Clean Docker cache
docker system prune -a --volumes -f

# Build fresh (no cache)
docker-compose build --no-cache

# Run
docker-compose up
```

This removes cached images and rebuilds from official `espressif/idf:v5.5.1` image.

## Flashing with esptool.py

```bash
# Install esptool
pip install esptool

# Flash all components
cd build
esptool.py -p COM3 -b 460800 write_flash @flash_args

# Monitor
esptool.py -p COM3 monitor
```

Replace `COM3` with your actual COM port.

## Features Implemented

### Milestone 1: USB Device Mode (MSC)
- ✅ USB Device Mode with internal FATFS
- ✅ Block device with sector-level operations
- ✅ SCSI START/STOP UNIT handling
- ✅ I/O activity monitoring
- ✅ LED status indicators
- ✅ 40+ unit tests

### Milestone 2: USB Host Mode (MSC)
- ✅ USB Host Mode for external USB drives
- ✅ File read/write/list operations
- ✅ Device detection and monitoring
- ✅ State management
- ✅ Error handling and recovery
- ✅ 18+ unit tests

### Milestone 3: Dual-Mode Integration
- ✅ Dual-mode architecture with mode switching
- ✅ Automatic mode detection
- ✅ LED priority management
- ✅ 16+ integration tests
- ✅ Complete API reference

## Testing

Run unit tests:

```bash
# With Docker
docker-compose run --rm esp32 idf.py test

# With native build
cd test
cmake -B build
cmake --build build
./build/esp32s3_dualusb_fw_test
```

## References

- [ESP-IDF Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/)
- [TinyUSB Documentation](https://docs.tinyusb.org/)
- [FatFS Documentation](http://elm-chan.org/fsw/ff/)
- [ESP32-S3 Datasheet](https://www.espressif.com/sites/default/files/documentation/esp32-s3_datasheet_en.pdf)

## Support

For issues or questions:
- Email: ansarirahim1@gmail.com
- GitHub: https://github.com/ansarirahim
