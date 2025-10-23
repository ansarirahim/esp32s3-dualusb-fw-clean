#!/bin/bash

################################################################################
# ESP32-S3 Dual USB Firmware - Build & Flash Script (Linux/Mac)
# Machine-independent, user-independent, path-independent
################################################################################

# Get the directory where this script is located
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd "$SCRIPT_DIR"

# Colors for output
GREEN='\033[92m'
YELLOW='\033[93m'
RED='\033[91m'
RESET='\033[0m'

echo ""
echo "============================================================================"
echo "ESP32-S3 Dual USB Firmware - Build & Flash"
echo "============================================================================"
echo ""

# Parse command line arguments FIRST (before Docker check)
COMMAND="$1"

# Allow help without Docker
if [ "$COMMAND" = "help" ]; then
    echo "Usage: ./build.sh [command]"
    echo ""
    echo "Commands:"
    echo "  build       - Build firmware"
    echo "  flash       - Flash to device"
    echo "  monitor     - Monitor serial output"
    echo "  test        - Run tests"
    echo "  clean       - Clean build artifacts"
    echo "  erase       - Erase flash memory"
    echo "  full        - Build and flash"
    echo "  help        - Show this help"
    echo ""
    exit 0
fi

# Allow no arguments to show usage without Docker
if [ $# -eq 0 ]; then
    echo "Usage: ./build.sh [command]"
    echo ""
    echo "Commands:"
    echo "  build       - Build firmware"
    echo "  flash       - Flash to device"
    echo "  monitor     - Monitor serial output"
    echo "  test        - Run tests"
    echo "  clean       - Clean build artifacts"
    echo "  erase       - Erase flash memory"
    echo "  full        - Build and flash"
    echo "  help        - Show this help"
    echo ""
    echo "Examples:"
    echo "  ./build.sh build"
    echo "  ./build.sh flash"
    echo "  ./build.sh full"
    echo ""
    exit 0
fi

# NOW check if docker is available (after help/no-args)
if ! command -v docker &> /dev/null; then
    echo -e "${RED}ERROR: Docker is not installed or not in PATH${RESET}"
    echo ""
    echo "Please install Docker from: https://www.docker.com/products/docker-desktop"
    echo ""
    exit 1
fi

echo -e "${GREEN}✓ Docker found${RESET}"
docker --version
echo ""

# Check if docker-compose.yml exists
if [ ! -f "docker-compose.yml" ]; then
    echo -e "${RED}ERROR: docker-compose.yml not found in current directory${RESET}"
    echo "Current directory: $(pwd)"
    echo ""
    exit 1
fi

echo -e "${GREEN}✓ docker-compose.yml found${RESET}"
echo ""

# Now execute the command
case "$COMMAND" in
    help)
        echo "Usage: ./build.sh [command]"
        echo ""
        echo "Commands:"
        echo "  build       - Build firmware"
        echo "  flash       - Flash to device"
        echo "  monitor     - Monitor serial output"
        echo "  test        - Run tests"
        echo "  clean       - Clean build artifacts"
        echo "  erase       - Erase flash memory"
        echo "  full        - Build and flash"
        echo "  help        - Show this help"
        echo ""
        ;;
    build)
        echo -e "${YELLOW}Building Docker image...${RESET}"
        docker compose build --no-cache
        if [ $? -ne 0 ]; then
            echo -e "${RED}Docker build failed${RESET}"
            exit 1
        fi
        echo -e "${YELLOW}Building firmware...${RESET}"
        docker compose run --rm esp32 bash -c "export IDF_PATH_FORCE=1 && . \$IDF_PATH/export.sh && idf.py build"
        if [ $? -ne 0 ]; then
            echo -e "${RED}Build failed${RESET}"
            exit 1
        fi
        echo -e "${GREEN}✓ Build successful${RESET}"
        ;;
    flash)
        echo -e "${YELLOW}Flashing firmware...${RESET}"
        docker compose run --rm esp32 bash -c "export IDF_PATH_FORCE=1 && . \$IDF_PATH/export.sh && idf.py flash"
        if [ $? -ne 0 ]; then
            echo -e "${RED}Flash failed${RESET}"
            exit 1
        fi
        echo -e "${GREEN}✓ Flash successful${RESET}"
        ;;
    monitor)
        echo -e "${YELLOW}Starting serial monitor...${RESET}"
        docker compose run --rm esp32 bash -c "export IDF_PATH_FORCE=1 && . \$IDF_PATH/export.sh && idf.py monitor"
        ;;
    test)
        echo -e "${YELLOW}Running tests...${RESET}"
        docker compose run --rm esp32 bash -c "export IDF_PATH_FORCE=1 && . \$IDF_PATH/export.sh && idf.py test"
        if [ $? -ne 0 ]; then
            echo -e "${RED}Tests failed${RESET}"
            exit 1
        fi
        echo -e "${GREEN}✓ Tests passed${RESET}"
        ;;
    clean)
        echo -e "${YELLOW}Cleaning build artifacts...${RESET}"
        docker compose run --rm esp32 bash -c "export IDF_PATH_FORCE=1 && . \$IDF_PATH/export.sh && idf.py fullclean"
        if [ $? -ne 0 ]; then
            echo -e "${RED}Clean failed${RESET}"
            exit 1
        fi
        echo -e "${GREEN}✓ Clean successful${RESET}"
        ;;
    erase)
        echo -e "${YELLOW}Erasing flash memory...${RESET}"
        docker compose run --rm esp32 bash -c "export IDF_PATH_FORCE=1 && . \$IDF_PATH/export.sh && idf.py erase-flash"
        if [ $? -ne 0 ]; then
            echo -e "${RED}Erase failed${RESET}"
            exit 1
        fi
        echo -e "${GREEN}✓ Erase successful${RESET}"
        ;;
    full)
        echo -e "${YELLOW}Building Docker image...${RESET}"
        docker compose build --no-cache
        if [ $? -ne 0 ]; then
            echo -e "${RED}Docker build failed${RESET}"
            exit 1
        fi
        echo -e "${YELLOW}Building and flashing...${RESET}"
        docker compose run --rm esp32 bash -c "export IDF_PATH_FORCE=1 && . \$IDF_PATH/export.sh && idf.py build && idf.py flash"
        if [ $? -ne 0 ]; then
            echo -e "${RED}Build or flash failed${RESET}"
            exit 1
        fi
        echo -e "${GREEN}✓ Build and flash successful${RESET}"
        ;;
    *)
        echo -e "${RED}Unknown command: $COMMAND${RESET}"
        echo "Run './build.sh help' for usage information"
        exit 1
        ;;
esac

exit 0

