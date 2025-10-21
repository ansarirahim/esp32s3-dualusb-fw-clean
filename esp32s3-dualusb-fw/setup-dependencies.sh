#!/bin/bash
# Setup script for ESP32-S3 Dual USB Firmware dependencies
# This script installs the esp_tinyusb component and prepares the build

set -e

echo ""
echo "╔════════════════════════════════════════════════════════════════╗"
echo "║                                                                ║"
echo "║     ESP32-S3 Dual USB Firmware - Dependency Setup             ║"
echo "║                                                                ║"
echo "╚════════════════════════════════════════════════════════════════╝"
echo ""

# Step 0: Set target to esp32s3
echo "Step 0: Setting build target to esp32s3..."
idf.py set-target esp32s3
if [ $? -ne 0 ]; then
    echo "❌ Failed to set target"
    exit 1
fi
echo "✅ Target set to esp32s3"

# Step 1: Add dependency
echo "Step 1: Adding esp_tinyusb dependency..."
idf.py add-dependency espressif/esp_tinyusb
if [ $? -ne 0 ]; then
    echo "❌ Failed to add dependency"
    exit 1
fi
echo "✅ Dependency added"

# Step 2: Reconfigure
echo ""
echo "Step 2: Reconfiguring build system..."
idf.py reconfigure
if [ $? -ne 0 ]; then
    echo "❌ Failed to reconfigure"
    exit 1
fi
echo "✅ Build system reconfigured"

# Step 3: Clean build
echo ""
echo "Step 3: Cleaning build artifacts..."
idf.py fullclean
if [ $? -ne 0 ]; then
    echo "❌ Failed to clean"
    exit 1
fi
echo "✅ Build cleaned"

# Step 4: Build
echo ""
echo "Step 4: Building firmware..."
idf.py build
if [ $? -ne 0 ]; then
    echo "❌ Build failed"
    exit 1
fi
echo "✅ Build successful"

echo ""
echo "╔════════════════════════════════════════════════════════════════╗"
echo "║                                                                ║"
echo "║              ✅ SETUP COMPLETE - BUILD SUCCESSFUL             ║"
echo "║                                                                ║"
echo "║  Binary: build/esp32s3-dualusb-fw.bin                         ║"
echo "║  Ready to flash: idf.py -p /dev/ttyUSB0 flash                ║"
echo "║                                                                ║"
echo "╚════════════════════════════════════════════════════════════════╝"
echo ""

