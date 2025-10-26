#!/bin/bash
# Flash partition table and verify FATFS mount
# Usage: ./flash_and_verify.sh [PORT]

set -e

PORT=${1:-/dev/ttyUSB0}
BAUD=460800

echo "=========================================="
echo "ESP32-S3 Partition Table Flash & Verify"
echo "=========================================="
echo "Port: $PORT"
echo "Baud: $BAUD"
echo ""

# Check if port exists
if [ ! -e "$PORT" ]; then
    echo "ERROR: Port $PORT not found"
    exit 1
fi

# Step 1: Build partition table
echo "[1/4] Building partition table..."
idf.py partition-table

# Step 2: Flash partition table
echo "[2/4] Flashing partition table..."
idf.py -p $PORT -b $BAUD partition-table-flash

# Step 3: Flash application
echo "[3/4] Flashing application..."
idf.py -p $PORT -b $BAUD flash

# Step 4: Verify partition table
echo "[4/4] Verifying partition table..."
echo ""
echo "Partition table contents:"
parttool.py -p $PORT get_partition_info --partition-type=data --partition-subtype=fat --info=name,size,offset

echo ""
echo "=========================================="
echo "✅ Flash and verify complete!"
echo "=========================================="
echo ""
echo "Next steps:"
echo "1. Monitor serial output: idf.py -p $PORT monitor"
echo "2. Check for 'FATFS mounted successfully' message"
echo "3. Verify 'storage' partition is found"
echo ""

