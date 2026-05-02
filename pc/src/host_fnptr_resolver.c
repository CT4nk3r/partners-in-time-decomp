/*
 * host_fnptr_resolver.c — NDS function-pointer → host symbol trampoline.
 *
 * The NDS game stores ARM-domain code pointers (values like 0x0202xxxx)
 * in vtables and DAT_ slots. On x86_64 we cannot jump to those addresses;
 * we have to translate them to host C function pointers obtained via
 * the compiler/linker.
 *
 * Registration happens once at startup via host_fnptr_register_decompiled()
 * (auto-generated, see tools/scripts/gen_fnptr_table.py). The runtime
 * call sites use:
 *   - host_fnptr_lookup(nds_addr): NULL if unknown
 *   - host_fnptr_register(nds_addr, fn): manual registration (e.g. for
 *     synthetic host trampolines mapped under reserved 0xFAxxxxxx range)
 *   - nds_call_2arg(nds_addr, a, b): canonical wrapper used by the
 *     scene-queue dispatcher.
 *
 * Unknown addresses are logged once per unique value to keep the log
 * useful even when a single missing function fires every frame.
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FNPTR_TABLE_CAP 4096

typedef struct {
    uint32_t nds_addr;
    void *host_fn;
} fnptr_entry_t;

static fnptr_entry_t g_table[FNPTR_TABLE_CAP];
static int g_table_count = 0;
static int g_initialised = 0;

#define UNMAPPED_LOG_CAP 256
static uint32_t g_unmapped_logged[UNMAPPED_LOG_CAP];
static int g_unmapped_count = 0;

void host_fnptr_register(uint32_t nds_addr, void *host_fn)
{
    if (!host_fn) return;
    if (g_table_count >= FNPTR_TABLE_CAP) {
        static int warned = 0;
        if (!warned) {
            warned = 1;
            fprintf(stderr, "[fnptr] table FULL (cap=%d) — dropping 0x%08x\n",
                    FNPTR_TABLE_CAP, (unsigned)nds_addr);
        }
        return;
    }
    /* O(N) dedupe: a duplicate registration replaces the existing slot. */
    for (int i = 0; i < g_table_count; ++i) {
        if (g_table[i].nds_addr == nds_addr) {
            g_table[i].host_fn = host_fn;
            return;
        }
    }
    g_table[g_table_count].nds_addr = nds_addr;
    g_table[g_table_count].host_fn  = host_fn;
    g_table_count++;
}

extern void host_fnptr_register_decompiled(void); /* generated */

void host_fnptr_init(void)
{
    if (g_initialised) return;
    g_initialised = 1;
    host_fnptr_register_decompiled();
    fprintf(stderr, "[fnptr] resolver initialised, %d entries registered\n",
            g_table_count);
}

void *host_fnptr_lookup(uint32_t nds_addr)
{
    if (!g_initialised) host_fnptr_init();
    /* Linear scan — table is < 1024 entries and lookups are rare
     * (vtable[2] dispatch ~once per scene node per frame). If this
     * shows up in profiling, replace with a hash table. */
    for (int i = 0; i < g_table_count; ++i) {
        if (g_table[i].nds_addr == nds_addr) {
            return g_table[i].host_fn;
        }
    }
    return NULL;
}

static void log_unmapped_once(uint32_t nds_addr, const char *ctx)
{
    for (int i = 0; i < g_unmapped_count; ++i) {
        if (g_unmapped_logged[i] == nds_addr) return;
    }
    if (g_unmapped_count < UNMAPPED_LOG_CAP) {
        g_unmapped_logged[g_unmapped_count++] = nds_addr;
    }
    fprintf(stderr, "[fnptr] unmapped 0x%08x (%s)\n",
            (unsigned)nds_addr, ctx ? ctx : "");
    fflush(stderr);
}

typedef void (*fn_2arg_t)(uintptr_t, uintptr_t);

void nds_call_2arg(uint32_t nds_addr, uintptr_t a, uintptr_t b)
{
    void *fn = host_fnptr_lookup(nds_addr);
    if (!fn) {
        log_unmapped_once(nds_addr, "2arg");
        return;
    }
    ((fn_2arg_t)fn)(a, b);
}

typedef void (*fn_1arg_t)(uintptr_t);

void nds_call_1arg(uint32_t nds_addr, uintptr_t a)
{
    void *fn = host_fnptr_lookup(nds_addr);
    if (!fn) {
        log_unmapped_once(nds_addr, "1arg");
        return;
    }
    ((fn_1arg_t)fn)(a);
}

int host_fnptr_count(void)
{
    return g_table_count;
}
