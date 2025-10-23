@echo off
REM ============================================================================
REM ESP32-S3 Dual USB Firmware - Build & Flash Script (Windows - Admin)
REM Runs build.bat with elevated privileges for Docker
REM ============================================================================

setlocal enabledelayedexpansion

REM Get the directory where this script is located
set SCRIPT_DIR=%~dp0
cd /d "%SCRIPT_DIR%"

REM Check if running as administrator
net session >nul 2>&1
if %errorlevel% neq 0 (
    echo.
    echo [*] Requesting administrator privileges...
    echo.
    
    REM Re-run this script with elevated privileges
    powershell -Command "Start-Process cmd -ArgumentList '/c cd /d \"%cd%\" && build.bat %*' -Verb RunAs"
    exit /b 0
)

REM If we get here, we're running as admin
call build.bat %*

