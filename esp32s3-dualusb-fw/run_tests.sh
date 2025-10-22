#!/bin/bash

##############################################################################
# Unit Test Runner Script for ESP32-S3 Dual USB Firmware
#
# Author: A.R. Ansari <ansarirahim1@gmail.com>
# Date: 2025-10-21
# Version: 1.0.0
#
# Description:
# This script builds and runs unit tests for the ESP32-S3 Dual USB Firmware.
# Supports Windows (PowerShell), Linux, WSL, and macOS.
#
# Usage:
#   ./run_tests.sh [options]
#
# Options:
#   --build         Build tests only
#   --flash         Flash tests to device
#   --monitor       Monitor test output
#   --clean         Clean build artifacts
#   --help          Show this help message
#
##############################################################################

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Script directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
TEST_DIR="$SCRIPT_DIR/test"

# Functions
print_header() {
    echo -e "${BLUE}╔════════════════════════════════════════════════════════════╗${NC}"
    echo -e "${BLUE}║   ESP32-S3 Dual USB Firmware - Unit Test Runner            ║${NC}"
    echo -e "${BLUE}║   Author: A.R. Ansari <ansarirahim1@gmail.com>            ║${NC}"
    echo -e "${BLUE}╚════════════════════════════════════════════════════════════╝${NC}"
    echo ""
}

print_success() {
    echo -e "${GREEN}✓ $1${NC}"
}

print_error() {
    echo -e "${RED}✗ $1${NC}"
}

print_info() {
    echo -e "${YELLOW}ℹ $1${NC}"
}

print_help() {
    echo "Usage: $0 [options]"
    echo ""
    echo "Options:"
    echo "  --build         Build tests only"
    echo "  --flash         Flash tests to device"
    echo "  --monitor       Monitor test output"
    echo "  --clean         Clean build artifacts"
    echo "  --help          Show this help message"
    echo ""
    echo "Examples:"
    echo "  $0 --build                    # Build tests"
    echo "  $0 --build --flash --monitor  # Build, flash, and monitor"
    echo "  $0 --clean                    # Clean build artifacts"
}

build_tests() {
    print_info "Building unit tests..."
    
    cd "$SCRIPT_DIR"
    
    # Set target
    print_info "Setting target to ESP32-S3..."
    idf.py set-target esp32s3
    
    # Build
    print_info "Building tests..."
    idf.py build
    
    print_success "Tests built successfully"
}

flash_tests() {
    print_info "Flashing tests to device..."
    
    cd "$SCRIPT_DIR"
    idf.py flash
    
    print_success "Tests flashed successfully"
}

monitor_tests() {
    print_info "Monitoring test output..."
    print_info "Press Ctrl+C to exit"
    echo ""
    
    cd "$SCRIPT_DIR"
    idf.py monitor
}

clean_tests() {
    print_info "Cleaning build artifacts..."
    
    cd "$SCRIPT_DIR"
    
    if [ -d "build" ]; then
        rm -rf build
        print_success "Build directory removed"
    fi
    
    if [ -d "test/build" ]; then
        rm -rf test/build
        print_success "Test build directory removed"
    fi
    
    print_success "Clean completed"
}

# Main script
main() {
    print_header
    
    # Parse arguments
    BUILD=false
    FLASH=false
    MONITOR=false
    CLEAN=false
    
    while [[ $# -gt 0 ]]; do
        case $1 in
            --build)
                BUILD=true
                shift
                ;;
            --flash)
                FLASH=true
                shift
                ;;
            --monitor)
                MONITOR=true
                shift
                ;;
            --clean)
                CLEAN=true
                shift
                ;;
            --help)
                print_help
                exit 0
                ;;
            *)
                print_error "Unknown option: $1"
                print_help
                exit 1
                ;;
        esac
    done
    
    # If no options, show help
    if [ "$BUILD" = false ] && [ "$FLASH" = false ] && [ "$MONITOR" = false ] && [ "$CLEAN" = false ]; then
        print_help
        exit 0
    fi
    
    # Execute actions
    if [ "$CLEAN" = true ]; then
        clean_tests
    fi
    
    if [ "$BUILD" = true ]; then
        build_tests
    fi
    
    if [ "$FLASH" = true ]; then
        flash_tests
    fi
    
    if [ "$MONITOR" = true ]; then
        monitor_tests
    fi
    
    echo ""
    print_success "Test runner completed"
}

# Run main function
main "$@"

