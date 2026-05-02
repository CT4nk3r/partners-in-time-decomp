#!/usr/bin/env python3
"""Scan all arm9/asm/func/FUN_*.s files for BL/B branches targeting given functions.

The .s files are the ground truth (they may not match arm9.bin if a different
build was extracted).  Each file's name FUN_<addr>.s encodes its start address,
and the file body is a sequence of `.word 0xXXXXXXXX` instruction encodings.
"""
import os
import re
import sys

ASM_DIR = r"C:\Users\ct4nk3r\Documents\marionluigi-pit-decomp\arm9\asm\func"

TARGETS = {
    0x0202a33c: "FUN_0202a33c (list processor)",
    0x0202a51c: "FUN_0202a51c (list update-bit walker)",
    0x0202a53c: "FUN_0202a53c (list-push entry)",
    0x0202a56c: "FUN_0202a56c (list-append fast path / mid-fn)",
    0x0202a5a4: "FUN_0202a5a4 (list first-insert)",
    0x0202a62c: "FUN_0202a62c (list unlink)",
    0x02005444: "FUN_02005444 (frame-tick? calls list processor)",
    0x0200528c: "FUN_0200528c (calls update-bit walker)",
    0x02005380: "FUN_02005380 (calls FUN_0200528c)",
    0x020050f4: "FUN_020050f4 (called first by FUN_02005444)",
    0x020051f4: "FUN_020051f4 (called by FUN_02005444 mid-loop)",
    0x02005300: "FUN_02005300 (called by FUN_02005380)",
}

WORD_RE = re.compile(r"\.word\s+0x([0-9a-fA-F]{8})")
NAME_RE = re.compile(r"FUN_([0-9a-fA-F]{8})\.s$", re.IGNORECASE)


def decode_branch(insn, pc):
    """If insn is an ARM B/BL with cond, return (kind, cond, target). Else None."""
    if ((insn >> 25) & 0x7) != 0x5:
        return None
    cond = (insn >> 28) & 0xF
    L = (insn >> 24) & 1
    imm = insn & 0xFFFFFF
    if imm & 0x800000:
        imm |= ~0xFFFFFF
    target = (pc + 8 + (imm << 2)) & 0xFFFFFFFF
    return ("BL" if L else "B", cond, target)


def scan_file(path):
    """Return (start_addr, list of (pc, kind, cond, target))."""
    m = NAME_RE.search(os.path.basename(path))
    if not m:
        return None, []
    start = int(m.group(1), 16)
    out = []
    pc = start
    with open(path, "r", encoding="utf-8", errors="replace") as f:
        for line in f:
            wm = WORD_RE.search(line)
            if not wm:
                continue
            insn = int(wm.group(1), 16)
            br = decode_branch(insn, pc)
            if br is not None:
                out.append((pc, *br))
            pc += 4
    return start, out


def main():
    callers = {t: [] for t in TARGETS}
    if not os.path.isdir(ASM_DIR):
        print(f"missing dir {ASM_DIR}", file=sys.stderr)
        return
    for name in sorted(os.listdir(ASM_DIR)):
        if not name.endswith(".s"):
            continue
        start, branches = scan_file(os.path.join(ASM_DIR, name))
        if start is None:
            continue
        for pc, kind, cond, target in branches:
            if target in TARGETS:
                callers[target].append((start, pc, kind, cond))

    for t, label in TARGETS.items():
        cs = callers[t]
        print(f"\n=== {label} @ 0x{t:08x} : {len(cs)} branch callsites ===")
        seen_funcs = sorted({c[0] for c in cs})
        for f in seen_funcs:
            sites = [c for c in cs if c[0] == f]
            kinds = ",".join(f"{k}@0x{pc:x}" for _, pc, k, _ in sites)
            print(f"  caller FUN_{f:08x}  ({len(sites)} site(s)): {kinds}")


if __name__ == "__main__":
    main()
