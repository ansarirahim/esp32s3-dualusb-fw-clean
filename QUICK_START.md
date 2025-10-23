# Quick Start - 5 Minutes to Firmware

## For the Impatient

### Windows Users (PowerShell)
```powershell
# 1. Install Docker Desktop (one-time)
# Download from: https://www.docker.com/products/docker-desktop
# Run installer, restart computer

# 2. Clone repository
git clone https://github.com/ansarirahim/esp32s3-dualusb-fw-clean.git
cd esp32s3-dualusb-fw-clean

# 3. Build firmware (first time: ~10 min, next times: ~2 min)
.\build.ps1 build

# 4. Flash to device (connect ESP32-S3 first)
.\build.ps1 flash

# Done! ✅
```

### Windows Users (Command Prompt)
```cmd
REM Same as above, but use:
build.bat build
build.bat flash
```

### Linux/Mac Users
```bash
# 1. Install Docker (one-time)
# Ubuntu: sudo apt-get install docker.io docker-compose
# Mac: Download Docker Desktop

# 2. Clone repository
git clone https://github.com/ansarirahim/esp32s3-dualusb-fw-clean.git
cd esp32s3-dualusb-fw-clean

# 3. Build firmware
chmod +x build.sh
./build.sh build

# 4. Flash to device
./build.sh flash

# Done! ✅
```

---

## What Happens

### First Build (10 minutes)
1. Downloads Docker image (~4GB)
2. Builds Docker container
3. Compiles firmware
4. Generates binary files

### Next Builds (2 minutes)
1. Compiles firmware
2. Generates binary files

### Flashing (1 minute)
1. Erases device
2. Flashes bootloader
3. Flashes firmware
4. Device reboots

---

## Verify Success

### After Build
You should see:
```
[OK] Build successful
```

Files created:
```
build/esp32s3_dualusb_fw.bin
build/bootloader/bootloader.bin
build/partition_table/partition-table.bin
```

### After Flash
You should see:
```
[OK] Flash successful
```

Device should reboot and show output on serial monitor.

---

## Common Issues

| Issue | Solution |
|-------|----------|
| Docker not found | Install Docker Desktop |
| Device not found | Connect USB cable, check Device Manager |
| Build fails | Ensure Docker is running, check internet |
| Permission denied (Linux) | `sudo usermod -aG docker $USER` then log out/in |

---

## Available Commands

```powershell
# Windows PowerShell
.\build.ps1 build      # Build only
.\build.ps1 flash      # Flash only
.\build.ps1 full       # Build + flash
.\build.ps1 monitor    # Serial monitor
.\build.ps1 clean      # Clean build
.\build.ps1 help       # Show help
```

```bash
# Linux/Mac
./build.sh build       # Build only
./build.sh flash       # Flash only
./build.sh full        # Build + flash
./build.sh monitor     # Serial monitor
./build.sh clean       # Clean build
./build.sh help        # Show help
```

---

## That's All!

No need to:
- Install ESP-IDF
- Install Python
- Install compiler
- Learn Docker
- Configure anything

Just run the script and it works! 🚀

---

For detailed information, see **SETUP_GUIDE.md**

