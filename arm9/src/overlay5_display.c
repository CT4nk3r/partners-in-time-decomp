/*
 * overlay5_display.c — Decompiled overlay 5 display management functions.
 *
 * Overlay 5 loads at 0x020659C0-0x02069DE0 (17,440 bytes code)
 * with BSS at 0x02069DE0-0x0206A800 (2,592 bytes).
 *
 * These functions manage the NDS display hardware:
 *   - BGxCNT register configuration (char_base, screen_base, etc.)
 *   - Display state initialization
 *   - Sprite/OBJ allocation
 *   - Callback registration
 *
 * All operate on NDS hardware registers at 0x04000000+ or
 * overlay BSS globals at 0x02069DE0+.
 */

#include "types.h"
#include <string.h>
#include <stdio.h>

/* ── NDS register helpers ── */

/* Returns pointer to BGxCNT register for given screen/bg.
 * screen 0 = main engine (0x04000008), screen 1 = sub (0x04001008).
 * bg 0-3 selects which BG control register. */
static volatile u16 *bgcnt_ptr(int screen, int bg)
{
    return (volatile u16 *)(uintptr_t)(0x04000008u + (u32)screen * 0x1000u + (u32)bg * 2u);
}

/* FUN_02067AF8: BGxCNT pointer lookup (called by all BG setter functions) */
volatile u16 *FUN_02067af8(int screen, int bg)
{
    return bgcnt_ptr(screen, bg);
}

/* ── BGxCNT field setters ── */

/* FUN_0206799C: Set char_base (tile data offset), BGxCNT bits 2-5 */
void FUN_0206799c(int screen, int bg, int char_base)
{
    volatile u16 *cnt = bgcnt_ptr(screen, bg);
    *cnt = (*cnt & ~0x3Cu) | (u16)(char_base << 2);
}

/* FUN_020679C8: Set screen_base (tilemap offset), BGxCNT bits 8-12 */
void FUN_020679c8(int screen, int bg, int screen_base)
{
    volatile u16 *cnt = bgcnt_ptr(screen, bg);
    *cnt = (*cnt & ~0x1F00u) | (u16)(screen_base << 8);
}

/* FUN_02067A20: Set color_mode (4bpp=0, 8bpp=1), BGxCNT bit 7 */
void FUN_02067a20(int screen, int bg, int color_mode)
{
    volatile u16 *cnt = bgcnt_ptr(screen, bg);
    *cnt = (*cnt & ~0x80u) | (u16)(color_mode << 7);
}

/* FUN_02067A4C: Set screen_size, BGxCNT bits 14-15 */
void FUN_02067a4c(int screen, int bg, int screen_size)
{
    volatile u16 *cnt = bgcnt_ptr(screen, bg);
    *cnt = (*cnt & ~0xC000u) | (u16)(screen_size << 14);
}

/* FUN_02067A78: Set mosaic enable, BGxCNT bit 6 */
void FUN_02067a78(int screen, int bg, int mosaic)
{
    volatile u16 *cnt = bgcnt_ptr(screen, bg);
    if (mosaic)
        *cnt |= 0x40u;
    else
        *cnt &= ~0x40u;
}

/* FUN_02067AA8: Set priority, BGxCNT bits 0-1 */
void FUN_02067aa8(int screen, int bg, int priority)
{
    volatile u16 *cnt = bgcnt_ptr(screen, bg);
    *cnt = (*cnt & ~3u) | (u16)(priority & 3);
}

/* FUN_02067AD8: Clear BGxCNT to zero */
void FUN_02067ad8(int screen, int bg)
{
    volatile u16 *cnt = bgcnt_ptr(screen, bg);
    *cnt = 0;
}

/* ── Display state management ── */

/* Overlay 5 BSS globals */
#define OV5_BSS_BASE 0x02069DE0u

extern void FUN_0203b914(void *dst, int val, u32 size); /* MI_CpuFill8 */
extern void FUN_02008fc0(int screen);
extern u32  FUN_02029bf8(u32 size, u32 dir, u32 type, u32 flag);
extern u32  FUN_02029c1c(u32 size, u32 dir, u32 type, u32 flag);
extern u32  FUN_0200cf90(u32 param);
extern u32  FUN_02009110(u32 param);
extern void FUN_02035110(u32 addr);
extern void FUN_0202c82c(u32 p1, u32 p2, u32 p3, u32 p4, u32 p5);

/* FUN_02065B80: Returns the display state pointer from global at 0x02069DF0 */
void *FUN_02065b80(void)
{
    volatile u32 *gptr = (volatile u32 *)(uintptr_t)0x02069DF0u;
    return (void *)(uintptr_t)*gptr;
}

/* FUN_020697D4: Initialize OAM double-buffer struct at 0x0206A6C0 */
void FUN_020697d4(void)
{
    volatile u32 *oam = (volatile u32 *)(uintptr_t)0x0206A6C0u;
    u32 second_buf = *(volatile u32 *)(uintptr_t)0x0206A6DCu;
    oam[1] = 0;             /* +4: current index */
    oam[2] = second_buf;    /* +8: buffer pointer */
    oam[8] = (u32)(uintptr_t)oam; /* +0x20: self-reference */
    oam[9] = 0;             /* +0x24: clear */
}

/* FUN_0206749C: Palette init — configures palette based on screen parameter */
void FUN_0206749c(int screen, int param)
{
    /* Screen 1: sub-engine palette setup at 0x0205FFAC-based addr */
    if (screen == 1) {
        /* Main engine palette base address */
        FUN_02035110(0x04000048u); /* VRAMCNT_E or palette control */
        return;
    }
    if (screen == 0) return;
    /* Screen 2 (or other): additional palette control */
    FUN_02035110(*(volatile u32 *)(uintptr_t)0x020674E0u);
}

/* FUN_02067ECC: Set display mode flags based on params.
 * Writes to globals at 0x02069F94 indexed by r0 (screen). */
void FUN_02067ecc(int screen, int mode)
{
    volatile u8 *flags = (volatile u8 *)(uintptr_t)0x02069F94u;
    switch (mode) {
    case 0:
    case 0x10:
        flags[screen] = 1;
        break;
    case 0x100:
        flags[screen] = 2;
        break;
    case 0x200:
        flags[screen] = 4;
        break;
    case 0x1000:
        flags[screen] = 8;
        break;
    default:
        break;
    }
}

/* FUN_02069618: Allocate and init VRAM buffer for specified config */
void FUN_02069618(u32 config)
{
    volatile u32 *cfg_ptr = (volatile u32 *)(uintptr_t)0x0206A6B0u;
    volatile u32 *buf_ptr = (volatile u32 *)(uintptr_t)0x0206A6ACu;

    /* Call sub-init function */
    extern void FUN_020695a8(void);
    FUN_020695a8();

    u32 alloc_size = (config & ~0xC0000000u) << 2;
    *cfg_ptr = config;
    u32 buf = (u32)FUN_02029bf8(alloc_size, 0, 0, 1);
    u32 total = *(volatile u32 *)cfg_ptr;
    total <<= 2;
    *buf_ptr = buf;
    FUN_0203b914((void *)(uintptr_t)buf, 0, total);
}

/* ── Forward declarations for complex functions ── */
extern void FUN_02068438(void);
extern void FUN_020699d0(void);
extern void FUN_02067914(int screen, u32 bg, int p2, int p3);
extern void FUN_02068040(int screen, int val);
extern void FUN_02067ff0(int screen, int val);
extern void FUN_02067fac(int screen, int val);
extern void FUN_02068314(int screen, int bg);
extern void FUN_02066324(void *dst, u32 count);
extern void FUN_02068da4(void *dst, u32 param);
extern void FUN_02068cbc(int param);

/* FUN_02065DA8: Main display initialization.
 * Clears 0xB0 bytes of display config struct, sets up globals. */
void FUN_02065da8(int screen)
{
    /* Clear display config area */
    void *cfg = (void *)(uintptr_t)0x02069E00u;
    FUN_0203b914(cfg, 0, 0xB0);

    /* Set up overlay 5 BSS globals */
    volatile u8  *g_screenA  = (volatile u8  *)(uintptr_t)0x02069DE4u;
    volatile u8  *g_screenB  = (volatile u8  *)(uintptr_t)0x02069DE0u;
    volatile u8  *g_flag     = (volatile u8  *)(uintptr_t)0x02069DE8u;
    volatile u32 *g_ptr1     = (volatile u32 *)(uintptr_t)0x02069DF0u;
    volatile u32 *g_ptr2     = (volatile u32 *)(uintptr_t)0x02069DECu;
    volatile u32 *g_ptr3     = (volatile u32 *)(uintptr_t)0x02069DF4u;
    volatile u32 *g_ptr4     = (volatile u32 *)(uintptr_t)0x02069DF8u;
    volatile u32 *g_ptr5     = (volatile u32 *)(uintptr_t)0x02069DFCu;

    u8 oldA = g_screenA[8];
    u8 oldB = g_screenA[9];

    *g_ptr3 = 1;
    *((volatile u8 *)(uintptr_t)(0x02069DE4u + 8)) = oldA;
    *((volatile u8 *)(uintptr_t)(0x02069DE4u + 9)) = oldB;
    *g_flag = 0;
    *g_ptr5 = 0;
    *g_ptr1 = (u32)(uintptr_t)cfg;

    if (screen == 0) return;

    /* Additional init for non-zero screen parameter */
    FUN_02068438();
    FUN_020699d0();

    fprintf(stderr, "[ov5] FUN_02065DA8(%d) display init done\n", screen);
}

/* FUN_0206805C: BG layer full init for a given screen.
 * Calls fade, clears all BG configs, sets defaults. */
void FUN_0206805c(int screen)
{
    FUN_02008fc0(screen);

    FUN_02068040(screen, 0);
    FUN_02067ff0(screen, 0);
    FUN_02067fac(screen, 0);

    /* Clear all 4 BG layers */
    for (int bg = 0; bg < 4; bg++) {
        FUN_02067914(screen, (u32)bg & 0xFF, 0, 0);
    }

    /* Init 6 BG metadata entries with defaults */
    for (int i = 0; i < 6; i++) {
        FUN_02068314(screen, i);
    }

    fprintf(stderr, "[ov5] FUN_0206805C(%d) BG init done\n", screen);
}

/* FUN_020662B8: Initialize callback dispatch table (12 slots at 0x02069ED4)
 * and the node pool at 0x02069EC0.
 * Each slot is 16 bytes: [data, list_head, sentinel, data].
 * Empty list: list_head → &sentinel, sentinel → &slot. */
void FUN_020662b8(u32 max_objs)
{
    /* 1. Initialize node pool */
    FUN_02066324((void *)(uintptr_t)0x02069EC0u, max_objs);

    /* 2. Initialize 12 callback dispatch slots at 0x02069ED4 */
    u32 table_base = 0x02069ED4u;
    for (u32 i = 0; i < 12; i++) {
        u32 slot_addr = table_base + i * 0x10;
        volatile u32 *slot = (volatile u32 *)(uintptr_t)slot_addr;
        slot[0] = 0;                          /* +0x00 = unused / data */
        slot[1] = slot_addr + 8;              /* +0x04 = list head → sentinel */
        slot[2] = slot_addr;                  /* +0x08 = sentinel back-pointer */
        slot[3] = 0;                          /* +0x0C = unused */
    }

    /* 3. Reset active callback counter at 0x02069EB0 */
    *(volatile u16 *)(uintptr_t)0x02069EB0u = 0;
}

/* FUN_0206621C: Register a callback into the dispatch table.
 * Table base = 0x02069ED4, each slot = 16 bytes.
 * List entry = table_base + id*16 + param*8.
 * Node pool at 0x02069EC0, counter at 0x02069EB0. */
void FUN_0206621c(u32 callback_addr, u16 id, u16 param)
{
    /* 1. Allocate a node from the pool */
    extern void *FUN_0206640c(void *pool);
    volatile u32 *node = (volatile u32 *)FUN_0206640c((void *)(uintptr_t)0x02069EC0u);

    if (!node) {
        fprintf(stderr, "[cb-reg] ERROR: pool exhausted for id=%u param=%u\n",
                (unsigned)id, (unsigned)param);
        return;
    }

    /* 2. Fill node fields */
    *(volatile u16 *)((u8 *)node + 0x0A) = id;   /* +0x0A: id */
    node[3] = callback_addr;                       /* +0x0C: callback function */
    *(volatile u16 *)((u8 *)node + 0x08) = 0;     /* +0x08: flags */
    node[4] = 0;                                   /* +0x10 */
    node[5] = 0;                                   /* +0x14 */

    /* 3. Compute list entry address: table_base + id*16 + param*8 */
    u32 slot_base = 0x02069ED4u + (u32)id * 16u;
    u32 list_entry_addr = slot_base + (u32)param * 8u;
    volatile u32 *list_entry = (volatile u32 *)(uintptr_t)list_entry_addr;

    /* 4. Insert node at head of doubly-linked list */
    u32 old_head_addr = *list_entry;
    node[0] = old_head_addr;                        /* node->prev = old_head */
    node[1] = (u32)(uintptr_t)list_entry;           /* node->next = list_entry */
    if (old_head_addr >= 0x02000000u && old_head_addr < 0x02400000u) {
        *(volatile u32 *)(uintptr_t)(old_head_addr + 4) = (u32)(uintptr_t)node;
    }
    *list_entry = (u32)(uintptr_t)node;

    /* 5. Clear remaining fields */
    node[6] = 0;  /* +0x18 */
    node[7] = 0;  /* +0x1C */
    node[8] = 0;  /* +0x20 */

    /* 6. Increment counter */
    volatile u16 *counter = (volatile u16 *)(uintptr_t)0x02069EB0u;
    *counter = *counter + 1;

    fprintf(stderr, "[cb-reg] registered callback 0x%08X at slot id=%u param=%u node=%p\n",
            (unsigned)callback_addr, (unsigned)id, (unsigned)param, (void *)node);
    fflush(stderr);
}

/* FUN_02068D70: OBJ/sprite manager init */
void FUN_02068d70(u32 max_objs)
{
    FUN_02068da4((void *)(uintptr_t)0x0206A078u, max_objs);
    FUN_02068cbc(0);
    FUN_02068cbc(1);
}

/* FUN_02068928: Trampoline to sprite pool allocator (0x02068964).
 * Original: sets r0 = global at 0x0206A058, r1 = count, jumps to init.
 * The real init allocates count*0x150+8 bytes and sets up a free-list pool.
 * Stub for now — sprite system not needed for basic display. */
void FUN_02068928(u32 param)
{
    (void)param;
    fprintf(stderr, "[ov5] FUN_02068928(%u) sprite pool init (stub)\n", param);
}

/* FUN_020692F4: Trampoline to render object pool allocator (0x02069330).
 * Original: sets r0 = global at 0x0206A698, r1 = count, jumps to init.
 * Allocates count*0x40+8 bytes. Stub for now. */
void FUN_020692f4(u32 param)
{
    (void)param;
    fprintf(stderr, "[ov5] FUN_020692F4(%u) render pool init (stub)\n", param);
}

/* FUN_02067AD8 already defined above */

/* ── Stub functions referenced by the above but not yet decompiled ── */

/* These need to exist to satisfy linker. They operate on overlay BSS state. */

void FUN_02068438(void)
{
    /* Zeroes sub-engine display config state. Benign no-op for now. */
}

void FUN_020699d0(void)
{
    /* Initializes extended palette state. Benign no-op for now. */
}

void FUN_02067914(int screen, u32 bg, int p2, int p3)
{
    /* Sets BG scroll registers + additional BG config.
     * For init: screen=X, bg=0..3, p2=0, p3=0 → clear scroll regs. */
    u32 base = 0x04000010u + (u32)screen * 0x1000u + bg * 4u;
    *(volatile u16 *)(uintptr_t)base = (u16)p2;       /* HOFS */
    *(volatile u16 *)(uintptr_t)(base + 2) = (u16)p3; /* VOFS */
}

void FUN_02068040(int screen, int val)
{
    /* Set DISPCNT display mode bits. For init: val=0 → clear mode. */
    u32 addr = 0x04000000u + (u32)screen * 0x1000u;
    volatile u32 *dispcnt = (volatile u32 *)(uintptr_t)addr;
    u32 d = *dispcnt;
    d &= ~0x30000u; /* clear mode bits 16-17 */
    d |= (u32)(val & 3) << 16;
    *dispcnt = d;
}

void FUN_02067ff0(int screen, int val)
{
    /* Set DISPCNT BG mode bits (bits 0-2). */
    u32 addr = 0x04000000u + (u32)screen * 0x1000u;
    volatile u32 *dispcnt = (volatile u32 *)(uintptr_t)addr;
    *dispcnt = (*dispcnt & ~7u) | (u32)(val & 7);
}

void FUN_02067fac(int screen, int val)
{
    /* Set 3D enable or other DISPCNT flags. For init: val=0 → clear. */
    (void)screen; (void)val;
}

void FUN_02068314(int screen, int bg)
{
    /* Clear BG extended metadata for a given screen/bg index.
     * Sets various tracking fields to zero. */
    (void)screen; (void)bg;
}

void FUN_02066324(void *dst, u32 count)
{
    /* Initialize a pool/allocator structure. */
    (void)dst; (void)count;
}

void FUN_02068da4(void *dst, u32 param)
{
    /* OBJ manager sub-init. */
    (void)dst; (void)param;
}

void FUN_02068cbc(int param)
{
    /* OBJ screen-specific init. */
    (void)param;
}

void FUN_020695a8(void)
{
    /* VRAM config sub-init. */
}

void *FUN_0206640c(void *pool)
{
    /* Pool allocator — return a node from NDS memory.
     * The callback dispatch table at 0x02069ED4 stores NDS addresses,
     * so allocated nodes must also be in NDS address space. */
    extern u32 nds_bump_alloc(u32 size);
    u32 addr = nds_bump_alloc(0x24);
    if (addr == 0) {
        fprintf(stderr, "[pool] nds_bump_alloc(0x24) failed!\n");
        return NULL;
    }
    void *p = (void *)(uintptr_t)addr;
    memset(p, 0, 0x24);
    return p;
}

/* FUN_02067204: Animation/sprite buffer initialization.
 * Allocates a large buffer (0x335C bytes) and links 0x80 entries
 * at stride 0x5C into a free list via the field at offset 8.
 * Called from overlay 8 title constructor. */
void FUN_02067204(u32 buf, int count, int p2, int p3, int p4)
{
    FUN_0202c82c(buf, count, p2, p3, p4);

    *(volatile u32 *)(uintptr_t)buf = 0x02069BE8u; /* vtable */

    /* Zero the header area and entry pool */
    FUN_0203b914((void *)(uintptr_t)(buf + 0x440), 0, 0x60);
    FUN_0203b914((void *)(uintptr_t)(buf + 0x4A0), 0, 0x2E00);

    /* Link 0x7F entries into a free list.
     * Entry[i] is at buf+0x4A0 + i*0x5C.
     * Entry[i].next (at offset 8) points to Entry[i+1]. */
    u32 entries = buf + 0x4A0;
    for (int i = 0; i < 0x7F; i++) {
        u32 cur = entries + (u32)i * 0x5C;
        u32 nxt = entries + (u32)(i + 1) * 0x5C;
        *(volatile u32 *)(uintptr_t)(cur + 8) = nxt;
    }

    /* Init bookkeeping fields at buf+0x3000 range */
    u32 base3k = buf + 0x3000;
    *(volatile u32 *)(uintptr_t)(base3k + 0x358) = entries; /* entries base */
    *(volatile u32 *)(uintptr_t)(base3k + 0x24C) = 0;
    *(volatile u32 *)(uintptr_t)(base3k + 0x2A0) = 0;
    *(volatile u32 *)(uintptr_t)(base3k + 0x2A4) = buf + 0x32FC;
    *(volatile u32 *)(uintptr_t)(base3k + 0x2FC) = buf + 0x32A0;
    *(volatile u32 *)(uintptr_t)(base3k + 0x300) = 0;
}

/* FUN_02067B44: Compute BG tile VRAM base address for a given screen/bg.
 * Uses DISPCNT VRAM mode bits and BGxCNT char_base field. */
u32 FUN_02067b44(int screen, int bg)
{
    volatile u16 *cnt = FUN_02067af8(screen, bg);
    u16 bgcnt = *cnt;
    u32 dispcnt = *(volatile u32 *)(uintptr_t)0x04000000u;
    u32 vram_mode = (dispcnt >> 24) & 7;
    u32 char_base = (bgcnt & 0x3C) >> 2;
    u32 base = ((u32)screen << 21) + 0x06000000u + (vram_mode << 16) + (char_base << 14);
    return base;
}

/* FUN_02077AF8: Sub-scene init (overlay 8).
 * Calls base init FUN_0202a20c and sets vtable.
 * ARM: R0=obj, R1=type, R2=param, R3=parent pass through to FUN_0202a20c. */
extern void FUN_0202a20c(u32 obj, u8 priority, u32 r2, u32 parent);

void FUN_02077af8(u32 obj, int type, int param, u32 parent)
{
    FUN_0202a20c(obj, (u8)type, (u32)param, parent);
    *(volatile u32 *)(uintptr_t)obj = 0x02077EB0u;
}

/* ── ARM9 base functions that were missing from the decomp ── */

/* FUN_02035158: GX_SetBankForBGExtPltt — configures VRAM bank for
 * BG extended palette use. On PC port, just write the VRAM bank
 * control register. */
void FUN_02035158(int bankA, int bankB, int bankC)
{
    (void)bankA; (void)bankB; (void)bankC;
    /* TODO: map VRAM banks for extended palette slot usage */
}

/* FUN_0203513c: GX_SetBankForSubBGExtPltt — sub engine version */
void FUN_0203513c(int bank)
{
    (void)bank;
}

/* FUN_02009040: Initialize OBJ VRAM pointers for a screen.
 * Sets up the mapping between OBJ data buffers and VRAM. */
void FUN_02009040(int screen, u32 base_addr, u32 end_addr)
{
    (void)screen; (void)base_addr; (void)end_addr;
    /* TODO: store OBJ VRAM mapping for software renderer */
}

/* FUN_02066430: recycle a callback node back to the free pool.
 * For now, just a no-op — the static pool in FUN_0206640c is simple. */
void FUN_02066430(void *pool, void *node)
{
    (void)pool; (void)node;
    /* On real NDS, this would link the node back into the free list.
     * Our pool allocator wraps, so this is safe to skip. */
}

/* FUN_02068214: display mode register commit.
 * On NDS, flushes DISPCNT/master brightness etc to hardware.
 * No-op on host — display registers are handled by the renderer. */
void FUN_02068214(void)
{
    /* stub */
}

/* FUN_02029488: Reset object/sprite state flags */
void FUN_02029488(int param1, int param2)
{
    (void)param1; (void)param2;
}

/* ── Callback dispatch system ── */

/* FUN_02066128: Execute and unlink a single callback node.
 * Node layout (0x24 bytes):
 *   +0x00 = prev pointer
 *   +0x04 = next pointer
 *   +0x08 = flags (u16: bit 1 = ready to fire)
 *   +0x18 = callback function pointer (BLX'd, then cleared)
 * After dispatch, the node is unlinked from its doubly-linked list and
 * recycled back to the pool via FUN_02066430. */
extern void FUN_02066430(void *pool, void *node);

static void FUN_02066128(volatile u32 *node)
{
    u32 cb = node[6];  /* +0x18 = callback function pointer */
    if (cb != 0) {
        /* Call the callback via our function pointer resolver */
        typedef u32 (*fn_t)(u32, u32, u32, u32);
        extern void *host_fnptr_lookup(u32 addr);
        fn_t fn = (fn_t)host_fnptr_lookup(cb);
        if (fn) {
            fn((u32)(uintptr_t)node, 0, 0, 0);
        } else {
            fprintf(stderr, "[cb-dispatch] callback 0x%08X not found!\n", (unsigned)cb);
        }
        node[6] = 0;  /* clear callback pointer */
    }

    /* Unlink node from doubly-linked list:
     * prev->next = node->next;  next->prev = node->prev; */
    volatile u32 *prev = (volatile u32 *)(uintptr_t)node[0];
    volatile u32 *next = (volatile u32 *)(uintptr_t)node[1];
    if (prev) next[0] = (u32)(uintptr_t)prev;   /* next->prev = prev */
    if (next) prev[1] = (u32)(uintptr_t)next;    /* prev->next = next */

    /* Recycle node back to pool */
    FUN_02066430((void *)(uintptr_t)0x02069EC0u, (void *)node);

    /* Decrement active callback count */
    volatile u16 *counter = (volatile u16 *)(uintptr_t)0x02069EB0u;
    if (*counter > 0) (*counter)--;
}

/* FUN_0206619C: PER-FRAME callback dispatch — walks callback slots
 * from index 'start' to 'end' inclusive and calls the +0x0C callback
 * for every node whose flags & 3 == 0 (i.e. active, not deactivating).
 * This is the main game tick driver — it fires scene state machines,
 * animation updates, etc. every frame.
 *
 * ARM signature:
 *   0x020661D8: LDRH R0, [R7, #8]     ; flags
 *   0x020661DC: ANDS R0, R0, #3
 *   0x020661E0: BNE skip
 *   0x020661E4: LDR R1, [R7, #0xC]    ; callback = node+0x0C
 *   0x020661E8: MOV R0, R7            ; arg = node
 *   0x020661EC: BLX R1                ; call callback(node)
 */
void FUN_0206619c(int start, int end)
{
    if (start > end) return;

    u32 table_base = 0x02069ED4u;

    static int s_log = 0;
    static int s_skip_log = 0;

    for (int i = start; i <= end; i++) {
        u32 slot_addr = table_base + (u32)i * 16;
        if (slot_addr < 0x02000000u || slot_addr > 0x02400000u) continue;

        volatile u32 *slot = (volatile u32 *)(uintptr_t)slot_addr;
        u32 node_addr = slot[1];      /* +0x04 = linked list head */
        u32 sentinel  = slot_addr + 8; /* sentinel address */

        while (node_addr != sentinel && node_addr != 0) {
            if (node_addr < 0x02000000u || node_addr > 0x02400000u) break;

            volatile u32 *node = (volatile u32 *)(uintptr_t)node_addr;
            u32 next_addr = node[1];  /* save next (in case callback modifies list) */

            u16 flags = *(volatile u16 *)((u8 *)(uintptr_t)node_addr + 8);
            if ((flags & 3) == 0) {
                /* Active callback — call +0x0C via nds_call_1arg which
                 * tries host_fnptr_lookup first, then ARM interpreter */
                u32 cb_addr = node[3];  /* +0x0C */
                if (cb_addr != 0) {
                    extern void nds_call_1arg(u32 nds_addr, uintptr_t a);
                    if (s_log < 100) {
                        s_log++;
                        fprintf(stderr, "[cb-tick] slot %d calling 0x%08X(node=0x%08X) flags=0x%04X\n",
                                i, (unsigned)cb_addr, (unsigned)node_addr, (unsigned)flags);
                        fflush(stderr);
                    }
                    nds_call_1arg(cb_addr, (uintptr_t)node_addr);
                    /* Detect state transitions — always fires, no log limit */
                    if (cb_addr == 0x020768F0u) {
                        static u32 s_prev_state = 0xFF;
                        u32 state_val = *(volatile u32 *)((u8 *)(uintptr_t)node_addr + 0x20);
                        u32 counter_val = *(volatile u32 *)((u8 *)(uintptr_t)node_addr + 0x24);
                        u16 pad_newly = *(volatile u16 *)(uintptr_t)0x0205FFAEu;
                        if (state_val != s_prev_state) {
                            fprintf(stderr, "[STATE] %u -> %u (counter=%u pad_newly=0x%04X)\n",
                                    (unsigned)s_prev_state, (unsigned)state_val,
                                    (unsigned)counter_val, (unsigned)pad_newly);
                            fflush(stderr);
                            s_prev_state = state_val;
                        }
                    }
                }
            } else {
                if (s_skip_log < 10) {
                    s_skip_log++;
                    u32 cb_addr = node[3];
                    fprintf(stderr, "[cb-tick] slot %d SKIP cb=0x%08X(node=0x%08X) flags=0x%04X (flags&3=%d)\n",
                            i, (unsigned)cb_addr, (unsigned)node_addr, (unsigned)flags, (int)(flags & 3));
                    fflush(stderr);
                }
            }

            node_addr = next_addr;
        }
    }
}

/* FUN_020660AC: CLEANUP callback dispatch — walks callback slots and
 * fires one-shot +0x18 callbacks for nodes with flag bit 1 set, then
 * unlinks and recycles the node. Used for scene teardown. */
void FUN_020660ac(int start, int end)
{
    if (start > end) return;

    u32 table_base = 0x02069ED4u;

    static int s_log = 0;
    if (s_log < 5) {
        s_log++;
        fprintf(stderr, "[cb-dispatch] FUN_020660AC(%d, %d) table=0x%08X\n",
                start, end, (unsigned)table_base);
        fflush(stderr);
    }

    for (int i = start; i <= end; i++) {
        u32 slot_addr = table_base + (u32)i * 16;
        volatile u32 *slot = (volatile u32 *)(uintptr_t)slot_addr;

        /* Safety: verify slot address is in NDS memory range */
        if (slot_addr < 0x02000000u || slot_addr > 0x02400000u) continue;

        u32 node_addr = slot[1];  /* +0x04 = linked list head */
        u32 sentinel  = slot_addr + 8;  /* +0x08 = sentinel address */

        if (s_log <= 5 && node_addr != sentinel && node_addr != 0) {
            fprintf(stderr, "[cb-dispatch] slot %d: head=0x%08X sentinel=0x%08X\n",
                    i, (unsigned)node_addr, (unsigned)sentinel);
            fflush(stderr);
        }

        while (node_addr != sentinel && node_addr != 0) {
            /* Safety: verify node address is in NDS memory range */
            if (node_addr < 0x02000000u || node_addr > 0x02400000u) {
                if (s_log < 3) {
                    fprintf(stderr, "[cb-dispatch] bad node 0x%08X in slot %d\n",
                            (unsigned)node_addr, i);
                    fflush(stderr);
                }
                break;
            }

            volatile u32 *node = (volatile u32 *)(uintptr_t)node_addr;
            u32 next_addr = node[1];  /* save next before dispatch (node may be unlinked) */

            u16 flags = *(volatile u16 *)((u8 *)(uintptr_t)node_addr + 8);
            if (flags & 2) {
                FUN_02066128(node);
            }

            node_addr = next_addr;
        }
    }
}

/* FUN_02065A7C: VBlank callback dispatcher.
 * Called once per frame from the VBlank IRQ handler on real NDS.
 * Clears the display gate, dispatches registered callbacks, then sets
 * the gate so the base tick's display pipeline can fire next frame. */
extern void FUN_0202958c(void);  /* OS_DisableScheduler or similar */
extern void FUN_02029724(void);  /* OS_EnableScheduler or similar */
extern void FUN_02068214(void);  /* display mode commit */

void FUN_02065a7c(void)
{
    static int s_log = 0;

    FUN_0202958c();

    /* Clear two globals */
    *(volatile u32 *)(uintptr_t)0x02069DF0u = 0;
    *(volatile u32 *)(uintptr_t)0x02069DF8u = 0;  /* display gate → 0 */

    FUN_02029724();

    /* If display mode pointer is valid, commit display registers */
    u32 mode_ptr = *(volatile u32 *)(uintptr_t)0x02069DF4u;
    if (mode_ptr != 0) {
        if (s_log < 3) {
            fprintf(stderr, "[vblank] FUN_02068214 (mode_ptr=0x%08X)\n", (unsigned)mode_ptr);
            fflush(stderr);
        }
        FUN_02068214();
    }

    /* Dispatch all registered callbacks in slots 0..0xA */
    FUN_020660ac(0, 0xA);

    /* Re-open the display gate so the base tick runs the pipeline */
    *(volatile u32 *)(uintptr_t)0x02069DF8u = 1;
}
