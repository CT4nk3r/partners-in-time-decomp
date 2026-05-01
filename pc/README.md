# Mario & Luigi: Partners in Time — PC Port

This directory contains the PC port platform layer for the decompiled game.

## Overview

The decompiled ARM9 code in `arm9/src/` was written for the Nintendo DS hardware.
To run on PC, we need to abstract the NDS-specific hardware behind a platform layer:

- **NDS 2D Engine** (BG layers, tile maps, OBJ sprites) → SDL2 textures + software rasterizer
- **NDS 3D Engine** (limited polygon pipeline) → OpenGL/SDL_Renderer
- **VRAM banks** (A-I, total 656 KB) → host heap allocations
- **Hardware registers** (REG_DISPCNT, REG_DMA*, etc.) → in-memory shadow regs + emulation
- **ARM7-ARM9 IPC** → in-process function calls (no separate ARM7)
- **Stylus / touch** → SDL mouse events
- **Audio** → SDL_Audio with NDS APU emulation

## Build Status

This is the initial scaffolding. Currently the PC build:
- ✅ Opens an SDL2 window
- ✅ Has a basic event loop
- ✅ Provides hardware register stubs
- ❌ Does NOT yet link with `arm9/src/` C files (different ABI, hardware deps)
- ❌ Does NOT render game content yet

Reaching playable state requires:
1. Adapt `arm9/src/` code to be host-compilable (currently targets armv5te)
2. Implement NDS 2D engine emulation (most game graphics)
3. Implement audio playback
4. Handle save/load (NDS SRAM → host filesystem)
5. Fill in gap data: ROM constants/tables that aren't decompiled

## Building

### Prerequisites
- CMake 3.20+
- A C compiler (GCC, Clang, or MSVC)
- SDL2 development libraries

On Windows (MSYS2):
```bash
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-SDL2 mingw-w64-x86_64-cmake
```

On Linux:
```bash
sudo apt install build-essential cmake libsdl2-dev
```

On macOS:
```bash
brew install cmake sdl2
```

### Build steps
```bash
cd pc
cmake -B build -G "MinGW Makefiles"   # Windows: -G "MinGW Makefiles"
cmake --build build
./build/mlpit_pc                       # or .\build\mlpit_pc.exe on Windows
```

## Architecture

```
pc/
├── src/
│   ├── main.c              SDL2 entry point + event loop
│   ├── platform_sdl.c      SDL2 window/render/audio/input
│   ├── nds_hw_stubs.c      Hardware register + DMA stubs
│   └── nds_vram.c          VRAM bank emulation
├── include/
│   ├── nds_platform.h      Platform interface (used by ARM9 code)
│   └── nds_hw.h            Hardware register definitions
└── CMakeLists.txt
```

## Legal

This port contains only original code. The user must supply their own legal
copy of the Mario & Luigi: Partners in Time ROM. No copyrighted Nintendo
content is distributed with this project.
