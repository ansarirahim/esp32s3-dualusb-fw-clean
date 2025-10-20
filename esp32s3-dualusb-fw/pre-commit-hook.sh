#!/bin/bash
# Pre-commit hook to validate code before committing
# Install with: cp pre-commit-hook.sh .git/hooks/pre-commit && chmod +x .git/hooks/pre-commit

set -e

echo "🔍 Running pre-commit checks..."

# Check for AI tool markers
echo "  ✓ Checking for AI tool markers..."
if grep -r "augment\|claude\|gpt\|openai\|anthropic" main/ --ignore-case 2>/dev/null; then
    echo "  ❌ ERROR: Found AI tool markers in source code!"
    exit 1
fi

# Check for author signatures
echo "  ✓ Checking for author signatures..."
for file in main/*.c main/*.h; do
    if [ -f "$file" ]; then
        if ! grep -q "Author: A.R. Ansari" "$file"; then
            echo "  ⚠️  WARNING: Missing author signature in $file"
        fi
    fi
done

# Check for undefined macros
echo "  ✓ Checking for undefined macros..."
for file in main/*.c; do
    if [ -f "$file" ]; then
        # Check if MOUNT_POINT is used but not defined in this file
        if grep -q "MOUNT_POINT" "$file" && ! grep -q "#define MOUNT_POINT" "$file"; then
            # It should be in the header
            header="${file%.c}.h"
            if [ -f "$header" ] && ! grep -q "#define MOUNT_POINT" "$header"; then
                echo "  ❌ ERROR: MOUNT_POINT used but not defined in $file or $header"
                exit 1
            fi
        fi
    fi
done

# Check for header guard issues
echo "  ✓ Checking for header guard issues..."
for file in main/*.h; do
    if [ -f "$file" ]; then
        # Count #ifndef and #endif
        ifndef_count=$(grep -c "^#ifndef" "$file" || true)
        endif_count=$(grep -c "^#endif" "$file" || true)
        if [ "$ifndef_count" -ne "$endif_count" ]; then
            echo "  ❌ ERROR: Header guard mismatch in $file (#ifndef: $ifndef_count, #endif: $endif_count)"
            exit 1
        fi
    fi
done

# Check for function signature mismatches
echo "  ✓ Checking for function signature consistency..."
for header in main/*.h; do
    if [ -f "$header" ]; then
        base=$(basename "$header" .h)
        source="main/${base}.c"
        if [ -f "$source" ]; then
            # Extract function declarations from header
            while IFS= read -r line; do
                if [[ $line =~ ^bool\ ([a-z_]+)\( ]]; then
                    func="${BASH_REMATCH[1]}"
                    # Check if function exists in source with same signature
                    if ! grep -q "^bool $func(" "$source"; then
                        echo "  ⚠️  WARNING: Function signature mismatch for $func"
                    fi
                fi
            done < "$header"
        fi
    fi
done

# Check for trailing whitespace
echo "  ✓ Checking for trailing whitespace..."
if grep -r '[[:space:]]$' main/*.c main/*.h 2>/dev/null; then
    echo "  ⚠️  WARNING: Found trailing whitespace"
fi

# Check for TODO/FIXME comments
echo "  ✓ Checking for unresolved TODOs..."
if grep -r "TODO\|FIXME" main/ 2>/dev/null; then
    echo "  ⚠️  WARNING: Found TODO/FIXME comments"
fi

echo ""
echo "✅ All pre-commit checks passed!"
exit 0

