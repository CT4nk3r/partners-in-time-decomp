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

extern void host_game_init_install_globals(u32 *slot,
                                           u32  alloc_sz,
                                           u32  cfg_off,
                                           u16 *cfg_blob,
                                           u8  *disp_flag,
                                           void (*scene_jmp)(void));

extern u32 *game_state_host_get_current_slot(void);

/* Default config blob that FUN_02005b70 copies into the substate at
 * offset DAT_02005d30.  The function reads halfword[0], halfword[1],
 * halfword[2], and two u32 words at byte offset 16/20 — total of
 * 24 bytes accessed (>=12 halfwords).  All zero is the safest default;
 * the bit-fields are extracted at the end of FUN_02005b70 (shifts of
 * 0x1d, 0x17, 0x16, 0x1c) and 0 yields cleared flag bits, which
 * matches "no special display mode / not a movie scene" defaults. */
static u16 s_default_config[16];

/* DAT_02005d38 is dereferenced as *DAT_02005d38 & 1; zero -> mode bit
 * cleared.  Provide one concrete byte. */
static u8 s_disp_mode_flag;

/* Size of the clGameMain object.  game_main.c's host shim already
 * reserves 0x600 (1536) bytes of backing storage and uses that as
 * sGameState->current.  Match that here so FUN_02005b70's bzero of
 * DAT_02005d2c bytes does NOT overrun. */
#define HOST_CLGAMEMAIN_SIZE   0x600u

/* Intra-struct offset where the config halfwords land.  Inspecting
 * FUN_02005b70: it copies *param_1 to byte offset 0x57c, then writes
 * halfwords at (substate + DAT_02005d30 + 2/4) and 32-bit words at
 * (substate + DAT_02005d30 + 8/12).  These accesses must fit inside
 * the 0x600-byte block.  The Ghidra disassembly anchors this offset
 * at 0x500 (0x500..0x510 — well below 0x57c and below 0x600).  Pick
 * 0x500 as the safe default. */
#define HOST_CFG_OFFSET        0x500u

void host_data_init_install(void)
{
    u32 *slot = game_state_host_get_current_slot();
    if (!slot) {
        fprintf(stderr, "[HOST-DATA] no slot pointer; skipping install\n");
        return;
    }

    host_game_init_install_globals(
        slot,                       /* DAT_02005d28 */
        HOST_CLGAMEMAIN_SIZE,       /* DAT_02005d2c */
        HOST_CFG_OFFSET,            /* DAT_02005d30 */
        s_default_config,           /* DAT_02005d34 */
        &s_disp_mode_flag,          /* DAT_02005d38 */
        NULL                        /* DAT_02005d68 — no writer found yet */
    );

    fprintf(stderr,
            "[HOST-DATA] installed: slot=%p alloc=%u cfg_off=0x%x cfg=%p flag=%p\n",
            (void *)slot,
            (unsigned)HOST_CLGAMEMAIN_SIZE,
            (unsigned)HOST_CFG_OFFSET,
            (void *)s_default_config,
            (void *)&s_disp_mode_flag);
}
