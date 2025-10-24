# ESP32-S3 Dual USB Firmware - Setup & Build Guide

## Quick Start (TL;DR)

### Windows Users
```bash
# Option 1: PowerShell (Recommended)
.\build.ps1 build

# Option 2: Command Prompt
build.bat build

# Option 3: Full build + flash
.\build.ps1 full
```

### Linux/Mac Users
```bash
chmod +x build.sh
./build.sh build

# Full build + flash
./build.sh full
```

---

## Prerequisites

### What You Need

1. **Docker Desktop** (Required)
   - Download: https://www.docker.com/products/docker-desktop
   - Windows: Install and run Docker Desktop
   - Linux: Install Docker and Docker Compose
   - Mac: Install Docker Desktop

2. **USB Cable** (For flashing)
   - USB-C or USB-A to USB-C cable
   - Must support data transfer (not just charging)

3. **ESP32-S3 Board**
   - Any ESP32-S3 development board
   - Supports both USB Device and Host modes

### What You DON'T Need

- ❌ ESP-IDF installed locally
- ❌ Python installed locally
- ❌ Xtensa compiler installed locally
- ❌ Any build tools installed locally
- ❌ Docker knowledge or experience

**Everything is containerized in Docker!**

---

## Installation Steps

### Step 1: Install Docker Desktop

**Windows:**
1. Download Docker Desktop from https://www.docker.com/products/docker-desktop
2. Run the installer
3. Follow the installation wizard
4. Restart your computer
5. Open PowerShell and verify: `docker --version`

**Linux:**
```bash
sudo apt-get update
sudo apt-get install docker.io docker-compose
sudo usermod -aG docker $USER
# Log out and log back in
```

**Mac:**
1. Download Docker Desktop from https://www.docker.com/products/docker-desktop
2. Open the .dmg file
3. Drag Docker to Applications
4. Open Docker from Applications
5. Verify: `docker --version`

### Step 2: Clone the Repository

```bash
git clone https://github.com/ansarirahim/esp32s3-dualusb-fw-clean.git
cd esp32s3-dualusb-fw-clean
```

### Step 2.5: (Optional) Pre-Pull Docker Image

This speeds up the first build by downloading the Docker image in advance:

```bash
# Pre-pull the official Espressif toolchain image
docker pull espressif/idf:v5.5.1
```

This is optional but recommended. The image is ~4GB and will be downloaded automatically on first build if you skip this step.

### Step 3: Connect ESP32-S3 Board

1. Connect ESP32-S3 board to your computer via USB cable
2. Verify connection:
   - **Windows**: Check Device Manager for COM port
   - **Linux**: Run `ls /dev/ttyUSB*` or `ls /dev/ttyACM*`
   - **Mac**: Run `ls /dev/tty.usbserial*`

---

## Building the Firmware

### Option 1: Build Only (Recommended First Time)

**Windows (PowerShell):**
```powershell
.\build.ps1 build
```

**Windows (Command Prompt):**
```cmd
build.bat build
```

**Linux/Mac:**
```bash
./build.sh build
```

**What happens:**
1. Docker image is built (first time only, ~5 minutes)
2. Firmware is compiled
3. Binary files are generated in `build/` directory

### Option 2: Build + Flash (One Command)

**Windows (PowerShell):**
```powershell
.\build.ps1 full
```

**Linux/Mac:**
```bash
./build.sh full
```

**What happens:**
1. Docker image is built
2. Firmware is compiled
3. Firmware is automatically flashed to ESP32-S3

### Option 3: Flash Only (After Build)

**Windows (PowerShell):**
```powershell
.\build.ps1 flash
```

**Linux/Mac:**
```bash
./build.sh flash
```

---

## Available Commands

| Command | Description |
|---------|-------------|
| `build` | Build firmware only |
| `flash` | Flash to connected device |
| `full` | Build and flash in one command |
| `monitor` | Monitor serial output |
| `clean` | Clean build artifacts |
| `erase` | Erase device flash memory |
| `test` | Run unit tests |
| `help` | Show help message |

---

## Troubleshooting

### PowerShell Execution Policy Error (Windows)
**Error:** "running scripts is disabled on this system"

**Solution 1 (Recommended - No system changes):**
```powershell
powershell -ExecutionPolicy Bypass -File .\build.ps1 build
powershell -ExecutionPolicy Bypass -File .\build.ps1 flash
```

**Solution 2 (Temporary - Current session only):**
```powershell
Set-ExecutionPolicy -Scope Process Bypass
.\build.ps1 build
```

**Solution 3 (Use Command Prompt instead):**
```cmd
build.bat build
build.bat flash
```

### Docker Not Found
**Error:** "Docker is not installed or not in PATH"

**Solution:**
1. Install Docker Desktop from https://www.docker.com/products/docker-desktop
2. Restart your computer
3. Try again

### Docker Desktop Won't Start (Windows)
**Error:** Docker Desktop fails to start or hangs

**Solution (Use WSL2 instead):**
```bash
# Install WSL2 if needed
wsl --install

# Open Ubuntu terminal
wsl -d Ubuntu

# Inside WSL Ubuntu, run:
docker --version
./build.sh build
./build.sh flash
```

WSL2 Docker works perfectly and is a great fallback if Docker Desktop has issues.

### Device Not Found During Flash
**Error:** "Could not find device"

**Solution:**
1. Check USB cable is connected
2. Verify device appears in Device Manager (Windows) or `ls /dev/tty*` (Linux/Mac)
3. Try different USB port
4. Restart Docker Desktop

### Build Fails with "Permission Denied"
**Error:** "Permission denied" on Linux

**Solution:**
```bash
sudo usermod -aG docker $USER
# Log out and log back in
```

### Out of Disk Space
**Error:** "No space left on device"

**Solution:**
```bash
# Clean up Docker images
docker system prune -a

# Or clean build artifacts
./build.ps1 clean  # Windows
./build.sh clean   # Linux/Mac
```

---

## Generated Files

After successful build, you'll find:

```
build/
├── esp32s3_dualusb_fw.bin      (Main firmware - 302 KB)
├── bootloader/
│   └── bootloader.bin           (Bootloader - 21 KB)
└── partition_table/
    └── partition-table.bin      (Partition table - 3 KB)
```

These files are ready for flashing to ESP32-S3.

---

## Next Steps

1. **Build the firmware**: `.\build.ps1 build`
2. **Flash to device**: `.\build.ps1 flash`
3. **Monitor output**: `.\build.ps1 monitor`
4. **Test functionality**: Connect USB and verify both Device and Host modes work

---

## Support

For issues or questions:
1. Check the Troubleshooting section above
2. Review Docker Desktop logs
3. Ensure Docker is running and updated

---

**Last Updated:** October 23, 2025
**Firmware Version:** v1.0
**Status:** Production Ready ✅

