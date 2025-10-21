#!/bin/bash
# Cross-platform build script for ESP32-S3 Dual USB Firmware
# Works on Linux, WSL, and macOS

set -e

PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$PROJECT_DIR"

echo "=========================================="
echo "ESP32-S3 Dual USB Firmware Build"
echo "=========================================="
echo ""

# Check if ESP-IDF is installed
if ! command -v idf.py &> /dev/null; then
    echo "ERROR: ESP-IDF not found. Please install ESP-IDF v5.5.1 or later."
    echo "See: https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/get-started/"
    exit 1
fi

echo "✓ ESP-IDF found: $(idf.py --version)"
echo ""

# Set target
echo "Setting target to ESP32-S3..."
idf.py set-target esp32s3
echo "✓ Target set"
echo ""

# Clean previous build (optional)
if [ "$1" == "clean" ]; then
    echo "Cleaning previous build..."
    idf.py fullclean
    echo "✓ Clean complete"
    echo ""
fi

# Build
echo "Building firmware..."
idf.py build
echo ""

# Check output
if [ -f "build/esp32s3_dualusb_fw.bin" ]; then
    SIZE=$(stat -f%z "build/esp32s3_dualusb_fw.bin" 2>/dev/null || stat -c%s "build/esp32s3_dualusb_fw.bin" 2>/dev/null)
    echo "=========================================="
    echo "Build Successful!"
    echo "=========================================="
    echo "Binary: build/esp32s3_dualusb_fw.bin"
    echo "Size: $SIZE bytes"
    echo ""
    echo "Next steps:"
    echo "1. Connect ESP32-S3 via USB-C cable"
    echo "2. Run: idf.py -p /dev/ttyUSB0 flash"
    echo "3. Run: idf.py monitor"
    echo ""
else
    echo "ERROR: Build failed - binary not found"
    exit 1
fi

