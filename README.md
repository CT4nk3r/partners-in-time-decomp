# Mario & Luigi: Partners in Time — NDS Decompilation

A work-in-progress decompilation of **Mario & Luigi: Partners in Time** for the Nintendo DS.

The goal of this project is to produce a fully matching decompilation of the game's ARM9 and ARM7 binaries in C, which can then be retargeted to other platforms (PC, Android, etc.).

## ⚖️ Legal Notice

**This repository does NOT contain any copyrighted material from Nintendo.**

- No ROM files, game assets, music, graphics, or any proprietary data are included.
- You **must** provide your own legally-obtained copy of the game ROM (`.nds` file) dumped from a cartridge you own.
- Place your ROM file in the `rom/` directory as `baserom.nds`.
- This project is for educational and interoperability purposes only.
- All decompiled code in this repository is the result of clean-room reverse engineering.

## Project Status

🚧 **Early Stage** — Project scaffolding and tooling setup. No functions decompiled yet.

| Component | Status |
|-----------|--------|
| ROM Extraction | ✅ Scripts ready |
| ARM9 Disassembly | 🔲 Not started |
| ARM7 Disassembly | 🔲 Not started |
| Function Decompilation | 0% |
| Matching Build | 🔲 Not started |
| PC Port | 🔲 Not started |
| Android Port | 🔲 Not started |

## Prerequisites

See [SETUP.md](SETUP.md) for detailed installation instructions.

- **Python 3.10+**
- **Git**
- **devkitPro + devkitARM** (ARM cross-compiler toolchain)
- **ndstool** (NDS ROM extraction, included with devkitPro)
- **Ghidra 11+** with Java 17+ (reverse engineering / decompilation)
- **GNU Make**

## PC Port — Quick Start

The PC port uses a **Ship-of-Harkinian-style** asset pack workflow:
your ROM is needed only **once** for extraction; after that you can
delete it and run purely from `assets/mlpit.assets`.

### Prerequisites

- CMake 3.20+, a C11 compiler (GCC/Clang/MSVC)
- SDL2 (e.g. `pacman -S mingw-w64-x86_64-SDL2` on MSYS2)
- Python 3.8+ (stdlib only — for the standalone extractor)

### Build & first run

```sh
# 1. Place your legally-dumped ROM (one time only)
cp /path/to/your.nds roms/baserom.nds

# 2. Configure and build
cmake -B pc/build pc
cmake --build pc/build

# 3. Run — assets auto-extract on first launch (a few seconds)
./pc/build/mlpit_pc.exe

# 4. After first run, the ROM is no longer needed
#    rm roms/baserom.nds   (optional)
```

On first launch the executable detects `roms/baserom.nds`, extracts all
assets to `assets/mlpit.assets`, then loads the pack and starts the game.
Every subsequent run loads the pack directly — no ROM required.

You can also run the extractor manually:

```sh
python tools/extract_assets.py
# custom paths:
python tools/extract_assets.py roms/baserom.nds assets/mlpit.assets
```

## Project Structure

```
├── arm9/               # ARM9 (main CPU) code
│   ├── src/            # Decompiled C source files
│   ├── include/        # Header files
│   ├── asm/            # Assembly files (not yet decompiled)
│   └── data/           # Binary data blobs
├── arm7/               # ARM7 (sub CPU) code
│   ├── src/            # Decompiled C source files
│   ├── include/        # Header files
│   ├── asm/            # Assembly files
│   └── data/           # Binary data blobs
├── data/               # Extracted game assets
│   ├── overlays/       # Runtime-loaded overlay binaries
│   ├── sound/          # Audio data
│   └── graphics/       # Graphical assets
├── tools/              # Build and analysis tools
│   └── scripts/        # Python helper scripts
├── config/             # Build configuration, linker scripts
├── lib/                # Libraries
├── rom/                # Your ROM goes here (gitignored)
├── build/              # Build output (gitignored)
├── Makefile            # Main build system
├── SETUP.md            # Tool installation guide
└── README.md           # This file
```

## Contributing

Contributions are welcome! The main workflow is:

1. Pick an undecompiled function from the assembly
2. Write equivalent C code
3. Verify it compiles to matching assembly using the build system
4. Submit a pull request

## Acknowledgments

- [pret](https://github.com/pret) — Pokémon reverse engineering projects that pioneered NDS decompilation methodology
- [devkitPro](https://devkitpro.org/) — ARM cross-compilation toolchain
- [Ghidra](https://ghidra-sre.org/) — NSA's reverse engineering framework
- The Mario & Luigi modding community

## Disclaimer

This project is not affiliated with or endorsed by Nintendo. Mario & Luigi: Partners in Time is a trademark of Nintendo. This project does not distribute any copyrighted material.
