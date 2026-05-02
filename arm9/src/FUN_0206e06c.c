/*
 * FUN_0206E06C.c — host stub for overlay-0 per-entity render dispatcher.
 *
 * Used by FUN_02065A10 in its 2-iteration entity loop.  Real impl
 * dispatches on entity type and forwards to a per-entity render
 * routine that ultimately emits OAM/GXFIFO commands.
 *
 * MLPIT_FAKE_NODE_FN=fcb4 (or =0x0200FCB4):
 *   Forward the call to FUN_0200FCB4 (the 3D textured-quad emitter).
 *   This lets the synth-scene path actually emit GXFIFO commands so
 *   the host observer / SDL rasteriser have something to consume.
 *   Without this hook the entity argument is simply logged.
 */

#include "nds_platform.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int FUN_0200fcb4(void *entity);

static int s_route_to_fcb4 = -1;  /* lazy: -1=unknown, 0=no, 1=yes */

void FUN_0206e06c(void *entity)
{
    static uint64_t s_calls = 0;
    s_calls++;
    if ((s_calls & 0xFF) == 1) {
        fprintf(stderr, "[render] FUN_0206e06c entity=%p calls=%llu\n",
                entity, (unsigned long long)s_calls);
    }

    if (s_route_to_fcb4 < 0) {
        const char *v = getenv("MLPIT_FAKE_NODE_FN");
        s_route_to_fcb4 = 0;
        if (v) {
            /* Accept "fcb4", "0xfcb4", "0x0200fcb4", "0200fcb4" — case insensitive. */
            const char *p = v;
            while (*p == ' ' || *p == '\t') p++;
            if (p[0] == '0' && (p[1] == 'x' || p[1] == 'X')) p += 2;
            unsigned long val = strtoul(p, NULL, 16);
            if (val == 0xfcb4u || val == 0x0200fcb4u) {
                s_route_to_fcb4 = 1;
                fprintf(stderr,
                        "[render] FUN_0206e06c: routing to FUN_0200fcb4 "
                        "(MLPIT_FAKE_NODE_FN=%s)\n", v);
            }
        }
    }

    if (s_route_to_fcb4 == 1 && entity) {
        (void)FUN_0200fcb4(entity);
        return;
    }

    (void)entity;
}
