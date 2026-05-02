# Project Progress

A running log of the visible milestones reached during the decompilation
and PC port effort. Screenshots are stored in [`docs/progress/`](progress/).

> **Note:** All screenshots are produced by running the PC port against the
> user's own legally-dumped ROM. No copyrighted assets are committed to
> this repository.

## Pipeline Overview

The PC port wires three parallel render paths from the original NDS hardware:

1. **2D BG tilemaps** via `tsClMapControl` → BG VRAM (background layers)
2. **3D textured quads** via `clCellAnimeTX` → GXFIFO (game sprites)
3. **2D OAM** via `RenderOam_Transfer` (`FUN_02029518`) → hardware OAM
   (HUD / text overlays)

## Milestones

### 01 — Shadow-OAM → SDL pipeline verified end-to-end

![Synthetic OAM pipeline](progress/01_synthetic_oam_pipeline.png)

With `MLPIT_TEST_SHADOW_OAM=1` the synthetic test injects three OBJ
descriptors into the main-engine shadow OAM at `0x0205FFC0`. The host
copies the shadow buffer to emulated hardware OAM each frame, the SDL
rasteriser composites the OBJs, and the boxes appear at their attr0/1/2
coordinates with palette colouring.

This proves every layer of the OBJ pipeline works: shadow buffer →
DC_FlushRange equivalent → DMA-copy stub → OAM mirror → SDL composite.
Real game code can now drop sprite descriptors into the shadow buffer
and they will be drawn.

**Commit:** `c31bb4d`

### 02 — Real VRAM tile data interpreted as pixels

![VRAM tile data](progress/02_vram_tile_data.png)

After overlays were loaded into the emulated NDS RAM (37 overlays parsed
from `arm9_ovT`, ov0 + ov6 mapped to their target addresses), the natural
init path populates character / tile VRAM with real graphics data. The
SDL renderer now interprets this as 8×8 tile data and produces the
patterned output above.

The duplicate top/bottom is an SDL-side compositing bug (sub engine is
fetching from the same VRAM bank as main); it is **not** a logic problem.
The structured 8×8 grid and palette stripes confirm the tile data is
real game graphics being read from the correct VRAM regions, just not
yet composited per-engine correctly.

**Commit:** `ffe98c7`

## Component Status

| Component                       | Status |
|---------------------------------|--------|
| ROM extraction & asset pack     | ✅ Working |
| ARM9 disassembly                | ✅ Full disassembly via objdump |
| Decompiled C functions          | 270+ files in `arm9/src/` |
| HOST_PORT compile path          | ✅ Builds clean on MSVC + MinGW |
| NDS RAM mmap (`0x02000000` 4 MB)| ✅ `pc/src/nds_arm9_ram.c` |
| Overlay loader                  | ✅ `pc/src/nds_overlay_loader.c` (ov0, ov6) |
| NDS→host fnptr trampoline       | ✅ ~1 050 entries, vtable indirect calls work |
| 2D OAM pipeline (shadow → hw)   | ✅ Verified end-to-end (milestone 01) |
| BG VRAM tile fetch              | ✅ Real tile data reaches VRAM (milestone 02) |
| Per-engine compositing          | 🔲 Sub engine duplicates main (milestone 02 bug) |
| GXFIFO → SDL 3D rasteriser      | 🔲 Not yet wired |
| Real scene struct populated     | 🔲 Natural init path still SIGSEGVs deeper in `game_start` |
| Audio (NDS APU shim)            | 🔲 Not started |
| Android port                    | 🔲 Blocked on Linux/mmap port (replace VirtualAlloc) |

## Current Blockers

- **Per-engine VRAM compositing** — sub engine reads from main's VRAM bank.
  Fix: separate `g_vram_main` / `g_vram_sub` mirrors in `pc/src/nds_hw_io.c`
  and route the SDL composite per-engine.
- **No live scene struct** — `FUN_02065A10` ticks every frame but its scene
  pointer is null, so it never walks any entities. Two paths forward:
  fix the deeper `game_start` SIGSEGV so the natural init populates the
  scene, or synthesise a minimal scene struct host-side with a single test
  entity.
- **GXFIFO not yet rasterised** — PiT renders character sprites as 3D
  textured quads through the GXFIFO. Once the 3D engine has a minimal
  rasteriser on the SDL side, `clCellAnimeTX_Draw` (`FUN_0200FCB4`) output
  will become visible.

## Reference

The Pokémon NDS decomps (`pret/pokediamond`, `pret/pokeheartgold`,
`pret/pokeplatinum`) use the same Nitro SDK and were heavily mined for
function-name mappings. See the cross-reference in
`docs/RENDER_PIPELINE.md` (extracted from the research notes) for
mapping `FUN_<addr>` symbols in this repo to their pokeplatinum
equivalents.
