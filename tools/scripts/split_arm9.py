#!/usr/bin/env python3
"""
Split the ARM9 binary into per-function assembly files.

Uses the Ghidra-exported function map (config/functions.csv) and the
extracted arm9.bin to produce individual .s files in arm9/asm/.

Each .s file contains the raw bytes of one function as .word directives,
with proper labels and alignment, ready for the assembler.

Usage:
    python tools/scripts/split_arm9.py
"""

import csv
import struct
import sys
from pathlib import Path

PROJECT_ROOT = Path(__file__).resolve().parent.parent.parent
FUNCTIONS_CSV = PROJECT_ROOT / "config" / "functions.csv"
ARM9_BIN = PROJECT_ROOT / "extracted" / "arm9.bin"
ASM_OUTPUT_DIR = PROJECT_ROOT / "arm9" / "asm"
INCLUDE_DIR = PROJECT_ROOT / "arm9" / "include"

ARM9_BASE_ADDR = 0x02004000


def load_functions(csv_path: Path) -> list:
    """Load function list from CSV."""
    functions = []
    with open(csv_path, "r") as f:
        reader = csv.DictReader(f)
        for row in reader:
            addr = int(row["address"], 16)
            size = int(row["size"])
            name = row["name"]
            is_thunk = row["is_thunk"].lower() == "true"
            functions.append({
                "address": addr,
                "size": size,
                "name": name,
                "is_thunk": is_thunk,
            })
    functions.sort(key=lambda x: x["address"])
    return functions


def detect_thumb(data: bytes, offset: int) -> bool:
    """Heuristic: check if code at offset looks like Thumb (16-bit) instructions."""
    if len(data) < offset + 4:
        return False
    # Check if the first instruction looks like a common Thumb push {r4-r7,lr}
    hw = struct.unpack_from("<H", data, offset)[0]
    # Common Thumb push: 0xb5xx
    if (hw & 0xFF00) == 0xB500:
        return True
    # Common Thumb mov: 0x46xx
    if (hw & 0xFF00) == 0x4600:
        return True
    # If the function size is odd or very small, likely Thumb
    return False


def generate_function_asm(func: dict, data: bytes, all_functions: list) -> str:
    """Generate assembly source for a single function."""
    addr = func["address"]
    size = func["size"]
    name = func["name"]
    offset = addr - ARM9_BASE_ADDR

    if offset < 0 or offset + size > len(data):
        return None

    func_data = data[offset:offset + size]
    is_thumb = detect_thumb(data, offset)

    lines = []
    lines.append(f"    .include \"macros.inc\"")
    lines.append(f"")
    lines.append(f"    .text")

    if is_thumb:
        lines.append(f"    .thumb")
        lines.append(f"    .thumb_func")
    else:
        lines.append(f"    .arm")

    lines.append(f"    .align 2")
    lines.append(f"    .global {name}")
    lines.append(f"    .type {name}, %function")
    lines.append(f"{name}: @ 0x{addr:08X}")

    if is_thumb:
        # Emit as 16-bit half-words
        for i in range(0, len(func_data), 2):
            if i + 2 <= len(func_data):
                hw = struct.unpack_from("<H", func_data, i)[0]
                lines.append(f"    .hword 0x{hw:04X}")
            else:
                lines.append(f"    .byte 0x{func_data[i]:02X}")
    else:
        # Emit as 32-bit words
        for i in range(0, len(func_data), 4):
            if i + 4 <= len(func_data):
                word = struct.unpack_from("<I", func_data, i)[0]
                lines.append(f"    .word 0x{word:08X}")
            elif i + 2 <= len(func_data):
                hw = struct.unpack_from("<H", func_data, i)[0]
                lines.append(f"    .hword 0x{hw:04X}")
            else:
                lines.append(f"    .byte 0x{func_data[i]:02X}")

    lines.append(f"    .size {name}, .-{name}")
    lines.append(f"")

    return "\n".join(lines)


def generate_main_asm(functions: list, gap_regions: list) -> str:
    """Generate the main arm9_main.s that includes all function files and gap data."""
    lines = []
    lines.append("@ Mario & Luigi: Partners in Time - ARM9 Main Assembly")
    lines.append("@ Auto-generated - includes all function files in address order")
    lines.append("@")
    lines.append(f"@ Total functions: {len(functions)}")
    lines.append("")
    lines.append('    .include "macros.inc"')
    lines.append("")
    lines.append("    .text")
    lines.append("    .arm")
    lines.append("")

    for func in functions:
        lines.append(f'    .include "func/{func["name"]}.s"')

    lines.append("")
    return "\n".join(lines)


def find_gaps(functions: list, total_size: int) -> list:
    """Find gaps between functions (data sections, padding, undetected code)."""
    gaps = []
    # Gap before first function
    if functions and functions[0]["address"] > ARM9_BASE_ADDR:
        gaps.append({
            "start": ARM9_BASE_ADDR,
            "end": functions[0]["address"],
            "size": functions[0]["address"] - ARM9_BASE_ADDR,
        })

    # Gaps between functions
    for i in range(len(functions) - 1):
        end_of_current = functions[i]["address"] + functions[i]["size"]
        start_of_next = functions[i + 1]["address"]
        if end_of_current < start_of_next:
            gaps.append({
                "start": end_of_current,
                "end": start_of_next,
                "size": start_of_next - end_of_current,
            })

    # Gap after last function
    if functions:
        end_of_last = functions[-1]["address"] + functions[-1]["size"]
        end_of_binary = ARM9_BASE_ADDR + total_size
        if end_of_last < end_of_binary:
            gaps.append({
                "start": end_of_last,
                "end": end_of_binary,
                "size": end_of_binary - end_of_last,
            })

    return gaps


def generate_gap_asm(gap: dict, data: bytes, index: int) -> str:
    """Generate assembly for a gap region (data/padding between functions)."""
    offset = gap["start"] - ARM9_BASE_ADDR
    size = gap["size"]
    gap_data = data[offset:offset + size]

    lines = []
    lines.append(f'    .include "macros.inc"')
    lines.append(f"")
    lines.append(f"    .section .data")
    lines.append(f"    .align 2")
    lines.append(f"gap_{index:04d}: @ 0x{gap['start']:08X} - 0x{gap['end']:08X} ({size} bytes)")

    for i in range(0, len(gap_data), 4):
        if i + 4 <= len(gap_data):
            word = struct.unpack_from("<I", gap_data, i)[0]
            lines.append(f"    .word 0x{word:08X}")
        elif i + 2 <= len(gap_data):
            hw = struct.unpack_from("<H", gap_data, i)[0]
            lines.append(f"    .hword 0x{hw:04X}")
        else:
            lines.append(f"    .byte 0x{gap_data[i]:02X}")

    lines.append(f"")
    return "\n".join(lines)


def main():
    print("=" * 60)
    print("ARM9 Binary Splitter")
    print("=" * 60)
    print()

    if not FUNCTIONS_CSV.exists():
        print(f"ERROR: Function map not found at {FUNCTIONS_CSV}")
        print("Run Ghidra analysis first.")
        sys.exit(1)

    if not ARM9_BIN.exists():
        print(f"ERROR: ARM9 binary not found at {ARM9_BIN}")
        print("Run extract_rom.py first.")
        sys.exit(1)

    # Load data
    functions = load_functions(FUNCTIONS_CSV)
    arm9_data = ARM9_BIN.read_bytes()
    print(f"Loaded {len(functions)} functions from Ghidra export")
    print(f"ARM9 binary: {len(arm9_data):,} bytes")
    print()

    # Create output directories
    func_dir = ASM_OUTPUT_DIR / "func"
    gap_dir = ASM_OUTPUT_DIR / "gaps"
    func_dir.mkdir(parents=True, exist_ok=True)
    gap_dir.mkdir(parents=True, exist_ok=True)

    # Generate per-function .s files
    print("Generating per-function assembly files...")
    generated = 0
    skipped = 0
    for func in functions:
        asm = generate_function_asm(func, arm9_data, functions)
        if asm:
            out_path = func_dir / f"{func['name']}.s"
            out_path.write_text(asm)
            generated += 1
        else:
            print(f"  WARNING: Skipped {func['name']} (out of bounds)")
            skipped += 1

    print(f"  Generated: {generated} function files")
    if skipped:
        print(f"  Skipped:   {skipped}")
    print()

    # Find and emit gap regions
    gaps = find_gaps(functions, len(arm9_data))
    total_gap_bytes = sum(g["size"] for g in gaps)
    total_func_bytes = sum(f["size"] for f in functions)

    print(f"Gap analysis:")
    print(f"  Function code:  {total_func_bytes:,} bytes ({100*total_func_bytes/len(arm9_data):.1f}%)")
    print(f"  Gaps/data:      {total_gap_bytes:,} bytes ({100*total_gap_bytes/len(arm9_data):.1f}%)")
    print(f"  Gap regions:    {len(gaps)}")
    print()

    print("Generating gap assembly files...")
    for i, gap in enumerate(gaps):
        asm = generate_gap_asm(gap, arm9_data, i)
        out_path = gap_dir / f"gap_{i:04d}.s"
        out_path.write_text(asm)

    # Generate macros.inc
    macros_path = ASM_OUTPUT_DIR / "macros.inc"
    macros_path.write_text(
        "@ Common assembly macros for M&L:PiT decompilation\n"
        "@ Include this in all .s files\n"
        "\n"
        ".macro thumb_func_start name\n"
        "    .thumb\n"
        "    .thumb_func\n"
        "    .align 2\n"
        "    .global \\name\n"
        "    .type \\name, %function\n"
        ".endm\n"
        "\n"
        ".macro arm_func_start name\n"
        "    .arm\n"
        "    .align 2\n"
        "    .global \\name\n"
        "    .type \\name, %function\n"
        ".endm\n"
        "\n"
        ".macro func_end name\n"
        "    .size \\name, .-\\name\n"
        ".endm\n"
        "\n"
    )

    # Generate symbol table header
    symbols_path = INCLUDE_DIR / "functions.h"
    symbols_path.parent.mkdir(parents=True, exist_ok=True)
    with open(symbols_path, "w") as f:
        f.write("/**\n")
        f.write(" * Auto-generated function declarations\n")
        f.write(f" * {len(functions)} functions from Ghidra analysis\n")
        f.write(" */\n\n")
        f.write("#ifndef FUNCTIONS_H\n")
        f.write("#define FUNCTIONS_H\n\n")
        f.write("#include \"types.h\"\n\n")
        f.write("/* Function declarations — signatures TBD during decompilation */\n")
        for func in functions:
            f.write(f"void {func['name']}(void);  // 0x{func['address']:08X}, {func['size']} bytes\n")
        f.write(f"\n#endif /* FUNCTIONS_H */\n")

    print()
    print("=" * 60)
    print("Split complete!")
    print(f"  {generated} function .s files in: {func_dir}")
    print(f"  {len(gaps)} gap .s files in:      {gap_dir}")
    print(f"  Macros:                       {macros_path}")
    print(f"  Function header:              {symbols_path}")
    print("=" * 60)


if __name__ == "__main__":
    main()
