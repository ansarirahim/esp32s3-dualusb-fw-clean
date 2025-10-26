#!/usr/bin/env python3
"""
Package release artifacts into ZIP with checksums and SBOM.

Usage:
  python scripts/package_release.py [--output FILENAME]
"""

import json
import hashlib
import zipfile
from pathlib import Path
from datetime import datetime
import sys

def calculate_sha256(file_path):
    """Calculate SHA256 checksum of a file."""
    sha256 = hashlib.sha256()
    with open(file_path, "rb") as f:
        sha256.update(f.read())
    return sha256.hexdigest()

def create_sbom():
    """Create Software Bill of Materials."""
    sbom = {
        "bomFormat": "CycloneDX",
        "specVersion": "1.4",
        "version": 1,
        "metadata": {
            "timestamp": datetime.now().isoformat(),
            "tools": [
                {
                    "vendor": "Espressif",
                    "name": "ESP-IDF",
                    "version": "5.2.1"
                },
                {
                    "vendor": "Espressif",
                    "name": "esptool.py",
                    "version": "4.7.0"
                },
                {
                    "vendor": "Espressif",
                    "name": "TinyUSB",
                    "version": "2.0.1"
                }
            ]
        },
        "components": [
            {
                "type": "application",
                "name": "ESP32-S3 Dual USB Firmware",
                "version": "1.0.0",
                "description": "Multi-MCU firmware with USB Device and Host support",
                "targets": [
                    {
                        "mcu": "esp32s3",
                        "board": "esp32s3-devkitc-1",
                        "features": ["usb_device", "usb_host", "rmt", "psram", "fatfs"]
                    },
                    {
                        "mcu": "esp32s2",
                        "board": "esp32s2-saola-1",
                        "features": ["usb_device", "fatfs"]
                    },
                    {
                        "mcu": "esp32c3",
                        "board": "esp32c3-devkitm-1",
                        "features": ["rmt", "fatfs"]
                    }
                ]
            }
        ]
    }
    return sbom

def create_env_versions():
    """Create environment versions file."""
    env_info = f"""Build Information
=================
Timestamp: {datetime.now().isoformat()}
Build System: ESP-IDF v5.2.1
esptool.py: v4.7.0
TinyUSB: v2.0.1

Target Configurations
====================
ESP32-S3 (esp32s3-devkitc-1):
  - USB Device Mode: Enabled
  - USB Host Mode: Enabled
  - RMT: Enabled
  - PSRAM: Enabled
  - FATFS: Enabled

ESP32-S2 (esp32s2-saola-1):
  - USB Device Mode: Enabled
  - USB Host Mode: Disabled
  - RMT: Disabled
  - PSRAM: Disabled
  - FATFS: Enabled

ESP32-C3 (esp32c3-devkitm-1):
  - USB Device Mode: Disabled
  - USB Host Mode: Disabled
  - RMT: Enabled
  - PSRAM: Disabled
  - FATFS: Enabled
"""
    return env_info

def package_release(output_file="Release_Artifacts.zip"):
    """Create release package."""
    print(f"[*] Creating release package: {output_file}")
    
    build_dir = Path("esp32s3-clean-source/build")
    release_dir = Path("release_artifacts")
    release_dir.mkdir(exist_ok=True)
    
    # Collect artifacts
    artifacts = []
    checksums = {}
    
    # Copy binaries
    for pattern in ["*.bin", "*.elf"]:
        for file in build_dir.glob(pattern):
            dest = release_dir / file.name
            import shutil
            shutil.copy(file, dest)
            artifacts.append(file.name)
            checksums[file.name] = calculate_sha256(dest)
            print(f"[+] {file.name} ({file.stat().st_size} bytes)")
    
    # Create SBOM
    sbom = create_sbom()
    sbom_file = release_dir / "SBOM.json"
    with open(sbom_file, "w") as f:
        json.dump(sbom, f, indent=2)
    artifacts.append("SBOM.json")
    print(f"[+] SBOM.json")
    
    # Create ENV_VERSIONS.txt
    env_file = release_dir / "ENV_VERSIONS.txt"
    with open(env_file, "w") as f:
        f.write(create_env_versions())
    artifacts.append("ENV_VERSIONS.txt")
    print(f"[+] ENV_VERSIONS.txt")
    
    # Create checksums.txt
    checksums_file = release_dir / "checksums.txt"
    with open(checksums_file, "w") as f:
        for name in sorted(checksums.keys()):
            f.write(f"{checksums[name]}  {name}\n")
    artifacts.append("checksums.txt")
    print(f"[+] checksums.txt")
    
    # Create ZIP
    print(f"\n[*] Creating ZIP archive...")
    with zipfile.ZipFile(output_file, "w", zipfile.ZIP_DEFLATED) as zf:
        for artifact in artifacts:
            file_path = release_dir / artifact
            zf.write(file_path, arcname=artifact)
            print(f"[+] Added {artifact}")
    
    zip_size = Path(output_file).stat().st_size
    print(f"\n[OK] Release package created: {output_file} ({zip_size} bytes)")
    
    # Cleanup
    import shutil
    shutil.rmtree(release_dir)
    
    return output_file

if __name__ == "__main__":
    output = "Release_Artifacts.zip"
    if len(sys.argv) > 1 and sys.argv[1].startswith("--output="):
        output = sys.argv[1].split("=")[1]
    
    package_release(output)

