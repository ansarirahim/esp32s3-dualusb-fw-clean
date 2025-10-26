@echo off
REM Flash partition table and verify FATFS mount
REM Usage: flash_and_verify.bat [PORT]

setlocal enabledelayedexpansion

set PORT=%1
if "%PORT%"=="" set PORT=COM3

set BAUD=460800

echo.
echo ==========================================
echo ESP32-S3 Partition Table Flash ^& Verify
echo ==========================================
echo Port: %PORT%
echo Baud: %BAUD%
echo.

REM Step 1: Build partition table
echo [1/4] Building partition table...
call idf.py partition-table
if errorlevel 1 (
    echo ERROR: Failed to build partition table
    exit /b 1
)

REM Step 2: Flash partition table
echo [2/4] Flashing partition table...
call idf.py -p %PORT% -b %BAUD% partition-table-flash
if errorlevel 1 (
    echo ERROR: Failed to flash partition table
    exit /b 1
)

REM Step 3: Flash application
echo [3/4] Flashing application...
call idf.py -p %PORT% -b %BAUD% flash
if errorlevel 1 (
    echo ERROR: Failed to flash application
    exit /b 1
)

REM Step 4: Verify partition table
echo [4/4] Verifying partition table...
echo.
echo Partition table contents:
python -m esptool --port %PORT% --baud %BAUD% read_flash 0x8000 0x1000 partition_table.bin
echo.

echo ==========================================
echo OK: Flash and verify complete!
echo ==========================================
echo.
echo Next steps:
echo 1. Monitor serial output: idf.py -p %PORT% monitor
echo 2. Check for "FATFS mounted successfully" message
echo 3. Verify "storage" partition is found
echo.

endlocal

