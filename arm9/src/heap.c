/**
 * Heap Allocator — Nitro SDK Memory Management
 *
 * Block-based heap allocator with free list. Supports two allocation
 * directions (front/back) and splits free blocks when possible.
 *
 * Block header layout (16 bytes):
 *   +0x00: prev pointer (linked list)
 *   +0x04: next pointer (linked list)
 *   +0x08: size (bit 0 = free flag)
 *   +0x0C: flags (bits 0-4 = heap id)
 *
 * Decompiled from:
 *   FUN_02029964 @ 0x02029964 (332 bytes) - OS_AllocFromHeap
 *   FUN_02029c1c @ 0x02029C1C (36 bytes)  - OS_Alloc (front wrapper)
 *   FUN_02029bf8 @ 0x02029BF8 (36 bytes)  - OS_AllocBack (back wrapper)
 *   FUN_02028ea8 @ 0x02028EA8 (120 bytes) - OS_InitHeap
 */

#include "types.h"
#include "heap.h"

/* Heap descriptor table (global, indexed by heap ID) */
typedef struct HeapBlock {
    struct HeapBlock *prev;
    struct HeapBlock *next;
    u32 size;       /* bit 0: 1=free, 0=allocated; rest = usable size */
    u32 flags;      /* bits 0-4: owning heap ID */
} HeapBlock;

/* Heap table: each entry is 16 bytes (4 pointers) */
extern u8 *sHeapTable;       /* DAT_02029ab0 — front-alloc list heads */
extern u8 *sHeapTableBack;   /* DAT_02029ab4 — back-alloc list heads */

#define BLOCK_HDR_SIZE  16
#define BLOCK_FREE_BIT  1
#define HEAP_ID_MASK    0x1F

#define HEAP_FRONT_HEAD(id) (*(HeapBlock **)(sHeapTable + (id) * 16))
#define HEAP_BACK_HEAD(id)  (*(HeapBlock **)(sHeapTableBack + (id) * 16))
#define HEAP_MIN_ADDR(id)   (*(HeapBlock **)(sHeapTable + 16))

/**
 * OS_AllocFromHeap — Allocate memory from a specific heap
 * Original: FUN_02029964 @ 0x02029964
 *
 * @param heap_id   Heap identifier (0 or 1)
 * @param size      Requested allocation size
 * @param block     Starting block to search (overridden for direction)
 * @param direction 0 = front (first-fit from low), 1 = back (first-fit from high)
 * @return          Pointer to allocated memory, or NULL
 */
void *OS_AllocFromHeap(u32 heap_id, int size, HeapBlock *block, int direction)
{
    u32 aligned_size;
    u32 block_size;

    if (size == 0) {
        return NULL;
    }

    /* Align size to 4 bytes */
    aligned_size = (size + 3) & ~3;

    /* Get list head based on direction */
    if (direction == 1) {
        block = HEAP_FRONT_HEAD(heap_id);
    } else {
        block = HEAP_BACK_HEAD(heap_id);
    }

    /* Back-alloc: dereference once more */
    if (heap_id == 1) {
        block = (HeapBlock *)block->prev;
    }

    /* Search free list */
    do {
        block_size = block->size;

        /* Check if block is free and large enough */
        if ((block_size & BLOCK_FREE_BIT) && (aligned_size <= block_size)) {
            block_size &= ~BLOCK_FREE_BIT;

            /* Split block if remainder is large enough for a new block */
            if (aligned_size + BLOCK_HDR_SIZE < block_size) {
                if (heap_id == 1) {
                    /* Back allocation: split from end */
                    HeapBlock *new_block = (HeapBlock *)((u8 *)block + (block_size - aligned_size));
                    new_block->size = aligned_size;
                    new_block->prev = (HeapBlock *)block;
                    new_block->next = block->next;

                    if (new_block->next != NULL) {
                        new_block->next->prev = new_block;
                    }

                    block->size = (block_size - (aligned_size + BLOCK_HDR_SIZE)) | BLOCK_FREE_BIT;
                    block->next = new_block;
                    block = new_block;

                    if (block < HEAP_MIN_ADDR(0)) {
                        HEAP_MIN_ADDR(0) = block;
                    }
                } else {
                    /* Front allocation: split from start */
                    HeapBlock *remainder = (HeapBlock *)((u8 *)block + aligned_size + BLOCK_HDR_SIZE);
                    remainder->size = (block_size - (aligned_size + BLOCK_HDR_SIZE)) | BLOCK_FREE_BIT;
                    remainder->prev = block;
                    remainder->next = block->next;

                    if (remainder->next != NULL) {
                        remainder->next->prev = remainder;
                    }

                    block->size = aligned_size;
                    block->next = remainder;

                    if ((u32)remainder > *(u32 *)(sHeapTable + heap_id * 16)) {
                        *(u32 *)(sHeapTable + heap_id * 16) = (u32)remainder;
                    }
                }
            } else {
                /* Block fits exactly (or remainder too small) */
                block->size = block_size;
            }

            /* Mark as allocated with heap ID */
            block->flags = (block->flags & ~HEAP_ID_MASK) | (heap_id & HEAP_ID_MASK);

            /* Return pointer past header */
            return (void *)(block + 1);
        }

        /* Traverse list based on direction */
        if (heap_id == 1) {
            block = block->prev;
        } else {
            block = block->next;
        }
    } while (block != NULL);

    return NULL;
}

/**
 * OS_Alloc — Allocate from heap (front direction)
 * Original: FUN_02029c1c @ 0x02029C1C
 */
void *OS_Alloc(u32 size, u32 heap_id)
{
#ifdef HOST_PORT
    /* The real heap requires OS_InitHeap + sHeapTable + an 8KB
     * sHeapBuffer that we don't yet wire up on host. Carve from a
     * low-memory arena so the returned pointers fit in the u32 slots
     * the decompiled code uses (e.g. *DAT_02005d28 = (u32)alloc_result;
     * later cast back to pointer). On 64-bit Windows libc malloc
     * hands back addresses far above 4GiB which silently truncate.
     *
     * Strategy: lazily VirtualAlloc one big region whose base address
     * fits in 32 bits, then bump-allocate from it. Never freed - this
     * is a one-shot leak suitable for boot-up factories. */
    extern void *VirtualAlloc(void *, unsigned long long, unsigned long, unsigned long);
    extern void *memset(void *, int, unsigned long long);

    static unsigned char *s_arena = 0;
    static unsigned long s_off = 0;
    static const unsigned long ARENA_SIZE = 4u * 1024u * 1024u; /* 4 MiB */

    if (!s_arena) {
        /* MEM_RESERVE|MEM_COMMIT = 0x3000, PAGE_READWRITE = 0x04. */
        for (unsigned long base = 0x10000000u; base < 0x80000000u && !s_arena;
             base += 0x01000000u) {
            s_arena = (unsigned char *)VirtualAlloc(
                (void *)(unsigned long long)base, ARENA_SIZE,
                0x3000u, 0x04u);
        }
        if (!s_arena) {
            s_arena = (unsigned char *)VirtualAlloc(
                (void *)0, ARENA_SIZE, 0x3000u, 0x04u);
        }
    }
    if (!s_arena) {
        return NULL;
    }

    u32 sz = (size + 7u) & ~7u;
    if (s_off + sz > ARENA_SIZE) {
        return NULL;
    }
    unsigned char *p = s_arena + s_off;
    s_off += sz;
    memset(p, 0, sz);
    return p;
#else
    return OS_AllocFromHeap(heap_id, size, NULL, 0);
#endif
}

/**
 * OS_AllocBack — Allocate from heap (back direction)
 * Original: FUN_02029bf8 @ 0x02029BF8
 */
void *OS_AllocBack(u32 size, u32 heap_id)
{
#ifdef HOST_PORT
    /* Use the same low-memory arena as OS_Alloc */
    return OS_Alloc(size, heap_id);
#else
    return OS_AllocFromHeap(heap_id, size, NULL, 1);
#endif
}

/**
 * OS_InitHeap — Initialize a memory heap
 * Original: FUN_02028ea8 @ 0x02028EA8
 *
 * Clears 8KB of heap memory, sets up the allocator tables,
 * creates the initial free block, and marks heap as ready.
 */
void OS_InitHeap(void)
{
    extern u8 *sHeapBuffer;     /* DAT_02028f24 — 8KB heap buffer */
    extern u32 *sHeapReady;     /* DAT_02028f20 — ready flag */
    extern void *sHeapConfig;   /* DAT_02028f28 — heap config */

    extern void OS_SetHeapMode(int mode);           /* FUN_02028774 */
    extern void OS_SetHeapConfig(void *config);     /* FUN_0202840c */
    extern int  OS_CreateHeap(int id, void *base, u32 size); /* FUN_02028528 */
    extern int  OS_ActivateHeap(int handle);        /* FUN_02028440 */

    int handle;

    *sHeapReady = 0;

    /* Clear 8KB heap memory */
    MI_CpuFill8(sHeapBuffer, 0, 0x2000);

    /* Configure heap subsystem */
    OS_SetHeapMode(1);
    OS_SetHeapConfig(sHeapConfig);

    /* Create and activate the heap */
    handle = OS_CreateHeap(0, sHeapBuffer, 0x2000);
    if (handle == 0) {
        return;
    }

    handle = OS_ActivateHeap(handle);
    if (handle != 0) {
        *sHeapReady = 1;
    }
}
