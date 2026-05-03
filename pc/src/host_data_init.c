/*
 * host_data_init.c - HOST_PORT only.
 *
 * On real NDS hardware the file-static globals DAT_02005d28..d38 and
 * DAT_02005d68 in arm9/src/game_init.c are populated either by
 * Ghidra-extracted .data literals or by C++ static constructors that
 * live inside scene-factory overlays we have not yet decompiled.  Their
 * zero-init at startup makes calling FUN_02005b70 (the legitimate
 * sGameState->current installer) immediately fault.
 *
 * This translation unit owns small, deterministic backing buffers for
 * those statics and installs them into game_init.c via the HOST_PORT
 * setter `host_game_init_install_globals`.
 *
 * The slot pointer DAT_02005d28 must alias the same word that
 * sGameState->current reads in arm9/src/game_main.c.  We expose
 * `game_state_host_get_current_slot()` from there to do exactly that
 * without leaking the file-static `s_host_game_state`.
 */
#include "types.h"

#include <stdio.h>
#include <string.h>

/* Generated at configure time from the user's own arm9.bin.
 * See tools/scripts/extract_data_inits.py. The header lives in the
 * build directory and is .gitignore'd - never commit ROM-derived bytes. */
#include "host_data_init_generated.h"

extern void host_game_init_install_globals(u32 *slot,
                                           u32  alloc_sz,
                                           u32  cfg_off,
                                           u16 *cfg_blob,
                                           u8  *disp_flag,
                                           void (*scene_jmp)(int, int));

extern void FUN_0202a56c(int param_1, int param_2);

extern u32 *game_state_host_get_current_slot(void);

/* Default config blob FUN_02005b70 copies into the substate.
 * On real hardware DAT_02005d34 = 0x02048F44 — a pointer into the
 * arm9 .data segment. We mirror those exact 32 bytes here from the
 * user's own ROM via the generated header. */
static u16 s_default_config[16];

/* DAT_02005d38 is dereferenced as *DAT_02005d38 & 1.
 * The real ROM-extracted byte at 0x02059C8C lives in BSS so 0 is the
 * correct boot-time value. */
static u8 s_disp_mode_flag;

/* clGameMain alloc size: ROM has DAT_02005d2c = 0x58C (1420 bytes).
 * The host substate buffer in game_main.c reserves 0x600 (1536) which
 * comfortably covers a 0x58C alloc + the [+0x57C..+0x588] writes. */
#define HOST_CLGAMEMAIN_SIZE   ((u32)HOSTDATA_DAT_02005D2C)

/* Intra-struct config-blob offset: ROM has DAT_02005d30 = 0x57C. */
#define HOST_CFG_OFFSET        ((u32)HOSTDATA_DAT_02005D30)

/* Scene jump target. On real hardware DAT_02005d68 = 0x0202A56C - a
 * doubly-linked-list "append" routine that the original decompiler
 * missed (it lives mid-function inside FUN_0202a53c).  We hand-decompiled
 * it in arm9/src/FUN_0202a56c.c; this trampoline simply forwards r0/r1
 * after a one-shot log so we can confirm dispatch on stderr. */
static void host_scene_jmp_trampoline(int param_1, int param_2)
{
    static int s_fired = 0;
    if (!s_fired) {
        s_fired = 1;
        fprintf(stderr,
                "[HOST-DATA] scene_jmp dispatched -> FUN_0202a56c"
                "(head=0x%08X, node=0x%08X)\n",
                (unsigned)param_1, (unsigned)param_2);
    }
    FUN_0202a56c(param_1, param_2);
}

void host_data_init_install(void)
{
    u32 *slot = game_state_host_get_current_slot();
    if (!slot) {
        fprintf(stderr, "[HOST-DATA] no slot pointer; skipping install\n");
        return;
    }

    /* Populate s_default_config from the ROM-extracted bytes. */
    memcpy(s_default_config,
           HOSTDATA_CONFIG_BLOB_02048F44_BYTES,
           sizeof(s_default_config) <
               sizeof(HOSTDATA_CONFIG_BLOB_02048F44_BYTES)
               ? sizeof(s_default_config)
               : sizeof(HOSTDATA_CONFIG_BLOB_02048F44_BYTES));

    host_game_init_install_globals(
        slot,                       /* DAT_02005d28 (host slot ptr) */
        HOST_CLGAMEMAIN_SIZE,       /* DAT_02005d2c = 0x58C */
        HOST_CFG_OFFSET,            /* DAT_02005d30 = 0x57C */
        s_default_config,           /* DAT_02005d34 (host blob ptr) */
        &s_disp_mode_flag,          /* DAT_02005d38 (host flag ptr) */
        host_scene_jmp_trampoline   /* DAT_02005d68 (host stub for 0x0202A56C) */
    );

    fprintf(stderr,
            "[HOST-DATA] installed: slot=%p alloc=0x%x cfg_off=0x%x cfg=%p flag=%p scene_jmp=%p\n",
            (void *)slot,
            (unsigned)HOST_CLGAMEMAIN_SIZE,
            (unsigned)HOST_CFG_OFFSET,
            (void *)s_default_config,
            (void *)&s_disp_mode_flag,
            (void *)host_scene_jmp_trampoline);

    fprintf(stderr,
            "[HOST-DATA] DAT_02005D68 ROM word = 0x%08X (expected 0x0202A56C)\n",
            (unsigned)HOSTDATA_DAT_02005D68);
}
