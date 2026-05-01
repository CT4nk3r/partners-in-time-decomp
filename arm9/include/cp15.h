/**
 * CP15 Coprocessor — Cache & Protection Unit
 */

#ifndef CP15_H
#define CP15_H

#include "types.h"

void CP15_InvalidateDataCacheRange(u32 addr, int size);
void CP15_CleanDataCacheRange(u32 addr, int size);
u32  CP15_DataSyncBarrier(void);
void CP15_InvalidateInsnCacheRange(u32 addr, int size);
u32  CP15_EnableProtectionUnit(void);
u32  CP15_DisableProtectionUnit(void);
void CP15_SetDataRegion(u32 region);
void CP15_SetInsnRegion(u32 region);
u32  CP15_GetInsnRegion(void);
u32  CP15_WaitForInterrupt(void);

#endif /* CP15_H */
