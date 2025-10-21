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

# Step 0: Delete corrupted build directory
echo "Step 0: Cleaning corrupted build directory..."
if [ -d build ]; then
    rm -rf build
    echo "✅ Build directory deleted"
else
    echo "✅ No build directory to clean"
fi

# Step 1: Delete managed_components
echo ""
echo "Step 1: Cleaning managed_components..."
if [ -d managed_components ]; then
    rm -rf managed_components
    echo "✅ managed_components deleted"
else
    echo "✅ No managed_components to clean"
fi

# Step 2: Delete dependencies.lock
echo ""
echo "Step 2: Cleaning dependencies.lock..."
if [ -f dependencies.lock ]; then
    rm dependencies.lock
    echo "✅ dependencies.lock deleted"
else
    echo "✅ No dependencies.lock to clean"
fi

# Step 3: Reconfigure to create dependencies.lock
echo ""
echo "Step 3: Reconfiguring to create dependencies.lock..."
idf.py reconfigure
if [ $? -ne 0 ]; then
    echo "⚠️  Reconfigure failed (this is expected on first run)"
fi
echo "✅ Proceeding with set-target"

# Step 4: Set target to esp32s3
echo ""
echo "Step 4: Setting build target to esp32s3..."
idf.py set-target esp32s3
if [ $? -ne 0 ]; then
    echo "❌ Failed to set target"
    exit 1
fi
echo "✅ Target set to esp32s3"

# Step 5: Reconfigure again
echo ""
echo "Step 5: Reconfiguring build system..."
idf.py reconfigure
if [ $? -ne 0 ]; then
    echo "❌ Failed to reconfigure"
    exit 1
fi
echo "✅ Build system reconfigured"

# Step 6: Build
echo ""
echo "Step 6: Building firmware..."
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

