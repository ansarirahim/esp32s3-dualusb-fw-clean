# Code Documentation Guide

## Overview

This project uses **Doxygen** for comprehensive code documentation. All source files include professional Doxygen comments with author information, function documentation, and usage examples.

---

## Documentation Structure

### File Headers

Every source file (.c) and header file (.h) includes:

```c
/**
 * @file filename.c
 * @brief Brief description of file purpose
 *
 * @author A.R. Ansari <ansarirahim1@gmail.com>
 * @date 2025-10-21
 * @version 1.0.0
 *
 * @section description Description
 * Detailed description of file functionality...
 *
 * @section features Features
 * - Feature 1
 * - Feature 2
 *
 * @section contact Contact
 * - Email: ansarirahim1@gmail.com
 * - Phone: +91 9024304883
 * - GitHub: https://github.com/ansarirahim
 *
 * @section license License
 * Copyright (c) 2025 A.R. Ansari. All rights reserved.
 */
```

### Function Documentation

Every function includes comprehensive Doxygen documentation:

```c
/**
 * @brief Brief function description
 *
 * Detailed description of what the function does,
 * how it works, and any important notes.
 *
 * @param[in] param1 Description of input parameter
 * @param[out] param2 Description of output parameter
 * @param[in,out] param3 Description of input/output parameter
 *
 * @return Description of return value
 * @retval true Success condition
 * @retval false Failure condition
 *
 * @details
 * Additional implementation details...
 *
 * @note Important notes about the function
 * @warning Warnings about usage
 * @see related_function()
 */
```

### Enumeration Documentation

```c
/**
 * @enum led_state_t
 * @brief LED State Enumeration
 *
 * Defines possible LED states with associated blinking patterns.
 */
typedef enum {
    LED_STATE_IDLE,   /**< Device idle: slow blink */
    LED_STATE_BUSY,   /**< Device active: fast blink */
    LED_STATE_ERROR,  /**< Error condition: solid then blink */
} led_state_t;
```

### Macro Documentation

```c
/** @defgroup usb_pins USB PHY Pins
 * @brief USB PHY pin definitions
 * @{
 */
/** @brief USB Data+ pin (GPIO20) */
#define PIN_USB_DP        20
/** @brief USB Data- pin (GPIO19) */
#define PIN_USB_DM        19
/** @} */
```

---

## Generating Documentation

### Prerequisites

Install Doxygen:

```bash
# Ubuntu/Debian
sudo apt-get install doxygen graphviz

# macOS
brew install doxygen graphviz

# Windows
# Download from https://www.doxygen.nl/download.html
```

### Generate HTML Documentation

```bash
cd esp32s3-dualusb-fw
doxygen Doxyfile
```

Documentation will be generated in `docs/doxygen/html/`

### View Documentation

```bash
# Open in browser
open docs/doxygen/html/index.html          # macOS
xdg-open docs/doxygen/html/index.html      # Linux
start docs/doxygen/html/index.html         # Windows
```

### Generate LaTeX/PDF Documentation

```bash
cd esp32s3-dualusb-fw/docs/doxygen/latex
make
# PDF will be generated as refman.pdf
```

---

## Documentation Standards

### Author Information

All files include author information:
- **Name**: A.R. Ansari
- **Email**: ansarirahim1@gmail.com
- **Phone**: +91 9024304883
- **GitHub**: https://github.com/ansarirahim
- **LinkedIn**: https://www.linkedin.com/in/abdul-raheem-ansari-a6871320/

### Professional Competencies

Documented in `AUTHOR.md`:
- Analog & Digital Hardware Design
- PCB Layout & Validation
- IoT Systems & BLE/Wi-Fi
- Embedded & Bare Metal Firmware
- RTOS & Linux Device Drivers
- Kernel Programming & Yocto Build System

### Code Quality Standards

- **Comprehensive Comments**: Every function documented
- **Parameter Documentation**: All parameters described
- **Return Value Documentation**: All return values documented
- **Usage Examples**: Code examples in documentation
- **Cross-References**: Related functions linked
- **Error Handling**: Error conditions documented

---

## File Documentation

### main.c
- Application entry point
- System initialization sequence
- Subsystem initialization order

### usb_device.c / usb_device.h
- USB Device Mode (MSC) implementation
- TinyUSB MSC driver integration
- Block device operations
- I/O activity monitoring

### filesystem.c / filesystem.h
- Internal FATFS filesystem
- SPI flash mount operations
- File operations and statistics
- Thread-safe access

### led_control.c / led_control.h
- LED status indication
- Blinking pattern definitions
- State machine implementation
- GPIO control

### board_pins.h
- Hardware pin definitions
- USB PHY pin mapping
- LED pin assignments
- Boot pin configuration

---

## Doxygen Configuration

The `Doxyfile` includes:

- **Project Information**
  - Project name: ESP32-S3 Dual USB Firmware
  - Version: 1.0.0
  - Author: A.R. Ansari

- **Output Formats**
  - HTML with search engine
  - LaTeX/PDF
  - XML
  - Man pages

- **Features**
  - Source code browsing
  - Call graphs
  - Dependency graphs
  - Class diagrams
  - Include graphs

- **Extraction Options**
  - Extract private members
  - Extract static members
  - Extract local classes
  - Extract anonymous namespaces

---

## Documentation Best Practices

### When Adding New Functions

1. Add comprehensive Doxygen header comment
2. Document all parameters with @param
3. Document return value with @return/@retval
4. Add @note for important information
5. Add @see for related functions
6. Include usage examples if complex

### When Modifying Existing Code

1. Update function documentation if signature changes
2. Update parameter descriptions if behavior changes
3. Update return value documentation if needed
4. Add @note about changes if significant

### When Adding New Files

1. Create comprehensive file header
2. Include author information
3. Include section descriptions
4. Document all public functions
5. Document all public structures/enums

---

## Viewing Documentation

### Online
- Generated HTML documentation in `docs/doxygen/html/`
- Search functionality available
- Cross-referenced links

### In IDE
- Most IDEs show Doxygen comments in hover tooltips
- VS Code: Install Doxygen Documentation Generator extension

### Command Line
```bash
# View function documentation
grep -A 20 "@brief function_name" main/*.c

# Generate and view
doxygen Doxyfile && open docs/doxygen/html/index.html
```

---

## Author Information

**A.R. Ansari**
- Email: ansarirahim1@gmail.com
- Phone: +91 9024304883
- GitHub: https://github.com/ansarirahim
- LinkedIn: https://www.linkedin.com/in/abdul-raheem-ansari-a6871320/

**Specialization**
- 20+ years in embedded systems
- IoT, BLE/Wi-Fi, Linux internals
- Hardware design and PCB layout
- RTOS and kernel programming

---

## License

Copyright (c) 2025 A.R. Ansari. All rights reserved.

---

## Additional Resources

- [Doxygen Manual](https://www.doxygen.nl/manual/index.html)
- [Doxygen Commands](https://www.doxygen.nl/manual/commands.html)
- [Markdown Support](https://www.doxygen.nl/manual/markdown.html)

---

**Last Updated**: 2025-10-21

**Documentation Version**: 1.0.0

