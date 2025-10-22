# ESP32-S3 Dual USB Firmware - Test Notes

## Milestone 1: Device Mode (MSC) Testing

### Test Environment
- **Board**: ESP32-S3 DevKitC-1
- **Host OS**: Windows 10/11 or Linux
- **USB Cable**: Standard USB-C cable
- **Test Files**: 5-20 MB test files

### Test Cases

#### TC1: Enumeration on Windows
**Steps**:
1. Connect ESP32-S3 to Windows PC via USB-C
2. Open "This PC" / File Explorer
3. Observe device enumeration

**Expected Result**:
- New drive letter appears (e.g., E:, F:)
- Drive label: "ESP32-S3 MSC"
- Drive shows as removable media
- No error dialogs

**Evidence**:
- Screenshot of File Explorer showing the new drive
- Serial log showing "USB Device (MSC) initialized"

#### TC2: Enumeration on Linux
**Steps**:
1. Connect ESP32-S3 to Linux PC via USB-C
2. Run `lsblk` or `fdisk -l`
3. Mount the device if not auto-mounted

**Expected Result**:
- New block device appears (e.g., /dev/sdb1)
- Device mounts automatically or via `mount` command
- No kernel errors in `dmesg`

**Evidence**:
- Output of `lsblk` showing the new device
- Serial log showing successful enumeration

#### TC3: Copy Large File
**Steps**:
1. Create a 20 MB test file on host
2. Copy file to ESP32-S3 MSC drive
3. Observe LED behavior
4. Wait for copy to complete
5. Verify file appears on device

**Expected Result**:
- LED shows fast blink during copy (LED_STATE_BUSY)
- Copy completes without errors
- File appears on device with correct size
- No data corruption

**Evidence**:
- Serial log showing I/O activity
- File verification (size, checksum)
- LED behavior observation

#### TC4: Safe Eject
**Steps**:
1. Right-click drive in File Explorer (Windows) or use `umount` (Linux)
2. Select "Eject" / "Safely Remove Hardware"
3. Wait for eject to complete
4. Observe LED behavior

**Expected Result**:
- LED returns to idle state (slow blink)
- Device remains connected
- No errors in host OS
- Device can be re-accessed after eject

**Evidence**:
- Serial log showing eject command
- LED returning to idle state
- Successful re-access after eject

#### TC5: Power Cycle After Write
**Steps**:
1. Copy a test file to device
2. Safely eject device
3. Power-cycle ESP32-S3 (reset button or power off/on)
4. Reconnect to PC
5. Verify file still exists

**Expected Result**:
- File survives power cycle
- Filesystem remains clean and mountable
- No data corruption
- Device enumerates normally after reset

**Evidence**:
- File verification after power cycle
- Serial log showing clean mount
- No filesystem errors

#### TC6: Negative Test - Unplug During Write
**Steps**:
1. Start copying a large file to device
2. Unplug USB cable during copy
3. Wait 5 seconds
4. Reconnect device
5. Check filesystem integrity

**Expected Result**:
- LED shows error state (solid 3s then slow blink)
- Device can be reconnected
- Filesystem remains mountable
- No permanent corruption

**Evidence**:
- Serial log showing error state
- Filesystem check after reconnect
- Device re-enumeration

### Serial Log Expectations

#### Successful Boot
```
I (XXX) app: ESP32-S3 Dual USB FW boot
I (XXX) led: Initializing LED on GPIO 6
I (XXX) led: LED initialized
I (XXX) fs: Initializing internal FATFS at /storage
I (XXX) fs: FATFS mounted successfully at /storage
I (XXX) fs: Created README.txt
I (XXX) usb_device: Initializing USB Device (MSC)
I (XXX) usb_device: USB Device (MSC) initialized
I (XXX) app: ESP32-S3 Dual USB FW ready - Device Mode (MSC)
```

#### File Copy Activity
```
I (XXX) usb_device: MSC capacity: XXXXX blocks of 512 bytes
I (XXX) led: LED state changed to 1  # LED_STATE_BUSY
I (XXX) led: LED state changed to 0  # LED_STATE_IDLE
```

#### Safe Eject
```
I (XXX) usb_device: MSC start_stop: power=0, start=0, eject=1
I (XXX) led: LED state changed to 0  # LED_STATE_IDLE
```

### Known Issues / Workarounds

1. **Windows Write Cache**: Windows may cache writes. Use "Safely Remove Hardware" to ensure all data is flushed.
   - Mitigation: CONFIG_FATFS_IMMEDIATE_FSYNC=y + f_sync() after writes

2. **Concurrent Access**: Do not access filesystem from both MSC and internal tasks simultaneously.
   - Mitigation: Mutex-protected filesystem access

3. **Large Files**: Files > 100 MB may take time to copy. Be patient and don't unplug.
   - Mitigation: Use progress indicator on host

### Performance Notes

- **Read Speed**: ~1-2 MB/s (SPI flash limited)
- **Write Speed**: ~500 KB/s (SPI flash + FATFS overhead)
- **Latency**: ~10-50 ms per operation

### Checklist for Release

- [ ] TC1 passed (Windows enumeration)
- [ ] TC2 passed (Linux enumeration)
- [ ] TC3 passed (Large file copy)
- [ ] TC4 passed (Safe eject)
- [ ] TC5 passed (Power cycle)
- [ ] TC6 passed (Negative test)
- [ ] Serial logs captured
- [ ] Screenshots/video evidence collected
- [ ] README updated with results
- [ ] Tag v0.1.0-m1-device created

