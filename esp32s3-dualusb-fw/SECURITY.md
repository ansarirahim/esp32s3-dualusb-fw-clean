# Security & Build Integrity

## Author Verification

All source code in this repository is authored by:
- **Name**: A.R. Ansari
- **Email**: ansarirahim1@gmail.com
- **Date**: 2025-10-20

Every source file contains the author signature in the file header.

## Code Quality Assurance

### Pre-Commit Hooks
- Verifies author signatures in all source files
- Detects trailing whitespace
- Identifies merge conflict markers
- Validates code formatting

### GitHub Actions CI/CD
- **Build Workflow** (`build.yml`):
  - Compiles firmware on every push
  - Verifies build artifacts
  - Uploads successful builds
  - Fails on compilation errors

- **Lint Workflow** (`lint.yml`):
  - Validates author signatures
  - Checks C syntax
  - Verifies code formatting
  - Ensures code quality standards

## Build Verification

Only successful builds are retained:
1. Code is committed
2. Pre-commit hooks run
3. GitHub Actions builds firmware
4. If build succeeds → artifact uploaded
5. If build fails → commit rejected

## Code Authenticity

All code in this repository is:
- Professionally authored
- Verified for quality
- Tested and validated
- Production-ready

Any code that fails quality checks will cause:
- Pre-commit hook to fail
- GitHub Actions workflow to fail
- Build to be rejected

## File Header Requirements

Every source file must include:
```c
/*
 * @file filename.c
 * @brief Description
 * 
 * Author: A.R. Ansari <ansarirahim1@gmail.com>
 * Date: 2025-10-20
 */
```

## Git Configuration

Ensure your git is configured with:
```bash
git config user.name "A.R. Ansari"
git config user.email "ansarirahim1@gmail.com"
```

## Build Process

### Local Build
```bash
cd esp32s3-dualusb-fw
idf.py set-target esp32s3
idf.py build
```

### Automated Build (GitHub Actions)
- Triggered on every push
- Runs on ubuntu-latest
- Uses ESP-IDF v5.0
- Generates build artifacts

## Verification Checklist

Before pushing code:
- [ ] Author signature in file header
- [ ] No trailing whitespace
- [ ] Code compiles locally
- [ ] Pre-commit hooks pass
- [ ] Git user configured correctly
- [ ] Code quality standards met

## Reporting Issues

If you find any security issues or code quality concerns:
1. Do not commit the code
2. Contact: ansarirahim1@gmail.com
3. Provide details of the issue
4. Include file paths and line numbers

## Compliance

This project maintains:
- ✅ 100% professionally authored code
- ✅ Clear author attribution
- ✅ Verified code quality
- ✅ Automated build verification
- ✅ Pre-commit quality checks
- ✅ CI/CD pipeline enforcement

---

**Last Updated**: 2025-10-20  
**Author**: A.R. Ansari <ansarirahim1@gmail.com>

