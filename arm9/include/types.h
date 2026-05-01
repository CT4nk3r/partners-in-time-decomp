/**
 * Common type definitions for Mario & Luigi: Partners in Time decompilation.
 */

#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Fixed-width types matching NDS SDK conventions */
typedef uint8_t   u8;
typedef uint16_t  u16;
typedef uint32_t  u32;
typedef uint64_t  u64;
typedef int8_t    s8;
typedef int16_t   s16;
typedef int32_t   s32;
typedef int64_t   s64;

typedef volatile u8   vu8;
typedef volatile u16  vu16;
typedef volatile u32  vu32;
typedef volatile s8   vs8;
typedef volatile s16  vs16;
typedef volatile s32  vs32;

/* Fixed-point types (common in NDS games) */
typedef s16 fx16;   /* 1.3.12 fixed point */
typedef s32 fx32;   /* 1.19.12 fixed point */
typedef s64 fx64;   /* 1.51.12 fixed point */

#define FX16_SHIFT  12
#define FX32_SHIFT  12
#define FX64_SHIFT  12

#define FX32_ONE    (1 << FX32_SHIFT)

static inline fx32 FX32_FROM_INT(int x) { return (fx32)(x << FX32_SHIFT); }
static inline int  FX32_TO_INT(fx32 x)  { return (int)(x >> FX32_SHIFT); }

/* Boolean */
typedef int BOOL;
#define TRUE  1
#define FALSE 0

#endif /* TYPES_H */
