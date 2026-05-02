/**
 * Game Logic B — Sound buffer allocation, entity processing, animation updates
 *
 * This module handles the sound/resource buffer allocation system (priority-based
 * linked list allocator), entity iteration with callbacks, and animation data
 * updates for game objects.
 *
 * Decompiled from:
 *   FUN_0200d6c8 @ 0x0200D6C8 (80 bytes)   - Initialize resource node
 *   FUN_0200d798 @ 0x0200D798 (2324 bytes)  - Resource allocator with linked list (huge)
 *   FUN_0200e0d8 @ 0x0200E0D8 (20 bytes)   - Get resource type from header
 *   FUN_0200e0f0 @ 0x0200E0F0 (252 bytes)  - Process resource node metadata
 *   FUN_0200e1f0 @ 0x0200E1F0 (460 bytes)  - Allocate resource with priority check
 *   FUN_0200e3c0 @ 0x0200E3C0 (172 bytes)  - Free resource and update list
 *   FUN_0200e484 @ 0x0200E484 (104 bytes)  - Validate resource allocation
 *   FUN_0200e5b4 @ 0x0200E5B4 (20 bytes)   - Get entity base offset
 *   FUN_0200e5cc @ 0x0200E5CC (112 bytes)  - Initialize entity list node
 *   FUN_0200e640 @ 0x0200E640 (120 bytes)  - Remove entity from linked list
 *   FUN_0200e6c0 @ 0x0200E6C0 (1096 bytes) - Sound buffer allocator (large)
 *   FUN_0200eb10 @ 0x0200EB10 (120 bytes)  - Remove sound node from linked list
 *   FUN_0200eb90 @ 0x0200EB90 (1676 bytes) - Allocate sound buffer with priority (huge)
 *   FUN_0200f224 @ 0x0200F224 (164 bytes)  - Iterate entities and call callback
 *   FUN_0200f32c @ 0x0200F32C (1180 bytes) - Update entity with animation data (large)
 *   FUN_0200f7cc @ 0x0200F7CC (32 bytes)   - Cleanup entity sound+resource nodes
 *   FUN_0200f7ec @ 0x0200F7EC (32 bytes)   - Call entity destructor via vtable
 *   FUN_0200f80c @ 0x0200F80C (20 bytes)   - Dispatch entity update (indirect)
 *   FUN_0200f824 @ 0x0200F824 (724 bytes)  - Entity spawn and initialization
 *   FUN_0200fbdc @ 0x0200FBDC (212 bytes)  - Entity update tick
 *   FUN_0200fcb4 @ 0x0200FCB4 (2060 bytes) - Entity state machine processor (huge)
 */

#include "types.h"

/* Forward declarations */
extern void FUN_0200d028(int, u32);
extern void FUN_0200eb10(int);
extern void FUN_0200e640(int);
extern u32 FUN_02035530(void);
extern u32 FUN_02035518(void);

/* Static data references */
static u32 DAT_0200d718;       /* vtable for resource node */
static u32 *DAT_0200e0ec;     /* resource type lookup table */
static u32 *DAT_0200e5c8;     /* entity base offset pointer */
static u32 *DAT_entity_list;  /* entity linked list head */

static u32 *DAT_heap_head;    /* resource heap list head */
static u32 *DAT_heap_tail;    /* resource heap list tail */
static u32 *DAT_sound_list_head;  /* sound node list head */
static u32 *DAT_sound_list_tail;  /* sound node list tail */
static u32 *DAT_sound_buffer_head; /* sound buffer list head */
static u32 *DAT_sound_buffer_tail; /* sound buffer list tail */

// FUN_0200d6c8 @ 0x0200D6C8 (80 bytes) — Initialize resource node
u32 *FUN_0200d6c8(u32 *param_1, u8 param_2, u32 param_3, int param_4)
{
    u32 uVar1;

    *param_1 = DAT_0200d718;
    *(u8 *)(param_1 + 1) = param_2;
    *(char *)((int)param_1 + 5) = (char)param_4;
    param_1[2] = param_3;
    uVar1 = 0;
    if (0 < param_4) {
        do {
            FUN_0200d028((int)param_1, uVar1 & 0xff);
            uVar1 = uVar1 + 1;
        } while ((int)uVar1 < param_4);
    }
    return param_1;
}

// FUN_0200e0d8 @ 0x0200E0D8 (20 bytes) — Get resource type from header
u8 FUN_0200e0d8(u16 *param_1)
{
#ifdef HOST_PORT
    /* HOST_PORT: DAT_0200e0ec is a `static u32*` zero-init host global —
     * the dat_init mechanism only seeds extern globals so this lookup
     * table is NULL.  Until we wire a real init, return 0 (the most
     * common resource type, "no special handling needed") so callers
     * downstream don't NPE.  Also avoids the (u32) Win64 truncation. */
    if (DAT_0200e0ec == NULL || param_1 == NULL) return 0;
    return *(u8 *)((uintptr_t)DAT_0200e0ec + (((u32)*param_1 << 0x13) >> 0x1d));
#else
    return *(u8 *)((u32)DAT_0200e0ec + (((u32)*param_1 << 0x13) >> 0x1d));
#endif
}

// FUN_0200e5b4 @ 0x0200E5B4 (20 bytes) — Get entity base offset
int FUN_0200e5b4(int *param_1)
{
    return *DAT_0200e5c8 + *param_1;
}

// FUN_0200e640 @ 0x0200E640 (120 bytes) — Remove entity from linked list
void FUN_0200e640_impl(int param_1)
{
    if ((*DAT_sound_list_head != 0) && (*DAT_sound_list_head == (u32)param_1)) {
        *DAT_sound_list_head = *(u32 *)(param_1 + 0xc);
    }
    if ((*DAT_sound_list_tail != 0) && (*DAT_sound_list_tail == (u32)param_1)) {
        *DAT_sound_list_tail = *(u32 *)(param_1 + 8);
    }
    if (*(int *)(param_1 + 8) != 0) {
        *(u32 *)(*(int *)(param_1 + 8) + 0xc) = *(u32 *)(param_1 + 0xc);
    }
    if (*(int *)(param_1 + 0xc) != 0) {
        *(u32 *)(*(int *)(param_1 + 0xc) + 8) = *(u32 *)(param_1 + 8);
    }
    *(u32 *)(param_1 + 0xc) = 0;
    *(u32 *)(param_1 + 8) = *(u32 *)(param_1 + 0xc);
    *(u8 *)(param_1 + 0x14) = *(u8 *)(param_1 + 0x14) & 0xf7;
    return;
}

// FUN_0200eb10 @ 0x0200EB10 (120 bytes) — Remove sound node from linked list
void FUN_0200eb10_impl(int param_1)
{
    if ((*DAT_sound_list_head != 0) && (*DAT_sound_list_head == (u32)param_1)) {
        *DAT_sound_list_head = *(u32 *)(param_1 + 0xc);
    }
    if ((*DAT_sound_list_tail != 0) && (*DAT_sound_list_tail == (u32)param_1)) {
        *DAT_sound_list_tail = *(u32 *)(param_1 + 8);
    }
    if (*(int *)(param_1 + 8) != 0) {
        *(u32 *)(*(int *)(param_1 + 8) + 0xc) = *(u32 *)(param_1 + 0xc);
    }
    if (*(int *)(param_1 + 0xc) != 0) {
        *(u32 *)(*(int *)(param_1 + 0xc) + 8) = *(u32 *)(param_1 + 8);
    }
    *(u32 *)(param_1 + 0xc) = 0;
    *(u32 *)(param_1 + 8) = *(u32 *)(param_1 + 0xc);
    *(u8 *)(param_1 + 0x14) = *(u8 *)(param_1 + 0x14) & 0xf7;
    return;
}

// FUN_0200f224 @ 0x0200F224 (164 bytes) — Iterate entities and call callback
void FUN_0200f224(u32 param_1, u32 param_2)
{
    int iVar1;
    int *piVar2;

    piVar2 = (int *)*DAT_entity_list;
    if (piVar2 != (int *)0) {
        do {
            iVar1 = (*(int (*)(void))(*(int *)(*piVar2 + 0xc)))();
            if ((iVar1 == 0x1b8) &&
               ((((param_2 & 1) != 0 && (-1 < piVar2[0x1f] << 9)) ||
                (((param_2 & 2) != 0 && (piVar2[0x1f] << 9 < 0)))))) {
                (*(void (*)(int *, u32))(*(int *)(*piVar2 + 0xb8)))(piVar2, param_1);
            }
            piVar2 = (int *)piVar2[2];
        } while (piVar2 != (int *)0);
        return;
    }
    return;
}

// FUN_0200f7cc @ 0x0200F7CC (32 bytes) — Cleanup entity sound+resource nodes
void FUN_0200f7cc(int param_1)
{
    FUN_0200eb10(param_1 + 0x130);
    FUN_0200e640(param_1 + 0x148);
    return;
}

// FUN_0200f7ec @ 0x0200F7EC (32 bytes) — Call entity destructor via vtable
void FUN_0200f7ec(int *param_1)
{
    (*(void (*)(void))(*(int *)(*param_1 + 0x5c)))();
    return;
}

// FUN_0200f80c @ 0x0200F80C (20 bytes) — Dispatch entity update (indirect)
void FUN_0200f80c(u32 param_1, u8 param_2)
{
    /* Indirect jump to entity update dispatcher */
    /* (*DAT_0200f820)(3, param_2, param_1); */
    return;
}
