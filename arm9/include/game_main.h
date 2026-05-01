/**
 * Game Main — Core game structures and function declarations
 */

#ifndef GAME_MAIN_H
#define GAME_MAIN_H

#include "types.h"

#define NORETURN __attribute__((noreturn))

/**
 * GameState — Main game state structure
 *
 * Reconstructed from field accesses observed in Ghidra decompilation.
 * Field names and types are best guesses based on usage context.
 *
 * TODO: Refine as more functions are decompiled.
 */
typedef struct GameState {
    u8  frame_rate;         // +0x08
    u8  mode;               // +0x09
    u8  running;            // +0x0A (flag)
    u8  fading;             // +0x0B (flag)
    u8  fade_target;        // +0x0C
    u8  fade_current;       // +0x0D
    u8  needs_reset;        // +0x0E
    u8  in_transition;      // +0x0F
    u8  pending_reset;      // +0x10
    u8  block_reset;        // +0x11
    u8  _pad[2];
    u32 overlay;            // +0x14
    u32 obj_handle;         // +0x18
    u16 disp_flag;          // +0x1C
    u16 disp_xor;           // +0x1E
    u32 disp_mask;          // +0x20
    u16 *disp_status;       // +0x24
    u8  *cur_brightness;    // +0x28
    u32 flags;              // +0x514
    u32 frame_count;        // +0x518
    struct GameState *current; // pointer to active sub-state
} GameState;

// Core game functions
void game_init(void);
NORETURN void game_start(void);
void game_check_reset(void);
void game_fade_out(u32 brightness);
u32  game_check_display_ready(void);
void game_update_display(void);
void game_setup_overlay(u32 id, u32 param);
void game_do_transition(u32 duration, u32 param1, u32 param2);

// External references
extern void FS_Update(void);
extern u32  GX_GetCurrentMode(void);

#endif /* GAME_MAIN_H */
