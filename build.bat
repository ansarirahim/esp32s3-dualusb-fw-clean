@echo off
REM ============================================================================
REM ESP32-S3 Dual USB Firmware - Build & Flash Script (Windows)
REM Machine-independent, user-independent, path-independent
REM ============================================================================

setlocal enabledelayedexpansion

REM Get the directory where this script is located
set SCRIPT_DIR=%~dp0
cd /d "%SCRIPT_DIR%"

REM Colors for output
set GREEN=[92m
set YELLOW=[93m
set RED=[91m
set RESET=[0m

echo.
echo ============================================================================
echo ESP32-S3 Dual USB Firmware - Build & Flash
echo ============================================================================
echo.

REM Check if docker is available
where docker >nul 2>&1
if errorlevel 1 (
    echo %RED%ERROR: Docker is not installed or not in PATH%RESET%
    echo.
    echo Please install Docker Desktop from: https://www.docker.com/products/docker-desktop
    echo.
    pause
    exit /b 1
)

echo %GREEN%✓ Docker found%RESET%
docker --version
echo.

REM Check if docker-compose.yml exists
if not exist "docker-compose.yml" (
    echo %RED%ERROR: docker-compose.yml not found in current directory%RESET%
    echo Current directory: %cd%
    echo.
    pause
    exit /b 1
)

echo %GREEN%✓ docker-compose.yml found%RESET%
echo.

REM Parse command line arguments
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

set COMMAND=%1

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

if /i "%COMMAND%"=="build" (
    echo %YELLOW%Building firmware...%RESET%
    docker compose run --rm esp32 idf.py build
    if errorlevel 1 (
        echo %RED%Build failed%RESET%
        pause
        exit /b 1
    )
    echo %GREEN%✓ Build successful%RESET%
    goto :eof
)

if /i "%COMMAND%"=="flash" (
    echo %YELLOW%Flashing firmware...%RESET%
    docker compose run --rm esp32 idf.py flash
    if errorlevel 1 (
        echo %RED%Flash failed%RESET%
        pause
        exit /b 1
    )
    echo %GREEN%✓ Flash successful%RESET%
    goto :eof
)

if /i "%COMMAND%"=="monitor" (
    echo %YELLOW%Starting serial monitor...%RESET%
    docker compose run --rm esp32 idf.py monitor
    goto :eof
)

if /i "%COMMAND%"=="test" (
    echo %YELLOW%Running tests...%RESET%
    docker compose run --rm esp32 idf.py test
    if errorlevel 1 (
        echo %RED%Tests failed%RESET%
        pause
        exit /b 1
    )
    echo %GREEN%✓ Tests passed%RESET%
    goto :eof
)

if /i "%COMMAND%"=="clean" (
    echo %YELLOW%Cleaning build artifacts...%RESET%
    docker compose run --rm esp32 idf.py clean
    if errorlevel 1 (
        echo %RED%Clean failed%RESET%
        pause
        exit /b 1
    )
    echo %GREEN%✓ Clean successful%RESET%
    goto :eof
)

if /i "%COMMAND%"=="erase" (
    echo %YELLOW%Erasing flash memory...%RESET%
    docker compose run --rm esp32 idf.py erase_flash
    if errorlevel 1 (
        echo %RED%Erase failed%RESET%
        pause
        exit /b 1
    )
    echo %GREEN%✓ Erase successful%RESET%
    goto :eof
)

if /i "%COMMAND%"=="full" (
    echo %YELLOW%Building and flashing...%RESET%
    docker compose run --rm esp32 idf.py build
    if errorlevel 1 (
        echo %RED%Build failed%RESET%
        pause
        exit /b 1
    )
    echo %GREEN%✓ Build successful%RESET%
    echo.
    docker compose run --rm esp32 idf.py flash
    if errorlevel 1 (
        echo %RED%Flash failed%RESET%
        pause
        exit /b 1
    )
    echo %GREEN%✓ Flash successful%RESET%
    goto :eof
)

echo %RED%Unknown command: %COMMAND%%RESET%
echo Run 'build.bat help' for usage information
pause
exit /b 1

