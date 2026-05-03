#!/usr/bin/env python3
"""
gen_host_stubs.py - Generate stubs for all undefined symbols when linking
arm9/src/*.c into the PC port.

Reads build_log.txt produced by the failed link step and emits
pc/src/host_undefined_stubs.c with placeholder definitions:
  - DAT_*  -> uintptr_t DAT_xxx;
  - FUN_*  -> void FUN_xxx(void) {}    (variadic-compatible)
  - thunk_*-> aliased to their FUN_ target
  - SDK    -> void FS_LoadOverlay(void) {} etc.

The stubs return zero / do nothing so the program links and runs but
the underlying SDK behaviours are no-ops. This is the starting point
for incremental porting.

CLI flags:
  --trace   Instrument every FUN_/thunk_/func_0x stub with first-call
            logging to stderr.  Used by tools/scripts/trace_stubs.py to
            discover the actual hot callees during a real game run.
"""
import re
import sys
from pathlib import Path
from collections import defaultdict

TRACE = "--trace" in sys.argv[1:]

ROOT = Path(__file__).resolve().parents[2]
LOG = ROOT / "build_log.txt"
OUT = ROOT / "pc" / "src" / "host_undefined_stubs.c"

# Symbols implemented in link_stubs.c or other hand-written files.
# These must NOT appear in the auto-generated stubs to avoid duplicate symbol errors.
EXCLUDE_SYMBOLS = {
    # Already in link_stubs.c (original stubs)
    "OS_Init", "GX_Init", "FS_Init", "OS_MemClear",
    "OS_LockMutex", "OS_UnlockMutex", "OS_CreateHeap", "OS_ActivateHeap",
    "OS_SetHeapConfig", "OS_SetHeapMode",
    "irq_enable", "irq_setup", "lock_acquire", "lock_release",
    "panic_check1", "panic_check2",
    "dma_command", "dma_channel_enable", "dma_channel_reset",
    "gx_display_reset", "gx_clear_flag", "gx_check_render_available",
    "gx_get_current_mode", "gx_get_display_buffer", "gx_get_display_status",
    "gx_refresh_display", "gx_update_base_display", "gx_update_display_entry",
    "GameProp_ExternalGet3000", "GameProp_ExternalGet4000", "GameProp_ExternalGet7000",
    "GameProp_ExternalSet3000", "GameProp_ExternalSet7000",
    "power_request_fifo", "get_priority_used1", "get_priority_used2",
    "struct_setup_advanced", "struct_setup_state", "NitroMain",
    # New SDK wrappers being added to link_stubs.c (Task 1)
    "GX_VBlankWait", "GX_SwapDisplay", "GX_SetMasterBrightness",
    "GX_SetVisiblePlane", "GX_UpdateDisplay", "GX_DisableInterrupts",
    "GX_FlushDisplay", "GX_ResetVisiblePlane", "GX_SetDispSelect",
    "GX_GetCurrentMode",
    "FS_InitOverlay", "FS_LoadOverlay", "FS_AttachOverlay", "FS_Update",
    "SND_Init", "SND_Update",
    "PAD_Read",
    "OBJ_Init", "OBJ_Create", "OBJ_Update",
    # Inner-loop named stubs hoisted into link_stubs.c (Task 4)
    "game_update_display", "game_do_transition",
    # Scene factory dispatch — implemented as a host shim in pc/src
    # (shadow-buffers-dma session, Task 4)
    "game_setup_overlay",
    # From nds_rom.c (Task 3)
    "rom_load", "rom_data", "rom_size", "rom_read_file", "rom_read_overlay",
}

if not LOG.exists():
    print(f"ERROR: {LOG} not found")
    sys.exit(1)

undef = set()
for line in LOG.read_text(errors="replace").splitlines():
    m = re.search(r"undefined reference to `([A-Za-z_][A-Za-z0-9_]*)'", line)
    if m and m.group(1) not in EXCLUDE_SYMBOLS:
        undef.add(m.group(1))

# Also preserve symbols from any existing stubs file (so iterative runs accumulate)
if OUT.exists():
    existing = OUT.read_text(errors="replace")
    # uint32_t SYM;  or  int SYM() { ... }
    for m in re.finditer(r"^(?:uintptr_t|uint32_t|int)\s+([A-Za-z_][A-Za-z0-9_]*)\s*[\(;]", existing, re.MULTILINE):
        sym = m.group(1)
        if sym not in EXCLUDE_SYMBOLS:
            undef.add(sym)

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
    f" * TRACE MODE: {'ON' if TRACE else 'OFF'}  (--trace adds first-call stderr logging)",
    " */",
    "#include <stdint.h>",
]
if TRACE:
    lines.append("#include <stdio.h>")
lines += [
    "",
    "/* Suppress warnings about unused/empty stubs */",
    "#pragma GCC diagnostic push",
    "#pragma GCC diagnostic ignored \"-Wunused-parameter\"",
    "#pragma GCC diagnostic ignored \"-Wmain\"",
    "",
]

def stub_body(sym: str) -> str:
    if TRACE:
        return ("{ static int s; if(!s){s=1; "
                f"fprintf(stderr,\"[stub] {sym}\\n\");}} return 0; }}")
    return "{ return 0; }"

# DAT_ globals - host storage. The arm9 decomp declares many of these as
# `extern u32 *DAT_xxx;` (pointer = 8 bytes on Win64). If we back them with
# only 4 bytes, an 8-byte load reads garbage in the high half and produces
# non-canonical pointers. Use uintptr_t so the slot is wide enough; dat-init
# still copies just 4 bytes from arm9.bin (low half), high half stays zero.
lines.append(f"/* === DAT_* storage ({len(groups['DAT'])} symbols) === */")
for s in groups["DAT"]:
    lines.append(f"uintptr_t {s};")
lines.append("")

# FUN_* function stubs - variadic-compatible, return zero
lines.append(f"/* === FUN_* stubs ({len(groups['FUN'])} symbols) === */")
for s in groups["FUN"]:
    lines.append(f"int {s}() {stub_body(s)}")
lines.append("")

# thunk_FUN_* - same treatment
lines.append(f"/* === thunk_FUN_* stubs ({len(groups['THUNK'])} symbols) === */")
for s in groups["THUNK"]:
    lines.append(f"int {s}() {stub_body(s)}")
lines.append("")

# func_0xHEX - boot ROM functions called via address
lines.append(f"/* === func_0x* stubs ({len(groups['FUNC_HEX'])} symbols) === */")
for s in groups["FUNC_HEX"]:
    lines.append(f"int {s}() {stub_body(s)}")
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
    if s in EXCLUDE_SYMBOLS:
        lines.append(f"/* excluded: {s} */")
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
