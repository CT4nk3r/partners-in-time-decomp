#!/usr/bin/env python3
"""find_uninit_data.py - static auditor for "DAT_xxxxxxxx" symbols that are
read (dereferenced or used as pointer base) under arm9/src/ without
having a host-side initializer wired in.

Heuristic, not exact:
  * scans `arm9/src/*.c` for tokens matching `DAT_[0-9A-Fa-f]{8}`
  * counts read-vs-write contexts crudely:
      read  : `*DAT_xxx`, `(...) + DAT_xxx`, `DAT_xxx[...]`, `(type)DAT_xxx`
      write : `DAT_xxx =`, `DAT_xxx +=` ... (assignment with DAT_ on the LHS)
  * cross-references against
      pc/src/host_data_init.c
      pc/src/host_display_data_init.c
      pc/src/host_undefined_stubs.c
      tools/scripts/extract_data_inits.py (slot manifest)
    to mark each DAT_xxx as
      INIT-EXTRACTED  (in extract_data_inits.py)
      INIT-HOST       (assigned in host_*_init.c)
      STUB-ZERO       (defined in host_undefined_stubs.c only - boots as 0)
      UNKNOWN         (no definition site found at all)

Outputs to stdout, sorted by risk: read-only && STUB-ZERO at the top.
This is the list a porter must walk through to extract more .data init
values from arm9.bin.

Usage:
  python tools/scripts/find_uninit_data.py
"""
from __future__ import annotations

import os
import re
import sys
from collections import defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
ARM9_SRC = ROOT / "arm9" / "src"
PC_SRC = ROOT / "pc" / "src"
MANIFEST = ROOT / "tools" / "scripts" / "extract_data_inits.py"

DAT_RE = re.compile(r"\bDAT_([0-9A-Fa-f]{8})\b")
WRITE_RE = re.compile(r"\bDAT_[0-9A-Fa-f]{8}\s*(?:=[^=]|\+=|-=|\*=|/=|<<=|>>=|&=|\|=|\^=)")


def scan_arm9() -> dict[str, dict[str, int]]:
    """Return {dat_lower: {"reads": n, "writes": n, "files": set(rel)}}."""
    out: dict[str, dict[str, object]] = defaultdict(
        lambda: {"reads": 0, "writes": 0, "files": set()}
    )
    for path in ARM9_SRC.rglob("*.c"):
        rel = path.relative_to(ROOT).as_posix()
        try:
            text = path.read_text(encoding="utf-8", errors="replace")
        except OSError:
            continue
        for line in text.splitlines():
            for m in DAT_RE.finditer(line):
                key = m.group(0)
                # crude write detection: this token immediately followed by `=` (not `==`)
                # or compound assignment.
                start = m.end()
                tail = line[start : start + 4]
                is_write = bool(re.match(r"\s*(=[^=]|\+=|-=|\*=|/=|<<=|>>=|&=|\|=|\^=)", tail))
                if is_write:
                    out[key]["writes"] += 1
                else:
                    out[key]["reads"] += 1
                out[key]["files"].add(rel)  # type: ignore[union-attr]
    return out


def scan_definers() -> tuple[set[str], set[str], set[str], set[str]]:
    """Return (extracted, host_init, stub_zero, asm_data)."""
    extracted: set[str] = set()
    if MANIFEST.exists():
        for m in DAT_RE.finditer(MANIFEST.read_text(encoding="utf-8", errors="replace")):
            extracted.add(m.group(0))
        # also pick up the 'addr' integer literals from the SLOTS list
        for m in re.finditer(r"0x([0-9A-Fa-f]{8})", MANIFEST.read_text("utf-8", "replace")):
            extracted.add(f"DAT_{m.group(1).upper()}")

    host_init: set[str] = set()
    for hpath in [
        PC_SRC / "host_data_init.c",
        PC_SRC / "host_display_data_init.c",
    ]:
        if not hpath.exists():
            continue
        text = hpath.read_text(encoding="utf-8", errors="replace")
        for line in text.splitlines():
            # any line that LHS-assigns g_disp_DAT_xxx or DAT_xxx
            mw = re.search(
                r"\b(?:g_disp_)?DAT_([0-9A-Fa-f]{8})\s*=", line
            )
            if mw:
                host_init.add(f"DAT_{mw.group(1).upper()}")
                host_init.add(f"DAT_{mw.group(1).lower()}")

    stub_zero: set[str] = set()
    stub_path = PC_SRC / "host_undefined_stubs.c"
    if stub_path.exists():
        text = stub_path.read_text(encoding="utf-8", errors="replace")
        # `uint32_t DAT_xxx = 0;` etc.
        for m in re.finditer(
            r"\b(?:u32|uint32_t|int|u16|short|u8|char|void\s*\*)\s+DAT_([0-9A-Fa-f]{8})\b",
            text,
        ):
            stub_zero.add(f"DAT_{m.group(1).upper()}")
            stub_zero.add(f"DAT_{m.group(1).lower()}")

    asm_data: set[str] = set()
    asm_dir = ROOT / "arm9" / "asm"
    if asm_dir.exists():
        for path in asm_dir.rglob("*.s"):
            try:
                text = path.read_text(encoding="utf-8", errors="replace")
            except OSError:
                continue
            for m in DAT_RE.finditer(text):
                asm_data.add(m.group(0))

    return extracted, host_init, stub_zero, asm_data


def classify(name: str, extracted, host_init, stub_zero, asm_data) -> str:
    upper = name.upper()
    if upper in extracted or name in extracted:
        return "INIT-EXTRACTED"
    if upper in host_init or name in host_init:
        return "INIT-HOST"
    if upper in asm_data or name in asm_data:
        return "ASM-DATA"
    if upper in stub_zero or name in stub_zero:
        return "STUB-ZERO"
    return "UNKNOWN"


def main() -> int:
    usage = scan_arm9()
    extracted, host_init, stub_zero, asm_data = scan_definers()

    rows = []
    for name, info in usage.items():
        cls = classify(name, extracted, host_init, stub_zero, asm_data)
        rows.append(
            (
                name,
                info["reads"],
                info["writes"],
                cls,
                sorted(info["files"])[:3],  # type: ignore[arg-type]
            )
        )

    # Risk ordering: read-only & STUB-ZERO first.
    def risk(r):
        name, reads, writes, cls, _ = r
        cls_rank = {"STUB-ZERO": 0, "UNKNOWN": 1, "ASM-DATA": 2, "INIT-EXTRACTED": 3, "INIT-HOST": 4}.get(cls, 5)
        rw = 0 if writes == 0 else 1
        return (cls_rank, rw, -reads, name)

    rows.sort(key=risk)

    print(f"# uninit-data audit  ({len(rows)} unique DAT_xxx symbols)\n")
    print(f"#  arm9 src dir : {ARM9_SRC.relative_to(ROOT).as_posix()}")
    print(f"#  manifest     : {MANIFEST.relative_to(ROOT).as_posix()}")
    print(f"#  classes      : STUB-ZERO < UNKNOWN < ASM-DATA < INIT-EXTRACTED < INIT-HOST\n")
    print(f"# {'name':<14}  {'cls':<14}  {'r':>4}  {'w':>3}  files")
    print(f"# {'-'*14}  {'-'*14}  {'-'*4}  {'-'*3}  {'-'*40}")
    for name, reads, writes, cls, files in rows:
        print(f"  {name:<14}  {cls:<14}  {reads:>4}  {writes:>3}  {','.join(files)}")

    # summary
    by_cls: dict[str, int] = defaultdict(int)
    for _, _, _, cls, _ in rows:
        by_cls[cls] += 1
    print("\n# summary by class:")
    for cls in ["STUB-ZERO", "UNKNOWN", "ASM-DATA", "INIT-EXTRACTED", "INIT-HOST"]:
        print(f"#   {cls:<15} {by_cls[cls]:>4}")

    risky = [r for r in rows if r[3] == "STUB-ZERO" and r[2] == 0]
    print(f"\n# {len(risky)} read-only DAT_xxx symbols are still STUB-ZERO -")
    print(f"# these are the prime candidates for the next round of .data extraction.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
