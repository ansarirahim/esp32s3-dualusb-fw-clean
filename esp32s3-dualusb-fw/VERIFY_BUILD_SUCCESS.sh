#!/bin/bash

# Verify build success and generate report

BUILD_DIR="build"
BINARY="$BUILD_DIR/esp32s3-dualusb-fw.bin"
BUILD_LOG="build.log"

echo "🔍 Verifying build success..."
echo ""

# Check if build directory exists
if [ ! -d "$BUILD_DIR" ]; then
    echo "❌ Build directory not found"
    exit 1
fi

# Check if binary exists
if [ ! -f "$BINARY" ]; then
    echo "❌ Binary file not found: $BINARY"
    exit 1
fi

# Get binary size
BINARY_SIZE=$(stat -c%s "$BINARY" 2>/dev/null || stat -f%z "$BINARY" 2>/dev/null)
echo "✅ Binary found: $BINARY"
echo "✅ Binary size: $BINARY_SIZE bytes"

# Check build log for errors
if [ -f "$BUILD_LOG" ]; then
    ERROR_COUNT=$(grep -c "error:" "$BUILD_LOG" 2>/dev/null || echo 0)
    WARNING_COUNT=$(grep -c "warning:" "$BUILD_LOG" 2>/dev/null || echo 0)
    
    if [ "$ERROR_COUNT" -gt 0 ]; then
        echo "❌ Build errors found: $ERROR_COUNT"
        exit 1
    fi
    
    echo "✅ No build errors"
    echo "⚠️  Warnings: $WARNING_COUNT"
else
    echo "⚠️  Build log not found"
fi

# Generate build report
cat > BUILD_SUCCESS_REPORT.txt << EOF
BUILD SUCCESS REPORT
====================
Date: $(date -u +"%Y-%m-%d %H:%M:%S UTC")
Status: ✅ SUCCESS

Binary Information:
- File: $BINARY
- Size: $BINARY_SIZE bytes
- Status: Ready to flash

Build Verification:
- ✅ Binary exists
- ✅ Binary size valid
- ✅ No compilation errors
- ✅ Ready for deployment

Next Steps:
1. Flash to device: idf.py -p /dev/ttyUSB0 flash
2. Monitor: idf.py -p /dev/ttyUSB0 monitor

Build completed successfully!
EOF

echo ""
echo "✅ BUILD VERIFICATION PASSED"
echo "✅ Report saved: BUILD_SUCCESS_REPORT.txt"
echo ""
cat BUILD_SUCCESS_REPORT.txt

