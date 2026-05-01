#!/usr/bin/env python3
"""
Verify that the built ROM matches the original baserom.

Usage:
    python tools/scripts/verify_match.py
"""

import hashlib
import sys
from pathlib import Path

PROJECT_ROOT = Path(__file__).resolve().parent.parent.parent
BASEROM = PROJECT_ROOT / "rom" / "baserom.nds"
BUILT_ROM = PROJECT_ROOT / "build" / "mlpit.nds"


def sha256(path: Path) -> str:
    h = hashlib.sha256()
    with open(path, "rb") as f:
        while True:
            chunk = f.read(8192)
            if not chunk:
                break
            h.update(chunk)
    return h.hexdigest()


def main():
    if not BASEROM.exists():
        print(f"ERROR: Base ROM not found at {BASEROM}")
        sys.exit(1)

    if not BUILT_ROM.exists():
        print(f"ERROR: Built ROM not found at {BUILT_ROM}")
        print("Run 'make' first to build the ROM.")
        sys.exit(1)

    base_hash = sha256(BASEROM)
    built_hash = sha256(BUILT_ROM)

    base_size = BASEROM.stat().st_size
    built_size = BUILT_ROM.stat().st_size

    print(f"Base ROM:  {base_hash}  ({base_size:,} bytes)")
    print(f"Built ROM: {built_hash}  ({built_size:,} bytes)")
    print()

    if base_hash == built_hash:
        print("✅ MATCH! The built ROM is identical to the base ROM.")
        sys.exit(0)
    else:
        print("❌ MISMATCH! The built ROM differs from the base ROM.")
        if base_size != built_size:
            print(f"   Size difference: {built_size - base_size:+,} bytes")
        sys.exit(1)


if __name__ == "__main__":
    main()
