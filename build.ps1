# ============================================================================
# ESP32-S3 Dual USB Firmware - Build & Flash Script (PowerShell)
# Machine-independent, user-independent, path-independent
# ============================================================================

param(
    [string]$Command = "",
    [string]$Port = ""
)

# Get the directory where this script is located
$SCRIPT_DIR = Split-Path -Parent $MyInvocation.MyCommand.Path
Set-Location $SCRIPT_DIR

# Add Docker to PATH if installed but not in PATH
$DOCKER_PATH = "C:\Program Files\Docker\Docker\resources\bin"
if ((Test-Path $DOCKER_PATH) -and ($env:PATH -notlike "*$DOCKER_PATH*")) {
    $env:PATH = "$DOCKER_PATH;$env:PATH"
}

# Clear screen
Clear-Host

Write-Host ""
Write-Host "============================================================================"
Write-Host "ESP32-S3 Dual USB Firmware - Build and Flash"
Write-Host "============================================================================"
Write-Host ""

# Parse command line arguments
if ([string]::IsNullOrEmpty($Command)) {
    Write-Host "Usage: .\build.ps1 [command]"
    Write-Host ""
    Write-Host "Commands:"
    Write-Host "  build       - Build firmware"
    Write-Host "  flash       - Flash to device"
    Write-Host "  monitor     - Monitor serial output"
    Write-Host "  test        - Run tests"
    Write-Host "  clean       - Clean build artifacts"
    Write-Host "  erase       - Erase flash memory"
    Write-Host "  full        - Build and flash"
    Write-Host "  help        - Show this help"
    Write-Host ""
    exit 0
}

if ($Command -eq "help") {
    Write-Host "Usage: .\build.ps1 [command]"
    Write-Host ""
    Write-Host "Commands:"
    Write-Host "  build       - Build firmware"
    Write-Host "  flash       - Flash to device"
    Write-Host "  monitor     - Monitor serial output"
    Write-Host "  test        - Run tests"
    Write-Host "  clean       - Clean build artifacts"
    Write-Host "  erase       - Erase flash memory"
    Write-Host "  full        - Build and flash"
    Write-Host "  help        - Show this help"
    Write-Host ""
    exit 0
}

# Check if Docker is available
$docker_check = & docker --version 2>&1
if ($LASTEXITCODE -ne 0) {
    Write-Host ""
    Write-Host "[ERROR] Docker is not installed or not in PATH"
    Write-Host ""
    Write-Host "Please install Docker Desktop from:"
    Write-Host "https://www.docker.com/products/docker-desktop"
    Write-Host ""
    Read-Host "Press Enter to continue"
    exit 1
}

Write-Host "[OK] Docker found"
Write-Host $docker_check
Write-Host ""

# Check if docker-compose.yml exists
if (-not (Test-Path "docker-compose.yml")) {
    Write-Host "[ERROR] docker-compose.yml not found"
    Write-Host ""
    Write-Host "Make sure you're in the project directory:"
    Write-Host "cd esp32s3-dualusb-fw-clean"
    Write-Host ""
    Read-Host "Press Enter to continue"
    exit 1
}

Write-Host "[OK] docker-compose.yml found"
Write-Host ""

# Execute the command
switch ($Command.ToLower()) {
    "build" {
        Write-Host "[*] Building firmware..."
        Write-Host ""
        & docker compose build --no-cache
        if ($LASTEXITCODE -ne 0) {
            Write-Host ""
            Write-Host "[ERROR] Build failed"
            exit 1
        }
        & docker compose run --rm esp32 bash -c "export IDF_PATH_FORCE=1 && . `$IDF_PATH/export.sh && idf.py build"
        if ($LASTEXITCODE -ne 0) {
            Write-Host ""
            Write-Host "[ERROR] Build failed"
            exit 1
        }
        Write-Host ""
        Write-Host "[OK] Build successful"
        Write-Host ""
    }
    "flash" {
        Write-Host "[*] Flashing firmware..."
        Write-Host ""
        if ($Port) {
            Write-Host "[*] Using port: $Port"
            & docker compose run --rm esp32 bash -c "export IDF_PATH_FORCE=1 && . `$IDF_PATH/export.sh && idf.py -p $Port flash"
        } else {
            & docker compose run --rm esp32 bash -c "export IDF_PATH_FORCE=1 && . `$IDF_PATH/export.sh && idf.py flash"
        }
        if ($LASTEXITCODE -ne 0) {
            Write-Host ""
            Write-Host "[ERROR] Flash failed"
            exit 1
        }
        Write-Host ""
        Write-Host "[OK] Flash successful"
        Write-Host ""
    }
    "monitor" {
        Write-Host "[*] Monitoring serial output..."
        Write-Host ""
        & docker compose run --rm esp32 bash -c "export IDF_PATH_FORCE=1 && . `$IDF_PATH/export.sh && idf.py monitor"
    }
    "test" {
        Write-Host "[*] Running tests..."
        Write-Host ""
        & docker compose run --rm esp32 bash -c "export IDF_PATH_FORCE=1 && . `$IDF_PATH/export.sh && idf.py test"
        if ($LASTEXITCODE -ne 0) {
            Write-Host ""
            Write-Host "[ERROR] Tests failed"
            exit 1
        }
        Write-Host ""
        Write-Host "[OK] Tests passed"
        Write-Host ""
    }
    "clean" {
        Write-Host "[*] Cleaning build artifacts..."
        Write-Host ""
        & docker compose run --rm esp32 bash -c "export IDF_PATH_FORCE=1 && . `$IDF_PATH/export.sh && idf.py fullclean"
        Write-Host ""
        Write-Host "[OK] Clean successful"
        Write-Host ""
    }
    "erase" {
        Write-Host "[*] Erasing flash memory..."
        Write-Host ""
        & docker compose run --rm esp32 bash -c "export IDF_PATH_FORCE=1 && . `$IDF_PATH/export.sh && idf.py erase-flash"
        if ($LASTEXITCODE -ne 0) {
            Write-Host ""
            Write-Host "[ERROR] Erase failed"
            exit 1
        }
        Write-Host ""
        Write-Host "[OK] Erase successful"
        Write-Host ""
    }
    "full" {
        Write-Host "[*] Building and flashing firmware..."
        Write-Host ""
        & docker compose build --no-cache
        if ($LASTEXITCODE -ne 0) {
            Write-Host ""
            Write-Host "[ERROR] Build failed"
            exit 1
        }
        if ($Port) {
            Write-Host "[*] Using port: $Port"
            & docker compose run --rm esp32 bash -c "export IDF_PATH_FORCE=1 && . `$IDF_PATH/export.sh && idf.py build && idf.py -p $Port flash"
        } else {
            & docker compose run --rm esp32 bash -c "export IDF_PATH_FORCE=1 && . `$IDF_PATH/export.sh && idf.py build && idf.py flash"
        }
        if ($LASTEXITCODE -ne 0) {
            Write-Host ""
            Write-Host "[ERROR] Build or flash failed"
            exit 1
        }
        Write-Host ""
        Write-Host "[OK] Build and flash successful"
        Write-Host ""
    }
    default {
        Write-Host "[ERROR] Unknown command: $Command"
        Write-Host ""
        Write-Host "Usage: .\build.ps1 [command]"
        Write-Host ""
        Write-Host "Commands:"
        Write-Host "  build       - Build firmware"
        Write-Host "  flash       - Flash to device"
        Write-Host "  monitor     - Monitor serial output"
        Write-Host "  test        - Run tests"
        Write-Host "  clean       - Clean build artifacts"
        Write-Host "  erase       - Erase flash memory"
        Write-Host "  full        - Build and flash"
        Write-Host "  help        - Show this help"
        Write-Host ""
        exit 1
    }
}

exit 0

