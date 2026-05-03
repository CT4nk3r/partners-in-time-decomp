/**
 * Allocation Wrappers — Heap & Object Factory Functions
 *
 * Thin wrappers around OS_Alloc that allocate fixed-size structs
 * and call their constructors, plus linked-list heap metadata ops
 * protected by mutex.
 *
 * Decompiled from Ghidra pseudocode.
 */

#include "types.h"

// ============================================================
// External declarations
// ============================================================

extern void *OS_Alloc(u32 size);
extern u32 OS_LockMutex(u32 id);
extern void OS_UnlockMutex(u32 lock);

// Constructor-like init functions (in overlays / other modules)
extern void FUN_020787b4(void *ptr, int type, int param);
extern void FUN_0206f080(void *ptr, int type, int param);
extern void FUN_02070a94(void *ptr, int type, int param);
extern void FUN_0206f600(void *ptr, int type, int param);
extern void FUN_0206c6cc(void *ptr, int type, int param);
extern void FUN_0207271c(void *ptr, int type, int param);
extern void FUN_02077784(void *ptr, int type, int param);
extern void FUN_020739ec(void *ptr, int type, int param);
extern void FUN_02029ab8(u32 val);
extern void FUN_0202807c(void);
extern void FUN_02028128(u32 a, u32 b, u32 c);
extern void FUN_0202f73c(u32 a, u32 b);
extern void FUN_02026868(u32 a, u32 b, u32 c);
extern void FUN_0203d220(u32 a);
extern void FUN_0203d26c(u32 a);
extern void FUN_0203bef4(u32 a, u32 b);
extern void FUN_0203bec8(u32 a, u32 b);
extern void FUN_0203bf20(u32 a, u32 b);

// ============================================================
// Object factory allocators (fixed-size alloc + constructor)
// ============================================================

// FUN_02028f48 @ 0x02028f48 (68 bytes) — Allocate and construct object type A
static u32 sObjSizeA = 0; // from DAT_02028f8c
void alloc_construct_obj_a(void)
{
    void *ptr = OS_Alloc(sObjSizeA);
    if (ptr != NULL) {
        FUN_020787b4(ptr, 8, 0);
    }
}

// FUN_02028f90 @ 0x02028f90 (68 bytes) — Allocate 0x30-byte object type B
void alloc_construct_obj_b(void)
{
    void *ptr = OS_Alloc(0x30);
    if (ptr != NULL) {
        FUN_0206f080(ptr, 8, 0);
    }
}

// FUN_02028fd4 @ 0x02028fd4 (68 bytes) — Allocate 0x30-byte object type C
void alloc_construct_obj_c(void)
{
    void *ptr = OS_Alloc(0x30);
    if (ptr != NULL) {
        FUN_02070a94(ptr, 8, 0);
    }
}

// FUN_02029018 @ 0x02029018 (68 bytes) — Allocate 0x30-byte object type D
void alloc_construct_obj_d(void)
{
    void *ptr = OS_Alloc(0x30);
    if (ptr != NULL) {
        FUN_0206f600(ptr, 8, 0);
    }
}

// FUN_0202905c @ 0x0202905c (68 bytes) — Allocate 0x30-byte object type E
void alloc_construct_obj_e(void)
{
    void *ptr = OS_Alloc(0x30);
    if (ptr != NULL) {
        FUN_0206c6cc(ptr, 8, 0);
    }
}

// FUN_020290a0 @ 0x020290a0 (68 bytes) — Allocate 0x38-byte object type F
void alloc_construct_obj_f(void)
{
    void *ptr = OS_Alloc(0x38);
    if (ptr != NULL) {
        FUN_0207271c(ptr, 8, 0);
    }
}

// FUN_020290e4 @ 0x020290e4 (68 bytes) — Allocate 0x30-byte object type G
void alloc_construct_obj_g(void)
{
    void *ptr = OS_Alloc(0x30);
    if (ptr != NULL) {
        FUN_02077784(ptr, 8, 0);
    }
}

// FUN_02029128 @ 0x02029128 (68 bytes) — Allocate 0x30-byte object type H
void alloc_construct_obj_h(void)
{
    void *ptr = OS_Alloc(0x30);
    if (ptr != NULL) {
        FUN_020739ec(ptr, 8, 0);
    }
}

// ============================================================
// Heap metadata operations
// ============================================================

// FUN_02029844 @ 0x02029844 (56 bytes) — Clear array slot and free associated data
static u32 sSlotArray[4];  // base at DAT_0202987c
void heap_clear_slot(int index)
{
    FUN_02029ab8(sSlotArray[index]);
    sSlotArray[index] = 0;
}

// FUN_02029bb0 @ 0x02029bb0 (48 bytes) — Merge adjacent free blocks
void heap_merge_blocks(u32 *dest, u32 *src)
{
    // Block header: +0=prev, +4=next, +8=size (bit0=free)
    dest[2] = dest[2] + (src[2] & 0xFFFFFFFE) + 0x10;
    dest[1] = src[1];
    if ((u32 *)dest[1] != NULL) {
        *(u32 *)dest[1] = (u32)dest;
    }
}

// ============================================================
// Mutex-protected object operations
// ============================================================

// FUN_02029e20 @ 0x02029e20 (80 bytes) — Call destructor with mutex lock
void obj_destroy_locked(u32 *obj)
{
    u32 lock;

    if (obj != NULL) {
        lock = OS_LockMutex(1);
        if (obj != NULL) {
            void (*destructor)(void) = (void (*)(void))obj[1];
            destructor();
        }
        OS_UnlockMutex(lock);
    }
}

// FUN_02029e70 @ 0x02029e70 (68 bytes) — Clear 4 list head pointers with mutex
static u32 *sListHead[4];  // DAT_02029eb4..02029ec0
void obj_clear_list_heads(void)
{
    u32 lock;

    lock = OS_LockMutex(1);
    sListHead[0] = NULL;
    sListHead[1] = NULL;
    sListHead[2] = NULL;
    sListHead[3] = NULL;
    OS_UnlockMutex(lock);
}

// FUN_02029ffc @ 0x02029ffc (56 bytes) — Set object flags with mutex
void obj_set_flags(u32 obj)
{
    u8 *flags = (u8 *)(obj + 0x12);
    u32 lock;

    lock = OS_LockMutex(1);
    *flags = (*flags & 0xFE) | 0x01;
    *flags = *flags | 0x02;
    OS_UnlockMutex(lock);
}

// FUN_0202a034 @ 0x0202a034 (52 bytes) — Clear and set object flags with mutex
void obj_clear_set_flags(u32 obj)
{
    u8 *flags = (u8 *)(obj + 0x12);
    u32 lock;

    lock = OS_LockMutex(1);
    *flags = *flags & 0xFE;
    *flags = *flags | 0x02;
    OS_UnlockMutex(lock);
}

// ============================================================
// Manager initialization
// ============================================================

// FUN_020264f8 @ 0x020264f8 (52 bytes) — Initialize heap manager
static u32 sHeapInitParam;  // DAT_0202652c
void heap_manager_init(void)
{
    FUN_0202807c();
    FUN_02028128(0, 0x4A000, 0x1000);
    FUN_0202f73c(0, sHeapInitParam);
}

// FUN_02026928 @ 0x02026928 (36 bytes) — Setup block with init and notify
void block_setup_notify(u32 block_id, u32 param2, u32 init_func, u32 param4)
{
    FUN_02026868(block_id, init_func, param4);
    FUN_0203d220(block_id);
}

// FUN_0202694c @ 0x0202694c (60 bytes) — Setup block range, fill, and notify
void block_setup_fill_notify(u32 block_addr, u32 block_size, u32 init_func, u32 param4)
{
    FUN_02026868(block_addr, init_func, param4);
    FUN_0203bef4(block_addr, block_addr + block_size);
    FUN_0203d26c(block_addr);
}

// FUN_02026988 @ 0x02026988 (60 bytes) — Setup block range, process, and notify
void block_setup_process_notify(u32 block_addr, u32 block_size, u32 init_func, u32 param4)
{
    FUN_02026868(block_addr, init_func, param4);
    FUN_0203bec8(block_addr, block_addr + block_size);
    FUN_0203d220(block_addr);
}

// FUN_020269c4 @ 0x020269c4 (52 bytes) — Setup block range with init and process
void block_setup_process(u32 block_addr, u32 block_size, u32 init_func, u32 param4)
{
    FUN_02026868(block_addr, init_func, param4);
    FUN_0203bf20(block_addr, block_addr + block_size);
}
