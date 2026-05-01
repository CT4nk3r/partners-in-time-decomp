/**
 * Heap Allocator
 */

#ifndef HEAP_H
#define HEAP_H

#include "types.h"
#include "mem_util.h"

struct HeapBlock;
void *OS_AllocFromHeap(u32 heap_id, int size, struct HeapBlock *block, int direction);
void *OS_Alloc(u32 size, u32 heap_id);
void *OS_AllocBack(u32 size, u32 heap_id);
void  OS_InitHeap(void);

#endif /* HEAP_H */
