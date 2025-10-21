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

REM Step 0: Delete corrupted build directory
echo Step 0: Cleaning corrupted build directory...
if exist build (
    rmdir /s /q build
    echo ✅ Build directory deleted
) else (
    echo ✅ No build directory to clean
)

REM Step 1: Delete managed_components
echo.
echo Step 1: Cleaning managed_components...
if exist managed_components (
    rmdir /s /q managed_components
    echo ✅ managed_components deleted
) else (
    echo ✅ No managed_components to clean
)

REM Step 2: Delete dependencies.lock
echo.
echo Step 2: Cleaning dependencies.lock...
if exist dependencies.lock (
    del dependencies.lock
    echo ✅ dependencies.lock deleted
) else (
    echo ✅ No dependencies.lock to clean
)

REM Step 3: Add dependency BEFORE set-target
echo.
echo Step 3: Adding esp_tinyusb dependency...
idf.py add-dependency espressif/esp_tinyusb
if errorlevel 1 (
    echo ❌ Failed to add dependency
    exit /b 1
)
echo ✅ Dependency added

REM Step 4: Set target to esp32s3
echo.
echo Step 4: Setting build target to esp32s3...
idf.py set-target esp32s3
if errorlevel 1 (
    echo ❌ Failed to set target
    exit /b 1
)
echo ✅ Target set to esp32s3

REM Step 5: Reconfigure
echo.
echo Step 5: Reconfiguring build system...
idf.py reconfigure
if errorlevel 1 (
    echo ❌ Failed to reconfigure
    exit /b 1
)
echo ✅ Build system reconfigured

REM Step 6: Build
echo.
echo Step 6: Building firmware...
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

