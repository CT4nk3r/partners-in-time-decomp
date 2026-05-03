#!/usr/bin/env python3
"""
ROM Extraction Script for Mario & Luigi: Partners in Time (NDS)

Extracts the NDS ROM into its component parts using ndstool:
  - ARM9 binary (main game code)
  - ARM7 binary (audio/wifi subsystem)
  - Overlay binaries (dynamically loaded code)
  - Filesystem data (graphics, sound, maps, etc.)
  - Header and metadata

Usage:
    python tools/scripts/extract_rom.py [path_to_rom]

If no path is given, defaults to rom/baserom.nds
"""

import os
import sys
import subprocess
import struct
import hashlib
from pathlib import Path

PROJECT_ROOT = Path(__file__).resolve().parent.parent.parent

ROM_PATH_DEFAULT = PROJECT_ROOT / "rom" / "baserom.nds"

EXTRACT_DIR = PROJECT_ROOT / "extracted"

# Known SHA256 hashes for verifying ROM versions
KNOWN_HASHES = {
    # USA version
    "usa": None,  # TODO: fill in once verified
    # EUR version
    "eur": None,
    # JPN version
    "jpn": None,
}

# NDS game codes for Mario & Luigi: Partners in Time
KNOWN_GAME_CODES = [
    "AYWE",  # USA (original)
    "AYWP",  # EUR (original)
    "AYWJ",  # JPN (original)
    "ARME",  # USA
    "ARMP",  # EUR
]


def sha256_file(filepath: Path) -> str:
    h = hashlib.sha256()
    with open(filepath, "rb") as f:
        while True:
            chunk = f.read(8192)
            if not chunk:
                break
            h.update(chunk)
    return h.hexdigest()


def read_nds_header(filepath: Path) -> dict:
    """Read basic NDS ROM header fields."""
    with open(filepath, "rb") as f:
        data = f.read(0x200)

    header = {}
    header["title"] = data[0x00:0x0C].decode("ascii", errors="replace").rstrip("\x00")
    header["game_code"] = data[0x0C:0x10].decode("ascii", errors="replace")
    header["maker_code"] = data[0x10:0x12].decode("ascii", errors="replace")

    header["arm9_rom_offset"] = struct.unpack_from("<I", data, 0x20)[0]
    header["arm9_entry_address"] = struct.unpack_from("<I", data, 0x24)[0]
    header["arm9_ram_address"] = struct.unpack_from("<I", data, 0x28)[0]
    header["arm9_size"] = struct.unpack_from("<I", data, 0x2C)[0]

    header["arm7_rom_offset"] = struct.unpack_from("<I", data, 0x30)[0]
    header["arm7_entry_address"] = struct.unpack_from("<I", data, 0x34)[0]
    header["arm7_ram_address"] = struct.unpack_from("<I", data, 0x38)[0]
    header["arm7_size"] = struct.unpack_from("<I", data, 0x3C)[0]

    header["fnt_offset"] = struct.unpack_from("<I", data, 0x40)[0]
    header["fnt_size"] = struct.unpack_from("<I", data, 0x44)[0]
    header["fat_offset"] = struct.unpack_from("<I", data, 0x48)[0]
    header["fat_size"] = struct.unpack_from("<I", data, 0x4C)[0]

    header["arm9_overlay_offset"] = struct.unpack_from("<I", data, 0x50)[0]
    header["arm9_overlay_size"] = struct.unpack_from("<I", data, 0x54)[0]
    header["arm7_overlay_offset"] = struct.unpack_from("<I", data, 0x58)[0]
    header["arm7_overlay_size"] = struct.unpack_from("<I", data, 0x5C)[0]

    header["rom_size"] = struct.unpack_from("<I", data, 0x80)[0]
    header["header_size"] = struct.unpack_from("<I", data, 0x84)[0]

    return header


def verify_rom(rom_path: Path) -> dict:
    """Verify the ROM is a valid Mario & Luigi: Partners in Time ROM."""
    if not rom_path.exists():
        print(f"ERROR: ROM file not found at: {rom_path}")
        print(f"Please place your legally-obtained ROM at: {rom_path}")
        sys.exit(1)

    header = read_nds_header(rom_path)

    print(f"ROM Title:      {header['title']}")
    print(f"Game Code:      {header['game_code']}")
    print(f"Maker Code:     {header['maker_code']}")
    print()

    if header["game_code"] not in KNOWN_GAME_CODES:
        print(f"WARNING: Game code '{header['game_code']}' not recognized as Mario & Luigi: Partners in Time")
        print(f"Expected one of: {KNOWN_GAME_CODES}")
        response = input("Continue anyway? (y/n): ").strip().lower()
        if response != "y":
            sys.exit(1)

    rom_hash = sha256_file(rom_path)
    print(f"SHA256:         {rom_hash}")

    print(f"ARM9 size:      {header['arm9_size']:,} bytes (loaded at 0x{header['arm9_ram_address']:08X})")
    print(f"ARM7 size:      {header['arm7_size']:,} bytes (loaded at 0x{header['arm7_ram_address']:08X})")
    print(f"ARM9 overlays:  {header['arm9_overlay_size'] // 32} entries")
    print(f"ARM7 overlays:  {header['arm7_overlay_size'] // 32} entries")
    print()

    return header


def run_ndstool(rom_path: Path, extract_dir: Path):
    """Extract ROM using ndstool."""
    extract_dir.mkdir(parents=True, exist_ok=True)

    arm9_bin = extract_dir / "arm9.bin"
    arm7_bin = extract_dir / "arm7.bin"
    y9_bin = extract_dir / "y9.bin"
    y7_bin = extract_dir / "y7.bin"
    header_bin = extract_dir / "header.bin"
    banner_bin = extract_dir / "banner.bin"
    data_dir = extract_dir / "data"
    overlay_dir = extract_dir / "overlay"

    cmd = [
        "ndstool",
        "-x", str(rom_path),
        "-9", str(arm9_bin),
        "-7", str(arm7_bin),
        "-y9", str(y9_bin),
        "-y7", str(y7_bin),
        "-h", str(header_bin),
        "-t", str(banner_bin),
        "-d", str(data_dir),
        "-y", str(overlay_dir),
    ]

    print(f"Running: {' '.join(cmd)}")
    print()

    try:
        result = subprocess.run(cmd, capture_output=True, text=True, check=True)
        if result.stdout:
            print(result.stdout)
    except FileNotFoundError:
        print("ERROR: ndstool not found!")
        print("Please install devkitPro and ensure ndstool is in your PATH.")
        print("See SETUP.md for installation instructions.")
        sys.exit(1)
    except subprocess.CalledProcessError as e:
        print(f"ERROR: ndstool failed with return code {e.returncode}")
        if e.stderr:
            print(e.stderr)
        sys.exit(1)

    # Verify extracted files
    for f in [arm9_bin, arm7_bin]:
        if f.exists():
            print(f"  ✓ {f.name}: {f.stat().st_size:,} bytes")
        else:
            print(f"  ✗ {f.name}: MISSING")

    if overlay_dir.exists():
        overlays = list(overlay_dir.glob("*"))
        print(f"  ✓ overlays: {len(overlays)} files")

    if data_dir.exists():
        data_files = list(data_dir.rglob("*"))
        data_files = [f for f in data_files if f.is_file()]
        print(f"  ✓ data: {len(data_files)} files")

    print()
    return extract_dir


def generate_disassembly(extract_dir: Path, header: dict):
    """Generate initial disassembly of ARM9 and ARM7 binaries."""
    arm9_bin = extract_dir / "arm9.bin"
    arm7_bin = extract_dir / "arm7.bin"

    arm9_addr = header["arm9_ram_address"]
    arm7_addr = header["arm7_ram_address"]

    for name, bin_path, arch, base_addr in [
        ("arm9", arm9_bin, "arm", arm9_addr),
        ("arm7", arm7_bin, "arm", arm7_addr),
    ]:
        if not bin_path.exists():
            print(f"Skipping {name} disassembly — binary not found")
            continue

        asm_output = extract_dir / f"{name}.asm"
        print(f"Disassembling {name}...")

        cmd = [
            "arm-none-eabi-objdump",
            "-D",
            "-b", "binary",
            "-m", arch,
            f"--adjust-vma=0x{base_addr:08X}",
            str(bin_path),
        ]

        try:
            result = subprocess.run(cmd, capture_output=True, text=True, check=True)
            with open(asm_output, "w") as f:
                f.write(result.stdout)
            print(f"  ✓ {asm_output.name}: {len(result.stdout):,} characters")
        except FileNotFoundError:
            print(f"  ⚠ arm-none-eabi-objdump not found, skipping disassembly")
            print(f"    Install devkitARM to enable this step")
            break
        except subprocess.CalledProcessError as e:
            print(f"  ✗ Disassembly failed: {e.stderr}")

    print()


def parse_overlay_table(extract_dir: Path):
    """Parse the ARM9 overlay table (y9.bin) and print overlay info."""
    y9_path = extract_dir / "y9.bin"
    if not y9_path.exists():
        print("No overlay table found (y9.bin missing)")
        return

    data = y9_path.read_bytes()
    num_overlays = len(data) // 32

    print(f"ARM9 Overlay Table ({num_overlays} entries):")
    print(f"{'ID':>4}  {'RAM Address':>12}  {'Size':>10}  {'BSS Size':>10}  {'File ID':>8}")
    print("-" * 55)

    overlay_info = []
    for i in range(num_overlays):
        offset = i * 32
        ov_id = struct.unpack_from("<I", data, offset + 0)[0]
        ram_addr = struct.unpack_from("<I", data, offset + 4)[0]
        ram_size = struct.unpack_from("<I", data, offset + 8)[0]
        bss_size = struct.unpack_from("<I", data, offset + 12)[0]
        file_id = struct.unpack_from("<I", data, offset + 24)[0]

        print(f"{ov_id:4d}  0x{ram_addr:08X}    {ram_size:8,}    {bss_size:8,}    {file_id:6d}")
        overlay_info.append({
            "id": ov_id,
            "ram_address": ram_addr,
            "ram_size": ram_size,
            "bss_size": bss_size,
            "file_id": file_id,
        })

    print()

    # Save overlay info as a config file
    config_path = PROJECT_ROOT / "config" / "overlays.txt"
    with open(config_path, "w") as f:
        f.write("# Auto-generated overlay configuration\n")
        f.write("# ID, RAM Address, Size, BSS Size, File ID\n")
        for ov in overlay_info:
            f.write(f"{ov['id']}, 0x{ov['ram_address']:08X}, {ov['ram_size']}, {ov['bss_size']}, {ov['file_id']}\n")
    print(f"Overlay config saved to: {config_path}")
    print()


def main():
    print("=" * 60)
    print("Mario & Luigi: Partners in Time — ROM Extraction")
    print("=" * 60)
    print()

    # Determine ROM path
    if len(sys.argv) > 1:
        rom_path = Path(sys.argv[1]).resolve()
    else:
        rom_path = ROM_PATH_DEFAULT

    # Step 1: Verify ROM
    print("[1/4] Verifying ROM...")
    header = verify_rom(rom_path)

    # Step 2: Extract with ndstool
    print("[2/4] Extracting ROM contents...")
    extract_dir = run_ndstool(rom_path, EXTRACT_DIR)

    # Step 3: Parse overlay table
    print("[3/4] Parsing overlay table...")
    parse_overlay_table(extract_dir)

    # Step 4: Generate initial disassembly
    print("[4/4] Generating initial disassembly...")
    generate_disassembly(extract_dir, header)

    print("=" * 60)
    print("Extraction complete!")
    print(f"Extracted files are in: {extract_dir}")
    print()
    print("Next steps:")
    print("  1. Open arm9.bin in Ghidra for analysis")
    print("     - Processor: ARM:LE:32:v5t")
    print(f"     - Base address: 0x{header['arm9_ram_address']:08X}")
    print("  2. Begin identifying and naming functions")
    print("  3. Start decompiling functions to C")
    print("=" * 60)


if __name__ == "__main__":
    main()
