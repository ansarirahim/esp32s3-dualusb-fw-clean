#!/bin/bash
# Build validation script - ensures build succeeds before pushing
# Usage: ./validate-build.sh

set -e

echo "════════════════════════════════════════════════════════════════"
echo "  ESP32-S3 Dual USB Firmware - Build Validation"
echo "════════════════════════════════════════════════════════════════"
echo ""

# Check if ESP-IDF is installed
if [ -z "$IDF_PATH" ]; then
    echo "❌ ERROR: ESP-IDF not found. Please set IDF_PATH environment variable."
    exit 1
fi

echo "✓ ESP-IDF found at: $IDF_PATH"
echo ""

# Step 1: Pre-commit checks
echo "Step 1: Running pre-commit checks..."
if [ -f "pre-commit-hook.sh" ]; then
    bash pre-commit-hook.sh
else
    echo "⚠️  pre-commit-hook.sh not found, skipping"
fi
echo ""

# Step 2: Clean build
echo "Step 2: Cleaning previous build..."
rm -rf build/
echo "✓ Build directory cleaned"
echo ""

# Step 3: Set target
echo "Step 3: Setting target to esp32s3..."
idf.py set-target esp32s3
echo "✓ Target set"
echo ""

# Step 4: Build
echo "Step 4: Building firmware..."
idf.py build 2>&1 | tee build.log
echo ""

# Step 5: Check for errors
echo "Step 5: Checking for compilation errors..."
if grep -i "error:" build.log; then
    echo "❌ FAILED: Compilation errors found!"
    exit 1
fi
echo "✓ No compilation errors"
echo ""

# Step 6: Check for warnings
echo "Step 6: Checking for compilation warnings..."
warning_count=$(grep -i "warning:" build.log | wc -l)
if [ "$warning_count" -gt 0 ]; then
    echo "⚠️  Found $warning_count warning(s):"
    grep -i "warning:" build.log | head -5
else
    echo "✓ No compilation warnings"
fi
echo ""

# Step 7: Verify build artifacts
echo "Step 7: Verifying build artifacts..."
if [ ! -f "build/esp32s3-dualusb-fw.bin" ]; then
    echo "❌ FAILED: Build artifact not found!"
    exit 1
fi

bin_size=$(stat -f%z "build/esp32s3-dualusb-fw.bin" 2>/dev/null || stat -c%s "build/esp32s3-dualusb-fw.bin")
echo "✓ Build artifact found: $bin_size bytes"
echo ""

# Step 8: Check build log for issues
echo "Step 8: Analyzing build log..."
if grep -i "undefined reference" build.log; then
    echo "❌ FAILED: Undefined references found!"
    exit 1
fi
echo "✓ No undefined references"
echo ""

echo "════════════════════════════════════════════════════════════════"
echo "✅ BUILD VALIDATION SUCCESSFUL!"
echo "════════════════════════════════════════════════════════════════"
echo ""
echo "Build artifact: build/esp32s3-dualusb-fw.bin ($bin_size bytes)"
echo ""
echo "Ready to push to GitHub!"
exit 0

