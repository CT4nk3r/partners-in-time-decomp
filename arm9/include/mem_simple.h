/**
 * Simple Memory Helpers
 */

#ifndef MEM_SIMPLE_H
#define MEM_SIMPLE_H

#include "types.h"

void MI_Fill16(u16 value, void *dest, int size);
void MI_Copy16(const void *src, void *dest, int size);
void MI_Fill32(u32 value, u32 *dest, int size);
void MI_Copy32(const u32 *src, u32 *dest, int size);
void MI_Fill32Fast(u32 value, u32 *dest, u32 size);
void MI_Copy32Fast(const u32 *src, u32 *dest, u32 size);
void MI_CopyStruct48(const u32 *src, u32 *dest);
void MI_Fill32_Alt(u32 value, u32 *dest, int size);

#endif /* MEM_SIMPLE_H */
