#!/usr/bin/env python3
"""
Build all targets in the build matrix.

Usage:
  python scripts/build_all_targets.py [--target TARGET] [--clean]

Options:
  --target TARGET   Build only specific target (s3-release, s2-release, c3-release)
  --clean           Clean before building
"""

import json
import subprocess
import sys
from pathlib import Path
import shutil
import hashlib
from datetime import datetime

def load_build_matrix():
    """Load build matrix configuration."""
    with open("config/build_matrix.json") as f:
        return json.load(f)

def load_pinmaps():
    """Load pinmap configurations."""
    with open("config/pinmaps.json") as f:
        return json.load(f)

def run_command(cmd, cwd=None):
    """Run shell command and return output."""
    print(f"[RUN] {' '.join(cmd)}")
    result = subprocess.run(cmd, cwd=cwd, capture_output=True, text=True)
    if result.returncode != 0:
        print(f"[ERROR] Command failed: {result.stderr}")
        return None
    return result.stdout

def detect_target():
    """Run autodetect script."""
    result = subprocess.run(
        ["python", "scripts/detect_target.py"],
        capture_output=True,
        text=True
    )
    if result.returncode == 0:
        return json.loads(result.stdout)
    return None

def build_target(target_id, matrix_entry, clean=False):
    """Build a single target."""
    mcu = matrix_entry["mcu"]
    board = matrix_entry["board"]
    
    print(f"\n{'='*70}")
    print(f"Building: {target_id} ({board})")
    print(f"{'='*70}")
    
    # Set environment
    env_vars = {
        "TARGET_MCU": mcu,
        "TARGET_BOARD": board,
    }
    
    # Clean if requested
    if clean:
        print("[*] Cleaning build artifacts...")
        build_dir = Path("esp32s3-clean-source/build")
        if build_dir.exists():
            shutil.rmtree(build_dir)
    
    # Build
    print("[*] Building firmware...")
    cmd = ["idf.py", "build"]
    result = subprocess.run(
        cmd,
        cwd="esp32s3-clean-source",
        capture_output=True,
        text=True,
        env={**subprocess.os.environ, **env_vars}
    )
    
    if result.returncode != 0:
        print(f"[ERROR] Build failed for {target_id}")
        print(result.stderr)
        return False
    
    print(f"[OK] Build successful for {target_id}")
    return True

def collect_artifacts(target_id, board):
    """Collect build artifacts."""
    artifacts_dir = Path(f"artifacts/{target_id}")
    artifacts_dir.mkdir(parents=True, exist_ok=True)
    
    build_dir = Path("esp32s3-clean-source/build")
    
    # Copy binaries
    for pattern in ["*.bin", "*.elf", "*.map"]:
        for file in build_dir.glob(pattern):
            shutil.copy(file, artifacts_dir)
    
    # Copy build log
    if (build_dir / "build.log").exists():
        shutil.copy(build_dir / "build.log", artifacts_dir)
    
    print(f"[OK] Artifacts collected to {artifacts_dir}")
    return artifacts_dir

def generate_checksums(artifacts_dir):
    """Generate SHA256 checksums."""
    checksums = {}
    for file in artifacts_dir.glob("*"):
        if file.is_file():
            sha256 = hashlib.sha256()
            with open(file, "rb") as f:
                sha256.update(f.read())
            checksums[file.name] = sha256.hexdigest()
    
    # Write checksums file
    checksums_file = artifacts_dir / "checksums.txt"
    with open(checksums_file, "w") as f:
        for name, hash_val in sorted(checksums.items()):
            f.write(f"{hash_val}  {name}\n")
    
    print(f"[OK] Checksums written to {checksums_file}")
    return checksums

def generate_env_versions(target_id):
    """Generate environment versions file."""
    env_file = Path(f"artifacts/{target_id}/ENV_VERSIONS.txt")
    
    # Get tool versions
    idf_version = subprocess.run(
        ["idf.py", "--version"],
        capture_output=True,
        text=True
    ).stdout.strip()
    
    esptool_version = subprocess.run(
        ["esptool.py", "--version"],
        capture_output=True,
        text=True
    ).stdout.strip()
    
    with open(env_file, "w") as f:
        f.write(f"Build Timestamp: {datetime.now().isoformat()}\n")
        f.write(f"Target: {target_id}\n")
        f.write(f"IDF Version: {idf_version}\n")
        f.write(f"esptool Version: {esptool_version}\n")
    
    print(f"[OK] Environment versions written to {env_file}")

def main():
    """Main build orchestration."""
    matrix = load_build_matrix()
    
    # Parse arguments
    target_filter = None
    clean = False
    for arg in sys.argv[1:]:
        if arg.startswith("--target="):
            target_filter = arg.split("=")[1]
        elif arg == "--clean":
            clean = True
    
    # Build enabled targets
    results = {}
    for entry in matrix["matrix"]:
        if not entry["enabled"]:
            print(f"[SKIP] {entry['id']} (disabled)")
            continue
        
        if target_filter and entry["id"] != target_filter:
            continue
        
        success = build_target(entry["id"], entry, clean=clean)
        results[entry["id"]] = success
        
        if success:
            artifacts_dir = collect_artifacts(entry["id"], entry["board"])
            generate_checksums(artifacts_dir)
            generate_env_versions(entry["id"])
    
    # Print summary
    print(f"\n{'='*70}")
    print("BUILD SUMMARY")
    print(f"{'='*70}")
    for target_id, success in results.items():
        status = "✓ OK" if success else "✗ FAILED"
        print(f"{status}  {target_id}")

if __name__ == "__main__":
    main()

