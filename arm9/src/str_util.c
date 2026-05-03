/**
 * String Utilities
 *
 * Decompiled from Ghidra pseudocode.
 */

#include "types.h"

// FUN_0202acdc @ 0x0202ACDC (40 bytes) — String length (strlen equivalent)
int STR_Length(const char *str)
{
    int len = 0;
    while (*str != '\0') {
        str++;
        len++;
    }
    return len;
}
