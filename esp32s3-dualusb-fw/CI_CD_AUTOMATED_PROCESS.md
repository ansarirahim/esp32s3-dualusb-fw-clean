# Automated CI/CD Build Process

## Overview

This document describes the automated build process that runs on every push to the repository.

## Process Flow

```
Developer Push → GitHub Actions → Build → Test → Artifacts → Auto-Commit → Success
                                    ↓
                                  FAIL → Notify
```

## What Happens Automatically

### 1. Build Trigger
- Every push to `main`, `master`, `develop`, or `feat/*` branches
- Every pull request to `main`, `master`, or `develop`

### 2. Build Process
```bash
cd esp32s3-dualusb-fw
idf.py set-target esp32s3
idf.py fullclean
idf.py build 2>&1 | tee build.log
```

### 3. Verification
- ✅ Check for compilation errors
- ✅ Verify build artifacts exist
- ✅ Validate binary size
- ✅ Archive build logs

### 4. Artifacts Generated
- `firmware-binary` - The compiled .bin file
- `build-logs` - Complete build output
- `build-report` - Detailed build report with timestamp

### 5. Auto-Commit on Success
If build succeeds on `main` branch:
- Commits build artifacts
- Commits build logs
- Commits build report
- Auto-pushes to repository

### 6. Notification
- ✅ Success: Build artifacts available
- ❌ Failure: Build logs available for debugging

## Build Artifacts Location

After successful build, artifacts are available in GitHub Actions:

1. **firmware-binary** - Ready to flash
2. **build-logs** - Complete build output
3. **build-report** - Timestamped report

## Build Report Contents

```
BUILD REPORT
============
Date: YYYY-MM-DD HH:MM:SS UTC
Commit: <commit-hash>
Branch: <branch-name>
Status: SUCCESS
Artifact Size: <bytes>

Build Log:
<complete build output>
```

## Accessing Build Artifacts

1. Go to GitHub repository
2. Click "Actions" tab
3. Select latest workflow run
4. Download artifacts from "Artifacts" section

## Build Logs

Complete build logs are archived for 90 days:
- Available in GitHub Actions artifacts
- Searchable for errors/warnings
- Timestamped for tracking

## Automatic Push on Success

When build succeeds on `main` branch:
- Build artifacts are committed
- Build logs are committed
- Build report is committed
- Changes are automatically pushed

This ensures:
- ✅ Build proof is in repository
- ✅ No manual push needed
- ✅ Automatic confirmation
- ✅ Complete audit trail

## Failure Handling

If build fails:
- ❌ Build stops immediately
- ❌ Error details in logs
- ❌ No artifacts committed
- ❌ No push occurs
- ❌ Developer must fix and retry

## ESP-IDF Version

Current version: **v5.4.1**

This matches the development environment and ensures consistency.

## Build Configuration

- Target: ESP32-S3
- Clean build: Yes (fullclean before build)
- Artifact retention: 90 days
- Log retention: 90 days

## Verification Steps

The CI/CD automatically verifies:
1. ✅ No compilation errors
2. ✅ Binary file exists
3. ✅ Binary size is reasonable
4. ✅ Build logs are captured
5. ✅ Artifacts are archived

## Status Indicators

- 🟢 **GREEN**: Build successful, artifacts available
- 🔴 **RED**: Build failed, check logs

## Next Steps

1. Push code to repository
2. GitHub Actions automatically builds
3. Check Actions tab for status
4. Download artifacts if needed
5. Build artifacts auto-committed on success

## Troubleshooting

If build fails:
1. Check build logs in GitHub Actions
2. Look for error messages
3. Fix issues locally
4. Push again to trigger rebuild

## Notes

- No manual build confirmation needed
- No manual push needed on success
- Complete audit trail in repository
- Build proof always available

