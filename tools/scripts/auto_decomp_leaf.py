#!/usr/bin/env python3
"""
auto_decomp_leaf.py — emit stub C bodies for short ARM leaf functions
in arm9.bin so they appear as FUN_<addr> symbols and get picked up by
the fnptr-table generator.

Given one or more NDS code addresses, this:
  1. Reads the function bytes from extracted/arm9.bin
  2. Walks ARM instructions until it finds a return (bx lr / pop {...,pc} /
     ldr pc,[...] / b <not-self>) or the size cap is reached.
  3. Recognises a small set of trivial patterns and translates them to C.
     Anything more complex falls through to a no-op stub.
  4. Writes arm9/src/FUN_<addr>.c (lowercase hex), refusing to overwrite
     an existing file.

Usage:
    python tools/scripts/auto_decomp_leaf.py 0x02012345 [0x02034567 ...]
    python tools/scripts/auto_decomp_leaf.py --from-file addrs.txt
    python tools/scripts/auto_decomp_leaf.py --vtable 0x02054E10

The --vtable form reads .session-work/vtable_<addr_lc>.txt produced by
tools/scripts/resolve_vtable.py and emits stubs for every entry whose
status is `arm9.bin (no decomp)`.
"""

from __future__ import annotations

import argparse
import os
import re
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
ARM9_BIN = ROOT / "extracted" / "arm9.bin"
ARM9_LOAD = 0x02000000
ARM9_SRC = ROOT / "arm9" / "src"
SESSION_WORK = ROOT / ".session-work"

MAX_INSNS = 16  # functions longer than this fall through to no-op stub


def read_bytes(nds_addr: int, n: int) -> bytes:
    off = nds_addr - ARM9_LOAD
    with open(ARM9_BIN, "rb") as f:
        f.seek(off)
        return f.read(n)


def is_return(word: int) -> bool:
    # bx lr  -> e12fff1e
    if word == 0xE12FFF1E:
        return True
    # pop {... pc}  -> ldmia sp! ... base
    if (word & 0x0FFF8000) == 0x08BD8000:
        return True
    # mov pc, lr -> e1a0f00e
    if word == 0xE1A0F00E:
        return True
    return False


def fmt_disasm_comment(nds_addr: int, words: list[int]) -> str:
    lines = []
    for i, w in enumerate(words):
        lines.append(f" *   {nds_addr + i * 4:08x}  {w:08x}")
    return "\n".join(lines)


# Trivial pattern translators ------------------------------------------------

def try_translate(words: list[int]) -> tuple[str, str] | None:
    """Return (return_type, body_lines) or None if pattern not recognised.

    Body lines do NOT include the surrounding `{ ... }`.
    """
    if not words:
        return None

    # Pattern: mov r0, #imm ; bx lr   (1-byte rotated immediate)
    if len(words) == 2 and is_return(words[1]):
        w = words[0]
        if (w & 0x0FFF0000) == 0x03A00000:  # mov r0, #imm8 (rotated)
            imm8 = w & 0xFF
            rot = ((w >> 8) & 0xF) * 2
            val = ((imm8 >> rot) | (imm8 << (32 - rot))) & 0xFFFFFFFF if rot else imm8
            return ("int", f"    return 0x{val:x};")

    # Pattern: bx lr (immediate return)
    if len(words) == 1 and is_return(words[0]):
        return ("void", "    /* immediate return */")

    # Pattern: mov r0, r0 ; bx lr  (identity)
    if len(words) == 2 and words[0] == 0xE1A00000 and is_return(words[1]):
        return ("int", "    return (int)(intptr_t)param_1;")

    return None


def emit_stub(nds_addr: int, force: bool = False) -> str:
    """Generate arm9/src/FUN_<addr>.c. Returns 'created'/'exists'/'skipped:reason'."""
    name = f"FUN_{nds_addr:08x}"
    out_path = ARM9_SRC / f"{name}.c"
    if out_path.exists() and not force:
        return "exists"

    raw = read_bytes(nds_addr, MAX_INSNS * 4)
    if len(raw) < 4:
        return "skipped:beyond-arm9.bin"

    words: list[int] = []
    for i in range(0, len(raw), 4):
        if i + 4 > len(raw):
            break
        w = struct.unpack_from("<I", raw, i)[0]
        words.append(w)
        if is_return(w):
            break
    else:
        # no return found inside MAX_INSNS — too complex
        words = words[: min(8, len(words))]

    translated = try_translate(words)

    body_lines: list[str]
    if translated is None:
        ret_type = "void"
        body_lines = [
            "    /* leaf body too complex for auto-decomp; no-op stub.",
            "     * The fnptr-table will still register this address so",
            "     * indirect calls through vtables resolve to a defined",
            "     * symbol instead of falling through to host_fnptr_lookup",
            "     * NULL.  Replace with a hand decompilation when it",
            "     * becomes hot. */",
            "    (void)param_1; (void)param_2;",
        ]
    else:
        ret_type, body = translated
        body_lines = body.splitlines()

    text = f"""/*
 * {name}.c — auto-generated stub by tools/scripts/auto_decomp_leaf.py
 *
 * Source: arm9.bin @ 0x{nds_addr:08x} (load base 0x{ARM9_LOAD:08x}).
 * Raw ARM instruction words (little-endian):
 *
{fmt_disasm_comment(nds_addr, words)}
 *
 * This translation unit exists so the fnptr-table generator picks up
 * a host-side symbol named "{name}".  When the underlying function
 * becomes hot enough to matter, replace this body with a hand-written
 * decompilation; until then it returns the AAPCS-compatible default.
 */

#include "types.h"
#include <stdint.h>

{ret_type} {name}(int param_1, int param_2)
{{
{chr(10).join(body_lines)}
{"    return 0;" if ret_type == "int" else ""}}}
"""
    out_path.write_text(text, encoding="utf-8")
    return "created"


def parse_vtable_file(path: Path) -> list[int]:
    out: list[int] = []
    seen: set[int] = set()
    rx = re.compile(r"->\s+([0-9a-fA-F]{8})\s+arm9\.bin \(no decomp\)")
    for line in path.read_text(encoding="ascii", errors="ignore").splitlines():
        m = rx.search(line)
        if m:
            a = int(m.group(1), 16)
            if a not in seen and 0x02000000 <= a < 0x02400000:
                seen.add(a)
                out.append(a)
    return out


def main(argv: list[str]) -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("addrs", nargs="*", help="NDS addresses, e.g. 0x02034074")
    ap.add_argument("--from-file", help="text file with one address per line")
    ap.add_argument("--vtable", action="append", default=[],
                    help="vtable address; reads .session-work/vtable_<addr>.txt")
    ap.add_argument("--force", action="store_true",
                    help="overwrite existing arm9/src/FUN_*.c files")
    ap.add_argument("--limit", type=int, default=0,
                    help="maximum number of stubs to generate (0 = no limit)")
    args = ap.parse_args(argv)

    targets: list[int] = []
    seen: set[int] = set()

    def add(a: int):
        if a in seen:
            return
        seen.add(a)
        targets.append(a)

    for a in args.addrs:
        add(int(a, 0))

    if args.from_file:
        for line in Path(args.from_file).read_text().splitlines():
            line = line.strip().split("#", 1)[0]
            if line:
                add(int(line, 0))

    for vt in args.vtable:
        vt_int = int(vt, 0)
        path = SESSION_WORK / f"vtable_0x{vt_int:08x}.txt"
        if not path.exists():
            print(f"warn: {path} not found; run resolve_vtable.py first", file=sys.stderr)
            continue
        for a in parse_vtable_file(path):
            add(a)

    if args.limit and len(targets) > args.limit:
        targets = targets[: args.limit]

    if not targets:
        ap.print_help()
        return 1

    counts = {"created": 0, "exists": 0}
    for a in targets:
        result = emit_stub(a, force=args.force)
        counts[result] = counts.get(result, 0) + 1
        if result == "created":
            print(f"  + arm9/src/FUN_{a:08x}.c")
    print(f"done: {counts}")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
