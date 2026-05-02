/*
 * FUN_0206E06C.c — host stub for overlay-0 per-entity render dispatcher.
 *
 * Used by FUN_02065A10 in its 2-iteration entity loop.  Real impl
 * dispatches on entity type and forwards to a per-entity render
 * routine that ultimately emits OAM/GXFIFO commands.
 *
 * Without a faithful entity struct, dereferencing offsets here would
 * NPE.  This stub just NULL-guards and returns.  Defining it as a
 * C symbol lets the fnptr trampoline resolve overlay-baked indirect
 * calls to a real host function rather than the [stub] sink.
 */

#include "nds_platform.h"
#include <stdint.h>
#include <stdio.h>

void FUN_0206e06c(void *entity)
{
    static uint64_t s_calls = 0;
    s_calls++;
    if ((s_calls & 0xFF) == 1) {
        fprintf(stderr, "[render] FUN_0206e06c entity=%p calls=%llu\n",
                entity, (unsigned long long)s_calls);
    }
    (void)entity;
}
