/**
 * BIOS System Call Wrappers
 */

#ifndef BIOS_H
#define BIOS_H

#include "types.h"

void BIOS_Stop(void);
void BIOS_VBlankIntrWait(void);
void BIOS_CpuSet(const void *src, void *dst, u32 mode);

#endif /* BIOS_H */
