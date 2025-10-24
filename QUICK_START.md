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

# 3. (Optional) Pre-pull Docker image to speed up first build
docker pull espressif/idf:v5.5.1

# 4. Build firmware (first time: ~10 min, next times: ~2 min)
.\build.ps1 build

# 5. Flash to device (connect ESP32-S3 first)
.\build.ps1 flash

# Done! ✅
```

### Windows Users (Command Prompt - No PowerShell Policy Issues)
```cmd
REM Same as above, but use:
build.bat build
build.bat flash
```

### Windows Users (PowerShell with Execution Policy Fix)
```powershell
# If you get "running scripts is disabled" error, use:
powershell -ExecutionPolicy Bypass -File .\build.ps1 build
powershell -ExecutionPolicy Bypass -File .\build.ps1 flash

# Or permanently allow for this session:
Set-ExecutionPolicy -Scope Process Bypass
.\build.ps1 build
```

### Linux/Mac Users
```bash
# 1. Install Docker (one-time)
# Ubuntu: sudo apt-get install docker.io docker-compose
# Mac: Download Docker Desktop

# 2. Clone repository
git clone https://github.com/ansarirahim/esp32s3-dualusb-fw-clean.git
cd esp32s3-dualusb-fw-clean

# 3. (Optional) Pre-pull Docker image to speed up first build
docker pull espressif/idf:v5.5.1

# 4. Build firmware
chmod +x build.sh
./build.sh build

# 5. Flash to device
./build.sh flash

# Done! ✅
```

### Linux/Mac Users (WSL2 Fallback - If Docker Desktop Won't Start)
```bash
# If Docker Desktop misbehaves on Windows, use WSL2:
wsl --install  # if needed
wsl -d Ubuntu
# Inside WSL Ubuntu terminal:
docker --version
./build.sh build
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

## Specifying Serial Port (If Auto-Detection Fails)

If you get "No serial ports found" error during flash, you can manually specify the port:

### Windows (PowerShell)
```powershell
# Find your COM port in Device Manager, then:
.\build.ps1 flash COM3
.\build.ps1 full COM3
```

### Windows (Command Prompt)
```cmd
REM Find your COM port in Device Manager, then:
build.bat flash COM3
build.bat full COM3
```

### Linux/Mac
```bash
# Find your port with: ls /dev/tty*
# Then use:
./build.sh flash /dev/ttyUSB0
./build.sh full /dev/ttyUSB0
```

---

## Common Issues

| Issue | Solution |
|-------|----------|
| "No serial ports found" during flash | Connect USB cable, or specify port manually: `build.bat flash COM3` (Windows) or `./build.sh flash /dev/ttyUSB0` (Linux/Mac) |
| "running scripts is disabled" (PowerShell) | Use `build.bat build` or `powershell -ExecutionPolicy Bypass -File .\build.ps1 build` |
| Docker not found | Install Docker Desktop from https://www.docker.com/products/docker-desktop |
| Docker Desktop won't start (Windows) | Use WSL2 instead: `wsl -d Ubuntu` then `./build.sh build` |
| Device not found | Connect USB cable, check Device Manager (Windows) or `ls /dev/tty*` (Linux/Mac) |
| Build fails | Ensure Docker is running, check internet connection |
| Permission denied (Linux) | `sudo usermod -aG docker $USER` then log out/in |
| First build is slow | This is normal (~10 min). Subsequent builds are ~2 min. Optional: pre-pull with `docker pull espressif/idf:v5.5.1` |

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

