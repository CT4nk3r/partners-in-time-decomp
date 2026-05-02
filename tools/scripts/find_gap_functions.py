"""
find_gap_functions.py — audit gaps between consecutive decompiled functions
for code-shaped regions the original Ghidra pass missed (e.g. mid-function
fast-path entries reachable only via indirect calls, leaf helpers tucked
into 'unowned' bytes).

Usage:
    python tools/scripts/find_gap_functions.py \
        [--functions config/functions.csv] \
        [--bin extracted/arm9.bin] \
        [--base 0x02000000] \
        [--min-gap 16] \
        [--out .session-work/gap_functions_report.txt]

The report flags gaps that look like ARM code (presence of bx lr / push
/ stmfd) rather than pure literal pools.
"""
from __future__ import annotations

import argparse
import csv
import os
import struct
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parents[2]


def parse_functions_csv(path: Path):
    rows = []
    with path.open() as f:
        rdr = csv.DictReader(f)
        for r in rdr:
            try:
                addr = int(r["address"], 16)
                size = int(r["size"])
            except (KeyError, ValueError):
                continue
            rows.append((addr, size, r.get("name", "")))
    rows.sort(key=lambda t: t[0])
    return rows


# ARM instruction patterns we treat as "looks like code"
RET_PATTERNS = {
    0xE12FFF1E,  # bx lr
}


def is_push_lr(word: int) -> bool:
    # stmfd sp!, {... lr} or push {... lr}: cond=1110, 100PUSWL=100100x0,
    # Rn=1101, register list with bit14 set.
    if (word & 0x0FFF0000) != 0x092D0000:
        return False
    return bool(word & 0x4000)


def is_ret_or_pop_pc(word: int) -> bool:
    if word in RET_PATTERNS:
        return True
    # ldmfd sp!, {... pc}: Rn=1101, L=1, register list bit15 set
    if (word & 0x0FFF0000) == 0x08BD0000 and (word & 0x8000):
        return True
    return False


def looks_like_code(words):
    has_ret = any(is_ret_or_pop_pc(w) for w in words)
    has_push = any(is_push_lr(w) for w in words)
    # Penalise if more than ~60% of the words look like data pointers into
    # the ROM image (0x02000000..0x02060000) — common for literal pools.
    if not words:
        return False, "empty"
    pool = sum(1 for w in words if 0x02000000 <= w < 0x02060000)
    pool_ratio = pool / len(words)
    if pool_ratio > 0.6 and not has_push:
        return False, f"literal-pool ({pool}/{len(words)})"
    if has_push or has_ret:
        return True, ("push+ret" if (has_push and has_ret)
                      else ("push" if has_push else "ret"))
    # Without prologue/epilogue markers we can still spot dense code
    # by counting how many words have a recognisable ARM cond field
    # (top nibble 0..E, never F) AND are not all-zero.
    code_like = sum(1 for w in words
                    if w != 0 and ((w >> 28) & 0xF) <= 0xE)
    if code_like / len(words) > 0.85:
        return True, "dense-arm"
    return False, f"unclear (code_like={code_like}/{len(words)})"


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--functions", default=str(REPO / "config" / "functions.csv"))
    ap.add_argument("--bin", default=str(REPO / "extracted" / "arm9.bin"))
    ap.add_argument("--base", default="0x02000000")
    ap.add_argument("--min-gap", type=int, default=16)
    ap.add_argument("--out", default=str(REPO / ".session-work" / "gap_functions_report.txt"))
    args = ap.parse_args()

    base = int(args.base, 16)
    funcs = parse_functions_csv(Path(args.functions))
    if not funcs:
        sys.exit("no functions parsed")
    data = Path(args.bin).read_bytes()

    out_lines = []
    out_lines.append(f"# Gap-function audit (min_gap={args.min_gap})")
    out_lines.append(f"# {len(funcs)} known functions, image {len(data):#x} bytes "
                     f"@ base {base:#010x}")
    out_lines.append("")

    interesting = 0
    for (a, sz, name), (b, _, nname) in zip(funcs, funcs[1:]):
        end = a + sz
        gap = b - end
        if gap < args.min_gap:
            continue
        off = end - base
        if off < 0 or off + gap > len(data):
            continue
        words = struct.unpack(f"<{gap // 4}I", data[off:off + (gap // 4) * 4])
        ok, why = looks_like_code(words)
        if ok:
            interesting += 1
            out_lines.append(f"GAP {end:#010x}..{b:#010x}  size={gap:4d}  "
                             f"after={name}  before={nname}  [{why}]")
            for i, w in enumerate(words):
                addr = end + i * 4
                marker = ""
                if is_push_lr(w):
                    marker = "  <- push lr"
                elif is_ret_or_pop_pc(w):
                    marker = "  <- ret"
                out_lines.append(f"   {addr:#010x}: {w:08x}{marker}")
            out_lines.append("")

    out_lines.insert(2, f"# {interesting} code-shaped gaps flagged")
    Path(args.out).parent.mkdir(parents=True, exist_ok=True)
    Path(args.out).write_text("\n".join(out_lines), encoding="utf-8")
    print(f"wrote {args.out} ({interesting} flagged gaps)")


if __name__ == "__main__":
    main()
