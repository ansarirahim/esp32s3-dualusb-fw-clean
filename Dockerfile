# ESP32-S3 Dual USB Firmware Build Environment
# Provides consistent build environment across Windows/Linux/WSL

FROM espressif/idf:v5.5.1

# Set working directory
WORKDIR /project

# Set IDF_PATH explicitly (espressif/idf image location)
ENV IDF_PATH=/opt/esp/idf

# Install additional tools
RUN apt-get update && apt-get install -y \
    git \
    curl \
    wget \
    nano \
    && rm -rf /var/lib/apt/lists/*

# Copy project files
COPY . .

# Source ESP-IDF environment and set target
RUN export IDF_PATH_FORCE=1 && . $IDF_PATH/export.sh && idf.py set-target esp32s3

# Pre-download dependencies
RUN export IDF_PATH_FORCE=1 && . $IDF_PATH/export.sh && idf.py build --dry-run || true

# Default command - source environment on entry
CMD ["/bin/bash", "-c", "export IDF_PATH_FORCE=1 && . $IDF_PATH/export.sh && /bin/bash"]

