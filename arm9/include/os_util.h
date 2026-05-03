/**
 * OS Utility Functions
 */

#ifndef OS_UTIL_H
#define OS_UTIL_H

#include "types.h"

void OS_Dummy(void);
void OS_Nop(void);
void OS_Stub(void);
u32  OS_ReturnZero(void);
int  OS_CountLeadingZeros(u32 value);
u32  OS_Swap(u32 new_val, u32 *ptr);
u16  OS_GetField_04(void *obj);
u32  OS_GetField_70(void *obj);
u32  OS_GetField_38(void *obj);

#endif /* OS_UTIL_H */
