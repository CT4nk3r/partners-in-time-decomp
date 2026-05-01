#!/usr/bin/env python3
"""
gen_host_stubs.py - Generate stubs for all undefined symbols when linking
arm9/src/*.c into the PC port.

Reads build_log.txt produced by the failed link step and emits
pc/src/host_undefined_stubs.c with placeholder definitions:
  - DAT_*  -> uint32_t DAT_xxx;
  - FUN_*  -> void FUN_xxx(void) {}    (variadic-compatible)
  - thunk_*-> aliased to their FUN_ target
  - SDK    -> void FS_LoadOverlay(void) {} etc.

The stubs return zero / do nothing so the program links and runs but
the underlying SDK behaviours are no-ops. This is the starting point
for incremental porting.
"""
import re
import sys
from pathlib import Path
from collections import defaultdict

ROOT = Path(__file__).resolve().parents[2]
LOG = ROOT / "build_log.txt"
OUT = ROOT / "pc" / "src" / "host_undefined_stubs.c"

if not LOG.exists():
    print(f"ERROR: {LOG} not found")
    sys.exit(1)

undef = set()
for line in LOG.read_text(errors="replace").splitlines():
    m = re.search(r"undefined reference to `([A-Za-z_][A-Za-z0-9_]*)'", line)
    if m:
        undef.add(m.group(1))

# Also preserve symbols from any existing stubs file (so iterative runs accumulate)
if OUT.exists():
    existing = OUT.read_text(errors="replace")
    # uint32_t SYM;  or  int SYM() { ... }
    for m in re.finditer(r"^(?:uint32_t|int)\s+([A-Za-z_][A-Za-z0-9_]*)\s*[\(;]", existing, re.MULTILINE):
        undef.add(m.group(1))

# DON'T strip leading underscores - Ghidra emits both `DAT_xxx` and `_DAT_xxx`
# variants for hardware register addresses (different symbols!)
# undef = clean (was stripping)

groups = defaultdict(list)
for s in sorted(undef):
    if s.startswith("DAT_") or s.startswith("_DAT_"):
        groups["DAT"].append(s)
    elif s.startswith("FUN_") or s.startswith("_FUN_"):
        groups["FUN"].append(s)
    elif s.startswith("thunk_FUN_") or s.startswith("_thunk_FUN_"):
        groups["THUNK"].append(s)
    elif s.startswith("func_0x"):
        groups["FUNC_HEX"].append(s)
    else:
        groups["OTHER"].append(s)

lines = [
    "/*",
    " * host_undefined_stubs.c - Auto-generated stubs for the PC port linker",
    " *",
    " * The decompiled arm9/src/*.c references many ARM-only symbols that are",
    " * normally resolved by the ARM linker script (config/undefined_syms.ld)",
    " * or come from the original ROM as raw assembly bytes. On the host we",
    " * cannot resolve them that way, so we provide weak stubs here just so",
    " * the program links and runs.",
    " *",
    " * REGENERATE WITH: python tools/scripts/gen_host_stubs.py",
    " */",
    "#include <stdint.h>",
    "",
    "/* Suppress warnings about unused/empty stubs */",
    "#pragma GCC diagnostic push",
    "#pragma GCC diagnostic ignored \"-Wunused-parameter\"",
    "#pragma GCC diagnostic ignored \"-Wmain\"",
    "",
]

# DAT_ globals - these are u32 storage. The original code accesses them
# as &DAT_xxx (pointer) and *DAT_xxx (value). We just provide bytes.
lines.append(f"/* === DAT_* storage ({len(groups['DAT'])} symbols) === */")
for s in groups["DAT"]:
    lines.append(f"uint32_t {s};")
lines.append("")

# FUN_* function stubs - variadic-compatible, return zero
lines.append(f"/* === FUN_* stubs ({len(groups['FUN'])} symbols) === */")
for s in groups["FUN"]:
    lines.append(f"int {s}() {{ return 0; }}")
lines.append("")

# thunk_FUN_* - same treatment
lines.append(f"/* === thunk_FUN_* stubs ({len(groups['THUNK'])} symbols) === */")
for s in groups["THUNK"]:
    lines.append(f"int {s}() {{ return 0; }}")
lines.append("")

# func_0xHEX - boot ROM functions called via address
lines.append(f"/* === func_0x* stubs ({len(groups['FUNC_HEX'])} symbols) === */")
for s in groups["FUNC_HEX"]:
    lines.append(f"int {s}() {{ return 0; }}")
lines.append("")

# Other named SDK / IRQ / engine symbols
lines.append(f"/* === Named SDK stubs ({len(groups['OTHER'])} symbols) === */")
for s in groups["OTHER"]:
    # Skip any symbols that are already provided by libc, SDL, or our platform
    if s in ("memcpy", "memset", "memmove", "strlen", "strcpy",
             "main", "__main", "_main",
             "SDL_Init", "SDL_Quit"):
        lines.append(f"/* skipped: {s} */")
        continue
    lines.append(f"int {s}() {{ return 0; }}")
lines.append("")

lines.append("#pragma GCC diagnostic pop")
lines.append("")

OUT.write_text("\n".join(lines), encoding="utf-8")
print(f"Wrote {OUT}")
print(f"  DAT:        {len(groups['DAT'])}")
print(f"  FUN:        {len(groups['FUN'])}")
print(f"  thunk_FUN:  {len(groups['THUNK'])}")
print(f"  func_0xHEX: {len(groups['FUNC_HEX'])}")
print(f"  Other:      {len(groups['OTHER'])}")
print(f"  Total:      {sum(len(v) for v in groups.values())}")
