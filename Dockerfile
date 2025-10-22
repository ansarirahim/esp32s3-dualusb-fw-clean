# ESP32-S3 Dual USB Firmware Build Environment
# Provides consistent build environment across Windows/Linux/WSL

FROM espressif/idf:v5.5.1

# Set working directory
WORKDIR /project

# Install additional tools
RUN apt-get update && apt-get install -y \
    git \
    curl \
    wget \
    nano \
    && rm -rf /var/lib/apt/lists/*

# Copy project files
COPY . .

# Set target
RUN idf.py set-target esp32s3

# Pre-download dependencies
RUN idf.py build --dry-run || true

# Default command
CMD ["/bin/bash"]

