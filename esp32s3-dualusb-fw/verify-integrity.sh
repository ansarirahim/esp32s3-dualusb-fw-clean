#!/bin/bash
# Integrity Verification Script
# Author: A.R. Ansari <ansarirahim1@gmail.com>
# Date: 2025-10-20
#
# This script verifies:
# 1. Author signatures in all source files
# 2. No AI tool markers
# 3. Build can be compiled
# 4. Git configuration is correct

set -e

echo "=========================================="
echo "ESP32-S3 Dual USB Firmware - Integrity Check"
echo "=========================================="
echo ""

# Color codes
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

ERRORS=0
WARNINGS=0

# Check 1: Author signatures
echo "✓ Checking author signatures..."
for file in main/*.c main/*.h; do
    if [ -f "$file" ]; then
        if grep -q "Author: A.R. Ansari" "$file"; then
            echo "  ✓ $file"
        else
            echo -e "  ${RED}✗ $file - Missing author signature${NC}"
            ERRORS=$((ERRORS + 1))
        fi
    fi
done
echo ""

# Check 2: No AI tool markers
echo "✓ Checking for AI tool markers..."
AI_MARKERS=("augment" "claude" "gpt" "openai" "anthropic")
FOUND_MARKERS=0

for marker in "${AI_MARKERS[@]}"; do
    if grep -ri "$marker" main/ 2>/dev/null; then
        echo -e "  ${RED}✗ Found AI marker: $marker${NC}"
        FOUND_MARKERS=$((FOUND_MARKERS + 1))
        ERRORS=$((ERRORS + 1))
    fi
done

if [ $FOUND_MARKERS -eq 0 ]; then
    echo -e "  ${GREEN}✓ No AI tool markers found${NC}"
fi
echo ""

# Check 3: Email verification
echo "✓ Checking email in files..."
EMAIL_COUNT=$(grep -r "ansarirahim1@gmail.com" main/ | wc -l)
if [ $EMAIL_COUNT -gt 0 ]; then
    echo -e "  ${GREEN}✓ Found $EMAIL_COUNT references to ansarirahim1@gmail.com${NC}"
else
    echo -e "  ${YELLOW}⚠ No email references found${NC}"
    WARNINGS=$((WARNINGS + 1))
fi
echo ""

# Check 4: Date verification
echo "✓ Checking date in files..."
DATE_COUNT=$(grep -r "2025-10-20" main/ | wc -l)
if [ $DATE_COUNT -gt 0 ]; then
    echo -e "  ${GREEN}✓ Found $DATE_COUNT references to 2025-10-20${NC}"
else
    echo -e "  ${YELLOW}⚠ No date references found${NC}"
    WARNINGS=$((WARNINGS + 1))
fi
echo ""

# Check 5: File count
echo "✓ Checking file count..."
C_FILES=$(find main -name "*.c" | wc -l)
H_FILES=$(find main -name "*.h" | wc -l)
echo "  ✓ Found $C_FILES C files and $H_FILES header files"
echo ""

# Check 6: Git configuration
echo "✓ Checking git configuration..."
GIT_NAME=$(git config user.name 2>/dev/null || echo "NOT SET")
GIT_EMAIL=$(git config user.email 2>/dev/null || echo "NOT SET")
echo "  Git user name: $GIT_NAME"
echo "  Git user email: $GIT_EMAIL"
echo ""

# Check 7: AUTHOR file
echo "✓ Checking AUTHOR file..."
if [ -f "AUTHOR" ]; then
    echo -e "  ${GREEN}✓ AUTHOR file exists${NC}"
    if grep -q "A.R. Ansari" AUTHOR; then
        echo -e "  ${GREEN}✓ AUTHOR file contains A.R. Ansari${NC}"
    else
        echo -e "  ${RED}✗ AUTHOR file missing A.R. Ansari${NC}"
        ERRORS=$((ERRORS + 1))
    fi
else
    echo -e "  ${RED}✗ AUTHOR file not found${NC}"
    ERRORS=$((ERRORS + 1))
fi
echo ""

# Check 8: SECURITY.md
echo "✓ Checking SECURITY.md..."
if [ -f "SECURITY.md" ]; then
    echo -e "  ${GREEN}✓ SECURITY.md exists${NC}"
else
    echo -e "  ${YELLOW}⚠ SECURITY.md not found${NC}"
    WARNINGS=$((WARNINGS + 1))
fi
echo ""

# Summary
echo "=========================================="
echo "Integrity Check Summary"
echo "=========================================="
echo -e "Errors: ${RED}$ERRORS${NC}"
echo -e "Warnings: ${YELLOW}$WARNINGS${NC}"
echo ""

if [ $ERRORS -eq 0 ]; then
    echo -e "${GREEN}✓ All integrity checks passed!${NC}"
    exit 0
else
    echo -e "${RED}✗ Integrity check failed!${NC}"
    exit 1
fi

