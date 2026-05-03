#!/usr/bin/env python3
"""
Generate a Ghidra import script for the extracted ARM9/ARM7 binaries.

This creates .gdt (Ghidra Data Type) and .properties files,
and a headless analysis batch script.

Usage:
    python tools/scripts/ghidra_setup.py
"""

import os
import sys
from pathlib import Path

PROJECT_ROOT = Path(__file__).resolve().parent.parent.parent
EXTRACT_DIR = PROJECT_ROOT / "extracted"
GHIDRA_DIR = PROJECT_ROOT / "ghidra_projects"


def create_import_script():
    """Create a Ghidra Python script for importing NDS binaries."""
    script_dir = PROJECT_ROOT / "tools" / "ghidra"
    script_dir.mkdir(parents=True, exist_ok=True)

    import_script = script_dir / "import_nds.py"
    import_script.write_text('''\
# Ghidra Python script for importing NDS ARM9/ARM7 binaries
# Run this from within Ghidra's Script Manager
#
# This script:
# 1. Imports arm9.bin at the correct base address
# 2. Sets the processor to ARM:LE:32:v5t
# 3. Begins auto-analysis
# 4. Labels known hardware registers

# @category NDS
# @author MLPiT Decomp Team

from ghidra.program.model.symbol import SourceType
from ghidra.program.model.data import *

def label(addr_val, name):
    """Create a label at the given address."""
    addr = currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(addr_val)
    currentProgram.getSymbolTable().createLabel(addr, name, SourceType.USER_DEFINED)

def main():
    print("=== Mario & Luigi: Partners in Time - NDS Analysis ===")
    print()

    # Label NDS hardware registers (ARM9 side)
    hw_regs = {
        0x04000000: "REG_DISPCNT",
        0x04000004: "REG_DISPSTAT",
        0x04000006: "REG_VCOUNT",
        0x04000008: "REG_BG0CNT",
        0x0400000A: "REG_BG1CNT",
        0x0400000C: "REG_BG2CNT",
        0x0400000E: "REG_BG3CNT",
        0x04000100: "REG_TM0CNT_L",
        0x04000102: "REG_TM0CNT_H",
        0x04000130: "REG_KEYINPUT",
        0x04000180: "REG_IPCSYNC",
        0x04000184: "REG_IPCFIFOCNT",
        0x04000208: "REG_IME",
        0x04000210: "REG_IE",
        0x04000214: "REG_IF",
    }

    for addr, name in hw_regs.items():
        try:
            label(addr, name)
            print(f"  Labeled 0x{addr:08X} as {name}")
        except Exception as e:
            print(f"  Could not label {name}: {e}")

    print()
    print("Hardware register labeling complete.")
    print("Run auto-analysis now for full decompilation.")

main()
''')

    print(f"Created Ghidra import script: {import_script}")
    return import_script


def create_headless_batch():
    """Create batch files for headless Ghidra analysis."""
    script_dir = PROJECT_ROOT / "tools" / "ghidra"
    ghidra_dir = GHIDRA_DIR

    # Windows batch script
    bat_content = f'''\
@echo off
REM Headless Ghidra analysis for Mario & Luigi: Partners in Time
REM
REM Usage: analyze_headless.bat <path_to_ghidra>
REM   e.g.: analyze_headless.bat C:\\ghidra\\ghidra_11.3

setlocal

if "%~1"=="" (
    echo Usage: analyze_headless.bat ^<ghidra_install_dir^>
    echo   e.g.: analyze_headless.bat C:\\ghidra\\ghidra_11.3
    exit /b 1
)

set GHIDRA_DIR=%~1
set PROJECT_DIR={ghidra_dir}
set EXTRACT_DIR={EXTRACT_DIR}
set SCRIPTS_DIR={script_dir}

if not exist "%EXTRACT_DIR%\\arm9.bin" (
    echo ERROR: arm9.bin not found. Run extract_rom.py first.
    exit /b 1
)

echo Creating Ghidra project directory...
if not exist "%PROJECT_DIR%" mkdir "%PROJECT_DIR%"

echo.
echo === Importing ARM9 binary ===
"%GHIDRA_DIR%\\support\\analyzeHeadless.bat" ^
    "%PROJECT_DIR%" MLPiT ^
    -import "%EXTRACT_DIR%\\arm9.bin" ^
    -processor "ARM:LE:32:v5t" ^
    -cspec "default" ^
    -loader BinaryLoader ^
    -loader-baseAddr 0x02000000 ^
    -postScript "%SCRIPTS_DIR%\\import_nds.py" ^
    -analysisTimeoutPerFile 600

echo.
echo === Importing ARM7 binary ===
"%GHIDRA_DIR%\\support\\analyzeHeadless.bat" ^
    "%PROJECT_DIR%" MLPiT ^
    -import "%EXTRACT_DIR%\\arm7.bin" ^
    -processor "ARM:LE:32:v4t" ^
    -cspec "default" ^
    -loader BinaryLoader ^
    -loader-baseAddr 0x037F8000 ^
    -analysisTimeoutPerFile 300

echo.
echo Analysis complete! Open the project in Ghidra GUI:
echo   Project directory: %PROJECT_DIR%
echo   Project name: MLPiT
'''

    bat_path = script_dir / "analyze_headless.bat"
    bat_path.write_text(bat_content)
    print(f"Created headless analysis script: {bat_path}")

    # Shell script for Linux/macOS
    sh_content = f'''\
#!/bin/bash
# Headless Ghidra analysis for Mario & Luigi: Partners in Time
#
# Usage: ./analyze_headless.sh <path_to_ghidra>

set -e

if [ -z "$1" ]; then
    echo "Usage: $0 <ghidra_install_dir>"
    exit 1
fi

GHIDRA_DIR="$1"
PROJECT_DIR="{ghidra_dir}"
EXTRACT_DIR="{EXTRACT_DIR}"
SCRIPTS_DIR="{script_dir}"

if [ ! -f "$EXTRACT_DIR/arm9.bin" ]; then
    echo "ERROR: arm9.bin not found. Run extract_rom.py first."
    exit 1
fi

mkdir -p "$PROJECT_DIR"

echo "=== Importing ARM9 binary ==="
"$GHIDRA_DIR/support/analyzeHeadless" \\
    "$PROJECT_DIR" MLPiT \\
    -import "$EXTRACT_DIR/arm9.bin" \\
    -processor "ARM:LE:32:v5t" \\
    -cspec "default" \\
    -loader BinaryLoader \\
    -loader-baseAddr 0x02000000 \\
    -postScript "$SCRIPTS_DIR/import_nds.py" \\
    -analysisTimeoutPerFile 600

echo ""
echo "=== Importing ARM7 binary ==="
"$GHIDRA_DIR/support/analyzeHeadless" \\
    "$PROJECT_DIR" MLPiT \\
    -import "$EXTRACT_DIR/arm7.bin" \\
    -processor "ARM:LE:32:v4t" \\
    -cspec "default" \\
    -loader BinaryLoader \\
    -loader-baseAddr 0x037F8000 \\
    -analysisTimeoutPerFile 300

echo ""
echo "Analysis complete! Open the project in Ghidra GUI."
'''

    sh_path = script_dir / "analyze_headless.sh"
    sh_path.write_text(sh_content)
    print(f"Created headless analysis script: {sh_path}")


def main():
    print("=" * 50)
    print("Ghidra Project Setup")
    print("=" * 50)
    print()

    create_import_script()
    print()
    create_headless_batch()
    print()

    print("Next steps:")
    print("  1. Install Ghidra (see SETUP.md)")
    print("  2. Extract the ROM first: python tools/scripts/extract_rom.py")
    print("  3. Run headless analysis or open Ghidra GUI")
    print("  4. Import arm9.bin with:")
    print("     - Processor: ARM:LE:32:v5t")
    print("     - Base address: 0x02000000")


if __name__ == "__main__":
    main()
