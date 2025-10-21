#!/usr/bin/env powershell

# Build script for ESP32-S3 Dual USB Firmware

$IDF_PATH = "C:\Users\Abdul\esp-idf-v5.5.1"
$PYTHON_ENV = "C:\Users\Abdul\.espressif\python_env\idf5.5_py3.10_env\Scripts\python.exe"

# Set environment variables
$env:IDF_PATH = $IDF_PATH

# Add tools to PATH
$toolPaths = @(
    "C:\Users\Abdul\.espressif\tools\xtensa-esp-elf\esp-14.2.0_20241119\xtensa-esp-elf\bin",
    "C:\Users\Abdul\.espressif\tools\cmake\3.30.2\bin",
    "C:\Users\Abdul\.espressif\tools\ninja\1.12.1",
    "C:\Users\Abdul\.espressif\python_env\idf5.5_py3.10_env\Scripts",
    "C:\Users\Abdul\esp-idf-v5.5.1\tools"
)

$env:PATH = ($toolPaths -join ";") + ";" + $env:PATH

Write-Host "IDF_PATH: $env:IDF_PATH"
Write-Host "Building ESP32-S3 Dual USB Firmware..."
Write-Host ""

# Clean build directory
if (Test-Path "build") {
    Write-Host "Cleaning build directory..."
    Remove-Item -Recurse -Force "build" -ErrorAction SilentlyContinue
}

# Run build
Write-Host "Running build..."
& $PYTHON_ENV "$IDF_PATH\tools\idf.py" build 2>&1 | Tee-Object -FilePath "build.log"

# Check result
if ($LASTEXITCODE -eq 0) {
    Write-Host ""
    Write-Host "Build successful!"
    Write-Host ""

    # Show binary info
    if (Test-Path "build\esp32s3-dualusb-fw.bin") {
        $binSize = (Get-Item "build\esp32s3-dualusb-fw.bin").Length
        Write-Host "Binary: build/esp32s3-dualusb-fw.bin - $binSize bytes"
    }

    # Show build log location
    Write-Host "Build log: build.log"
} else {
    Write-Host ""
    Write-Host "Build failed!"
    Write-Host "Check build.log for details"
    exit 1
}

