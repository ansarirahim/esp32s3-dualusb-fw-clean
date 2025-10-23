@echo off
REM ============================================================================
REM ESP32-S3 Dual USB Firmware - Build & Flash Script (Windows)
REM Machine-independent, user-independent, path-independent
REM ============================================================================

setlocal enabledelayedexpansion

REM Get the directory where this script is located
set SCRIPT_DIR=%~dp0
cd /d "%SCRIPT_DIR%"

REM Add Docker to PATH if installed but not in PATH
if exist "C:\Program Files\Docker\Docker\resources\bin\docker.exe" (
    set "PATH=C:\Program Files\Docker\Docker\resources\bin;!PATH!"
)

echo.
echo ============================================================================
echo ESP32-S3 Dual USB Firmware - Build and Flash
echo ============================================================================
echo.

REM Parse command line arguments FIRST (before Docker check)
set COMMAND=%1

REM Allow help without Docker
if /i "%COMMAND%"=="help" (
    echo Usage: build.bat [command]
    echo.
    echo Commands:
    echo   build       - Build firmware
    echo   flash       - Flash to device
    echo   monitor     - Monitor serial output
    echo   test        - Run tests
    echo   clean       - Clean build artifacts
    echo   erase       - Erase flash memory
    echo   full        - Build and flash
    echo   help        - Show this help
    echo.
    exit /b 0
)

if "%1"=="" (
    echo Usage: build.bat [command]
    echo.
    echo Commands:
    echo   build       - Build firmware
    echo   flash       - Flash to device
    echo   monitor     - Monitor serial output
    echo   test        - Run tests
    echo   clean       - Clean build artifacts
    echo   erase       - Erase flash memory
    echo   full        - Build and flash
    echo   help        - Show this help
    echo.
    echo Examples:
    echo   build.bat build
    echo   build.bat flash
    echo   build.bat full
    echo.
    pause
    exit /b 0
)

REM NOW check if docker is available (after help/no-args)
docker --version >nul 2>&1
if errorlevel 1 (
    echo.
    echo ERROR: Docker is not installed or not in PATH
    echo.
    echo Please install Docker Desktop from:
    echo https://www.docker.com/products/docker-desktop
    echo.
    echo If Docker is installed, try:
    echo 1. Restart PowerShell
    echo 2. Start Docker Desktop application
    echo 3. Run this script again
    echo.
    pause
    exit /b 1
)

echo [OK] Docker found
docker --version
echo.

REM Check if docker-compose.yml exists
if not exist "docker-compose.yml" (
    echo.
    echo ERROR: docker-compose.yml not found in current directory
    echo Current directory: %cd%
    echo.
    pause
    exit /b 1
)

echo [OK] docker-compose.yml found
echo.

if /i "%COMMAND%"=="build" (
    echo.
    echo [*] Building firmware...
    docker compose run --rm esp32 idf.py build
    if errorlevel 1 (
        echo.
        echo [ERROR] Build failed
        pause
        exit /b 1
    )
    echo.
    echo [OK] Build successful
    goto :eof
)

if /i "%COMMAND%"=="flash" (
    echo.
    echo [*] Flashing firmware...
    docker compose run --rm esp32 idf.py flash
    if errorlevel 1 (
        echo.
        echo [ERROR] Flash failed
        pause
        exit /b 1
    )
    echo.
    echo [OK] Flash successful
    goto :eof
)

if /i "%COMMAND%"=="monitor" (
    echo.
    echo [*] Starting serial monitor...
    docker compose run --rm esp32 idf.py monitor
    goto :eof
)

if /i "%COMMAND%"=="test" (
    echo.
    echo [*] Running tests...
    docker compose run --rm esp32 idf.py test
    if errorlevel 1 (
        echo.
        echo [ERROR] Tests failed
        pause
        exit /b 1
    )
    echo.
    echo [OK] Tests passed
    goto :eof
)

if /i "%COMMAND%"=="clean" (
    echo.
    echo [*] Cleaning build artifacts...
    docker compose run --rm esp32 idf.py clean
    if errorlevel 1 (
        echo.
        echo [ERROR] Clean failed
        pause
        exit /b 1
    )
    echo.
    echo [OK] Clean successful
    goto :eof
)

if /i "%COMMAND%"=="erase" (
    echo.
    echo [*] Erasing flash memory...
    docker compose run --rm esp32 idf.py erase_flash
    if errorlevel 1 (
        echo.
        echo [ERROR] Erase failed
        pause
        exit /b 1
    )
    echo.
    echo [OK] Erase successful
    goto :eof
)

if /i "%COMMAND%"=="full" (
    echo.
    echo [*] Building and flashing...
    docker compose run --rm esp32 idf.py build
    if errorlevel 1 (
        echo.
        echo [ERROR] Build failed
        pause
        exit /b 1
    )
    echo.
    echo [OK] Build successful
    echo.
    docker compose run --rm esp32 idf.py flash
    if errorlevel 1 (
        echo.
        echo [ERROR] Flash failed
        pause
        exit /b 1
    )
    echo.
    echo [OK] Flash successful
    goto :eof
)

echo.
echo [ERROR] Unknown command: %COMMAND%
echo Run 'build.bat help' for usage information
pause
exit /b 1

