# ESP32-S3 Dual USB Firmware - Hardware Wiring

## Board: ESP32-S3 DevKitC-1

### USB Pins (Native PHY)
- **D+ (GPIO20)**: USB Data+ line
- **D- (GPIO19)**: USB Data- line

These pins are connected to the native USB PHY on the ESP32-S3 and do not require external configuration.

### LED Pins
- **Red LED (GPIO6)**: Status indicator for Device Mode
  - Slow blink (500ms ON / 1500ms OFF): Device idle
  - Fast blink (200ms ON / 200ms OFF): Device active I/O
  - Solid 3s then slow blink: Error state

### Boot/Mode Selection
- **BOOT1 (GPIO0)**: Reserved for future mode selection (Device vs Host)
  - Currently read-only in Milestone 1
  - Will be used in M2 to select between Device and Host modes at boot

## Pin Summary

| Function | GPIO | Notes |
|----------|------|-------|
| USB D+ | 20 | Native PHY |
| USB D- | 19 | Native PHY |
| LED Red | 6 | Status indicator |
| BOOT1 | 0 | Mode select (future) |

## Connections

### USB Connection
Connect the ESP32-S3 to a PC via USB-C cable. The native USB PHY will handle the connection.

### LED Connection
Connect a red LED with appropriate current-limiting resistor (typically 220Ω) between GPIO6 and GND.

## Troubleshooting

If the device doesn't enumerate:
1. Check USB cable connection
2. Verify GPIO20/GPIO19 are not used by other peripherals
3. Check sdkconfig for USB device configuration
4. Review serial logs for initialization errors

If LED doesn't blink:
1. Verify GPIO6 is available on your board
2. Check LED polarity (anode to GPIO6, cathode to GND)
3. Verify resistor value (220Ω typical)
4. Check serial logs for LED initialization

