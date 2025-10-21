@echo off
REM Cross-platform build script for ESP32-S3 Dual USB Firmware
REM Works on Windows with ESP-IDF installed

setlocal enabledelayedexpansion

echo.
echo ==========================================
echo ESP32-S3 Dual USB Firmware Build
echo ==========================================
echo.

REM Check if idf.py is available
where idf.py >nul 2>&1
if errorlevel 1 (
    echo ERROR: ESP-IDF not found. Please install ESP-IDF v5.5.1 or later.
    echo See: https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/get-started/
    exit /b 1
)

echo Setting target to ESP32-S3...
call idf.py set-target esp32s3
if errorlevel 1 (
    echo ERROR: Failed to set target
    exit /b 1
)
echo OK - Target set
echo.

REM Clean previous build (optional)
if "%1"=="clean" (
    echo Cleaning previous build...
    call idf.py fullclean
    if errorlevel 1 (
        echo ERROR: Failed to clean
        exit /b 1
    )
    echo OK - Clean complete
    echo.
)

REM Build
echo Building firmware...
call idf.py build
if errorlevel 1 (
    echo ERROR: Build failed
    exit /b 1
)
echo.

REM Check output
if exist "build\esp32s3_dualusb_fw.bin" (
    for %%A in ("build\esp32s3_dualusb_fw.bin") do set SIZE=%%~zA
    echo ==========================================
    echo Build Successful!
    echo ==========================================
    echo Binary: build\esp32s3_dualusb_fw.bin
    echo Size: !SIZE! bytes
    echo.
    echo Next steps:
    echo 1. Connect ESP32-S3 via USB-C cable
    echo 2. Run: idf.py -p COM3 flash
    echo 3. Run: idf.py monitor
    echo.
) else (
    echo ERROR: Build failed - binary not found
    exit /b 1
)

endlocal

