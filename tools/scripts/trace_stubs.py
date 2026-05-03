#!/usr/bin/env python3
"""
trace_stubs.py — temporarily instrument host_undefined_stubs.c to count stub calls.

Usage:
  python tools/scripts/trace_stubs.py instrument   # rewrite stubs to call stub_count
  python tools/scripts/trace_stubs.py revert       # restore from backup
"""
import re, sys, shutil
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
STUBS = ROOT / "pc" / "src" / "host_undefined_stubs.c"
BAK   = STUBS.with_suffix(".c.trace_bak")
TRACE = ROOT / "pc" / "src" / "stub_trace.c"

TRACE_C = r"""/* Auto-generated runtime stub call counter. Remove when done tracing. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STUB_TRACE_BUCKETS 4096
typedef struct { const char *name; unsigned long count; } stub_entry;
static stub_entry g_stub_table[STUB_TRACE_BUCKETS];
static int        g_stub_dumped = 0;

static unsigned long stub_hash(const char *s) {
    unsigned long h = 5381;
    while (*s) h = (h * 33) ^ (unsigned char)*s++;
    return h;
}

void stub_count(const char *name) {
    unsigned long h = stub_hash(name) & (STUB_TRACE_BUCKETS - 1);
    for (int i = 0; i < STUB_TRACE_BUCKETS; i++) {
        unsigned long idx = (h + i) & (STUB_TRACE_BUCKETS - 1);
        if (g_stub_table[idx].name == NULL) {
            g_stub_table[idx].name  = name;
            g_stub_table[idx].count = 1;
            return;
        }
        if (g_stub_table[idx].name == name ||
            strcmp(g_stub_table[idx].name, name) == 0) {
            g_stub_table[idx].count++;
            return;
        }
    }
}

static int cmp_count(const void *a, const void *b) {
    const stub_entry *ea = a, *eb = b;
    if (ea->count > eb->count) return -1;
    if (ea->count < eb->count) return 1;
    return 0;
}

static void stub_trace_dump(void) {
    if (g_stub_dumped) return;
    g_stub_dumped = 1;
    stub_entry sorted[STUB_TRACE_BUCKETS];
    int n = 0;
    for (int i = 0; i < STUB_TRACE_BUCKETS; i++)
        if (g_stub_table[i].name) sorted[n++] = g_stub_table[i];
    qsort(sorted, n, sizeof(stub_entry), cmp_count);
    fprintf(stderr, "[stub-trace] %d distinct stubs called\n", n);
    int show = n < 30 ? n : 30;
    for (int i = 0; i < show; i++)
        fprintf(stderr, "[stub-trace] %6lu  %s\n", sorted[i].count, sorted[i].name);
    fflush(stderr);
}

__attribute__((constructor)) static void stub_trace_init(void) {
    atexit(stub_trace_dump);
}

/* Public hook so the host can flush the trace from a periodic timer
 * (atexit may not fire when the SDL window is killed forcefully). */
void stub_trace_flush(void) {
    /* Reset dumped flag so we can call multiple times. */
    g_stub_dumped = 0;
    stub_trace_dump();
    g_stub_dumped = 0;
}
"""

PAT = re.compile(
    r'^(int|void)\s+((?:FUN_|func_|thunk_)[A-Za-z0-9_]+)\s*\(\s*\)\s*\{\s*(return\s+0;\s*)?\}',
    re.MULTILINE)

def instrument():
    if BAK.exists():
        print(f"backup already exists at {BAK} — revert first")
        sys.exit(1)
    shutil.copy(STUBS, BAK)
    src = STUBS.read_text()
    decl = "extern void stub_count(const char *);\n"
    def repl(m):
        ret, name, retstmt = m.group(1), m.group(2), m.group(3)
        body = f'stub_count("{name}");'
        if ret == "int":
            body += " return 0;"
        return f'{ret} {name}() {{ {body} }}'
    new = PAT.sub(repl, src)
    new = new.replace("#include <stdint.h>", "#include <stdint.h>\n" + decl, 1)
    STUBS.write_text(new)
    TRACE.write_text(TRACE_C)
    n = len(PAT.findall(src))
    print(f"instrumented {n} stubs; wrote {TRACE.name}")

def revert():
    if not BAK.exists():
        print("no backup found")
        sys.exit(1)
    shutil.copy(BAK, STUBS)
    BAK.unlink()
    if TRACE.exists():
        TRACE.unlink()
    print("reverted")

if __name__ == "__main__":
    if len(sys.argv) != 2 or sys.argv[1] not in ("instrument", "revert"):
        print(__doc__); sys.exit(1)
    {"instrument": instrument, "revert": revert}[sys.argv[1]]()
