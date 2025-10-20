# ESP32-S3 Dual USB Firmware

Firmware for ESP32-S3 supporting both USB Mass Storage (Device) and USB Host modes using ESP-IDF and TinyUSB.

## Build
idf.py set-target esp32s3
idf.py menuconfig
idf.py build
idf.py flash -p <PORT>
idf.py monitor
