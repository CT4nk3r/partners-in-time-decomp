/**
 * Memory Utilities
 */

#ifndef MEM_UTIL_H
#define MEM_UTIL_H

#include "types.h"

void MI_CpuFill8(void *dest, u8 fill, u32 size);
void MI_CpuCopy8(const void *src, void *dest, u32 size);
void MI_CpuFill32Fast(u32 *dest, u32 fill, int size);
void RTC_SetDate(u32 date);

#endif /* MEM_UTIL_H */
