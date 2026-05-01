/**
 * Simple Memory Helpers — Unrolled fill/copy variants
 *
 * These are simpler, less-optimized memory routines used alongside
 * the MI_CpuFill8/MI_CpuCopy8 functions. Used for small known-size
 * or word-aligned operations.
 *
 * Decompiled from Ghidra pseudocode.
 */

#include "types.h"

// FUN_0203b7a8 @ 0x0203B7A8 (24 bytes) — Fill memory with 16-bit value
void MI_Fill16(u16 value, void *dest, int size)
{
    int i;
    for (i = 0; i < size; i += 2) {
        *(u16 *)((u8 *)dest + i) = value;
    }
}

// FUN_0203b7c0 @ 0x0203B7C0 (28 bytes) — Copy memory in 16-bit units
void MI_Copy16(const void *src, void *dest, int size)
{
    int i;
    for (i = 0; i < size; i += 2) {
        *(u16 *)((u8 *)dest + i) = *(u16 *)((const u8 *)src + i);
    }
}

// FUN_0203b7dc @ 0x0203B7DC (20 bytes) — Fill memory with 32-bit value (simple loop)
void MI_Fill32(u32 value, u32 *dest, int size)
{
    u32 *end = (u32 *)((u8 *)dest + size);
    for (; dest < end; dest++) {
        *dest = value;
    }
}

// FUN_0203b7f0 @ 0x0203B7F0 (24 bytes) — Copy memory in 32-bit units (simple loop)
void MI_Copy32(const u32 *src, u32 *dest, int size)
{
    u32 *end = (u32 *)((u8 *)dest + size);
    for (; dest < end; dest++) {
        *dest = *src++;
    }
}

// FUN_0203b808 @ 0x0203B808 (76 bytes) — Fast 32-bit fill (unrolled 8x)
void MI_Fill32Fast(u32 value, u32 *dest, u32 size)
{
    u32 *end_aligned = (u32 *)((u8 *)dest + (size & ~0x1F));
    u32 *end = (u32 *)((u8 *)dest + size);

    // Unrolled loop: 8 words (32 bytes) at a time
    for (; dest < end_aligned; dest += 8) {
        dest[0] = value;
        dest[1] = value;
        dest[2] = value;
        dest[3] = value;
        dest[4] = value;
        dest[5] = value;
        dest[6] = value;
        dest[7] = value;
    }

    // Remainder
    for (; dest < end; dest++) {
        *dest = value;
    }
}

// FUN_0203b854 @ 0x0203B854 (56 bytes) — Fast 32-bit copy (unrolled 8x)
void MI_Copy32Fast(const u32 *src, u32 *dest, u32 size)
{
    u32 *end_aligned = (u32 *)((u8 *)dest + (size & ~0x1F));
    u32 *end = (u32 *)((u8 *)dest + size);

    // Unrolled loop: 8 words (32 bytes) at a time
    for (; dest < end_aligned; dest += 8, src += 8) {
        u32 v0 = src[0];
        u32 v1 = src[1];
        u32 v2 = src[2];
        u32 v3 = src[3];
        u32 v4 = src[4];
        u32 v5 = src[5];
        u32 v6 = src[6];
        u32 v7 = src[7];
        dest[0] = v0;
        dest[1] = v1;
        dest[2] = v2;
        dest[3] = v3;
        dest[4] = v4;
        dest[5] = v5;
        dest[6] = v6;
        dest[7] = v7;
    }

    // Remainder
    for (; dest < end; dest++) {
        *dest = *src++;
    }
}

// FUN_0203b8c4 @ 0x0203B8C4 (36 bytes) — Copy 48-byte struct (12 words, unrolled 3x)
void MI_CopyStruct48(const u32 *src, u32 *dest)
{
    u32 v1, v2;

    v1 = src[1]; v2 = src[2];
    dest[0] = src[0]; dest[1] = v1; dest[2] = v2;

    v1 = src[4]; v2 = src[5];
    dest[3] = src[3]; dest[4] = v1; dest[5] = v2;

    v1 = src[7]; v2 = src[8];
    dest[6] = src[6]; dest[7] = v1; dest[8] = v2;

    v1 = src[10]; v2 = src[11];
    dest[9] = src[9]; dest[10] = v1; dest[11] = v2;
}

// FUN_02059e1c @ 0x02059E1C (20 bytes) — Fill memory with 32-bit value (duplicate of MI_Fill32)
void MI_Fill32_Alt(u32 value, u32 *dest, int size)
{
    u32 *end = (u32 *)((u8 *)dest + size);
    for (; dest < end; dest++) {
        *dest = value;
    }
}
