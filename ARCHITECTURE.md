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

### Object Factory System (decompiled in `alloc_wrappers.c`)
- 8 factory functions allocate fixed-size structs (0x30 or 0x38 bytes) via `OS_Alloc`
- Each immediately calls a constructor-like init function (in overlay space 0x0206-0x0207xxxx)
- All constructors take `(ptr, type=8, param=0)` — suggests a common base class init
- Heap metadata operations: block merging (`heap_merge_blocks`), slot clearing
- Mutex-protected operations for thread safety: `obj_set_flags`, `obj_clear_set_flags`

### Memory Copy/Fill Utilities
Two tiers of memory functions:
1. **MI_CpuFill8 / MI_CpuCopy8** (`mem_util.c`) — Byte-granular with alignment handling
2. **MI_Fill16/32, MI_Copy16/32** (`mem_simple.c`) — Simple loops for known-aligned data
3. **MI_Fill32Fast / MI_Copy32Fast** (`mem_simple.c`) — Unrolled 8× for large transfers

---

## 7. Graphics Subsystem (0x02027000 - 0x0202BFFF)

47 graphics functions identified in this address range, 23 decompiled in `gx_display.c`.

### Display Pipeline (decompiled)
- **gx_init_display_layers** (0x02027710, 64B) — Init 4 BG layer entries (pos, priority, data)
- **gx_main_update** (0x02028028, 76B) — Per-frame update: process delta frames, refresh, animate
- **gx_cleanup_render** (0x020286B0, 92B) — Disable VRAM/OAM mapping, release resources
- **gx_alloc_display_resources** (0x02028710, 96B) — Alloc render memory, enable VRAM mapping
- **gx_queue_render** (0x02028528, 136B) — Queue render op via callback function pointer

### Display Mode & State
- **gx_init_mode_a / gx_init_mode_b** — Two display mode initializers
- **gx_check_ready** — Returns FALSE if display status is 0 or state == 2
- **gx_set_mode** — Mode switch with default fallback to mode 8

### CRC / Integrity Verification
- **gx_verify_crc** (0x020283c8, 64B) — Compare computed CRC vs stored u16
- **gx_validate_buffer**, **gx_check_header**, **gx_check_footer** — Integrity checks
- **gx_compare_signature** (0x02028c20, 76B) — 6-byte signature match + magic byte check

### Background System
- **GX_SetBGAffineMatrix** (0x02027404, 248B) — Affine transform setup
- **GX_LoadScreenData** (0x020277B4, 144B) — Load map/screen data
- **GX_LoadCharacterData** (0x020278FC, 348B) — Load sprite graphics + OAM

### Animation Queue
- **GX_BG_QueueAnimation** (0x020275E4, 104B) — Queue BG animations
- **GX_AnimationQueue_Update** (0x020276A0, 108B) — Tick animation timers

---

## 8. OS Services (decompiled in `os_system.c`)

31 functions decompiled covering the core OS layer:

### IRQ System
- **os_irq_init** (0x0203a598) — Top-level IRQ + system init
- **os_irq_state_init** (0x0203b160) — Zero 9 IRQ state table entries

### DMA Channel Wrappers (14 functions)
All dispatch through `dma_command` (FUN_0203bda8) with different command IDs:
- Commands 6, 7, 17-27, 30-32 mapped to specific DMA operations
- **dma_cmd_17** uses bit-packed flags (5 boolean params → single u32)
- **dma_cmd_18** computes auxiliary param via `dma_aux_param()`

### IPC (Inter-Processor Communication)
- **os_ipc_init** — One-time init: set flag, reset cmd, enable interrupt 0x10
- **os_ipc_send / os_ipc_recv** — Dispatch through function pointer tables
- **os_ipc_sync / os_ipc_async** — Sync/async variants with stored params

### Power Management
- **os_power_fifo_wait** — Busy-loop sending power FIFO commands
- **os_panic_shutdown** — Infinite loop (unrecoverable error)

### Timer & Priority
- **os_timer_dispatch** — Timer callback via function pointer
- **os_priority_increment** — Increment task priority counter (0xC-byte stride)

---

## 9. Math & State Management (decompiled in `math_state.c`)

35 functions covering low-level math, synchronization, and data structures:

### VRAM Address Calculation
- 4 functions compute VRAM character/screen base addresses from DISPCNT register
- 2 validation getters check mode flags before returning addresses
- All use `REG_DISPCNT` (0x04000000) bitfield extraction

### Color Blending (RGB555)
- **color_blend** — Linear interpolation between two 15-bit colors
- **color_brighten** — Blend toward white with saturation
- **color_darken** — Blend toward black with saturation
- **color_interpolate** — Single-color interpolation toward complement
- All use expanded RGB555 mask technique: `(color | color<<15) & 0x03E07C1F`

### Random Number Generator
- **rng_next** — Linear congruential generator: `seed = (seed * 0x29 + scramble) << 15`
- Returns upper 16 bits as random value

### Mutex / IRQ-Safe Operations
- **os_swap_value / os_set_bits / os_clear_bits** — Disable IME, modify, restore
- Pattern: save `*IME`, set `*IME=0`, modify data, restore `*IME`
- Two callback queue variants using `os_set_bits` for slot registration

### Linked List Operations
- **list_pop_head** — Remove and return head node
- **list_remove_node** — Unlink from doubly-linked list (update prev/next)
- **list_insert_tail** — Append to tail of doubly-linked list
- Node layout: `+0xC = prev`, `+0x10 = next`; list header: `+0x7C = head`, `+0x80 = tail`

---

## 10. Sound System (partially decompiled in `snd_fs_misc.c`)

Sound data loaded from `sound/sound_data.sdat` (NDS SDAT format).
Memory region named "SoundHeap" at runtime.
- Init function at 0x020424E4
- IPC commands sent to ARM7 for audio playback (0x27, 0x12 command IDs observed)

---

## 11. Filesystem

ROM filesystem mounted via Nitro SDK `FS_Init`. Key paths:
- `/FMap/FMapData.dat` — Field map data (overworld maps, room layouts)
- `sound/sound_data.sdat` — Sound archive

37 overlays loaded dynamically for different game contexts (battle, menus, etc.).

---

## 12. Function Statistics

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

## 13. Address Map (Key Regions)

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
