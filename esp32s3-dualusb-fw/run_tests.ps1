# Unit Test Runner Script for ESP32-S3 Dual USB Firmware (PowerShell)
#
# Author: A.R. Ansari <ansarirahim1@gmail.com>
# Date: 2025-10-21
# Version: 1.0.0
#
# Description:
# This script builds and runs unit tests for the ESP32-S3 Dual USB Firmware.
# Windows PowerShell version.
#
# Usage:
#   .\run_tests.ps1 [options]
#
# Options:
#   -Build          Build tests only
#   -Flash          Flash tests to device
#   -Monitor        Monitor test output
#   -Clean          Clean build artifacts
#   -Help           Show this help message

param(
    [switch]$Build,
    [switch]$Flash,
    [switch]$Monitor,
    [switch]$Clean,
    [switch]$Help
)

# Colors
$Colors = @{
    Red    = "Red"
    Green  = "Green"
    Yellow = "Yellow"
    Blue   = "Blue"
    Cyan   = "Cyan"
}

# Functions
function Print-Header {
    Write-Host "╔════════════════════════════════════════════════════════════╗" -ForegroundColor Blue
    Write-Host "║   ESP32-S3 Dual USB Firmware - Unit Test Runner            ║" -ForegroundColor Blue
    Write-Host "║   Author: A.R. Ansari <ansarirahim1@gmail.com>            ║" -ForegroundColor Blue
    Write-Host "╚════════════════════════════════════════════════════════════╝" -ForegroundColor Blue
    Write-Host ""
}

function Print-Success {
    param([string]$Message)
    Write-Host "✓ $Message" -ForegroundColor Green
}

function Print-Error {
    param([string]$Message)
    Write-Host "✗ $Message" -ForegroundColor Red
}

function Print-Info {
    param([string]$Message)
    Write-Host "ℹ $Message" -ForegroundColor Yellow
}

function Print-Help {
    Write-Host "Usage: .\run_tests.ps1 [options]"
    Write-Host ""
    Write-Host "Options:"
    Write-Host "  -Build          Build tests only"
    Write-Host "  -Flash          Flash tests to device"
    Write-Host "  -Monitor        Monitor test output"
    Write-Host "  -Clean          Clean build artifacts"
    Write-Host "  -Help           Show this help message"
    Write-Host ""
    Write-Host "Examples:"
    Write-Host "  .\run_tests.ps1 -Build                    # Build tests"
    Write-Host "  .\run_tests.ps1 -Build -Flash -Monitor    # Build, flash, and monitor"
    Write-Host "  .\run_tests.ps1 -Clean                    # Clean build artifacts"
}

function Build-Tests {
    Print-Info "Building unit tests..."
    
    # Set target
    Print-Info "Setting target to ESP32-S3..."
    & idf.py set-target esp32s3
    if ($LASTEXITCODE -ne 0) {
        Print-Error "Failed to set target"
        exit 1
    }
    
    # Build
    Print-Info "Building tests..."
    & idf.py build
    if ($LASTEXITCODE -ne 0) {
        Print-Error "Failed to build tests"
        exit 1
    }
    
    Print-Success "Tests built successfully"
}

function Flash-Tests {
    Print-Info "Flashing tests to device..."
    
    & idf.py flash
    if ($LASTEXITCODE -ne 0) {
        Print-Error "Failed to flash tests"
        exit 1
    }
    
    Print-Success "Tests flashed successfully"
}

function Monitor-Tests {
    Print-Info "Monitoring test output..."
    Print-Info "Press Ctrl+C to exit"
    Write-Host ""
    
    & idf.py monitor
}

function Clean-Tests {
    Print-Info "Cleaning build artifacts..."
    
    if (Test-Path "build") {
        Remove-Item -Recurse -Force "build"
        Print-Success "Build directory removed"
    }
    
    if (Test-Path "test\build") {
        Remove-Item -Recurse -Force "test\build"
        Print-Success "Test build directory removed"
    }
    
    Print-Success "Clean completed"
}

# Main script
function Main {
    Print-Header
    
    # If no options, show help
    if (-not $Build -and -not $Flash -and -not $Monitor -and -not $Clean -and -not $Help) {
        Print-Help
        exit 0
    }
    
    if ($Help) {
        Print-Help
        exit 0
    }
    
    # Execute actions
    if ($Clean) {
        Clean-Tests
    }
    
    if ($Build) {
        Build-Tests
    }
    
    if ($Flash) {
        Flash-Tests
    }
    
    if ($Monitor) {
        Monitor-Tests
    }
    
    Write-Host ""
    Print-Success "Test runner completed"
}

# Run main function
Main

