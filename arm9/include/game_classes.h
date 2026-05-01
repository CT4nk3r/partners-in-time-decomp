/**
 * Game Engine Class Definitions
 *
 * Struct layouts reconstructed from Ghidra decompilation by analyzing
 * field access patterns (pointer + offset) across multiple functions.
 *
 * These structs represent the C++ classes identified via RTTI strings
 * embedded in the binary. Since we decompile to C, classes are modeled
 * as structs with function-pointer vtables.
 */

#ifndef GAME_CLASSES_H
#define GAME_CLASSES_H

#include "types.h"

// ============================================================
// Forward declarations
// ============================================================
typedef struct clGameMain      clGameMain;
typedef struct clCellAnime     clCellAnime;
typedef struct clCellAnimePA   clCellAnimePA;
typedef struct clObjPA         clObjPA;
typedef struct clVBlankTask    clVBlankTask;
typedef struct tsClMapControl  tsClMapControl;
typedef struct clMesWinObj     clMesWinObj;

// ============================================================
// clGameMain — Root game manager (RTTI: "10clGameMain" @ 0x0204FFC4)
// ============================================================
struct clGameMain {
    /* +0x000 */ void **vtable;
    /* +0x004 */ u32 field_04;
    /* +0x008 */ u32 field_08;
    /* +0x00C */ u32 field_0C;
    /* +0x010 */ u32 field_10;
    /* +0x014 */ u32 field_14;
    /* +0x018 */ u32 field_18;
    /* +0x01C */ u32 field_1C;
    /* +0x020 */ u32 field_20;
    /* +0x024 */ u32 field_24;
    /* +0x028 */ u8  pad_028[0x20];
    /* +0x048 */ u32 bitfield_48[2];   // event/flag bitfields
    /* +0x050 */ u32 bitfield_50[32];  // more flag storage
    /* +0x0D0 */ u8  byte_array_D0[0x120]; // byte property array
    /* +0x1F0 */ u32 bitfield_1F0[128]; // large bitfield array
    /* +0x3F0 */ u8  pad_3F0[0x124];
    /* +0x514 */ u8  flags_514;        // bit 0: V-Blank active, bit 1: state flag, bit 6: frame update
    /* +0x515 */ u8  flags_515;        // secondary flags
    /* +0x516 */ u8  pad_516[2];
    /* +0x518 */ u32 frame_count;      // updated each frame
    /* +0x51C */ u32 anim_counter;     // animation frame counter
    /* +0x520 */ u8  pad_520[0x5C];
    /* +0x57C */ u16 display_flags;    // display configuration bits
};

// ============================================================
// clObjPA — Game object / entity (RTTI: "7clObjPA" @ 0x02050B20)
// ============================================================
struct clObjPA {
    /* +0x00 */ void **vtable;
    /* +0x04 */ u32 field_04;
    /* +0x08 */ u32 field_08;     // address or size, varies by flag at +0x1F
    /* +0x0C */ u32 field_0C;
    /* +0x10 */ u32 field_10;
    /* +0x14 */ u32 field_14;
    /* +0x18 */ u32 field_18;
    /* +0x1C */ u32 field_1C;     // used as 24-bit value (& 0xFFFFFF)
    /* +0x1F */ u8  ctrl_flags;   // bit 0: addr mode, bits 3/6/9: direction/rotation
    /* +0x20 */ u8  pad_20[0x5C];
    /* +0x7C */ u32 field_7C;     // state/parameter
};

// ============================================================
// clCellAnime — Sprite cell animation (RTTI: "11clCellAnime" @ 0x02050970)
//   Base class: clCellAnimeBase (RTTI: "15clCellAnimeBase" @ 0x0205093C)
// ============================================================
struct clCellAnime {
    /* +0x00 */ void **vtable;
    /* +0x04 */ void *parent;     // parent class or container
    /* +0x08 */ u32 field_08;
    /* +0x0C */ u32 field_0C;
    /* +0x10 */ u32 field_10;
    /* +0x14 */ u32 field_14;
    /* +0x18 */ u32 field_18;
    /* +0x1C */ u32 field_1C;
    /* +0x20 */ u32 field_20;
};

// ============================================================
// clCellAnimePA — Partner action animation (RTTI: "13clCellAnimePA" @ 0x02050A48)
//   Derived from clCellAnime
// ============================================================
struct clCellAnimePA {
    /* +0x00 */ clCellAnime base;
    /* extended fields TBD */
};

// ============================================================
// clVBlankTask — VSync task handler (RTTI: "12clVBlankTask" @ 0x02050C5C)
// ============================================================
struct clVBlankTask {
    /* +0x00 */ void **vtable;
    /* +0x04 */ u32 field_04;
    /* +0x08 */ u32 field_08;
};

// ============================================================
// tsClMapControl — Tilemap/world controller (RTTI: "14tsClMapControl" @ 0x02050C6C)
//   Has 4 layers, each at 0x48-byte stride starting at +0x3F8
// ============================================================
#define MAP_LAYER_STRIDE 0x48

struct MapLayer {
    /* +0x00 */ u16 state;        // animation/map state
    /* +0x02 */ u16 field_02;
    /* +0x04 */ u16 field_04;
    /* +0x06 */ u8  pad_06[2];
    /* +0x08 */ u8  anim_frame;   // current animation frame
    /* +0x09 */ u8  pad_09[0x3F];
};

struct tsClMapControl {
    /* +0x000 */ void **vtable;
    /* +0x004 */ u8  pad_004[0x3F4];
    /* +0x3F8 */ MapLayer layers[4]; // 4 BG layers
};

// ============================================================
// clMesWinObj — Message/dialog window (RTTI: "11clMesWinObj" @ 0x0205628C)
// ============================================================
struct clMesWinObj {
    /* +0x00 */ void **vtable;
    /* +0x04 */ u32 field_04;
    /* +0x08 */ u32 field_08;
};

#endif /* GAME_CLASSES_H */
