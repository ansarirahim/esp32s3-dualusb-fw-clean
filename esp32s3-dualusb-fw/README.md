# ESP32-S3 Dual USB Firmware

**Author**: A.R. Ansari <ansarirahim1@gmail.com>
**Date**: 2025-10-20
**Project**: ESP32-S3 Dual USB Firmware for Michael Steinmann

Firmware for ESP32-S3 supporting USB Mass Storage (Device) mode using ESP-IDF and TinyUSB.

**Milestone 1**: Device Mode (MSC) with internal FATFS volume.

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

- **ESP-IDF 5.x** installed and configured
- **ESP32-S3 DevKitC-1** board
- **USB-C cable** for connection
- **Python 3.7+** with esptool

## Quick Start

### 1. Set Target and Configure

```bash
cd esp32s3-dualusb-fw
idf.py set-target esp32s3
idf.py menuconfig
```

The `sdkconfig.defaults` file contains pre-configured settings for:
- FreeRTOS tick rate (1000 Hz)
- FATFS with long filename support
- TinyUSB MSC enabled
- Immediate FSYNC for data safety

### 2. Build

```bash
idf.py build
```

### 3. Flash

```bash
idf.py -p <COM_PORT> flash
```

Replace `<COM_PORT>` with your serial port (e.g., `COM3` on Windows, `/dev/ttyUSB0` on Linux).

### 4. Monitor

```bash
idf.py monitor
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

### Device doesn't enumerate
- Check USB cable connection
- Verify GPIO20/GPIO19 are not used by other peripherals
- Check serial logs for initialization errors
- Try different USB port on host

### LED doesn't blink
- Verify GPIO6 is available on your board
- Check LED polarity (anode to GPIO6, cathode to GND)
- Verify resistor value (220Ω typical)
- Check serial logs for LED initialization

### Files disappear after power cycle
- Ensure safe eject before power-off
- Check filesystem logs for errors
- Try reformatting the device (will erase all data)

### Slow copy speed
- This is normal for SPI flash (500 KB/s write)
- Larger files take proportionally longer
- Do not interrupt the copy process

## Future Milestones

- **M2**: USB Host Mode (MSC) + mode selection via BOOT1
- **M3**: File copy from Device to Host (calculator support)
- **M4**: RGB LED mapping, advanced error codes, telemetry

## License

[Add your license here]

## Support

For issues or questions, please refer to:
- [ESP-IDF Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/)
- [TinyUSB Documentation](https://docs.tinyusb.org/)
- [FatFS Documentation](http://elm-chan.org/fsw/ff/)
