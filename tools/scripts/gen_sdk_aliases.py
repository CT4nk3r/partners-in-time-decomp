#!/usr/bin/env python3
"""
gen_sdk_aliases.py — Build pc/include/sdk_symbol_aliases.h.

Strategy
--------
The decompiled `arm9/src/*.c` files reference functions both by their
Ghidra-style anonymous names (FUN_xxxxxxxx) AND by hand-renamed
identifiers (game_start, BIOS_Stop, FS_OpenFile, …).  Different files
were renamed at different times, so the same physical address may be
reachable via two different identifiers from two different translation
units, and `host_undefined_stubs.c` ends up shadowing the real symbol
with a `return 0;` placeholder.

This script collects every `FUN_<addr> -> real_name` correspondence we
can find and emits a single header that #defines the FUN_ alias to its
real name.  The header is force-included into every arm9/src/*.c via
CMake `-include`, which means callers compile against the real names
and the link succeeds without a shadow stub.

Sources of truth (in priority order):
  1. `Original: FUN_<addr>` doc-comments paired with the next function
     definition in the same file.
  2. `arm9/src/bios.c` header lists `FUN_<addr> @ ... - SWI 0x..`,
     which lines up 1:1 with the BIOS_* helpers below it.
  3. `config/known_symbols.txt` — manually-curated address -> name
     map (LZ77 decompressors, crt0_copy_sections, …).

Symbols that exist in `config/known_symbols.txt` as `(gap - …)` are
*not* present as FUN_<addr> in the decomp (they live between known
functions), so we skip them.
"""
from __future__ import annotations
import re
from pathlib import Path
from collections import OrderedDict

ROOT = Path(__file__).resolve().parents[2]
ARM9_SRC = ROOT / "arm9" / "src"
KNOWN_SYMS = ROOT / "config" / "known_symbols.txt"
OUT = ROOT / "pc" / "include" / "sdk_symbol_aliases.h"

# FUN_<addr> appearing as a function-definition target (via the
# `Original: FUN_xxxxxxxx` comment) and the C identifier that follows.
ORIG_RE = re.compile(r"Original:\s*FUN_([0-9a-fA-F]+)")
DEFN_RE = re.compile(
    r"^\s*(?:static\s+|inline\s+|NORETURN\s+)*"
    r"(?:void|int|unsigned|signed|s?32|u?32|s?16|u?16|s?8|u?8|bool|char)"
    r"\s*\*?\s*([A-Za-z_][A-Za-z0-9_]*)\s*\(",
    re.MULTILINE,
)

# Lines like:  FUN_02046ffc, 20, FUN_02046ffc, , bios (calls BIOS LZ77UnCompVram)
KNOWN_FUN_RE = re.compile(
    r"@\s*0x([0-9a-fA-F]+)\s*\(FUN_[0-9a-fA-F]+",
)
# [sdk]    LZ77_UncompVram_1   @ 0x02046FFC (FUN_02046ffc - calls BIOS LZ77UnCompVram)
KNOWN_LINE_RE = re.compile(
    r"^\[\w+\]\s+([A-Za-z_][A-Za-z0-9_]*)\s+@\s*0x([0-9a-fA-F]+)\s*\(FUN_[0-9a-fA-F]+",
    re.MULTILINE,
)


def collect_from_arm9_src() -> "OrderedDict[str,str]":
    """Pair each `Original: FUN_<addr>` with the next C function definition."""
    out: "OrderedDict[str,str]" = OrderedDict()
    for c in sorted(ARM9_SRC.glob("*.c")):
        text = c.read_text(errors="replace")
        # Walk through `Original:` markers in order; for each, scan from
        # its position forward for the first function-definition signature.
        for m in ORIG_RE.finditer(text):
            addr = m.group(1).lower()
            if len(addr) != 8:
                continue
            tail = text[m.end():m.end() + 4000]
            d = DEFN_RE.search(tail)
            if not d:
                continue
            real = d.group(1)
            # Skip self-aliases (the comment is just describing FUN_x as itself)
            if real.lower().startswith("fun_") or real == "main":
                continue
            fun = f"FUN_{addr}"
            # First mapping wins (don't overwrite hand-curated entries)
            out.setdefault(fun, real)
    return out


def collect_from_known_symbols() -> "OrderedDict[str,str]":
    """Parse config/known_symbols.txt entries that reference an existing FUN_<addr>."""
    out: "OrderedDict[str,str]" = OrderedDict()
    if not KNOWN_SYMS.exists():
        return out
    text = KNOWN_SYMS.read_text(errors="replace")
    for m in KNOWN_LINE_RE.finditer(text):
        real, addr = m.group(1), m.group(2).lower()
        if real.startswith("FUN_"):
            continue
        out[f"FUN_{addr}"] = real
    return out


def collect_bios_explicit() -> "OrderedDict[str,str]":
    """bios.c lists each FUN_<addr> in its header comment;
    pair them positionally with the BIOS_* helpers below."""
    bios = ARM9_SRC / "bios.c"
    if not bios.exists():
        return OrderedDict()
    text = bios.read_text(errors="replace")
    addrs = re.findall(r"FUN_([0-9a-f]{8})\s*@\s*0x[0-9A-F]+", text)
    names = re.findall(r"^\s*void\s+(BIOS_[A-Za-z0-9_]+)\s*\(", text, re.MULTILINE)
    out: "OrderedDict[str,str]" = OrderedDict()
    for a, n in zip(addrs, names):
        out[f"FUN_{a}"] = n
    return out


def main() -> int:
    aliases: "OrderedDict[str,str]" = OrderedDict()
    for src in (collect_bios_explicit(),
                collect_from_arm9_src(),
                collect_from_known_symbols()):
        for k, v in src.items():
            aliases.setdefault(k, v)

    # Sort by address for stable output
    items = sorted(aliases.items(), key=lambda kv: kv[0])

    OUT.parent.mkdir(parents=True, exist_ok=True)
    lines = [
        "/*",
        " * sdk_symbol_aliases.h - Auto-generated FUN_<addr> -> real_name macros.",
        " *",
        " * Force-included into every arm9/src/*.c on HOST_PORT builds via CMake",
        " * `-include`. This makes anonymous Ghidra calls land on the hand-named",
        " * implementations (game_start, BIOS_Stop, …) instead of the no-op",
        " * shadow stubs in pc/src/host_undefined_stubs.c.",
        " *",
        " * Regenerate with:  python tools/scripts/gen_sdk_aliases.py",
        " */",
        "#ifndef SDK_SYMBOL_ALIASES_H",
        "#define SDK_SYMBOL_ALIASES_H",
        "",
        f"/* {len(items)} mappings */",
        "",
    ]
    for fun, real in items:
        lines.append(f"#define {fun} {real}")
    lines += [
        "",
        "#endif /* SDK_SYMBOL_ALIASES_H */",
        "",
    ]
    OUT.write_text("\n".join(lines), encoding="utf-8")

    # Also emit a sidecar list of the real names so gen_host_stubs.py can
    # exclude them (and the FUN_ aliases) from the auto-stub file.
    sidecar = ROOT / "pc" / "include" / "sdk_symbol_aliases.txt"
    with sidecar.open("w", encoding="utf-8") as f:
        for fun, real in items:
            f.write(f"{fun} {real}\n")

    print(f"Wrote {OUT}  ({len(items)} aliases)")
    print(f"Wrote {sidecar}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
