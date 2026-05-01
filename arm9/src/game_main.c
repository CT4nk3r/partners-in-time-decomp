/**
 * Game Main Loop and Initialization
 *
 * This module contains the core game loop and initialization functions.
 * Called from NitroMain (CRT0) after SDK initialization.
 *
 * Decompiled from:
 *   FUN_020050f4 @ 0x020050F4 (228 bytes) - game_init
 *   FUN_02005444 @ 0x02005444 (344 bytes) - game_start (main loop)
 *   FUN_02005380 @ 0x02005380 (176 bytes) - game_check_reset
 *   FUN_02004ef8 @ 0x02004EF8 (116 bytes) - game_setup_overlay
 *   FUN_0200528c @ 0x0200528C (100 bytes) - game_fade_out
 *   FUN_02005300 @ 0x02005300 (112 bytes) - game_check_display_ready
 */

#include "types.h"
#include "game_main.h"

// External SDK functions (to be decompiled separately)
extern void OS_MemClear(void *ptr, u32 offset, u32 size);
extern void GX_SetDispSelect(void);
extern void GX_SetMasterBrightness(u32 screen, u16 val);
extern void GX_SwapDisplay(void);
extern void GX_VBlankWait(void);
extern void GX_SetVisiblePlane(u32 mask, u16 bits);
extern void GX_UpdateDisplay(void);
extern void GX_DisableInterrupts(void);
extern void GX_FlushDisplay(void);
extern void GX_ResetVisiblePlane(void);
extern void FS_InitOverlay(void);
extern u32  FS_LoadOverlay(u32 id, u32 flags, void *callback, u32 param);
extern void FS_AttachOverlay(u32 handle, u32 id);
extern void SND_Init(void);
extern void SND_Update(void);
extern void PAD_Read(void);
extern void OBJ_Init(void);
extern u32  OBJ_Create(void *param);
extern void OBJ_Update(u32 handle);

// Game state globals (addresses from Ghidra data references)
// These are defined in the BSS section and initialized at runtime
static GameState *sGameState;       // DAT_020055B4
static u8 *sDisplayEnabled;         // DAT_020055BC
static u8 *sPauseFlag;              // DAT_020055B8
static u8 *sResetFlag;              // DAT_020055B0
static u16 *sDispCnt;               // DAT_020055A0

/**
 * game_init — Initialize game subsystems
 * Original: FUN_020050f4 @ 0x020050F4
 *
 * Sets up the game state structure, initializes graphics, overlay system,
 * file system, object system, and sound.
 */
void game_init(void)
{
    u32 overlay_handle;
    u32 obj_handle;
    int date_lo, date_mid, date_hi;

    OS_MemClear(sGameState, 0, 0x14);
    sGameState->frame_rate = 10;
    sGameState->mode = 2;

    GX_SetDispSelect();
    GX_SetMasterBrightness(0, 0);
    GX_SetVisiblePlane(0xFFFFFFFF, 0);

    overlay_handle = FS_LoadOverlay(
        GX_GetCurrentMode(),
        1,
        NULL,  // callback
        0      // param
    );
    sGameState->overlay = overlay_handle;
    FS_AttachOverlay(overlay_handle, GX_GetCurrentMode());

    SND_Init();
    obj_handle = OBJ_Create(NULL);
    sGameState->obj_handle = obj_handle;

    if (obj_handle != 0) {
        OBJ_Update(obj_handle);
    }

    FS_InitOverlay();
    // Additional init...
}

/**
 * game_start — Main game loop
 * Original: FUN_02005444 @ 0x02005444
 *
 * This is the infinite main loop of the game. Called from NitroMain
 * after all initialization is complete. Never returns.
 *
 * Each frame:
 *   1. Wait for VBlank
 *   2. Read input
 *   3. Update file system
 *   4. Check display state
 *   5. Update game state
 *   6. Render frame
 *   7. Handle fade/transition if needed
 */
NORETURN void game_start(void)
{
    game_init();

    GX_SetMasterBrightness(0, 0x5800);
    GX_SwapDisplay();
    GX_SetVisiblePlane(
        sGameState->disp_mask,
        ((*sDispCnt | sGameState->disp_flag) ^ sGameState->disp_xor)
            & sGameState->disp_xor & 0xFFFF
    );
    GX_UpdateDisplay();
    GX_SwapDisplay();
    GX_SetVisiblePlane(
        sGameState->disp_mask,
        ((*sDispCnt | sGameState->disp_flag) ^ sGameState->disp_xor)
            & sGameState->disp_xor & 0xFFFF
    );

    sGameState->running = 1;
    game_setup_overlay(0xFFFFFFFF, 0);

    // Main game loop — runs forever
    for (;;) {
        // Inner frame loop
        do {
            GX_VBlankWait();
            PAD_Read();
            FS_Update();

            if (*sDisplayEnabled != 0) {
                game_update_display();
            }

            // Update frame counter if game state is active
            if (sGameState->current != NULL) {
                GameState *gs = sGameState->current;
                if ((gs->flags & 0x40) != 0) {
                    gs->frame_count++;
                }
            }
        } while ((*sPauseFlag != 0) ||
                 ((s32)(*sDispCnt & 0x8000) >> 15 != 1));

        // Frame transition
        GX_SetMasterBrightness(0xFFFFFFFF, 0);

        if (*sResetFlag != 0) {
            SND_Update();
        }

        // Select transition effect duration
        u32 duration = 0x0C;
        if (*sResetFlag != 0) {
            duration = 0x1C;
        }

        game_do_transition(duration, 0, 0);
    }
}

/**
 * game_check_reset — Check if reset/transition should occur
 * Original: FUN_02005380 @ 0x02005380
 */
void game_check_reset(void)
{
    if (*sDisplayEnabled == 0) {
        return;
    }

    if (sGameState->needs_reset != 0) {
        SND_Update();
    }

    if (sGameState->in_transition == 0) {
        if (sGameState->pending_reset == 0) {
            int ready = game_check_display_ready();
            if (ready != 0) {
                if (sGameState->block_reset == 0) {
                    game_fade_out(0xFFFFFFFF);
                } else {
                    sGameState->pending_reset = 1;
                }
            }
        } else if (sGameState->block_reset == 0) {
            game_fade_out(0xFFFFFFFF);
        }
    }

    GX_UpdateDisplay();
}

/**
 * game_fade_out — Begin a screen fade out
 * Original: FUN_0200528c @ 0x0200528C
 *
 * @param brightness  Target brightness level (0-16, or 0xFFFFFFFF for current)
 */
void game_fade_out(u32 brightness)
{
    if (brightness == 0xFFFFFFFF) {
        brightness = (u32)*sGameState->cur_brightness;
    }

    sGameState->fading = 1;

    if ((s32)brightness < 0 || (s32)brightness > 16) {
        brightness = 16;
    }

    sGameState->fade_target = (u8)brightness;
    sGameState->fade_current = sGameState->fade_target;

    GX_DisableInterrupts();
    GX_FlushDisplay();
    GX_ResetVisiblePlane();
}

/**
 * game_check_display_ready — Check if display is ready for transition
 * Original: FUN_02005300 @ 0x02005300
 *
 * @return 1 if display is ready, 0 otherwise
 */
u32 game_check_display_ready(void)
{
    GX_SetVisiblePlane(
        sGameState->disp_mask,
        ((*sDispCnt | sGameState->disp_flag) ^ sGameState->disp_xor)
            & sGameState->disp_xor & 0xFFFF
    );

    if (*sGameState->disp_status == 0x30C &&
        (sGameState->disp_status[1] & 0x30C) != 0) {
        return 1;
    }

    return 0;
}

#ifdef HOST_PORT
/**
 * game_state_host_init — PC port helper.
 *
 * On real NDS hardware the file-static pointers above were initialized by
 * CRT0 / data section relocation to point at fixed RAM addresses
 * (DAT_020055A0..DAT_020055BC).  In our host build those addresses do not
 * exist, so we allocate real backing storage and patch the pointers.
 *
 * Must be called once before game_init() / game_start() on the PC port.
 *
 * NOT part of the original game — exists only to satisfy host linkage.
 */
#include <stdlib.h>
#include <string.h>

static GameState  s_host_game_state;
static u8         s_host_display_enabled = 1;
static u8         s_host_pause_flag      = 0;
static u8         s_host_reset_flag      = 0;
static u16        s_host_disp_cnt        = 0;
static u16        s_host_disp_status[2]  = { 0x30C, 0x30C };
static u8         s_host_cur_brightness  = 0;

void game_state_host_init(void)
{
    memset(&s_host_game_state, 0, sizeof(s_host_game_state));
    s_host_game_state.disp_status    = s_host_disp_status;
    s_host_game_state.cur_brightness = &s_host_cur_brightness;

    sGameState        = &s_host_game_state;
    sDisplayEnabled   = &s_host_display_enabled;
    sPauseFlag        = &s_host_pause_flag;
    sResetFlag        = &s_host_reset_flag;
    sDispCnt          = &s_host_disp_cnt;
}
#endif /* HOST_PORT */
