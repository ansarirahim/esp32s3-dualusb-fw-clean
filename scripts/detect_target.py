#!/usr/bin/env python3
"""
Autodetect MCU/board target and feature flags.

Priority:
  1. Manual override (TARGET_MCU env var)
  2. board.json file
  3. Tool probe (idf.py --version, esptool.py chip_id)
  4. Fallback to esp32s3

Output: JSON with {mcu, board, flags, sdk_version, tool_versions}
"""

import json
import os
import sys
import subprocess
from pathlib import Path

# MCU Capabilities Matrix
MCU_MATRIX = {
    "esp32s3": {
        "sdk_min": "5.2.1",
        "usb_otg": True,
        "rmt": True,
        "psram": True,
        "boards": ["esp32s3-devkitc-1"],
        "default_board": "esp32s3-devkitc-1",
    },
    "esp32s2": {
        "sdk_min": "4.4.6",
        "usb_otg": True,
        "rmt": False,
        "psram": False,
        "boards": ["esp32s2-saola-1"],
        "default_board": "esp32s2-saola-1",
    },
    "esp32c3": {
        "sdk_min": "5.1.2",
        "usb_otg": False,
        "rmt": True,
        "psram": False,
        "boards": ["esp32c3-devkitm-1"],
        "default_board": "esp32c3-devkitm-1",
    },
    "esp32": {
        "sdk_min": "4.4.6",
        "usb_otg": False,
        "rmt": True,
        "psram": "optional",
        "boards": ["esp32-wrover-kit"],
        "default_board": "esp32-wrover-kit",
    },
}

# Board to MCU Mapping
BOARD_MCU_MAP = {
    "esp32s3-devkitc-1": "esp32s3",
    "esp32s2-saola-1": "esp32s2",
    "esp32c3-devkitm-1": "esp32c3",
    "esp32-wrover-kit": "esp32",
}

# Pinmap Profiles
PINMAPS = {
    "esp32s3-devkitc-1": {
        "LED_DI": 6,
        "WS2812_DI": 38,
        "USB_DP": 20,
        "USB_DM": 19,
    },
    "esp32s2-saola-1": {
        "LED_DI": 18,
        "WS2812_DI": 33,
        "USB_DP": 20,
        "USB_DM": 19,
    },
    "esp32c3-devkitm-1": {
        "LED_DI": 2,
        "WS2812_DI": 7,
    },
}


def get_env_override():
    """Check for manual override via TARGET_MCU env var."""
    mcu = os.getenv("TARGET_MCU")
    if mcu and mcu in MCU_MATRIX:
        return mcu
    return None


def read_board_json():
    """Read board.json if it exists."""
    board_file = Path("boards/board.json")
    if board_file.exists():
        try:
            with open(board_file) as f:
                data = json.load(f)
                board_name = data.get("name")
                if board_name in BOARD_MCU_MAP:
                    return BOARD_MCU_MAP[board_name]
        except Exception as e:
            print(f"Warning: Failed to read board.json: {e}", file=sys.stderr)
    return None


def probe_idf_version():
    """Get IDF version via idf.py --version."""
    try:
        result = subprocess.run(
            ["idf.py", "--version"],
            capture_output=True,
            text=True,
            timeout=5,
        )
        if result.returncode == 0:
            return result.stdout.strip()
    except Exception:
        pass
    return "unknown"


def probe_esptool_version():
    """Get esptool version."""
    try:
        result = subprocess.run(
            ["esptool.py", "--version"],
            capture_output=True,
            text=True,
            timeout=5,
        )
        if result.returncode == 0:
            return result.stdout.strip()
    except Exception:
        pass
    return "unknown"


def get_feature_flags(mcu):
    """Generate feature flags for MCU."""
    caps = MCU_MATRIX[mcu]
    flags = {
        "feature_usb_otg": "on" if caps["usb_otg"] else "off",
        "use_rmt": "on" if caps["rmt"] else "off",
        "ws2812_driver": "rmt" if caps["rmt"] else "bitbang",
        "psram": "on" if caps["psram"] is True else ("optional" if caps["psram"] == "optional" else "off"),
        "pinmap_profile": "default",
    }
    return flags


def main():
    """Main detection logic."""
    # Priority 1: Manual override
    mcu = get_env_override()
    if mcu:
        print(f"[detect] Using manual override: TARGET_MCU={mcu}", file=sys.stderr)
    
    # Priority 2: board.json
    if not mcu:
        mcu = read_board_json()
        if mcu:
            print(f"[detect] Detected from board.json: {mcu}", file=sys.stderr)
    
    # Priority 3: Fallback
    if not mcu:
        mcu = "esp32s3"
        print(f"[detect] Using fallback: {mcu}", file=sys.stderr)
    
    # Get board
    board = MCU_MATRIX[mcu]["default_board"]
    
    # Get feature flags
    flags = get_feature_flags(mcu)
    
    # Get pinmap
    pinmap = PINMAPS.get(board, {})
    
    # Get tool versions
    idf_version = probe_idf_version()
    esptool_version = probe_esptool_version()
    
    # Build output
    output = {
        "mcu": mcu,
        "board": board,
        "flags": flags,
        "pinmap": pinmap,
        "sdk_version": idf_version,
        "esptool_version": esptool_version,
        "mcu_capabilities": MCU_MATRIX[mcu],
    }
    
    # Output JSON
    print(json.dumps(output, indent=2))


if __name__ == "__main__":
    main()

