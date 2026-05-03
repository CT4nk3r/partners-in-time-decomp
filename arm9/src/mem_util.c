/**
 * Memory Utilities — Nitro SDK
 *
 * Optimized memset/memcpy implementations with alignment handling.
 * These are the SDK's internal memory routines used throughout the game.
 *
 * Decompiled from:
 *   FUN_0203b914 @ 0x0203B914 (148 bytes) - MI_CpuFill8 (byte-granular memset)
 *   FUN_0203b9a8 @ 0x0203B9A8 (304 bytes) - MI_CpuCopy8 (byte-granular memcpy)
 *   FUN_0202cc10 @ 0x0202CC10 (72 bytes)  - MI_CpuFill32Fast (unrolled word fill)
 *   FUN_0202cc00 @ 0x0202CC00 (12 bytes)  - RTC_SetDate (simple global setter)
 */

#include "types.h"

/**
 * MI_CpuFill8 — Fill memory with a byte value, handling arbitrary alignment
 * Original: FUN_0203b914 @ 0x0203B914
 *
 * Handles unaligned start/end by writing halfwords at boundaries,
 * then fills aligned middle portion with 32-bit writes.
 */
void MI_CpuFill8(void *dest, u8 fill, u32 size)
{
    u8 *dst = (u8 *)dest;
    u16 val16;
    u32 val32;

    if (size == 0) return;

    // Handle odd start address
    if ((u32)dst & 1) {
        val16 = *(u16 *)(dst - 1);
        *(u16 *)(dst - 1) = (val16 & 0xFF) | ((u16)fill << 8);
        dst++;
        size--;
        if (size == 0) return;
    }

    if (size > 1) {
        val16 = (u16)fill | ((u16)fill << 8);

        // Handle 2-byte alignment for 4-byte boundary
        if ((u32)dst & 2) {
            *(u16 *)dst = val16;
            dst += 2;
            size -= 2;
            if (size == 0) return;
        }

        val32 = (u32)val16 | ((u32)val16 << 16);

        // Fill aligned 32-bit words
        if ((size & ~3) != 0) {
            u32 *end = (u32 *)(dst + (size & ~3));
            u32 *p = (u32 *)dst;
            do {
                *p++ = val32;
            } while (p < end);
            dst = (u8 *)p;
        }

        // Handle trailing 2 bytes
        if (size & 2) {
            *(u16 *)dst = val16;
            dst += 2;
        }
    }

    // Handle trailing odd byte
    if (size & 1) {
        *(u16 *)dst = (*(u16 *)dst & 0xFF00) | (fill & 0xFF);
    }
}

/**
 * MI_CpuCopy8 — Copy memory with arbitrary alignment handling
 * Original: FUN_0203b9a8 @ 0x0203B9A8
 *
 * Optimized copy that handles all alignment combinations:
 * - Both aligned: uses 32-bit word copies
 * - Same 2-byte alignment: uses 16-bit copies
 * - Misaligned: uses shift-based byte extraction
 */
void MI_CpuCopy8(const void *src, void *dest, u32 size)
{
    const u16 *s = (const u16 *)src;
    u16 *d = (u16 *)dest;

    if (size == 0) return;

    // Handle odd destination address
    if ((u32)d & 1) {
        u16 val;
        if (((u32)s & 1) == 0) {
            val = *s;
        } else {
            val = *(u16 *)((u8 *)s - 1) >> 8;
        }
        *(u16 *)((u8 *)d - 1) = (*(u16 *)((u8 *)d - 1) & 0xFF) | (val << 8);
        s = (const u16 *)((u8 *)s + 1);
        d = (u16 *)((u8 *)d + 1);
        size--;
        if (size == 0) return;
    }

    // Check if src and dst have same byte alignment
    if ((((u32)d ^ (u32)s) & 1) == 0) {
        // Same byte alignment — check word alignment
        if ((((u32)d ^ (u32)s) & 2) == 0) {
            // Both have same word alignment
            if (size > 1) {
                // Handle 2-byte alignment
                if ((u32)d & 2) {
                    *d = *s;
                    s++;
                    d++;
                    size -= 2;
                    if (size == 0) return;
                }

                // Copy aligned 32-bit words
                if ((size & ~3) != 0) {
                    u32 *end = (u32 *)((u8 *)d + (size & ~3));
                    const u32 *sp = (const u32 *)s;
                    u32 *dp = (u32 *)d;
                    do {
                        *dp++ = *sp++;
                    } while (dp < end);
                    s = (const u16 *)sp;
                    d = (u16 *)dp;
                }

                // Copy trailing halfword
                if (size & 2) {
                    *d = *s;
                    s++;
                    d++;
                }
            }
        } else {
            // Same byte alignment but different word alignment — halfword copy
            if ((size & ~1) != 0) {
                u16 *end = (u16 *)((u8 *)d + (size & ~1));
                do {
                    *d++ = *s++;
                } while (d < end);
            }
        }

        // Copy trailing byte
        if (size & 1) {
            *d = (*d & 0xFF00) | (*s & 0xFF);
        }
    } else {
        // Misaligned — need to do shift-based copy
        s = (const u16 *)((u32)s & ~1);
        u16 prev = *s >> 8;

        while (size > 1) {
            s++;
            u16 cur = *s;
            *d = (u16)prev | (cur << 8);
            prev = cur >> 8;
            d++;
            size -= 2;
        }

        if ((size - 2) & 1) {
            *d = (*d & 0xFF00) | prev;
        }
    }
}

/**
 * MI_CpuFill32Fast — Fast unrolled 32-bit memory fill
 * Original: FUN_0202cc10 @ 0x0202CC10
 *
 * Fills memory 12 words (48 bytes) at a time for performance,
 * then uses a computed jump for the remainder.
 *
 * NOTE: The original uses a jump table for the tail. This version
 * approximates the behavior. Needs refinement for exact matching.
 */
void MI_CpuFill32Fast(u32 *dest, u32 fill, int size)
{
    int remaining;

    if (size < 1) return;

    while ((remaining = size - 0x30), remaining != 0 && size > 0x2F) {
        dest[0]  = fill;
        dest[1]  = fill;
        dest[2]  = fill;
        dest[3]  = fill;
        dest[4]  = fill;
        dest[5]  = fill;
        dest[6]  = fill;
        dest[7]  = fill;
        dest[8]  = fill;
        dest[9]  = fill;
        dest[10] = fill;
        dest[11] = fill;
        dest += 12;
        size = remaining;
    }

    // Remainder handled by computed jump in original
    // TODO: Match the jump table exactly
    while (remaining > 0) {
        *dest++ = fill;
        remaining -= 4;
    }
}

/**
 * RTC_SetDate — Store date value in global
 * Original: FUN_0202cc00 @ 0x0202CC00
 */
static u32 *sRtcDate;  // DAT_0202cc0c

void RTC_SetDate(u32 date)
{
    *sRtcDate = date;
}
