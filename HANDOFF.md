# Mario & Luigi: Partners in Time — Decompilation Handoff Guide

> **Purpose:** This document contains everything a new agent or developer needs to continue
> the decompilation work from where we left off. Read this before doing anything.

---

## 1. Project Overview

This is a **matching decompilation** of Mario & Luigi: Partners in Time (NDS) following the
pret-style methodology (similar to pokediamond). The goal is:

1. Disassemble the original ROM into annotated assembly
2. Decompile assembly into matching C code, function by function
3. Build system recompiles C → ARM → .nds ROM that is **byte-identical** to original
4. Once 100% decompiled, retarget to PC (SDL2) and Android (NDK)

**Legal model:** No copyrighted assets in the repo. Users must supply their own legally-owned
ROM file (`baserom.nds`) which is `.gitignore`d.

---

## 2. Target ROM

| Field | Value |
|-------|-------|
| Game | Mario & Luigi: Partners in Time |
| Platform | Nintendo DS |
| Primary target | **USA Rev 0** (game code `ARME`) |
| SHA256 | `6782543eaf82b2f72aaeb594eec5ff547128137427a9b51502b94be305d3c8c0` |
| ROM header title | `MARIO&LUIGI2` |
| ARM9 size | 353,240 bytes, loaded at **0x02004000** |
| ARM7 size | 159,524 bytes, loaded at **0x02380000** |
| Overlays | 37 total |
| Entry point | 0x02004800 (CRT0 startup) |

**Other supported versions** (config/versions.ini has SHA256 hashes):
- USA Rev 1 (`ARME`) — ARM9 differs ~864 bytes from Rev0
- EUR (`ARMP`) — ARM9 differs ~896 bytes from Rev0
- All share identical ARM7, same memory layout, same overlay count

---

## 3. Development Environment

### Installed tools (on this Windows machine):

| Tool | Version | Location |
|------|---------|----------|
| devkitARM | r67 (GCC 15.2.0) | `C:\devkitPro\devkitARM\` |
| ndstool | 2.3.1 | `C:\devkitPro\tools\bin\` |
| Ghidra | 12.0.4 | `C:\ghidra\ghidra_12.0.4_PUBLIC\` |
| Java (Temurin) | 17.0.18 | `C:\Program Files\Eclipse Adoptium\jdk-17.0.18.8-hotspot\` |
| Python | 3.14.3 | System PATH |
| GNU Make | 4.4.1 | System PATH |
| Git | Available | System PATH |

### Environment variables (set as user env vars):
```
DEVKITPRO=C:\devkitPro
DEVKITARM=C:\devkitPro\devkitARM
JAVA_HOME=C:\Program Files\Eclipse Adoptium\jdk-17.0.18.8-hotspot
```

### PATH additions needed per PowerShell session:
```powershell
$env:DEVKITPRO = "C:\devkitPro"
$env:DEVKITARM = "C:\devkitPro\devkitARM"
$env:PATH = "C:\devkitPro\devkitARM\bin;C:\devkitPro\tools\bin;$env:PATH"
```

### Compilation command for testing individual .c files:
```powershell
arm-none-eabi-gcc -c -mcpu=arm946e-s -mthumb-interwork -O2 -I arm9/include -o build/<name>.o arm9/src/<name>.c
```

### Disassemble compiled object to verify matching:
```powershell
arm-none-eabi-objdump -d build/<name>.o
```

---

## 4. Repository Structure

```
marionluigi-pit-decomp/
├── README.md                  # Project description, legal notices
├── SETUP.md                   # Tool installation guide
├── Makefile                   # Build system (devkitARM)
├── .gitignore                 # Excludes ROMs, build/, ghidra_projects/
│
├── rom/
│   └── README.txt             # Instructions for placing baserom.nds
│
├── arm9/
│   ├── src/                   # ★ DECOMPILED C SOURCE FILES (this is where work happens)
│   │   ├── bios.c             # SWI wrappers (3 functions)
│   │   ├── os_util.c          # SDK stubs, CLZ, swap, getters (9 functions)
│   │   ├── mem_util.c         # memset/memcpy variants (4 functions)
│   │   ├── heap.c             # Block-based heap allocator (4 functions)
│   │   ├── game_main.c        # Main game loop, init, fade (5 functions)
│   │   └── game_prop.c        # Tag-based property system (2 functions)
│   ├── include/               # Headers for decompiled code
│   │   ├── types.h            # Core types: u8-u64, s8-s64, fx16/fx32, BOOL
│   │   ├── nds_hardware.h     # NDS hardware register addresses
│   │   ├── functions.h        # Auto-generated declarations for all 1275 functions
│   │   ├── bios.h / os_util.h / mem_util.h / heap.h / game_main.h / game_prop.h
│   └── asm/
│       ├── crt0.s             # Annotated CRT0 startup assembly
│       ├── macros.inc         # Assembly macros
│       ├── func/              # 1,275 per-function .s files (raw disassembly)
│       └── gaps/              # 824 gap regions (data/padding/undetected funcs)
│
├── config/
│   ├── arm9.ld                # ARM9 linker script (base 0x02004000)
│   ├── arm7.ld                # ARM7 linker script (base 0x02380000)
│   ├── versions.ini           # SHA256 hashes for all 3 ROM versions
│   ├── overlays.txt           # Overlay table (37 entries)
│   ├── functions.txt          # Human-readable function map (1,275 entries)
│   ├── functions.csv          # Machine-readable: address,size,name,is_thunk,param_count
│   ├── symbols.txt            # Auto-generated symbol analysis
│   ├── known_symbols.txt      # ★ MANUALLY ANNOTATED symbol map (30+ identified)
│   └── decompiled/            # ★ GHIDRA PSEUDOCODE (1,274 individual .c files)
│       ├── FUN_02004228.c     # One file per function
│       ├── FUN_02005444.c     # e.g., game_start main loop
│       ├── ...                # Named by Ghidra function address
│       └── all_functions.c    # Combined 1.3MB pseudocode reference
│
├── tools/
│   ├── scripts/
│   │   ├── extract_rom.py     # ROM extraction using ndstool
│   │   ├── split_arm9.py      # Binary splitter → per-function .s files
│   │   ├── identify_symbols.py # Symbol identification/analysis
│   │   ├── verify_match.py    # ROM matching verification
│   │   └── ghidra_setup.py    # Ghidra project setup helper
│   └── ghidra/
│       ├── ExportFunctions.java   # Export function list from Ghidra (works)
│       ├── ExportDecompiled.java  # Export C pseudocode from Ghidra (works)
│       └── export_functions.py    # Python version (broken with Ghidra 12)
│
├── extracted/                 # Extracted ROM components (arm9.bin, arm7.bin, overlays, data)
├── ghidra_projects/           # Ghidra project files (gitignored)
├── build/                     # Compiled .o files (gitignored)
├── data/                      # Game data files (future)
└── lib/                       # Libraries (future)
```

---

## 5. Current Progress

### What's done:
- ✅ Full project scaffolding and build system
- ✅ ROM extraction and verification pipeline
- ✅ Ghidra headless analysis of ARM9 (1,275 functions detected)
- ✅ All 1,275 functions exported as individual Ghidra pseudocode .c files
- ✅ ARM9 binary split into per-function .s files
- ✅ Boot chain fully traced: `_start → OS_Init → GX_Init → FS_Init → NitroMain → game_start`
- ✅ C++ RTTI class hierarchy identified (9 game engine classes)
- ✅ **27 functions decompiled to C** across 6 source files, all compiling cleanly

### Function coverage:
- **1,275** total functions detected by Ghidra
- **27** decompiled to C so far (~2.1%)
- **241,782** bytes of code in detected functions (68.4% of ARM9)
- **112,178** bytes in gaps (31.6% — data, padding, undetected functions)
- **824** gap regions (many contain data tables, some contain undetected functions)

### What's NOT done:
- [ ] No matching verification yet (compiled C vs original binary comparison)
- [ ] CRT0 startup not written as matching assembly yet
- [ ] Most SDK functions still in raw disassembly
- [ ] Game engine classes (clGameMain, clCellAnime, etc.) not yet decompiled
- [ ] Overlay decompilation not started
- [ ] ARM7 decompilation not started
- [ ] No PC/Android port work yet

---

## 6. Architecture & Key Technical Details

### NDS Hardware:
- **ARM9** (main CPU): ARMv5TE, runs game logic, graphics setup
- **ARM7** (sub CPU): ARMv4T, handles audio, WiFi, touchscreen, RTC
- Mixed ARM/Thumb instruction set — some functions are Thumb (16-bit)

### Game Engine Architecture:
- C++ class-based with RTTI (runtime type information)
- Classes discovered via RTTI strings embedded in binary:
  - `clGameMain` — main game controller
  - `clCellAnime`, `clCellAnimePA`, `clCellAnimeTX` — sprite animation system
  - `clObjPA` — object/entity system
  - `clVBlankTask` — VSync task handlers
  - `tsClMapControl`, `tsClMapControlVBlank` — tilemap/world rendering
  - `clMesWinObj`, `clMesWinVBlank` — message/dialog windows

### Memory Map:
- ARM9 loaded at **0x02004000** (NOT 0x02000000 — important!)
- ARM7 loaded at **0x02380000**
- Entry point at **0x02004800** (CRT0, falls in a gap — not detected by Ghidra)
- NitroMain at **0x02004B20** (also in a gap)

### Boot Chain (from CRT0):
```
_start (0x02004800)
  → CP15 setup (0x02004A14)
  → Memory clear/copy
  → OS_Init (0x02046DFC)
  → GX_Init (0x0202976C)
  → FS_Init (0x02048618)
  → NitroMain (0x02004B20)
    → game_init_1..N (subsystem init chain)
    → game_start (0x02005444) — infinite main loop
```

### Key Data Paths in ROM Filesystem:
- `/FMap/FMapData.dat` — field map data
- `sound/sound_data.sdat` — sound archive (SDAT format)
- `FileSystem`, `SoundHeap` — named memory regions

---

## 7. How to Decompile a Function

### Step-by-step workflow:

1. **Pick a function** from `config/functions.csv` (start with small ones <128 bytes)

2. **Read the Ghidra pseudocode** in `config/decompiled/FUN_<address>.c`

3. **Read the raw assembly** in `arm9/asm/func/FUN_<address>.s`

4. **Write clean C** in a file under `arm9/src/` — translate Ghidra's pseudocode:
   - Replace `undefined4` → `u32`, `undefined2` → `u16`, `undefined1` → `u8`
   - Replace `DAT_XXXXXXXX` references with properly named globals/externs
   - Replace `FUN_XXXXXXXX` calls with named function calls (or leave as extern)
   - Reconstruct structs from `*(type *)(base + offset)` patterns
   - Use `config/known_symbols.txt` for already-identified function names

5. **Create matching header** in `arm9/include/`

6. **Compile and verify**:
   ```powershell
   arm-none-eabi-gcc -c -mcpu=arm946e-s -mthumb-interwork -O2 -I arm9/include -o build/test.o arm9/src/test.c
   arm-none-eabi-objdump -d build/test.o
   ```
   Compare the objdump output against the original assembly in `arm9/asm/func/`

7. **Check `known_symbols.txt`** and update it if you identify a new function name

### Tips for matching:
- Compiler is GCC with `-O2 -mcpu=arm946e-s -mthumb-interwork`
- The original likely used a similar devkitARM or official Nintendo SDK compiler
- Some functions may need `-mthumb` if they were compiled as Thumb code
- Watch for functions that Ghidra mis-identified (wrong boundaries, missed functions)
- Gap regions (`arm9/asm/gaps/`) may contain undetected functions — check manually

### Priority order for decompilation:
1. **Tiny functions** (<32 bytes): stubs, getters, BIOS wrappers — easiest to match
2. **SDK functions**: memset, memcpy, heap, DMA — well-known patterns
3. **Game init chain**: the functions called from NitroMain
4. **Core engine**: game_start, property system, object system
5. **Game logic**: battle system, map control, animations
6. **Large functions** (>2KB): state machines, likely need struct reconstruction first

---

## 8. Known Ghidra Quirks

- **Python scripts (.py) don't work** in Ghidra 12 headless mode without PyGhidra. Use Java scripts instead.
- **The decompiler API method** is `getDecompiledFunction()` (not `depiledFunction`)
- Use `-noanalysis` flag when re-processing already-analyzed binaries
- ARM9 processor spec: `ARM:LE:32:v5t`; ARM7: `ARM:LE:32:v4t`
- Many important functions fall in **gaps** (CRT0, NitroMain, SDK init) — Ghidra didn't auto-detect them
- To run Ghidra headless analysis:
  ```powershell
  $env:MLPIT_OUTPUT_DIR = "C:\Users\ct4nk3r\Documents\marionluigi-pit-decomp\config"
  & "C:\ghidra\ghidra_12.0.4_PUBLIC\support\analyzeHeadless.bat" `
    "C:\Users\ct4nk3r\Documents\marionluigi-pit-decomp\ghidra_projects" mlpit_arm9 `
    -noanalysis -scriptPath "C:\Users\ct4nk3r\Documents\marionluigi-pit-decomp\tools\ghidra" `
    -postScript ExportDecompiled.java
  ```

---

## 9. Already Decompiled Functions (Address → Name Mapping)

| Address | Ghidra Name | Decompiled Name | File | Size |
|---------|-------------|-----------------|------|------|
| 0x02004228 | FUN_02004228 | BIOS_Stop | bios.c | 4B |
| 0x0200429C | FUN_0200429c | BIOS_VBlankIntrWait | bios.c | 6B |
| 0x0200445A | FUN_0200445a | BIOS_CpuSet | bios.c | 4B |
| 0x0203928C | FUN_0203928c | OS_Dummy | os_util.c | 4B |
| 0x020106F4 | FUN_020106f4 | OS_Nop | os_util.c | 8B |
| 0x02044644 | FUN_02044644 | OS_Stub | os_util.c | 4B |
| 0x02039BE8 | FUN_02039be8 | OS_ReturnZero | os_util.c | 8B |
| 0x0203B0D0 | FUN_0203b0d0 | OS_CountLeadingZeros | os_util.c | 8B |
| 0x0203BAD8 | FUN_0203bad8 | OS_Swap | os_util.c | 8B |
| 0x02038F14 | FUN_02038f14 | OS_GetField_04 | os_util.c | 8B |
| 0x020392D8 | FUN_020392d8 | OS_GetField_70 | os_util.c | 8B |
| 0x0203CE28 | FUN_0203ce28 | OS_GetField_38 | os_util.c | 8B |
| 0x0203B914 | FUN_0203b914 | MI_CpuFill8 | mem_util.c | 148B |
| 0x0203B9A8 | FUN_0203b9a8 | MI_CpuCopy8 | mem_util.c | 304B |
| 0x0202CC10 | FUN_0202cc10 | MI_CpuFill32Fast | mem_util.c | 72B |
| 0x0202CC00 | FUN_0202cc00 | RTC_SetDate | mem_util.c | 12B |
| 0x02029964 | FUN_02029964 | OS_AllocFromHeap | heap.c | 332B |
| 0x02029C1C | FUN_02029c1c | OS_Alloc | heap.c | 36B |
| 0x02029BF8 | FUN_02029bf8 | OS_AllocBack | heap.c | 36B |
| 0x02028EA8 | FUN_02028ea8 | OS_InitHeap | heap.c | 120B |
| 0x020050F4 | FUN_020050f4 | game_init | game_main.c | 228B |
| 0x02005444 | FUN_02005444 | game_start | game_main.c | 344B |
| 0x02005380 | FUN_02005380 | game_check_reset | game_main.c | 176B |
| 0x0200528C | FUN_0200528c | game_fade_out | game_main.c | 100B |
| 0x02005300 | FUN_02005300 | game_check_display_ready | game_main.c | 112B |
| 0x020055C0 | FUN_020055c0 | GameProp_Set | game_prop.c | 828B |
| 0x02005900 | FUN_02005900 | GameProp_Get | game_prop.c | 616B |

---

## 10. Immediate Next Steps (Priority Order)

### A. Continue small function decompilation
- There are **183 functions under 32 bytes** — most are trivial getters/setters/stubs
- Batch these by examining `config/decompiled/` files and writing C equivalents
- Good candidates: any function that returns a constant, reads a field, or is an empty stub

### B. Write matching CRT0 startup
- `arm9/asm/crt0.s` exists with annotations but needs to be a proper matching source
- CRT0 lives in a gap region (0x02004800), not detected by Ghidra
- Must set up CP15, clear BSS, copy initialized data, then jump to NitroMain

### C. Decompile SDK functions
- Many Nitro SDK functions follow well-documented patterns
- Key targets: `OS_Init`, `GX_Init`, `FS_Init`, interrupt handlers, DMA
- Reference: Nintendo SDK documentation (if available) or other NDS decomp projects

### D. Reconstruct game structs
- Ghidra pseudocode shows `*(type *)(base + offset)` patterns everywhere
- Need to define proper C structs matching these offset patterns
- Start with GameState struct (partially defined in `game_main.h`)
- RTTI classes need vtable + member layout reconstruction

### E. Build system integration
- Current Makefile is a skeleton — needs to actually link .c + .s files together
- Goal: `make` produces arm9.bin that matches `extracted/arm9.bin` byte-for-byte
- Each decompiled .c replaces its corresponding .s file in the build

### F. Matching verification
- `tools/scripts/verify_match.py` exists but needs testing
- Should compare compiled output against original binary section by section
- Per-function matching is the gold standard

---

## 11. Function Size Distribution (for planning)

| Category | Count | Bytes | % of Code |
|----------|-------|-------|-----------|
| Tiny (<32B) | 183 | 3,692 | 1.5% |
| Small (32-128B) | 602 | 44,640 | 18.5% |
| Medium (128-512B) | 409 | 103,870 | 42.9% |
| Large (512-2048B) | 70 | 63,788 | 26.4% |
| Huge (>2048B) | 11 | 25,792 | 10.7% |
| **Total** | **1,275** | **241,782** | **100%** |

Plus 824 gap regions totaling 112,178 bytes (data, padding, undetected functions).

---

## 12. Git History

```
a9c57e9 Decompile first batch: BIOS, OS utils, memory, heap, game main, property system
4180be1 Split ARM9 into 1275 functions, map game engine structure
2d0d3b4 Add Ghidra analysis: 1275 ARM9 functions detected
74d80a3 Fix disassembly bug, add multi-version support
6a0f958 Fix game code, ARM9 base address, and add overlay config
d311619 Initial project scaffolding for M&L:PiT NDS decompilation
```

---

## 13. Reference Projects

Study these for patterns and conventions:
- **pret/pokediamond** — NDS Pokemon Diamond decomp (most mature NDS decomp)
- **pret/pokeemerald** — GBA Pokemon Emerald decomp (mature, good build system reference)
- **zeldaret/tp** — Zelda Twilight Princess decomp (GameCube, good struct reconstruction examples)

---

## 14. Important Warnings

1. **ARM9 base is 0x02004000**, NOT 0x02000000 — this caused bugs early on
2. **Ghidra Python scripts don't work** in headless mode with Ghidra 12 — use Java
3. **Some critical functions are in gaps** — Ghidra didn't detect them. Check `arm9/asm/gaps/`
4. **The ROM must NEVER be committed** to the repository
5. **`config/decompiled/all_functions.c`** is 1.3MB — don't try to load it all at once, use individual files
6. **USA Rev 0 is the primary target** — always match against that version first
7. The game uses **C++ RTTI** but the decompilation is in C — reconstruct classes as structs with function pointer tables (vtables)
