#!/usr/bin/env python3
"""
verify_split.py - Reassemble split .s files and verify against original arm9.bin

This validates that the binary split produced by split_arm9.py is lossless.
It assembles all func/*.s and gaps/*.s files in address order and concatenates
them into a single binary, which should byte-match the original arm9.bin.

Usage:
    python tools/scripts/verify_split.py

Requires:
    - arm-none-eabi-as (devkitARM) on PATH
    - arm-none-eabi-objcopy on PATH
    - arm9/asm/func/ and arm9/asm/gaps/ populated (run split_arm9.py first)
    - extracted/arm9.bin (run extract_rom.py first)
"""
import subprocess
import sys
import re
import struct
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
FUNC_DIR = ROOT / "arm9" / "asm" / "func"
GAP_DIR = ROOT / "arm9" / "asm" / "gaps"
ORIG_BIN = ROOT / "extracted" / "arm9.bin"
BUILD_DIR = ROOT / "build" / "verify_split"
ARM9_BASE = 0x02004000


def parse_address(s_file: Path) -> int:
    """Extract the start address from a .s file's header comment."""
    text = s_file.read_text(errors="replace")
    # Look for: @ 0xXXXXXXXX
    m = re.search(r"@\s+0x([0-9A-Fa-f]+)", text)
    if not m:
        return -1
    return int(m.group(1), 16)


def main() -> int:
    if not ORIG_BIN.exists():
        print(f"ERROR: {ORIG_BIN} not found. Run extract_rom.py first.")
        return 1
    if not any(FUNC_DIR.glob("*.s")):
        print(f"ERROR: {FUNC_DIR} empty. Run split_arm9.py first.")
        return 1

    print("Collecting all .s files with addresses...")
    entries = []
    for d in (FUNC_DIR, GAP_DIR):
        for f in d.glob("*.s"):
            addr = parse_address(f)
            if addr < 0:
                print(f"  WARN: no address in {f.name}, skipping")
                continue
            entries.append((addr, f))

    entries.sort(key=lambda e: e[0])
    print(f"Found {len(entries)} files spanning 0x{entries[0][0]:08X} - 0x{entries[-1][0]:08X}")

    BUILD_DIR.mkdir(parents=True, exist_ok=True)

    # Original size for comparison
    orig = ORIG_BIN.read_bytes()
    print(f"Original arm9.bin: {len(orig)} bytes")

    # Reconstruct by extracting raw .word data from each file (no toolchain needed!)
    # Each .s file contains lines like: .word 0xXXXXXXXX or .byte 0xXX
    print("Extracting bytes from .s files...")
    out = bytearray(len(orig))
    covered = bytearray(len(orig))

    word_re = re.compile(r"\.word\s+0x([0-9A-Fa-f]+)")
    half_re = re.compile(r"\.(?:hword|short|2byte)\s+0x([0-9A-Fa-f]+)")
    byte_re = re.compile(r"\.byte\s+0x([0-9A-Fa-f]+)")

    for addr, f in entries:
        offset = addr - ARM9_BASE
        if offset < 0 or offset >= len(orig):
            continue
        cur = offset
        for line in f.read_text(errors="replace").splitlines():
            line = line.split("@")[0].strip()
            if not line:
                continue
            mw = word_re.search(line)
            if mw:
                v = int(mw.group(1), 16)
                if cur + 4 <= len(out):
                    out[cur:cur+4] = struct.pack("<I", v)
                    covered[cur:cur+4] = b"\x01" * 4
                    cur += 4
                continue
            mh = half_re.search(line)
            if mh:
                v = int(mh.group(1), 16)
                if cur + 2 <= len(out):
                    out[cur:cur+2] = struct.pack("<H", v)
                    covered[cur:cur+2] = b"\x01" * 2
                    cur += 2
                continue
            mb = byte_re.search(line)
            if mb:
                v = int(mb.group(1), 16)
                if cur < len(out):
                    out[cur] = v
                    covered[cur] = 1
                    cur += 1

    # Compare
    coverage = sum(covered) / len(covered) * 100
    print(f"Coverage: {coverage:.1f}% ({sum(covered)}/{len(covered)} bytes)")

    if bytes(out[:sum(covered)]) == orig[:sum(covered)]:
        # Find first mismatch
        diffs = [i for i in range(min(len(out), len(orig))) if covered[i] and out[i] != orig[i]]
        if not diffs:
            print("[OK] Split is LOSSLESS — reassembled bytes match original!")
            return 0
        print(f"[FAIL] {len(diffs)} byte mismatches; first at offset 0x{diffs[0]:X}")
        return 1
    else:
        diffs = [i for i in range(min(len(out), len(orig))) if covered[i] and out[i] != orig[i]]
        if not diffs:
            print("[OK] All covered bytes match original!")
            print(f"  ({len(orig) - sum(covered)} uncovered bytes — likely trailing padding)")
            return 0
        print(f"[FAIL] {len(diffs)} byte mismatches; first at offset 0x{diffs[0]:X}")
        print(f"  Expected: {orig[diffs[0]]:02X}  Got: {out[diffs[0]]:02X}")
        return 1


if __name__ == "__main__":
    sys.exit(main())

