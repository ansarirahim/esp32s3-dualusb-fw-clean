# CI/CD Pipeline Setup Guide

## Overview

This project uses GitHub Actions for continuous integration and continuous deployment (CI/CD). The pipeline ensures that:

1. ✅ All code compiles successfully
2. ✅ No AI tool markers are present
3. ✅ Author signatures are present
4. ✅ Code quality standards are met
5. ✅ Build artifacts are generated

## GitHub Actions Workflows

### 1. Build & Test Workflow (`.github/workflows/build.yml`)

**Triggers**: Push to `main`, `master`, `develop`, or `feat/*` branches

**Steps**:
1. Checkout code
2. Set up ESP-IDF v5.0
3. Build firmware for ESP32-S3
4. Check for compilation errors
5. Verify build artifacts
6. Run static analysis
7. Upload build artifacts

**Success Criteria**:
- ✅ Firmware builds without errors
- ✅ Build artifact exists and is valid
- ✅ No undefined references

### 2. Code Quality & Lint Workflow (`.github/workflows/lint.yml`)

**Triggers**: Push to `main`, `master`, `develop`, or `feat/*` branches

**Steps**:
1. Check for author signatures
2. Check for AI tool markers
3. Validate C syntax
4. Check for undefined macros
5. Check code formatting
6. Verify git configuration

**Success Criteria**:
- ✅ No AI tool markers (augment, claude, gpt, openai, anthropic)
- ✅ Author signatures present in all source files
- ✅ Valid C syntax
- ✅ No trailing whitespace

## Local Development Setup

### Pre-Commit Hook

Install the pre-commit hook to validate code before committing:

```bash
cp pre-commit-hook.sh .git/hooks/pre-commit
chmod +x .git/hooks/pre-commit
```

The hook will check:
- ✅ No AI tool markers
- ✅ Author signatures present
- ✅ Undefined macros
- ✅ Header guard consistency
- ✅ Function signature mismatches
- ✅ Trailing whitespace

### Build Validation Script

Before pushing, run the build validation script:

```bash
./validate-build.sh
```

This script will:
1. Run pre-commit checks
2. Clean previous build
3. Set target to esp32s3
4. Build firmware
5. Check for compilation errors
6. Verify build artifacts
7. Analyze build log

## Workflow: Development to Production

### 1. Local Development

```bash
# Make changes
vim main/filesystem.c

# Run pre-commit checks
bash pre-commit-hook.sh

# Commit changes
git add main/filesystem.c
git commit -m "fix: Update filesystem implementation"
```

### 2. Local Build Validation

```bash
# Validate build before pushing
./validate-build.sh

# If successful, push to GitHub
git push origin main
```

### 3. GitHub Actions Validation

When you push to GitHub:
1. Build workflow runs automatically
2. Lint workflow runs automatically
3. Both must pass before merging to main

### 4. Artifact Generation

If build succeeds:
- ✅ Firmware binary is generated
- ✅ Build artifacts are uploaded to GitHub
- ✅ Artifacts are available for 30 days

## Troubleshooting

### Build Fails with "MOUNT_POINT undeclared"

**Solution**: Ensure `filesystem.h` has:
```c
#define MOUNT_POINT "/storage"
```

### Build Fails with "#endif without #if"

**Solution**: Check header guard in `.h` files:
```c
#ifndef FILENAME_H
#define FILENAME_H
...
#endif /* FILENAME_H */
```

### Build Fails with "Function signature mismatch"

**Solution**: Ensure function declarations in `.h` match implementations in `.c`:
```c
// In .h file
bool fs_write_test_file(void);

// In .c file
bool fs_write_test_file(void) {
    ...
}
```

### Lint Fails with "AI tool markers found"

**Solution**: Remove any references to:
- augment
- claude
- gpt
- openai
- anthropic

### Lint Fails with "Missing author signature"

**Solution**: Add author header to all source files:
```c
/*
 * @file filename.c
 * @brief Description
 *
 * Author: A.R. Ansari <ansarirahim1@gmail.com>
 * Date: 2025-10-20
 */
```

## GitHub Actions Status

Check the status of workflows:
1. Go to: https://github.com/ansarirahim/esp32s3-dualusb-fw/actions
2. View workflow runs
3. Click on a run to see detailed logs

## Best Practices

1. **Always run `validate-build.sh` before pushing**
2. **Install pre-commit hook for local validation**
3. **Check GitHub Actions status after pushing**
4. **Fix errors immediately if CI/CD fails**
5. **Never force-push to main branch**
6. **Use feature branches for development**

## Continuous Improvement

The CI/CD pipeline is designed to:
- ✅ Catch errors early
- ✅ Prevent broken code from reaching users
- ✅ Maintain code quality standards
- ✅ Ensure professional delivery

If you encounter issues not covered here, update this guide!

