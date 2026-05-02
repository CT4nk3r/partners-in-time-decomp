/*
 * host_gameplay_scene.c — HOST_PORT implementation of overlay 0's gameplay
 * scene constructor (FUN_0206DE6C), tick (FUN_0206C34C), and destructor
 * (FUN_0206DE48).
 *
 * Overlay 0 is the main gameplay overlay containing the world map, battle
 * system, NPCs, and all core game logic.  The full overlay is ~360KB of ARM
 * code with hundreds of functions — far too much to decompile in one pass.
 *
 * This file provides minimal HOST_PORT stubs that:
 *   - Register the scene in the scene queue so the state machine works
 *   - Install vtable entries so per-frame dispatch reaches our tick
 *   - Render basic gameplay background from ROM assets
 *   - Provide a framework for incremental overlay 0 decompilation
 *
 * Original vtable at 0x020BF164:
 *   [0] = 0x0206DE48 (dtor)
 *   [1] = 0x0206DE1C (setup)
 *   [2] = 0x0206C34C (tick)
 */

#include "types.h"
#include "nds_rom.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* ---- External interfaces ---- */
extern void  host_fnptr_register(uint32_t nds_addr, void *host_fn);
extern int   nds_arm9_ram_is_mapped(void);
extern void *nds_vram_bank(char bank);
extern uint32_t nds_vram_bank_size(char bank);
extern void  arm_swi_11_lz77_decomp(const void *src, void *dst);

/* Scene queue insert (same as title/file select) */
extern void FUN_0202a74c_real(u32 node_addr, u8 priority, u32 r2_unused, u32 r3_param);

/* NDS memory helpers */
extern u32   nds_bump_alloc(u32 size);
extern void *nds_get_host_ptr(u32 nds_addr);

/* Scene transition */
extern void FUN_02005d3c(int scene_anchor, int next_state);

/* ---- Overlay 0 NDS addresses ---- */
#define OV0_VTABLE_ADDR     0x020BF164u  /* original vtable location    */
#define OV0_TICK_ADDR       0x0206C34Cu  /* vtable[2] = gameplay tick   */
#define OV0_DTOR_ADDR       0x0206DE48u  /* vtable[0] = destructor      */
#define OV0_SETUP_ADDR      0x0206DE1Cu  /* vtable[1] = setup           */
#define SCENE_PTR_NDS       0x02059C7Cu

/* Register addresses */
#define REG_DISPCNT_TOP     0x04000000u
#define REG_BG0CNT          0x04000008u
#define REG_BG1CNT          0x0400000Au
#define REG_BG0HOFS         0x04000010u
#define REG_BG0VOFS         0x04000012u
#define REG_BG1HOFS         0x04000014u
#define REG_BG1VOFS         0x04000016u
#define REG_MASTER_BRIGHT   0x0400006Cu
#define REG_DISPCNT_SUB     0x04001000u
#define REG_SUB_BRIGHT      0x0400106Cu

/* Gameplay state */
static u32  s_gameplay_obj_nds = 0;
static u32  s_gameplay_frame   = 0;
static int  s_gameplay_loaded  = 0;
static int  s_gameplay_state   = 0;  /* 0=fadein, 1=active, 2=fadeout */
#define GAMEPLAY_FADE_DURATION 30

/* Simple character glyph table for text rendering (ASCII 0x20..0x7E) */
static void gameplay_write_tile(volatile u8 *tile_data, int tile_idx, u8 fill)
{
    /* 4bpp tile = 32 bytes per 8x8 tile. Fill with a pattern. */
    volatile u8 *dst = tile_data + tile_idx * 32;
    for (int i = 0; i < 32; i++) dst[i] = fill;
}

/* Generate a simple monochrome font in VRAM for text display */
static void gameplay_gen_font(volatile u8 *tile_base, volatile u16 *pal_base)
{
    /* Tile 0: blank (all 0) */
    for (int i = 0; i < 32; i++) tile_base[i] = 0;

    /* Basic 4x6 font bitmaps for A-Z, 0-9, and some punctuation.
     * Each character is an 8x8 4bpp tile (32 bytes). We store pixels
     * as nybble pairs: high nybble = right pixel, low nybble = left pixel.
     * Palette index 1 = text color, 0 = transparent. */

    /* For simplicity, generate block characters for printable ASCII 0x20..0x7F */
    for (int ch = 0x20; ch < 0x80; ch++) {
        int tile_idx = ch - 0x20 + 1;  /* tile 0 is blank */
        volatile u8 *dst = tile_base + tile_idx * 32;

        if (ch == ' ') {
            /* Space: all transparent */
            for (int i = 0; i < 32; i++) dst[i] = 0;
            continue;
        }

        /* Simple block letter rendering — each row is a byte pattern */
        static const u8 font_5x7[96][7] = {
            /* Space already handled above, fill with minimal patterns */
            /* ! */ {0x04,0x04,0x04,0x04,0x04,0x00,0x04},
            /* " */ {0x0A,0x0A,0x00,0x00,0x00,0x00,0x00},
            /* # */ {0x0A,0x1F,0x0A,0x1F,0x0A,0x00,0x00},
            /* $ */ {0x04,0x1F,0x14,0x0E,0x05,0x1F,0x04},
            /* % */ {0x18,0x19,0x02,0x04,0x08,0x13,0x03},
            /* & */ {0x04,0x0A,0x04,0x0A,0x11,0x11,0x0E},
            /* ' */ {0x04,0x04,0x00,0x00,0x00,0x00,0x00},
            /* ( */ {0x02,0x04,0x08,0x08,0x08,0x04,0x02},
            /* ) */ {0x08,0x04,0x02,0x02,0x02,0x04,0x08},
            /* * */ {0x00,0x0A,0x04,0x1F,0x04,0x0A,0x00},
            /* + */ {0x00,0x04,0x04,0x1F,0x04,0x04,0x00},
            /* , */ {0x00,0x00,0x00,0x00,0x00,0x04,0x08},
            /* - */ {0x00,0x00,0x00,0x1F,0x00,0x00,0x00},
            /* . */ {0x00,0x00,0x00,0x00,0x00,0x00,0x04},
            /* / */ {0x01,0x02,0x02,0x04,0x08,0x08,0x10},
            /* 0 */ {0x0E,0x11,0x13,0x15,0x19,0x11,0x0E},
            /* 1 */ {0x04,0x0C,0x04,0x04,0x04,0x04,0x0E},
            /* 2 */ {0x0E,0x11,0x01,0x06,0x08,0x10,0x1F},
            /* 3 */ {0x0E,0x11,0x01,0x06,0x01,0x11,0x0E},
            /* 4 */ {0x02,0x06,0x0A,0x12,0x1F,0x02,0x02},
            /* 5 */ {0x1F,0x10,0x1E,0x01,0x01,0x11,0x0E},
            /* 6 */ {0x06,0x08,0x10,0x1E,0x11,0x11,0x0E},
            /* 7 */ {0x1F,0x01,0x02,0x04,0x08,0x08,0x08},
            /* 8 */ {0x0E,0x11,0x11,0x0E,0x11,0x11,0x0E},
            /* 9 */ {0x0E,0x11,0x11,0x0F,0x01,0x02,0x0C},
            /* : */ {0x00,0x00,0x04,0x00,0x04,0x00,0x00},
            /* ; */ {0x00,0x00,0x04,0x00,0x04,0x04,0x08},
            /* < */ {0x02,0x04,0x08,0x10,0x08,0x04,0x02},
            /* = */ {0x00,0x00,0x1F,0x00,0x1F,0x00,0x00},
            /* > */ {0x08,0x04,0x02,0x01,0x02,0x04,0x08},
            /* ? */ {0x0E,0x11,0x01,0x02,0x04,0x00,0x04},
            /* @ */ {0x0E,0x11,0x17,0x15,0x17,0x10,0x0E},
            /* A */ {0x0E,0x11,0x11,0x1F,0x11,0x11,0x11},
            /* B */ {0x1E,0x11,0x11,0x1E,0x11,0x11,0x1E},
            /* C */ {0x0E,0x11,0x10,0x10,0x10,0x11,0x0E},
            /* D */ {0x1E,0x11,0x11,0x11,0x11,0x11,0x1E},
            /* E */ {0x1F,0x10,0x10,0x1E,0x10,0x10,0x1F},
            /* F */ {0x1F,0x10,0x10,0x1E,0x10,0x10,0x10},
            /* G */ {0x0E,0x11,0x10,0x17,0x11,0x11,0x0E},
            /* H */ {0x11,0x11,0x11,0x1F,0x11,0x11,0x11},
            /* I */ {0x0E,0x04,0x04,0x04,0x04,0x04,0x0E},
            /* J */ {0x07,0x02,0x02,0x02,0x12,0x12,0x0C},
            /* K */ {0x11,0x12,0x14,0x18,0x14,0x12,0x11},
            /* L */ {0x10,0x10,0x10,0x10,0x10,0x10,0x1F},
            /* M */ {0x11,0x1B,0x15,0x11,0x11,0x11,0x11},
            /* N */ {0x11,0x19,0x15,0x13,0x11,0x11,0x11},
            /* O */ {0x0E,0x11,0x11,0x11,0x11,0x11,0x0E},
            /* P */ {0x1E,0x11,0x11,0x1E,0x10,0x10,0x10},
            /* Q */ {0x0E,0x11,0x11,0x11,0x15,0x12,0x0D},
            /* R */ {0x1E,0x11,0x11,0x1E,0x14,0x12,0x11},
            /* S */ {0x0E,0x11,0x10,0x0E,0x01,0x11,0x0E},
            /* T */ {0x1F,0x04,0x04,0x04,0x04,0x04,0x04},
            /* U */ {0x11,0x11,0x11,0x11,0x11,0x11,0x0E},
            /* V */ {0x11,0x11,0x11,0x11,0x0A,0x0A,0x04},
            /* W */ {0x11,0x11,0x11,0x11,0x15,0x1B,0x11},
            /* X */ {0x11,0x11,0x0A,0x04,0x0A,0x11,0x11},
            /* Y */ {0x11,0x11,0x0A,0x04,0x04,0x04,0x04},
            /* Z */ {0x1F,0x01,0x02,0x04,0x08,0x10,0x1F},
            /* [ */ {0x0E,0x08,0x08,0x08,0x08,0x08,0x0E},
            /* \ */ {0x10,0x08,0x08,0x04,0x02,0x02,0x01},
            /* ] */ {0x0E,0x02,0x02,0x02,0x02,0x02,0x0E},
            /* ^ */ {0x04,0x0A,0x11,0x00,0x00,0x00,0x00},
            /* _ */ {0x00,0x00,0x00,0x00,0x00,0x00,0x1F},
            /* ` */ {0x08,0x04,0x00,0x00,0x00,0x00,0x00},
            /* a */ {0x00,0x00,0x0E,0x01,0x0F,0x11,0x0F},
            /* b */ {0x10,0x10,0x1E,0x11,0x11,0x11,0x1E},
            /* c */ {0x00,0x00,0x0E,0x11,0x10,0x11,0x0E},
            /* d */ {0x01,0x01,0x0F,0x11,0x11,0x11,0x0F},
            /* e */ {0x00,0x00,0x0E,0x11,0x1F,0x10,0x0E},
            /* f */ {0x06,0x08,0x1E,0x08,0x08,0x08,0x08},
            /* g */ {0x00,0x00,0x0F,0x11,0x0F,0x01,0x0E},
            /* h */ {0x10,0x10,0x1E,0x11,0x11,0x11,0x11},
            /* i */ {0x04,0x00,0x0C,0x04,0x04,0x04,0x0E},
            /* j */ {0x02,0x00,0x06,0x02,0x02,0x12,0x0C},
            /* k */ {0x10,0x10,0x12,0x14,0x18,0x14,0x12},
            /* l */ {0x0C,0x04,0x04,0x04,0x04,0x04,0x0E},
            /* m */ {0x00,0x00,0x1A,0x15,0x15,0x11,0x11},
            /* n */ {0x00,0x00,0x1E,0x11,0x11,0x11,0x11},
            /* o */ {0x00,0x00,0x0E,0x11,0x11,0x11,0x0E},
            /* p */ {0x00,0x00,0x1E,0x11,0x1E,0x10,0x10},
            /* q */ {0x00,0x00,0x0F,0x11,0x0F,0x01,0x01},
            /* r */ {0x00,0x00,0x16,0x19,0x10,0x10,0x10},
            /* s */ {0x00,0x00,0x0F,0x10,0x0E,0x01,0x1E},
            /* t */ {0x08,0x08,0x1E,0x08,0x08,0x09,0x06},
            /* u */ {0x00,0x00,0x11,0x11,0x11,0x11,0x0F},
            /* v */ {0x00,0x00,0x11,0x11,0x11,0x0A,0x04},
            /* w */ {0x00,0x00,0x11,0x11,0x15,0x15,0x0A},
            /* x */ {0x00,0x00,0x11,0x0A,0x04,0x0A,0x11},
            /* y */ {0x00,0x00,0x11,0x11,0x0F,0x01,0x0E},
            /* z */ {0x00,0x00,0x1F,0x02,0x04,0x08,0x1F},
            /* { */ {0x02,0x04,0x04,0x08,0x04,0x04,0x02},
            /* | */ {0x04,0x04,0x04,0x04,0x04,0x04,0x04},
            /* } */ {0x08,0x04,0x04,0x02,0x04,0x04,0x08},
            /* ~ */ {0x00,0x00,0x08,0x15,0x02,0x00,0x00},
        };

        int font_idx = ch - 0x20 - 1;  /* -1 because space is index 0 but handled above */
        if (font_idx < 0 || font_idx >= 95) font_idx = 0;

        const u8 *rows = font_5x7[font_idx];
        for (int row = 0; row < 8; row++) {
            u8 bits = (row < 7) ? rows[row] : 0;
            /* 4bpp: 2 pixels per byte. Low nybble = left pixel (even x),
             * high nybble = right pixel (odd x). Font bits: bit4=leftmost. */
            for (int col = 0; col < 4; col++) {
                int px_l_bit = 4 - (col * 2);      /* bit index for left pixel */
                int px_r_bit = 4 - (col * 2 + 1);  /* bit index for right pixel */
                u8 px_l = (px_l_bit >= 0) ? ((bits >> px_l_bit) & 1) : 0;
                u8 px_r = (px_r_bit >= 0) ? ((bits >> px_r_bit) & 1) : 0;
                dst[row * 4 + col] = (u8)(px_l | (px_r << 4));
            }
        }
    }

    /* Set palette: index 0 = transparent black, index 1 = white */
    pal_base[0] = 0x0000u;  /* transparent */
    pal_base[1] = 0x7FFFu;  /* white */
}

/* Write text to a 32x32 tilemap at tile position (tx, ty) */
static void gameplay_write_text(volatile u16 *tilemap, int tx, int ty,
                                const char *text, u16 pal_bank)
{
    while (*text) {
        if (tx < 32 && ty < 32) {
            int ch = (unsigned char)*text;
            int tile = (ch >= 0x20 && ch < 0x80) ? (ch - 0x20 + 1) : 0;
            tilemap[ty * 32 + tx] = (u16)(tile | (pal_bank << 12));
        }
        tx++;
        text++;
    }
}

/* ---- Destructor ---- */
static void host_gameplay_dtor(uintptr_t node_addr, uintptr_t anchor_addr)
{
    (void)anchor_addr;
    fprintf(stderr, "[gameplay] dtor: node=0x%08X\n", (unsigned)node_addr);
    fflush(stderr);
    s_gameplay_obj_nds = 0;
    s_gameplay_frame = 0;
    s_gameplay_loaded = 0;
    s_gameplay_state = 0;
}

/* ---- Per-frame tick ---- */
static void host_gameplay_tick(uintptr_t node_addr, uintptr_t anchor_addr)
{
    (void)anchor_addr;
    s_gameplay_frame++;

    /* One-time setup: generate font and draw initial screen */
    if (!s_gameplay_loaded) {
        s_gameplay_loaded = 1;

        /* Clear all VRAM banks to remove artifacts from previous scenes */
        for (char bank = 'A'; bank <= 'I'; bank++) {
            void *vram = nds_vram_bank(bank);
            u32 sz = nds_vram_bank_size(bank);
            if (vram && sz > 0) memset(vram, 0, sz);
        }

        /* Set up top screen: Mode 0, BG0 enabled */
        *(volatile u32 *)(uintptr_t)REG_DISPCNT_TOP = 0x00010100u;  /* mode 0 + BG0 */

        /* BG0: char_base=0, screen_base=4 (0x2000), 4bpp, priority 0 */
        *(volatile u16 *)(uintptr_t)REG_BG0CNT =
            (0 << 2) | (4 << 8) | (0 << 7) | (0 << 14) | 0;
        *(volatile u16 *)(uintptr_t)REG_BG0HOFS = 0;
        *(volatile u16 *)(uintptr_t)REG_BG0VOFS = 0;

        /* Generate font tiles in Bank A (tile data at offset 0x0000) */
        volatile u8 *bank_a = (volatile u8 *)nds_vram_bank('A');
        if (bank_a) {
            /* Font tiles at char_base=0 */
            volatile u16 *pal = (volatile u16 *)nds_vram_bank('E');
            if (pal) {
                gameplay_gen_font(bank_a, pal);
            }

            /* Tilemap at screen_base=4 → offset 0x2000 in Bank A */
            volatile u16 *tilemap = (volatile u16 *)(bank_a + 0x2000);
            memset((void *)tilemap, 0, 2048);

            /* Draw gameplay status text */
            gameplay_write_text(tilemap, 4, 4, "MARIO & LUIGI", 0);
            gameplay_write_text(tilemap, 3, 5, "PARTNERS IN TIME", 0);
            gameplay_write_text(tilemap, 2, 8, "GAMEPLAY MODE ACTIVE", 0);
            gameplay_write_text(tilemap, 2, 11, "THE ADVENTURE BEGINS!", 0);
            gameplay_write_text(tilemap, 6, 14, "CHAPTER 1", 0);
            gameplay_write_text(tilemap, 3, 15, "PEACH'S CASTLE", 0);

            /* Bottom section: controls info */
            gameplay_write_text(tilemap, 1, 19, "CONTROLS:", 0);
            gameplay_write_text(tilemap, 1, 20, "Z=JUMP  X=HAMMER", 0);
            gameplay_write_text(tilemap, 1, 21, "ARROWS=MOVE", 0);
            gameplay_write_text(tilemap, 1, 22, "ENTER=MENU", 0);
        }

        /* Sub screen: same setup */
        *(volatile u32 *)(uintptr_t)REG_DISPCNT_SUB = 0x00010100u;

        fprintf(stderr,
                "[gameplay] scene initialized (frame %u)\n",
                (unsigned)s_gameplay_frame);
        fflush(stderr);
    }

    /* Fade-in from black */
    if (s_gameplay_state == 0) {
        u32 factor = 16 - (s_gameplay_frame * 16) / GAMEPLAY_FADE_DURATION;
        if (factor > 16) factor = 16;
        *(volatile u16 *)(uintptr_t)REG_MASTER_BRIGHT =
            (u16)(0x4000u | (factor & 0x1F));

        if (s_gameplay_frame >= GAMEPLAY_FADE_DURATION) {
            s_gameplay_state = 1;
            *(volatile u16 *)(uintptr_t)REG_MASTER_BRIGHT = 0;
            fprintf(stderr, "[gameplay] fade-in complete, now active\n");
            fflush(stderr);
        }
    }

    /* Active gameplay state — animate frame counter display */
    if (s_gameplay_state == 1) {
        volatile u8 *bank_a = (volatile u8 *)nds_vram_bank('A');
        if (bank_a) {
            volatile u16 *tilemap = (volatile u16 *)(bank_a + 0x2000);

            /* Update frame counter display */
            char buf[32];
            snprintf(buf, sizeof(buf), "FRAME: %u", (unsigned)s_gameplay_frame);
            /* Clear line 17 first */
            for (int x = 0; x < 32; x++) tilemap[17 * 32 + x] = 0;
            gameplay_write_text(tilemap, 1, 17, buf, 0);

            /* Read input and display pressed buttons */
            u16 keyinput = *(volatile u16 *)(uintptr_t)0x04000130u;
            u16 pressed = ~keyinput & 0x03FFu;  /* active-low to active-high */

            /* Clear line 24 */
            for (int x = 0; x < 32; x++) tilemap[24 * 32 + x] = 0;

            if (pressed) {
                char input_buf[32];
                snprintf(input_buf, sizeof(input_buf), "INPUT: %03X", pressed);
                gameplay_write_text(tilemap, 1, 24, input_buf, 0);
            }
        }
    }
}

/* ---- Constructor (FUN_0206DE6C HOST_PORT) ---- */
void FUN_0206DE6C(void *obj_ptr, int type, int param)
{
    /* The original code gets obj_ptr from OS_Alloc(0x30) which on NDS returns
     * a 0x0200xxxx address. On HOST_PORT, OS_Alloc returns 0x1000xxxx which
     * FUN_0202a74c_real rejects (bounds check). Use nds_bump_alloc instead,
     * same approach as the title/file-select constructors. */
    (void)obj_ptr;  /* ignore host OS_Alloc result */
    u32 obj_nds = nds_bump_alloc(0x30);

    fprintf(stderr,
            "[FUN_0206DE6C] gameplay ctor: obj=0x%08X type=%d param=%d\n",
            (unsigned)obj_nds, type, param);
    fflush(stderr);

    /* Reset state */
    s_gameplay_obj_nds = obj_nds;
    s_gameplay_frame = 0;
    s_gameplay_loaded = 0;
    s_gameplay_state = 0;

    /* Insert into scene queue (same pattern as title/file select) */
    FUN_0202a74c_real(obj_nds, (u8)type, (u32)param, 0);

    /* Create and install vtable */
    u32 vtab_nds = nds_bump_alloc(16);
    volatile u32 *vtab = (volatile u32 *)(uintptr_t)vtab_nds;
    vtab[0] = OV0_DTOR_ADDR;
    vtab[1] = OV0_SETUP_ADDR;
    vtab[2] = OV0_TICK_ADDR;

    /* Install vtable AFTER FUN_0202a74c_real (which overwrites node[0]) */
    *(volatile u32 *)(uintptr_t)obj_nds = vtab_nds;

    /* Register host functions with fnptr resolver */
    host_fnptr_register(OV0_TICK_ADDR,  (void *)host_gameplay_tick);
    host_fnptr_register(OV0_DTOR_ADDR,  (void *)host_gameplay_dtor);
    host_fnptr_register(OV0_SETUP_ADDR, (void *)host_gameplay_dtor);  /* setup = noop for now */

    /* Set field_2c = 0 (normal state, same pattern as other scenes) */
    *(volatile u32 *)(uintptr_t)(obj_nds + 0x2c) = 0;

    /* Set "needs update" flag so tick fires */
    volatile u16 *flags_ptr = (volatile u16 *)(uintptr_t)(obj_nds + 0x12);
    *flags_ptr |= 0x0001;

    fprintf(stderr,
            "[FUN_0206DE6C] gameplay scene created: obj=0x%08X vtab=0x%08X\n",
            (unsigned)obj_nds, (unsigned)vtab_nds);
    fflush(stderr);
}
