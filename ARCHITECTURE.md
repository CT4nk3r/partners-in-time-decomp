# Game Engine Architecture — Mario & Luigi: Partners in Time

> Reconstructed from Ghidra analysis of the ARM9 binary (USA Rev 0).
> All addresses are for the USA Rev 0 ROM (game code ARME).

---

## 1. System Overview

The game runs on the NDS dual-CPU architecture:
- **ARM9** (ARMv5TE, 67 MHz): Main game logic, graphics setup, filesystem
- **ARM7** (ARMv4T, 33 MHz): Audio, touchscreen, WiFi, RTC

The ARM9 binary is the primary decompilation target. It uses Nintendo's
**Nitro SDK** (official NDS development kit) and a custom C++ game engine
built on top of it.

---

## 2. Boot Sequence

```
_start (0x02004800) — CRT0 startup stub
  │
  ├─ CP15 setup (0x02004A14) — cache, protection unit, TCM regions
  ├─ BSS clear + initialized data copy
  │
  ├─ OS_Init   (0x02046DFC) — Nitro SDK OS initialization
  ├─ GX_Init   (0x0202976C) — Graphics engine initialization
  ├─ FS_Init   (0x02048618) — Filesystem initialization
  │
  └─ NitroMain (0x02004B20) — Game entry point
       │
       ├─ game_init_heap  (0x02038DCC) — Heap allocator setup (8KB initial)
       ├─ game_init_gfx   (0x02035260) — VRAM banks, display modes
       ├─ game_init_fs    (0x0202A304) — Mount ROM filesystem
       ├─ game_init_sound (0x020424E4) — Sound system + SDAT load
       ├─ game_init       (0x020050F4) — Game state struct setup
       │
       └─ game_start      (0x02005444) — INFINITE MAIN LOOP
            └─ (never returns)
```

**Important:** `_start`, `NitroMain`, and several SDK init functions fall
in "gap" regions not detected by Ghidra's auto-analysis. They exist in
`arm9/asm/gaps/` and `arm9/asm/crt0.s` but NOT in `config/decompiled/`.

---

## 3. Main Loop (game_start @ 0x02005444)

Each frame the main loop does:

```
forever:
    inner_loop:
        GX_VBlankWait()          — Wait for vertical blank interrupt
        PAD_Read()               — Read button/touchscreen input
        FS_Update()              — Process pending filesystem operations
        game_update_display()    — Render current frame
        update_frame_counter()   — Increment if game is active
    until (not_paused AND display_ready)

    GX_SetMasterBrightness()     — Apply brightness/fade
    SND_Update()                 — Update sound playback (if reset pending)
    game_do_transition()         — Execute screen transition
```

---

## 4. Game Engine Classes (from C++ RTTI)

The binary contains embedded RTTI (Run-Time Type Information) strings that
reveal the C++ class hierarchy:

### 4.1 clGameMain (RTTI @ 0x0204FFC4)
**The root game manager.** Owns the main game state struct (~0x580 bytes).
Contains display flags, event bitfields, frame counters, and property arrays.

Key fields:
- `+0x048`: Event/flag bitfields (2 × u32)
- `+0x0D0`: Byte property array (indexed by GameProp tag type 0x6000)
- `+0x1F0`: Large bitfield array (128 × u32, indexed by tag type 0xE000)
- `+0x514`: Status flags (VBlank active, paused, frame update)
- `+0x518`: Frame counter
- `+0x57C`: Display configuration bits

### 4.2 clObjPA (RTTI @ 0x02050B20)
**Game object / entity base.** Used for characters, NPCs, items.
- `+0x1F`: Control flags byte (direction, rotation, address mode)
- `+0x7C`: State/parameter field

### 4.3 Animation System
Three related classes for sprite animation:
- **clCellAnimeBase** (RTTI @ 0x0205093C) — Abstract base
- **clCellAnime** (RTTI @ 0x02050970) — Standard sprite animation
- **clCellAnimePA** (RTTI @ 0x02050A48) — Partner Action animation variant
- **clCellAnimeTX** (RTTI @ 0x02050B54) — Texture-based animation variant

### 4.4 tsClMapControl (RTTI @ 0x02050C6C)
**Tilemap/background controller.** Manages up to 4 BG layers.
Each layer is a 0x48-byte struct at offsets +0x3F8, +0x440, +0x488, +0x4D0.
Layer fields include animation state, frame index, scroll position.

Related: **tsClMapControlVBlank** (RTTI @ 0x02050C80) — VBlank update variant.

### 4.5 clVBlankTask (RTTI @ 0x02050C5C)
**VSync task scheduler.** Manages functions that must run during VBlank.

### 4.6 clMesWinObj / clMesWinVBlank (RTTI @ 0x0205628C / 0x0205612C)
**Message/dialog window system.** Handles text boxes, dialog rendering.

---

## 5. Property System (GameProp)

The game uses a **tag-based property system** (decompiled in `game_prop.c`)
for reading/writing game state. Properties are identified by a 16-bit tag:

```
  Tag format:  [TTTT IIII IIII IIII]
                ^^^^                  = Type (4 bits)
                     ^^^^^^^^^^^^^^^^ = Index (12 bits)
```

| Type   | Storage | Description |
|--------|---------|-------------|
| 0x1000 | `context + idx*4 + 4` | Direct u32 array on context object |
| 0x2000 | `state + 0x48` | Bitfield in global state |
| 0x3000 | External | Dispatched to overlay handler |
| 0x4000 | External | Dispatched to overlay handler |
| 0x5000 | `state + idx*4` | Direct u32 array in global state |
| 0x6000 | `state + idx + 0xD0` | Byte array in global state |
| 0x7000 | External | Dispatched to overlay handler |
| 0x8000 | `param2 + idx*4 + 0x10` | Array on secondary context |
| 0x9000 | `context + idx*4 + 0x24` | Array on context object |
| 0xB000 | `context + 100` | Bitfield on context object |
| 0xC000 | `state + idx*4 + 0x40` | Array in global state |
| 0xD000 | `state + 0x50` | Bitfield in global state |
| 0xE000 | `state + 0x1F0` | Large bitfield in global state |

Types 0x3000, 0x4000, and 0x7000 dispatch to functions in overlays,
suggesting these properties are context-dependent (e.g., battle vs overworld).

---

## 6. Memory Management

### Heap Allocator (decompiled in `heap.c`)
- Block-based with free list, 16-byte headers
- Supports front-allocation (first-fit from low) and back-allocation (from high)
- Block splitting when remainder > 16 bytes
- Initial heap: 8KB buffer at BSS
- Block header: `prev | next | size (bit0=free) | flags (bits0-4=heap_id)`

### Memory Copy/Fill Utilities
Two tiers of memory functions:
1. **MI_CpuFill8 / MI_CpuCopy8** (`mem_util.c`) — Byte-granular with alignment handling
2. **MI_Fill16/32, MI_Copy16/32** (`mem_simple.c`) — Simple loops for known-aligned data
3. **MI_Fill32Fast / MI_Copy32Fast** (`mem_simple.c`) — Unrolled 8× for large transfers

---

## 7. Graphics Subsystem (0x02027000 - 0x0202BFFF)

47 graphics functions identified in this address range:

### Display Pipeline
- **GX_InitGraphicsEngine** (0x02028128, 508B) — VRAM allocation, buffer setup
- **GX_SetupVBlankRoutine** (0x02028774, 248B) — VBlank interrupt handler
- **GX_UpdateDisplay** (0x02028028, 76B) — Main VBlank display update
- **GX_SwapCommandBuffer** (0x020286B0, 92B) — Double-buffering

### Background System
- **GX_SetBGAffineMatrix** (0x02027404, 248B) — Affine transform setup
- **GX_LoadScreenData** (0x020277B4, 144B) — Load map/screen data
- **GX_LoadCharacterData** (0x020278FC, 348B) — Load sprite graphics + OAM

### Animation Queue
- **GX_BG_QueueAnimation** (0x020275E4, 104B) — Queue BG animations
- **GX_AnimationQueue_Update** (0x020276A0, 108B) — Tick animation timers

### Display Object System
- **GX_DisplayObjectQueue_Update** (0x02029EC4, 192B) — Process render queue
- **GX_DisplayObject_Show/Hide** — Visibility control

---

## 8. Sound System

Sound data loaded from `sound/sound_data.sdat` (NDS SDAT format).
Memory region named "SoundHeap" at runtime.
- Init function at 0x020424E4
- IPC commands sent to ARM7 for audio playback (0x27, 0x12 command IDs observed)

---

## 9. Filesystem

ROM filesystem mounted via Nitro SDK `FS_Init`. Key paths:
- `/FMap/FMapData.dat` — Field map data (overworld maps, room layouts)
- `sound/sound_data.sdat` — Sound archive

37 overlays loaded dynamically for different game contexts (battle, menus, etc.).

---

## 10. Function Statistics

| Category | Count | Bytes | % of ARM9 |
|----------|-------|-------|-----------|
| Tiny (<32B) | 183 | 3,692 | 1.0% |
| Small (32-128B) | 602 | 44,640 | 12.6% |
| Medium (128-512B) | 409 | 103,870 | 29.4% |
| Large (512-2048B) | 70 | 63,788 | 18.1% |
| Huge (>2048B) | 11 | 25,792 | 7.3% |
| **Detected** | **1,275** | **241,782** | **68.4%** |
| Gaps (data/undetected) | 824 | 112,178 | 31.6% |
| **ARM9 Total** | — | **353,240** (rev0) | **100%** |

---

## 11. Address Map (Key Regions)

| Range | Content |
|-------|---------|
| 0x02004000-0x02004800 | ARM9 header / padding |
| 0x02004800-0x02004B20 | CRT0 startup (gaps) |
| 0x02004B20-0x02006000 | Game core (init, main loop, property system) |
| 0x02006000-0x0200A000 | Object/entity system (clObjPA) |
| 0x0200A000-0x02010000 | Game logic |
| 0x02010000-0x02018000 | Sprite, math, animation |
| 0x02018000-0x02020000 | Map control (tsClMapControl), animation |
| 0x02020000-0x02027000 | Battle system, dialog |
| 0x02027000-0x0202C000 | Graphics engine (GX_*) |
| 0x0202C000-0x02030000 | Date/time, utility, filesystem wrappers |
| 0x02030000-0x02038000 | Sound, save, menu systems |
| 0x02038000-0x02040000 | OS services, memory, interrupts, CP15 |
| 0x02040000-0x02048000 | SDK core (OS_Init, timers, DMA) |
| 0x02048000-0x0204A000 | Filesystem (FS_Init, file ops) |
| 0x0204A000-0x02060000 | Data tables, RTTI strings, string literals |
