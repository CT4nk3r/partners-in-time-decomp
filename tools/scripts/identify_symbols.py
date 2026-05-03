#!/usr/bin/env python3
"""
Identify known NDS SDK functions and game entry points.

Analyzes the ARM9 binary to find:
  - NDS boot entry point and interrupt vectors
  - Common SDK function patterns (memory ops, math, etc.)
  - Main game loop candidates
  - String references for context

Usage:
    python tools/scripts/identify_symbols.py
"""

import struct
import csv
import re
from pathlib import Path

PROJECT_ROOT = Path(__file__).resolve().parent.parent.parent
ARM9_BIN = PROJECT_ROOT / "extracted" / "arm9.bin"
FUNCTIONS_CSV = PROJECT_ROOT / "config" / "functions.csv"
HEADER_BIN = PROJECT_ROOT / "extracted" / "header.bin"
SYMBOLS_OUT = PROJECT_ROOT / "config" / "symbols.txt"

ARM9_BASE_ADDR = 0x02004000


def read_entry_point():
    """Read the ARM9 entry point from the NDS header."""
    with open(HEADER_BIN, "rb") as f:
        data = f.read(0x200)
    entry = struct.unpack_from("<I", data, 0x24)[0]
    return entry


def find_strings(data: bytes, base_addr: int, min_len: int = 6) -> list:
    """Find ASCII strings in binary data."""
    strings = []
    current = b""
    start = 0
    for i, b in enumerate(data):
        if 0x20 <= b < 0x7F:
            if not current:
                start = i
            current += bytes([b])
        else:
            if len(current) >= min_len:
                strings.append({
                    "address": base_addr + start,
                    "text": current.decode("ascii", errors="replace"),
                    "offset": start,
                })
            current = b""
    return strings


def find_function_at(functions: list, addr: int):
    """Find which function contains a given address."""
    for f in functions:
        if f["address"] <= addr < f["address"] + f["size"]:
            return f
    return None


def analyze_function_patterns(data: bytes, functions: list):
    """Analyze functions for known patterns."""
    identified = {}

    for func in functions:
        offset = func["address"] - ARM9_BASE_ADDR
        size = func["size"]
        if offset < 0 or offset + size > len(data):
            continue
        func_data = data[offset:offset + size]

        # Pattern: memcpy-like (simple loop with ldr/str pairs)
        # Pattern: memset-like (loop with str and constant)
        # Pattern: division routines (common in ARM without HW divide)
        # Pattern: interrupt handler (reads from 0x04000214 = REG_IF)
        if size >= 4:
            # Check for reads/writes to interrupt registers
            for i in range(0, min(size, 64), 4):
                if i + 4 <= len(func_data):
                    word = struct.unpack_from("<I", func_data, i)[0]
                    # SWI instruction (software interrupt / BIOS call)
                    if (word & 0x0F000000) == 0x0F000000 and (word & 0xF0000000) in (0xE0000000, 0x00000000):
                        swi_num = (word >> 16) & 0xFF
                        swi_names = {
                            0x00: "SoftReset",
                            0x01: "RegisterRamReset",
                            0x02: "Halt",
                            0x03: "Stop",
                            0x04: "IntrWait",
                            0x05: "VBlankIntrWait",
                            0x06: "Div",
                            0x07: "DivArm",
                            0x08: "Sqrt",
                            0x09: "ArcTan",
                            0x0A: "ArcTan2",
                            0x0B: "CpuSet",
                            0x0C: "CpuFastSet",
                            0x0D: "GetBiosChecksum",
                            0x0E: "BgAffineSet",
                            0x0F: "ObjAffineSet",
                            0x10: "BitUnPack",
                            0x11: "LZ77UnCompWram",
                            0x12: "LZ77UnCompVram",
                            0x13: "HuffUnComp",
                            0x14: "RLUnCompWram",
                            0x15: "RLUnCompVram",
                            0x16: "Diff8bitUnFilterWram",
                            0x17: "Diff8bitUnFilterVram",
                            0x18: "Diff16bitUnFilter",
                            0x19: "SoundBias",
                        }
                        swi_name = swi_names.get(swi_num, f"SWI_0x{swi_num:02X}")
                        if func["name"] not in identified:
                            identified[func["name"]] = {
                                "original_name": func["name"],
                                "address": func["address"],
                                "size": func["size"],
                                "hints": [],
                            }
                        identified[func["name"]]["hints"].append(f"calls BIOS {swi_name}")

    return identified


def categorize_by_size(functions: list) -> dict:
    """Categorize functions by size for prioritization."""
    categories = {
        "tiny": [],      # < 32 bytes (likely getters/setters/stubs)
        "small": [],     # 32-128 bytes (utility functions)
        "medium": [],    # 128-512 bytes (typical game logic)
        "large": [],     # 512-2048 bytes (complex logic, state machines)
        "huge": [],      # > 2048 bytes (main loops, init, large switch)
    }
    for f in functions:
        if f["size"] < 32:
            categories["tiny"].append(f)
        elif f["size"] < 128:
            categories["small"].append(f)
        elif f["size"] < 512:
            categories["medium"].append(f)
        elif f["size"] < 2048:
            categories["large"].append(f)
        else:
            categories["huge"].append(f)
    return categories


def main():
    print("=" * 60)
    print("Symbol Identification & Analysis")
    print("=" * 60)
    print()

    # Load data
    arm9_data = ARM9_BIN.read_bytes()
    functions = []
    with open(FUNCTIONS_CSV) as f:
        for row in csv.DictReader(f):
            functions.append({
                "address": int(row["address"], 16),
                "size": int(row["size"]),
                "name": row["name"],
            })
    functions.sort(key=lambda x: x["address"])

    # Entry point
    entry_point = read_entry_point()
    entry_func = find_function_at(functions, entry_point)
    print(f"ARM9 Entry Point: 0x{entry_point:08X}")
    if entry_func:
        print(f"  → Function: {entry_func['name']} ({entry_func['size']} bytes)")
    print()

    # Strings
    print("Searching for strings...")
    strings = find_strings(arm9_data, ARM9_BASE_ADDR, min_len=8)
    print(f"  Found {len(strings)} strings (8+ chars)")

    # Filter interesting strings
    interesting_keywords = [
        "error", "assert", "fail", "debug", "main", "init", "game",
        "battle", "map", "save", "load", "sound", "menu", "title",
        "mario", "luigi", "item", "enemy", "npc", "event", "script",
        "sprite", "anim", "bg", "obj", "alloc", "free", "heap",
        "file", "read", "write", "open", "close", "dma", "irq",
        "vblank", "timer", "input", "key", "touch", "pad",
    ]
    interesting = []
    for s in strings:
        lower = s["text"].lower()
        if any(kw in lower for kw in interesting_keywords):
            interesting.append(s)

    print(f"  Interesting strings: {len(interesting)}")
    for s in interesting[:30]:
        containing = find_function_at(functions, s["address"])
        loc = f" (in {containing['name']})" if containing else ""
        print(f"    0x{s['address']:08X}: \"{s['text']}\"{loc}")
    if len(interesting) > 30:
        print(f"    ... and {len(interesting) - 30} more")
    print()

    # Function patterns
    print("Analyzing function patterns...")
    identified = analyze_function_patterns(arm9_data, functions)
    if identified:
        print(f"  Functions with BIOS calls: {len(identified)}")
        for name, info in list(identified.items())[:20]:
            print(f"    {name} @ 0x{info['address']:08X}: {', '.join(info['hints'])}")
    print()

    # Size categories
    categories = categorize_by_size(functions)
    print("Function size distribution:")
    for cat_name, cat_funcs in categories.items():
        print(f"  {cat_name:8s}: {len(cat_funcs):4d} functions")
    print()

    # Priority targets: huge functions (likely main loop, init, state machines)
    print("Priority decompilation targets (largest functions):")
    big_funcs = sorted(functions, key=lambda x: x["size"], reverse=True)
    for f in big_funcs[:15]:
        print(f"  {f['name']:30s}  {f['size']:6d} bytes @ 0x{f['address']:08X}")
    print()

    # Write symbol table
    with open(SYMBOLS_OUT, "w") as f:
        f.write("# Mario & Luigi: Partners in Time — Symbol Table\n")
        f.write("# Auto-generated initial mapping\n")
        f.write("#\n")
        f.write(f"# Entry point: 0x{entry_point:08X}\n")
        if entry_func:
            f.write(f"# Entry function: {entry_func['name']}\n")
        f.write(f"# Total functions: {len(functions)}\n")
        f.write("#\n")
        f.write("# Format: address, size, current_name, suggested_name, category\n")
        f.write("#\n\n")

        f.write("# === ENTRY POINT ===\n")
        if entry_func:
            f.write(f"0x{entry_func['address']:08X}, {entry_func['size']}, {entry_func['name']}, arm9_entry, entry\n")
        f.write("\n")

        f.write("# === BIOS CALLERS ===\n")
        for name, info in identified.items():
            hints = "; ".join(info["hints"])
            f.write(f"0x{info['address']:08X}, {info['size']}, {name}, , bios ({hints})\n")
        f.write("\n")

        f.write("# === LARGEST FUNCTIONS (priority targets) ===\n")
        for func in big_funcs[:30]:
            f.write(f"0x{func['address']:08X}, {func['size']}, {func['name']}, , large\n")
        f.write("\n")

        f.write("# === STRING REFERENCES ===\n")
        for s in interesting[:50]:
            containing = find_function_at(functions, s["address"])
            if containing:
                f.write(f"# 0x{s['address']:08X} in {containing['name']}: \"{s['text']}\"\n")
            else:
                f.write(f"# 0x{s['address']:08X}: \"{s['text']}\"\n")

    print(f"Symbol table written to: {SYMBOLS_OUT}")
    print()
    print("=" * 60)
    print("Next: Open Ghidra GUI to inspect the largest functions")
    print(f"  Entry point function: {entry_func['name'] if entry_func else 'unknown'}")
    print("  Start renaming functions as you identify them")
    print("=" * 60)


if __name__ == "__main__":
    main()
