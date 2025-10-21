@echo off
REM Setup script for ESP32-S3 Dual USB Firmware dependencies
REM This script installs the esp_tinyusb component and prepares the build

echo.
echo ╔════════════════════════════════════════════════════════════════╗
echo ║                                                                ║
echo ║     ESP32-S3 Dual USB Firmware - Dependency Setup             ║
echo ║                                                                ║
echo ╚════════════════════════════════════════════════════════════════╝
echo.

REM Step 0: Set target to esp32s3
echo Step 0: Setting build target to esp32s3...
idf.py set-target esp32s3
if errorlevel 1 (
    echo ❌ Failed to set target
    exit /b 1
)
echo ✅ Target set to esp32s3

REM Step 1: Add dependency
echo Step 1: Adding esp_tinyusb dependency...
python -m idf add-dependency espressif/esp_tinyusb
if errorlevel 1 (
    echo ❌ Failed to add dependency
    exit /b 1
)
echo ✅ Dependency added

REM Step 2: Reconfigure
echo.
echo Step 2: Reconfiguring build system...
idf.py reconfigure
if errorlevel 1 (
    echo ❌ Failed to reconfigure
    exit /b 1
)
echo ✅ Build system reconfigured

REM Step 3: Clean build
echo.
echo Step 3: Cleaning build artifacts...
idf.py fullclean
if errorlevel 1 (
    echo ❌ Failed to clean
    exit /b 1
)
echo ✅ Build cleaned

REM Step 4: Build
echo.
echo Step 4: Building firmware...
idf.py build
if errorlevel 1 (
    echo ❌ Build failed
    exit /b 1
)
echo ✅ Build successful

echo.
echo ╔════════════════════════════════════════════════════════════════╗
echo ║                                                                ║
echo ║              ✅ SETUP COMPLETE - BUILD SUCCESSFUL             ║
echo ║                                                                ║
echo ║  Binary: build\esp32s3-dualusb-fw.bin                         ║
echo ║  Ready to flash: idf.py -p COM3 flash                         ║
echo ║                                                                ║
echo ╚════════════════════════════════════════════════════════════════╝
echo.

