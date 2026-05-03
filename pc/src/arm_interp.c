/*
 * arm_interp.c — Minimal ARMv5TE interpreter for NDS overlay code.
 *
 * Executes ARM and Thumb instructions from NDS-mapped memory so that
 * un-decompiled overlay functions run on PC without manual porting.
 * When a BL/BLX targets a known decompiled function, calls it natively.
 *
 * Memory access routes through the existing NDS memory emulation:
 *   0x01FF0000-0x01FFFFFF : WRAM (VirtualAlloc'd)
 *   0x02000000-0x02FFFFFF : Main RAM (VirtualAlloc'd)
 *   0x04000000-0x04001FFF : I/O registers (nds_hw_io)
 *   0x05000000+           : Palette (Bank E)
 *   0x06000000+           : VRAM (Banks A-I)
 *   0x07000000+           : OAM
 */
#include "arm_interp.h"
#include "nds_platform.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ---- Configuration ---- */
#define MAX_CYCLES_PER_CALL  5000000  /* safety limit per call */
#define INTERP_STACK_BASE    0x023FF000u
#define INTERP_STACK_SIZE    0x4000u
#define SENTINEL_LR          0xDEAD0000u  /* return address sentinel */

static int      g_enabled = 1;
static uint64_t g_total_cycles = 0;
static int      g_call_count = 0;
static int      g_log_count = 0;
static int      g_verbose_trace = 0;  /* enabled for one call to trace BL targets */
#define MAX_LOG_LINES 2000

void arm_interp_set_enabled(int e) { g_enabled = e; }
int  arm_interp_is_enabled(void)   { return g_enabled; }
uint64_t arm_interp_total_cycles(void) { return g_total_cycles; }
int  arm_interp_call_count(void)   { return g_call_count; }

/* ---- Memory access ---- */
/* These functions route NDS addresses to the correct host memory. */

extern void    nds_reg_write32(uint32_t addr, uint32_t val);
extern void    nds_reg_write16(uint32_t addr, uint16_t val);
extern void    nds_reg_write8(uint32_t addr, uint8_t val);
extern uint32_t nds_reg_read32(uint32_t addr);
extern uint16_t nds_reg_read16(uint32_t addr);
extern uint8_t  nds_reg_read8(uint32_t addr);
extern void   *nds_vram_bank(char bank);
extern int     nds_arm9_ram_is_mapped(void);

/* Heap arena bounds (set by OS_Alloc in heap.c) */
extern uintptr_t g_heap_arena_base;
extern uintptr_t g_heap_arena_end;

static int is_ram_addr(uint32_t addr) {
    if ((addr >= 0x01FF0000u && addr < 0x02000000u) ||
        (addr >= 0x02000000u && addr < 0x03000000u))
        return 1;
    /* Also accept addresses in the OS_Alloc heap arena (0x10000000+) */
    if (g_heap_arena_base && addr >= (uint32_t)g_heap_arena_base &&
        addr < (uint32_t)g_heap_arena_end)
        return 1;
    return 0;
}

static uint8_t mem_read8(uint32_t addr) {
    if (is_ram_addr(addr))
        return *(volatile uint8_t *)(uintptr_t)addr;
    if ((addr >> 24) == 0x04)
        return nds_reg_read8(addr);
    if ((addr >> 24) == 0x05) {
        void *e = nds_vram_bank('E');
        if (e) return ((uint8_t*)e)[addr & 0xFFFF];
    }
    if ((addr >> 24) == 0x06) {
        uint32_t off = addr & 0xFFFFF;
        if (off < 128*1024) {
            void *a = nds_vram_bank('A');
            if (a) return ((uint8_t*)a)[off];
        } else if (off < 256*1024) {
            void *b = nds_vram_bank('B');
            if (b) return ((uint8_t*)b)[off - 128*1024];
        }
    }
    if ((addr >> 24) == 0x07) {
        void *e = nds_vram_bank('E');
        if (e) return ((uint8_t*)e)[0x800 + (addr & 0x7FF)];
    }
    return 0;
}

static uint16_t mem_read16(uint32_t addr) {
    addr &= ~1u;
    if (is_ram_addr(addr))
        return *(volatile uint16_t *)(uintptr_t)addr;
    if ((addr >> 24) == 0x04)
        return nds_reg_read16(addr);
    if ((addr >> 24) == 0x05) {
        void *e = nds_vram_bank('E');
        if (e) { uint32_t o = addr & 0xFFFF; return ((uint8_t*)e)[o] | (((uint8_t*)e)[o+1] << 8); }
    }
    if ((addr >> 24) == 0x06) {
        uint32_t off = addr & 0xFFFFF;
        if (off < 128*1024) {
            void *a = nds_vram_bank('A');
            if (a) return ((uint8_t*)a)[off] | (((uint8_t*)a)[off+1] << 8);
        } else if (off < 256*1024) {
            void *b = nds_vram_bank('B');
            uint32_t o2 = off - 128*1024;
            if (b) return ((uint8_t*)b)[o2] | (((uint8_t*)b)[o2+1] << 8);
        }
    }
    if ((addr >> 24) == 0x07) {
        void *e = nds_vram_bank('E');
        if (e) { uint32_t o = 0x800 + (addr & 0x7FF); return ((uint8_t*)e)[o] | (((uint8_t*)e)[o+1] << 8); }
    }
    return 0;
}

static uint32_t mem_read32(uint32_t addr) {
    addr &= ~3u;
    if (is_ram_addr(addr))
        return *(volatile uint32_t *)(uintptr_t)addr;
    if ((addr >> 24) == 0x04)
        return nds_reg_read32(addr);
    if ((addr >> 24) == 0x05 || (addr >> 24) == 0x06 || (addr >> 24) == 0x07) {
        return (uint32_t)mem_read16(addr) | ((uint32_t)mem_read16(addr + 2) << 16);
    }
    return 0;
}

static void mem_write8(uint32_t addr, uint8_t val) {
    if (is_ram_addr(addr)) {
        *(volatile uint8_t *)(uintptr_t)addr = val;
        return;
    }
    if ((addr >> 24) == 0x04) { nds_reg_write8(addr, val); return; }
    if ((addr >> 24) == 0x05) {
        void *e = nds_vram_bank('E');
        if (e) ((uint8_t*)e)[addr & 0xFFFF] = val;
        return;
    }
    if ((addr >> 24) == 0x06) {
        uint32_t off = addr & 0xFFFFF;
        if (off < 128*1024) { void *a = nds_vram_bank('A'); if (a) ((uint8_t*)a)[off] = val; }
        else if (off < 256*1024) { void *b = nds_vram_bank('B'); if (b) ((uint8_t*)b)[off-128*1024] = val; }
        return;
    }
    if ((addr >> 24) == 0x07) {
        void *e = nds_vram_bank('E');
        if (e) ((uint8_t*)e)[0x800 + (addr & 0x7FF)] = val;
        return;
    }
}

static void mem_write16(uint32_t addr, uint16_t val) {
    addr &= ~1u;
    if (is_ram_addr(addr)) {
        *(volatile uint16_t *)(uintptr_t)addr = val;
        return;
    }
    if ((addr >> 24) == 0x04) { nds_reg_write16(addr, val); return; }
    if ((addr >> 24) == 0x05) {
        void *e = nds_vram_bank('E');
        if (e) { uint32_t o = addr & 0xFFFF; ((uint8_t*)e)[o] = val & 0xFF; ((uint8_t*)e)[o+1] = val >> 8; }
        return;
    }
    if ((addr >> 24) == 0x06) {
        uint32_t off = addr & 0xFFFFF;
        if (off < 128*1024) {
            void *a = nds_vram_bank('A');
            if (a) { ((uint8_t*)a)[off] = val & 0xFF; ((uint8_t*)a)[off+1] = val >> 8; }
        } else if (off < 256*1024) {
            void *b = nds_vram_bank('B');
            if (b) { uint32_t o2 = off-128*1024; ((uint8_t*)b)[o2] = val & 0xFF; ((uint8_t*)b)[o2+1] = val >> 8; }
        }
        return;
    }
    if ((addr >> 24) == 0x07) {
        void *e = nds_vram_bank('E');
        if (e) { uint32_t o = 0x800 + (addr & 0x7FF); ((uint8_t*)e)[o] = val & 0xFF; ((uint8_t*)e)[o+1] = val >> 8; }
        return;
    }
}

static void mem_write32(uint32_t addr, uint32_t val) {
    addr &= ~3u;
    /* Watchpoint: catch writes to title node's next pointer (0x0230200C) */
    if (addr == 0x0230200Cu) {
        static int s_watch = 0;
        if (s_watch < 10) {
            s_watch++;
            fprintf(stderr, "[WATCH-0C] write 0x%08X to 0x0230200C (interp)\n",
                    (unsigned)val);
            fflush(stderr);
        }
    }
    if (is_ram_addr(addr)) {
        *(volatile uint32_t *)(uintptr_t)addr = val;
        return;
    }
    if ((addr >> 24) == 0x04) { nds_reg_write32(addr, val); return; }
    mem_write16(addr, (uint16_t)(val & 0xFFFF));
    mem_write16(addr + 2, (uint16_t)(val >> 16));
}

/* ---- Native function bridge ---- */
extern void *host_fnptr_lookup(uint32_t nds_addr);

typedef uint32_t (*native_fn_t)(uint32_t, uint32_t, uint32_t, uint32_t);
typedef void (*native_void_t)(uint32_t, uint32_t, uint32_t, uint32_t);

/* ARM9 base range — these functions are already compiled as native C.
 * If the fnptr table doesn't have an entry, we should NOT interpret the
 * ARM binary (it would be extremely slow and might infinite-loop).
 * Only overlay addresses should be interpreted. */
#define ARM9_BASE_START  0x02004000u
#define ARM9_BASE_END    0x020563D8u

/* Try to call a native function. Returns 1 if found and called, 0 if not.
 * For arm9-base addresses not in the fnptr table, returns 1 (skip) with r0=0. */
/* ---------- Native call dispatch ---------- */
extern int host_fnptr_is_safe_native(uint32_t addr);

static int try_native_call(ArmCpu *cpu, uint32_t target) {
    uint32_t addr = target & ~1u;

    /* Force-interpret list: functions that MUST be interpreted because
     * their C decompilation loses parameters (>4 stack args, or sub-calls
     * with zero-arg Ghidra artifacts that actually pass regs in ARM),
     * or they double-deref NDS pointers as u32** which reads 8 bytes
     * on 64-bit host instead of 4. */
    static const uint32_t force_interpret[] = {
        0x0201FA50u, /* FUN_0201fa50 — scene constructor, 11 params */
        0x02015C38u, /* FUN_02015c38 — anim manager constructor, 11 params + NDS ptr DATs */
    };
    for (int i = 0; i < (int)(sizeof(force_interpret)/sizeof(force_interpret[0])); i++) {
        if (addr == force_interpret[i]) {
            return 0;  /* not handled — interpreter will execute */
        }
    }

    /* Force-interpret ranges: whole subsystems where DAT globals are used
     * as NDS pointers and double-dereferenced (u32**, u16**) — breaks on
     * 64-bit host because pointer reads get garbage upper 4 bytes. */
    static const struct { uint32_t lo; uint32_t hi; } force_ranges[] = {
        { 0x0200D000u, 0x0200E100u }, /* resource/tile copy cluster */
        { 0x02020F00u, 0x02023000u }, /* scene/entity/sprite engine */
        { 0x02015C00u, 0x02016000u }, /* anim manager */
    };
    for (int i = 0; i < (int)(sizeof(force_ranges)/sizeof(force_ranges[0])); i++) {
        if (addr >= force_ranges[i].lo && addr < force_ranges[i].hi) {
            return 0;
        }
    }

    /* Check fnptr table — includes both auto-generated and manual overrides */
    void *fn = host_fnptr_lookup(addr);
    if (fn) {
        /* Log native calls to diagnose crashes from indirect NDS fn ptrs */
        static int s_native_log = 0;
        if (s_native_log < 500) {
            s_native_log++;
            fprintf(stderr, "[arm-interp] native 0x%08X (r0=0x%X r1=0x%X r2=0x%X r3=0x%X)\n",
                    addr, cpu->r[0], cpu->r[1], cpu->r[2], cpu->r[3]);
            fflush(stderr);
        }
        /* Call native function with r0-r3 as arguments */
        uint32_t ret = ((native_fn_t)fn)(cpu->r[0], cpu->r[1], cpu->r[2], cpu->r[3]);
        cpu->r[0] = ret;
        return 1;
    }

    return 0;  /* not handled — interpreter should execute the code */
}

/* ---- Condition check ---- */
static int check_cond(uint32_t cpsr, uint32_t cond) {
    int N = !!(cpsr & CPSR_N);
    int Z = !!(cpsr & CPSR_Z);
    int C = !!(cpsr & CPSR_C);
    int V = !!(cpsr & CPSR_V);
    switch (cond) {
        case 0x0: return Z;             /* EQ */
        case 0x1: return !Z;            /* NE */
        case 0x2: return C;             /* CS/HS */
        case 0x3: return !C;            /* CC/LO */
        case 0x4: return N;             /* MI */
        case 0x5: return !N;            /* PL */
        case 0x6: return V;             /* VS */
        case 0x7: return !V;            /* VC */
        case 0x8: return C && !Z;       /* HI */
        case 0x9: return !C || Z;       /* LS */
        case 0xA: return N == V;        /* GE */
        case 0xB: return N != V;        /* LT */
        case 0xC: return !Z && (N==V);  /* GT */
        case 0xD: return Z || (N!=V);   /* LE */
        case 0xE: return 1;             /* AL */
        case 0xF: return 1;             /* unconditional (NV/special) */
    }
    return 0;
}

/* ---- Barrel shifter ---- */
static uint32_t barrel_shift(uint32_t val, int type, int amount, int *carry_out, uint32_t old_c) {
    *carry_out = (old_c != 0);
    if (amount == 0) {
        /* Special cases for shift-by-0 */
        switch (type) {
            case 0: return val; /* LSL #0 = no shift */
            case 1: *carry_out = (val >> 31) & 1; return 0; /* LSR #32 */
            case 2: *carry_out = (val >> 31) & 1; return (int32_t)val >> 31; /* ASR #32 */
            case 3: *carry_out = val & 1; return (old_c ? 0x80000000u : 0) | (val >> 1); /* RRX */
        }
    }
    switch (type) {
        case 0: /* LSL */
            if (amount >= 32) { *carry_out = (amount == 32) ? (val & 1) : 0; return 0; }
            *carry_out = (val >> (32 - amount)) & 1;
            return val << amount;
        case 1: /* LSR */
            if (amount >= 32) { *carry_out = (amount == 32) ? ((val >> 31) & 1) : 0; return 0; }
            *carry_out = (val >> (amount - 1)) & 1;
            return val >> amount;
        case 2: /* ASR */
            if (amount >= 32) { *carry_out = (val >> 31) & 1; return (int32_t)val >> 31; }
            *carry_out = (val >> (amount - 1)) & 1;
            return (uint32_t)((int32_t)val >> amount);
        case 3: /* ROR */
            amount &= 31;
            if (amount == 0) return val;
            val = (val >> amount) | (val << (32 - amount));
            *carry_out = (val >> 31) & 1;
            return val;
    }
    return val;
}

/* ---- CPSR flag update helpers ---- */
static void set_nz(ArmCpu *cpu, uint32_t result) {
    cpu->cpsr &= ~(CPSR_N | CPSR_Z);
    if (result == 0) cpu->cpsr |= CPSR_Z;
    if (result & 0x80000000u) cpu->cpsr |= CPSR_N;
}

static void set_nzc(ArmCpu *cpu, uint32_t result, int carry) {
    set_nz(cpu, result);
    if (carry) cpu->cpsr |= CPSR_C;
    else cpu->cpsr &= ~CPSR_C;
}

static void set_add_flags(ArmCpu *cpu, uint32_t a, uint32_t b, uint64_t result64) {
    uint32_t result = (uint32_t)result64;
    set_nz(cpu, result);
    cpu->cpsr &= ~(CPSR_C | CPSR_V);
    if (result64 > 0xFFFFFFFFu) cpu->cpsr |= CPSR_C;
    if (((a ^ result) & (b ^ result)) & 0x80000000u) cpu->cpsr |= CPSR_V;
}

static void set_sub_flags(ArmCpu *cpu, uint32_t a, uint32_t b, uint64_t result64) {
    uint32_t result = (uint32_t)result64;
    set_nz(cpu, result);
    cpu->cpsr &= ~(CPSR_C | CPSR_V);
    if (a >= b) cpu->cpsr |= CPSR_C;  /* borrow = NOT carry for SUB */
    if (((a ^ b) & (a ^ result)) & 0x80000000u) cpu->cpsr |= CPSR_V;
}

/* ---- ARM instruction execution ---- */
static int exec_arm(ArmCpu *cpu) {
    uint32_t pc = cpu->r[15];
    uint32_t inst = mem_read32(pc);
    cpu->r[15] = pc + 4;

    /* Targeted trace: state 2 fallback bit manipulation (one-shot) */
    static int s_state2_traced = 0;
    if (!s_state2_traced && pc >= 0x02076AE4u && pc <= 0x02076B08u) {
        fprintf(stderr, "[trace-s2] pc=0x%08X inst=0x%08X r0=0x%X r1=0x%X r6=0x%X r7=0x%X r10=0x%X cpsr=0x%X\n",
                pc, inst, cpu->r[0], cpu->r[1], cpu->r[6], cpu->r[7], cpu->r[10], cpu->cpsr);
        fflush(stderr);
        if (pc == 0x02076B08u) s_state2_traced = 1;
    }
    /* Also trace state 3 decision */
    if (!s_state2_traced && pc >= 0x02076B6Cu && pc <= 0x02076BB8u) {
        fprintf(stderr, "[trace-s3] pc=0x%08X inst=0x%08X r0=0x%X r4=0x%X cpsr=0x%X\n",
                pc, inst, cpu->r[0], cpu->r[4], cpu->cpsr);
        fflush(stderr);
        if (pc == 0x02076BB0u) s_state2_traced = 1;
    }

    uint32_t cond = (inst >> 28) & 0xF;
    if (!check_cond(cpu->cpsr, cond)) return 0;

    uint32_t op_class = (inst >> 25) & 0x7;

    /* ---- Branch (B/BL) ---- */
    if (op_class == 5) {
        int link = (inst >> 24) & 1;
        int32_t offset = (int32_t)(inst << 8) >> 6;  /* sign-extend 24-bit, shift left 2 */
        uint32_t target = pc + 8 + (uint32_t)offset;

        if (link) cpu->r[14] = pc + 4;

        /* Try native call first */
        if (link && try_native_call(cpu, target)) {
            /* Native function returned, continue after BL */
            return 0;
        }

        /* Verbose trace: log non-native BL targets */
        if (link && g_verbose_trace) {
            fprintf(stderr, "[trace] BL pc=0x%08X -> 0x%08X r0=0x%X r1=0x%X\n",
                    pc, target, cpu->r[0], cpu->r[1]);
        }

        cpu->r[15] = target;
        return 0;
    }

    /* ---- BX / BLX (register) ---- */
    if ((inst & 0x0FFFFFF0) == 0x012FFF10) {
        int rm = inst & 0xF;
        uint32_t target = cpu->r[rm];

        /* BLX variant */
        int is_blx = ((inst >> 4) & 0xF) == 3;
        if (is_blx) cpu->r[14] = pc + 4;

        /* Check for return */
        if ((target & ~1u) == SENTINEL_LR) {
            cpu->halted = 1;
            return 0;
        }

        /* Try native call only for BLX (not plain BX LR which is a return) */
        if (is_blx && try_native_call(cpu, target & ~1u)) {
            return 0;
        }

        /* For BX to a non-LR register that isn't a return, try native call
         * (e.g. BX R3 used as indirect function call) */
        if (!is_blx && rm != 14 && try_native_call(cpu, target & ~1u)) {
            return 0;
        }

        if (target & 1) {
            cpu->cpsr |= CPSR_T;  /* switch to Thumb */
            cpu->r[15] = target & ~1u;
        } else {
            cpu->cpsr &= ~CPSR_T;
            cpu->r[15] = target & ~3u;
        }
        return 0;
    }

    /* ---- BLX immediate (cond=0xF) ---- */
    if (cond == 0xF && op_class == 5) {
        int H = (inst >> 24) & 1;
        int32_t offset = (int32_t)(inst << 8) >> 6;
        uint32_t target = pc + 8 + (uint32_t)offset + (H ? 2 : 0);
        cpu->r[14] = pc + 4;
        if (try_native_call(cpu, target)) return 0;
        cpu->cpsr |= CPSR_T;
        cpu->r[15] = target;
        return 0;
    }

    /* ---- Data Processing ---- */
    if (op_class == 0 || op_class == 1) {
        /* Check for multiply first (op_class==0, bits 7:4 = 1001) */
        if (op_class == 0 && (inst & 0xF0) == 0x90) {
            /* Could be MUL/MLA/UMULL/etc or SWP or misc */
            uint32_t op2 = (inst >> 20) & 0xFF;

            /* MUL: 0000 00AS */
            if ((op2 & 0xFC) == 0x00) {
                int S = (op2 >> 0) & 1;
                int A = (op2 >> 1) & 1;
                int rd = (inst >> 16) & 0xF;
                int rn = (inst >> 12) & 0xF;
                int rs = (inst >> 8) & 0xF;
                int rm = inst & 0xF;
                uint32_t result = cpu->r[rm] * cpu->r[rs];
                if (A) result += cpu->r[rn];
                cpu->r[rd] = result;
                if (S) set_nz(cpu, result);
                return 0;
            }

            /* UMULL/UMLAL/SMULL/SMLAL: 0000 1UAS */
            if ((op2 & 0xF8) == 0x08) {
                int S = (op2 >> 0) & 1;
                int A = (op2 >> 1) & 1;
                int U = (op2 >> 2) & 1;  /* 0=unsigned, 1=signed */
                int rdhi = (inst >> 16) & 0xF;
                int rdlo = (inst >> 12) & 0xF;
                int rs = (inst >> 8) & 0xF;
                int rm = inst & 0xF;
                uint64_t result;
                if (U) {
                    result = (uint64_t)((int64_t)(int32_t)cpu->r[rm] * (int64_t)(int32_t)cpu->r[rs]);
                } else {
                    result = (uint64_t)cpu->r[rm] * (uint64_t)cpu->r[rs];
                }
                if (A) result += ((uint64_t)cpu->r[rdhi] << 32) | cpu->r[rdlo];
                cpu->r[rdlo] = (uint32_t)result;
                cpu->r[rdhi] = (uint32_t)(result >> 32);
                if (S) set_nz(cpu, cpu->r[rdhi]);
                return 0;
            }

            /* SWP/SWPB: 0001 0B00 */
            if ((op2 & 0xFB) == 0x10) {
                int B = (op2 >> 2) & 1;
                int rn = (inst >> 16) & 0xF;
                int rd = (inst >> 12) & 0xF;
                int rm = inst & 0xF;
                uint32_t addr = cpu->r[rn];
                if (B) {
                    uint8_t tmp = mem_read8(addr);
                    mem_write8(addr, (uint8_t)cpu->r[rm]);
                    cpu->r[rd] = tmp;
                } else {
                    uint32_t tmp = mem_read32(addr);
                    mem_write32(addr, cpu->r[rm]);
                    cpu->r[rd] = tmp;
                }
                return 0;
            }

            /* Halfword load/store (misc): handled below */
        }

        /* Halfword / signed load/store: bits 7:4 = 1011, 1101, 1111 */
        if (op_class == 0 && ((inst & 0x90) == 0x90) && ((inst & 0x60) != 0)) {
            int P = (inst >> 24) & 1;
            int U = (inst >> 23) & 1;
            int I = (inst >> 22) & 1;
            int W = (inst >> 21) & 1;
            int L = (inst >> 20) & 1;
            int rn = (inst >> 16) & 0xF;
            int rd = (inst >> 12) & 0xF;
            int SH = (inst >> 5) & 3;
            uint32_t offset;
            if (I) {
                offset = ((inst >> 4) & 0xF0) | (inst & 0xF);
            } else {
                offset = cpu->r[inst & 0xF];
            }
            uint32_t base = cpu->r[rn];
            uint32_t addr = P ? (U ? base + offset : base - offset) : base;

            if (L) {
                switch (SH) {
                    case 1: cpu->r[rd] = mem_read16(addr); break; /* LDRH */
                    case 2: cpu->r[rd] = (uint32_t)(int32_t)(int8_t)mem_read8(addr); break; /* LDRSB */
                    case 3: cpu->r[rd] = (uint32_t)(int32_t)(int16_t)mem_read16(addr); break; /* LDRSH */
                }
            } else {
                if (SH == 1) mem_write16(addr, (uint16_t)cpu->r[rd]); /* STRH */
                /* LDRD/STRD (SH=2,3 with L=0) — rare, skip for now */
            }

            if (!P) {
                addr = U ? base + offset : base - offset;
            }
            if (W || !P) cpu->r[rn] = addr;
            return 0;
        }

        /* Standard data processing */
        int I = (inst >> 25) & 1;
        int opcode = (inst >> 21) & 0xF;
        int S = (inst >> 20) & 1;
        int rn = (inst >> 16) & 0xF;
        int rd = (inst >> 12) & 0xF;

        uint32_t op2;
        int shifter_carry;
        uint32_t old_c = (cpu->cpsr & CPSR_C) ? 1 : 0;

        if (I) {
            /* Immediate: 8-bit value rotated right by 2*rot */
            uint32_t imm = inst & 0xFF;
            int rot = ((inst >> 8) & 0xF) * 2;
            if (rot) {
                op2 = (imm >> rot) | (imm << (32 - rot));
                shifter_carry = (op2 >> 31) & 1;
            } else {
                op2 = imm;
                shifter_carry = old_c;
            }
        } else {
            int rm = inst & 0xF;
            uint32_t rm_val = cpu->r[rm];
            if (rm == 15) rm_val += 8;  /* PC + 8 for register operand */
            int shift_type = (inst >> 5) & 3;
            int shift_amount;
            if (inst & 0x10) {
                /* Register-specified shift */
                int rs = (inst >> 8) & 0xF;
                shift_amount = cpu->r[rs] & 0xFF;
                if (shift_amount == 0) {
                    op2 = rm_val;
                    shifter_carry = old_c;
                } else {
                    op2 = barrel_shift(rm_val, shift_type, shift_amount, &shifter_carry, old_c);
                }
            } else {
                shift_amount = (inst >> 7) & 0x1F;
                op2 = barrel_shift(rm_val, shift_type, shift_amount, &shifter_carry, old_c);
            }
        }

        uint32_t rn_val = cpu->r[rn];
        if (rn == 15) rn_val = pc + 8;
        uint32_t result;
        uint64_t result64;
        int write_rd = 1;

        switch (opcode) {
            case 0x0: result = rn_val & op2; if (S) set_nzc(cpu, result, shifter_carry); break; /* AND */
            case 0x1: result = rn_val ^ op2; if (S) set_nzc(cpu, result, shifter_carry); break; /* EOR */
            case 0x2: /* SUB */
                result64 = (uint64_t)rn_val - (uint64_t)op2;
                result = (uint32_t)result64;
                if (S) set_sub_flags(cpu, rn_val, op2, result64);
                break;
            case 0x3: /* RSB */
                result64 = (uint64_t)op2 - (uint64_t)rn_val;
                result = (uint32_t)result64;
                if (S) set_sub_flags(cpu, op2, rn_val, result64);
                break;
            case 0x4: /* ADD */
                result64 = (uint64_t)rn_val + (uint64_t)op2;
                result = (uint32_t)result64;
                if (S) set_add_flags(cpu, rn_val, op2, result64);
                break;
            case 0x5: /* ADC */
                result64 = (uint64_t)rn_val + (uint64_t)op2 + old_c;
                result = (uint32_t)result64;
                if (S) set_add_flags(cpu, rn_val, op2 + old_c, result64);
                break;
            case 0x6: /* SBC */
                result64 = (uint64_t)rn_val - (uint64_t)op2 - (1 - old_c);
                result = (uint32_t)result64;
                if (S) set_sub_flags(cpu, rn_val, op2 + (1 - old_c), result64);
                break;
            case 0x7: /* RSC */
                result64 = (uint64_t)op2 - (uint64_t)rn_val - (1 - old_c);
                result = (uint32_t)result64;
                if (S) set_sub_flags(cpu, op2, rn_val + (1 - old_c), result64);
                break;
            case 0x8: result = rn_val & op2; set_nzc(cpu, result, shifter_carry); write_rd = 0; break; /* TST */
            case 0x9: result = rn_val ^ op2; set_nzc(cpu, result, shifter_carry); write_rd = 0; break; /* TEQ */
            case 0xA: /* CMP */
                result64 = (uint64_t)rn_val - (uint64_t)op2;
                result = (uint32_t)result64;
                set_sub_flags(cpu, rn_val, op2, result64);
                write_rd = 0;
                break;
            case 0xB: /* CMN */
                result64 = (uint64_t)rn_val + (uint64_t)op2;
                result = (uint32_t)result64;
                set_add_flags(cpu, rn_val, op2, result64);
                write_rd = 0;
                break;
            case 0xC: result = rn_val | op2; if (S) set_nzc(cpu, result, shifter_carry); break; /* ORR */
            case 0xD: result = op2;          if (S) set_nzc(cpu, result, shifter_carry); break; /* MOV */
            case 0xE: result = rn_val & ~op2; if (S) set_nzc(cpu, result, shifter_carry); break; /* BIC */
            case 0xF: result = ~op2;          if (S) set_nzc(cpu, result, shifter_carry); break; /* MVN */
            default: result = 0; break;
        }

        if (write_rd) {
            cpu->r[rd] = result;
            if (rd == 15) {
                /* Writing to PC — branch */
                if (S) {
                    /* MOVS PC, LR — return from exception (restore CPSR from SPSR) */
                }
            }
        }
        return 0;
    }

    /* ---- Load/Store word/byte ---- */
    if (op_class == 2 || op_class == 3) {
        int I = (inst >> 25) & 1;
        int P = (inst >> 24) & 1;
        int U = (inst >> 23) & 1;
        int B = (inst >> 22) & 1;
        int W = (inst >> 21) & 1;
        int L = (inst >> 20) & 1;
        int rn = (inst >> 16) & 0xF;
        int rd = (inst >> 12) & 0xF;

        uint32_t offset;
        if (!I) {
            offset = inst & 0xFFF;
        } else {
            int rm = inst & 0xF;
            int shift_type = (inst >> 5) & 3;
            int shift_amount = (inst >> 7) & 0x1F;
            int carry_dummy;
            offset = barrel_shift(cpu->r[rm], shift_type, shift_amount, &carry_dummy,
                                  (cpu->cpsr & CPSR_C) ? 1 : 0);
        }

        uint32_t base = (rn == 15) ? (pc + 8) : cpu->r[rn];
        uint32_t addr = P ? (U ? base + offset : base - offset) : base;

        if (L) {
            if (B) {
                cpu->r[rd] = mem_read8(addr);
            } else {
                cpu->r[rd] = mem_read32(addr);
                /* Handle misaligned LDR: rotate */
                uint32_t misalign = addr & 3;
                if (misalign) {
                    cpu->r[rd] = (cpu->r[rd] >> (misalign * 8)) |
                                 (cpu->r[rd] << (32 - misalign * 8));
                }
            }
            /* Verbose trace: log reads from pad state area */
            if (g_verbose_trace && addr >= 0x0205FF90u && addr <= 0x02060400u) {
                fprintf(stderr, "[trace] LDR r%d, [0x%08X] = 0x%X  (pad/state area)\n",
                        rd, addr, cpu->r[rd]);
            }
            if (rd == 15) {
                /* LDR PC — branch */
                if (cpu->r[15] & 1) {
                    cpu->cpsr |= CPSR_T;
                    cpu->r[15] &= ~1u;
                }
            }
        } else {
            uint32_t val = cpu->r[rd];
            if (rd == 15) val = pc + 12;
            if (B) {
                mem_write8(addr, (uint8_t)val);
            } else {
                mem_write32(addr, val);
            }
            /* Watch for writes to title object field_2C */
            if (addr == 0x0230202Cu) {
                fprintf(stderr, "[WATCH] write to title.field_2C: val=0x%X pc=0x%08X\n", val, pc);
            }
        }

        if (!P) {
            addr = U ? base + offset : base - offset;
        }
        if (W || !P) {
            if (rn != 15) cpu->r[rn] = addr;
        }
        return 0;
    }

    /* ---- Load/Store Multiple (LDM/STM) ---- */
    if (op_class == 4) {
        int P = (inst >> 24) & 1;
        int U = (inst >> 23) & 1;
        int S = (inst >> 22) & 1;  /* user mode regs (ignored in user mode) */
        int W = (inst >> 21) & 1;
        int L = (inst >> 20) & 1;
        int rn = (inst >> 16) & 0xF;
        uint16_t reg_list = inst & 0xFFFF;

        (void)S;  /* We're always in user/system mode for game code */

        uint32_t base = cpu->r[rn];
        int count = 0;
        for (int i = 0; i < 16; i++) {
            if (reg_list & (1 << i)) count++;
        }

        uint32_t addr;
        if (U) {
            addr = P ? base + 4 : base;  /* IB / IA */
        } else {
            addr = P ? base - count * 4 : base - count * 4 + 4;  /* DB / DA */
        }

        for (int i = 0; i < 16; i++) {
            if (!(reg_list & (1 << i))) continue;
            if (L) {
                cpu->r[i] = mem_read32(addr);
                if (i == 15) {
                    if (cpu->r[15] & 1) {
                        cpu->cpsr |= CPSR_T;
                        cpu->r[15] &= ~1u;
                    }
                }
            } else {
                uint32_t val = cpu->r[i];
                if (i == 15) val = pc + 12;
                mem_write32(addr, val);
            }
            addr += 4;
        }

        if (W) {
            if (U) cpu->r[rn] = base + count * 4;
            else   cpu->r[rn] = base - count * 4;
        }

        /* Check if we just loaded LR from stack and it's our sentinel */
        if (L && (reg_list & (1 << 15))) {
            if ((cpu->r[15] & ~1u) == SENTINEL_LR) {
                cpu->halted = 1;
            }
        }
        return 0;
    }

    /* ---- SWI (Software Interrupt) ---- */
    if (op_class == 7 && (inst & 0x01000000)) {
        uint32_t swi_num = inst & 0xFFFFFF;
        /* Route SWI to our BIOS stubs */
        switch (swi_num & 0xFF) {
            case 0x00: /* SoftReset — no-op in interpreter */
                break;
            case 0x04: /* IntrWait — no-op (we don't emulate IRQs) */
                break;
            case 0x05: /* VBlankIntrWait — just return */
                break;
            case 0x06: /* Div */
                if (cpu->r[1] != 0) {
                    int32_t num = (int32_t)cpu->r[0];
                    int32_t den = (int32_t)cpu->r[1];
                    cpu->r[0] = (uint32_t)(num / den);
                    cpu->r[1] = (uint32_t)(num % den);
                    cpu->r[3] = (uint32_t)(num < 0 ? -num : num) / (uint32_t)(den < 0 ? -den : den);
                }
                break;
            case 0x09: /* Sqrt */
                {
                    uint32_t val = cpu->r[0];
                    uint32_t root = 0;
                    for (uint32_t bit = 1u << 30; bit; bit >>= 2) {
                        uint32_t trial = root + bit;
                        if (trial <= val) { val -= trial; root = (root >> 1) + bit; }
                        else root >>= 1;
                    }
                    cpu->r[0] = root;
                }
                break;
            case 0x0B: /* CpuSet */
            case 0x0C: /* CpuFastSet */
                {
                    uint32_t src = cpu->r[0];
                    uint32_t dst = cpu->r[1];
                    uint32_t cnt = cpu->r[2];
                    int fill = (cnt >> 24) & 1;
                    int words = (cnt & 0x1FFFFF);
                    if ((cnt >> 26) & 1) words *= 8; /* 32-byte blocks for CpuFastSet */
                    uint32_t val = fill ? mem_read32(src) : 0;
                    for (int i = 0; i < words; i++) {
                        if (!fill) val = mem_read32(src + i * 4);
                        mem_write32(dst + i * 4, val);
                    }
                }
                break;
            case 0x11: /* LZ77UnCompWram/Vram */
                {
                    extern void arm_swi_11_lz77_decomp(const void *src, void *dst);
                    uint32_t src_addr = cpu->r[0];
                    uint32_t dst_addr = cpu->r[1];
                    if (is_ram_addr(src_addr) && is_ram_addr(dst_addr)) {
                        arm_swi_11_lz77_decomp(
                            (const void *)(uintptr_t)src_addr,
                            (void *)(uintptr_t)dst_addr);
                    }
                }
                break;
            default:
                if (g_log_count < MAX_LOG_LINES) {
                    g_log_count++;
                    fprintf(stderr, "[arm-interp] unhandled SWI 0x%02X at PC=0x%08X\n",
                            (unsigned)(swi_num & 0xFF), pc);
                }
                break;
        }
        return 0;
    }

    /* ---- MRS (read CPSR/SPSR) ---- */
    if ((inst & 0x0FBF0FFF) == 0x010F0000) {
        int rd = (inst >> 12) & 0xF;
        int R = (inst >> 22) & 1;
        cpu->r[rd] = R ? 0 : cpu->cpsr;  /* SPSR = 0 (no exception mode) */
        return 0;
    }

    /* ---- MSR (write CPSR/SPSR) ---- */
    if ((inst & 0x0DB0F000) == 0x0120F000) {
        uint32_t val;
        if (inst & (1 << 25)) {
            uint32_t imm = inst & 0xFF;
            int rot = ((inst >> 8) & 0xF) * 2;
            val = rot ? ((imm >> rot) | (imm << (32 - rot))) : imm;
        } else {
            val = cpu->r[inst & 0xF];
        }
        uint32_t mask = 0;
        if (inst & (1 << 19)) mask |= 0xFF000000u;  /* flags field */
        if (inst & (1 << 16)) mask |= 0x000000FFu;  /* control field */
        cpu->cpsr = (cpu->cpsr & ~mask) | (val & mask);
        return 0;
    }

    /* ---- CLZ ---- */
    if ((inst & 0x0FFF0FF0) == 0x016F0F10) {
        int rd = (inst >> 12) & 0xF;
        int rm = inst & 0xF;
        uint32_t val = cpu->r[rm];
        if (val == 0) { cpu->r[rd] = 32; }
        else {
            int n = 0;
            if (!(val & 0xFFFF0000u)) { n += 16; val <<= 16; }
            if (!(val & 0xFF000000u)) { n += 8;  val <<= 8; }
            if (!(val & 0xF0000000u)) { n += 4;  val <<= 4; }
            if (!(val & 0xC0000000u)) { n += 2;  val <<= 2; }
            if (!(val & 0x80000000u)) { n += 1; }
            cpu->r[rd] = n;
        }
        return 0;
    }

    /* ---- Coprocessor (MCR/MRC) — stub ---- */
    if ((inst & 0x0F000010) == 0x0E000010) {
        /* Ignore CP15 reads/writes — game uses these for cache/TCM config */
        int L = (inst >> 20) & 1;
        if (L) {
            int rd = (inst >> 12) & 0xF;
            cpu->r[rd] = 0;  /* MRC: return 0 */
        }
        return 0;
    }

    /* ---- Unhandled ---- */
    if (g_log_count < MAX_LOG_LINES) {
        g_log_count++;
        fprintf(stderr, "[arm-interp] unhandled ARM inst 0x%08X at PC=0x%08X\n",
                inst, pc);
    }
    return -1;
}

/* ---- Thumb instruction execution ---- */
static int exec_thumb(ArmCpu *cpu) {
    uint32_t pc = cpu->r[15];
    uint16_t inst = mem_read16(pc);
    cpu->r[15] = pc + 2;

    /* Format 1: Move shifted register (LSL/LSR/ASR) */
    if ((inst >> 13) == 0) {
        int op = (inst >> 11) & 3;
        int offset = (inst >> 6) & 0x1F;
        int rs = (inst >> 3) & 7;
        int rd = inst & 7;
        int carry;
        uint32_t old_c = (cpu->cpsr & CPSR_C) ? 1 : 0;
        uint32_t result = barrel_shift(cpu->r[rs], op, offset, &carry, old_c);
        cpu->r[rd] = result;
        set_nzc(cpu, result, carry);
        return 0;
    }

    /* Format 2: Add/Sub */
    if ((inst >> 11) == 3) {
        int I = (inst >> 10) & 1;
        int op = (inst >> 9) & 1;
        int rn_or_imm = (inst >> 6) & 7;
        int rs = (inst >> 3) & 7;
        int rd = inst & 7;
        uint32_t operand = I ? rn_or_imm : cpu->r[rn_or_imm];
        uint64_t result64;
        if (op) {
            result64 = (uint64_t)cpu->r[rs] - (uint64_t)operand;
            cpu->r[rd] = (uint32_t)result64;
            set_sub_flags(cpu, cpu->r[rs], operand, result64);
        } else {
            result64 = (uint64_t)cpu->r[rs] + (uint64_t)operand;
            cpu->r[rd] = (uint32_t)result64;
            set_add_flags(cpu, cpu->r[rs], operand, result64);
        }
        return 0;
    }

    /* Format 3: MOV/CMP/ADD/SUB immediate */
    if ((inst >> 13) == 1) {
        int op = (inst >> 11) & 3;
        int rd = (inst >> 8) & 7;
        uint32_t imm = inst & 0xFF;
        uint64_t result64;
        switch (op) {
            case 0: cpu->r[rd] = imm; set_nz(cpu, imm); break; /* MOV */
            case 1: /* CMP */
                result64 = (uint64_t)cpu->r[rd] - (uint64_t)imm;
                set_sub_flags(cpu, cpu->r[rd], imm, result64);
                break;
            case 2: /* ADD */
                result64 = (uint64_t)cpu->r[rd] + (uint64_t)imm;
                cpu->r[rd] = (uint32_t)result64;
                set_add_flags(cpu, cpu->r[rd], imm, result64);
                break;
            case 3: /* SUB */
                result64 = (uint64_t)cpu->r[rd] - (uint64_t)imm;
                set_sub_flags(cpu, cpu->r[rd], imm, result64);
                cpu->r[rd] = (uint32_t)result64;
                break;
        }
        return 0;
    }

    /* Format 4: ALU operations */
    if ((inst >> 10) == 0x10) {
        int op = (inst >> 6) & 0xF;
        int rs = (inst >> 3) & 7;
        int rd = inst & 7;
        uint32_t a = cpu->r[rd], b = cpu->r[rs];
        uint64_t result64;
        uint32_t result;
        int carry;
        uint32_t old_c = (cpu->cpsr & CPSR_C) ? 1 : 0;

        switch (op) {
            case 0x0: result = a & b; set_nz(cpu, result); cpu->r[rd] = result; break; /* AND */
            case 0x1: result = a ^ b; set_nz(cpu, result); cpu->r[rd] = result; break; /* EOR */
            case 0x2: /* LSL */
                b &= 0xFF;
                if (b == 0) result = a;
                else if (b < 32) { carry = (a >> (32-b)) & 1; result = a << b; set_nzc(cpu, result, carry); }
                else { carry = (b == 32) ? (a & 1) : 0; result = 0; set_nzc(cpu, result, carry); }
                cpu->r[rd] = result;
                break;
            case 0x3: /* LSR */
                b &= 0xFF;
                if (b == 0) result = a;
                else if (b < 32) { carry = (a >> (b-1)) & 1; result = a >> b; set_nzc(cpu, result, carry); }
                else { carry = (b == 32) ? ((a >> 31) & 1) : 0; result = 0; set_nzc(cpu, result, carry); }
                cpu->r[rd] = result;
                break;
            case 0x4: /* ASR */
                b &= 0xFF;
                if (b == 0) result = a;
                else if (b < 32) { carry = (a >> (b-1)) & 1; result = (uint32_t)((int32_t)a >> b); set_nzc(cpu, result, carry); }
                else { carry = (a >> 31) & 1; result = (uint32_t)((int32_t)a >> 31); set_nzc(cpu, result, carry); }
                cpu->r[rd] = result;
                break;
            case 0x5: /* ADC */
                result64 = (uint64_t)a + (uint64_t)b + old_c;
                cpu->r[rd] = (uint32_t)result64;
                set_add_flags(cpu, a, b + old_c, result64);
                break;
            case 0x6: /* SBC */
                result64 = (uint64_t)a - (uint64_t)b - (1 - old_c);
                cpu->r[rd] = (uint32_t)result64;
                set_sub_flags(cpu, a, b + (1 - old_c), result64);
                break;
            case 0x7: /* ROR */
                b &= 0xFF;
                if (b == 0) result = a;
                else { b &= 31; if (b == 0) result = a; else result = (a >> b) | (a << (32 - b)); }
                carry = (result >> 31) & 1;
                set_nzc(cpu, result, carry);
                cpu->r[rd] = result;
                break;
            case 0x8: result = a & b; set_nz(cpu, result); break; /* TST */
            case 0x9: /* NEG */
                result64 = 0 - (uint64_t)b;
                cpu->r[rd] = (uint32_t)result64;
                set_sub_flags(cpu, 0, b, result64);
                break;
            case 0xA: /* CMP */
                result64 = (uint64_t)a - (uint64_t)b;
                set_sub_flags(cpu, a, b, result64);
                break;
            case 0xB: /* CMN */
                result64 = (uint64_t)a + (uint64_t)b;
                set_add_flags(cpu, a, b, result64);
                break;
            case 0xC: result = a | b; set_nz(cpu, result); cpu->r[rd] = result; break; /* ORR */
            case 0xD: result = a * b; set_nz(cpu, result); cpu->r[rd] = result; break; /* MUL */
            case 0xE: result = a & ~b; set_nz(cpu, result); cpu->r[rd] = result; break; /* BIC */
            case 0xF: result = ~b; set_nz(cpu, result); cpu->r[rd] = result; break; /* MVN */
        }
        return 0;
    }

    /* Format 5: Hi register ops / BX */
    if ((inst >> 10) == 0x11) {
        int op = (inst >> 8) & 3;
        int H1 = (inst >> 7) & 1;
        int H2 = (inst >> 6) & 1;
        int rs = ((H2 << 3) | ((inst >> 3) & 7));
        int rd = ((H1 << 3) | (inst & 7));
        uint32_t rs_val = cpu->r[rs];
        if (rs == 15) rs_val = pc + 4;

        switch (op) {
            case 0: cpu->r[rd] += rs_val; break; /* ADD */
            case 1: { /* CMP */
                uint64_t r64 = (uint64_t)cpu->r[rd] - (uint64_t)rs_val;
                set_sub_flags(cpu, cpu->r[rd], rs_val, r64);
                break;
            }
            case 2: cpu->r[rd] = rs_val; break; /* MOV */
            case 3: /* BX / BLX */
                if (H1) cpu->r[14] = (pc + 2) | 1; /* BLX */
                if ((rs_val & ~1u) == SENTINEL_LR) {
                    cpu->halted = 1;
                    return 0;
                }
                if (try_native_call(cpu, rs_val & ~1u) && (H1 || rs == 14)) {
                    return 0;
                }
                if (rs_val & 1) {
                    cpu->cpsr |= CPSR_T;
                    cpu->r[15] = rs_val & ~1u;
                } else {
                    cpu->cpsr &= ~CPSR_T;
                    cpu->r[15] = rs_val & ~3u;
                }
                break;
        }
        if (op != 3 && rd == 15) {
            /* Branch via ADD/MOV to PC */
        }
        return 0;
    }

    /* Format 6: PC-relative load (LDR Rd, [PC, #imm]) */
    if ((inst >> 11) == 9) {
        int rd = (inst >> 8) & 7;
        uint32_t offset = (inst & 0xFF) * 4;
        uint32_t addr = ((pc + 4) & ~3u) + offset;
        cpu->r[rd] = mem_read32(addr);
        return 0;
    }

    /* Format 7/8: Load/Store with register offset */
    if ((inst >> 12) == 5) {
        int L = (inst >> 11) & 1;
        int B = (inst >> 10) & 1;
        int ro = (inst >> 6) & 7;
        int rb = (inst >> 3) & 7;
        int rd = inst & 7;
        uint32_t addr = cpu->r[rb] + cpu->r[ro];

        if ((inst >> 9) & 1) {
            /* Format 8: Sign-extended / halfword */
            int H = (inst >> 11) & 1;
            int S = (inst >> 10) & 1;
            if (!S && !H) { mem_write16(addr, (uint16_t)cpu->r[rd]); } /* STRH */
            else if (!S && H) { cpu->r[rd] = (uint32_t)(int32_t)(int8_t)mem_read8(addr); } /* LDSB */
            else if (S && !H) { cpu->r[rd] = mem_read16(addr); } /* LDRH */
            else { cpu->r[rd] = (uint32_t)(int32_t)(int16_t)mem_read16(addr); } /* LDSH */
        } else {
            /* Format 7: word/byte */
            if (L) {
                cpu->r[rd] = B ? mem_read8(addr) : mem_read32(addr);
            } else {
                if (B) mem_write8(addr, (uint8_t)cpu->r[rd]);
                else mem_write32(addr, cpu->r[rd]);
            }
        }
        return 0;
    }

    /* Format 9: Load/Store with immediate offset */
    if ((inst >> 13) == 3) {
        int B = (inst >> 12) & 1;
        int L = (inst >> 11) & 1;
        int offset = (inst >> 6) & 0x1F;
        int rb = (inst >> 3) & 7;
        int rd = inst & 7;
        uint32_t addr = cpu->r[rb] + (B ? offset : offset * 4);
        if (L) {
            cpu->r[rd] = B ? mem_read8(addr) : mem_read32(addr);
        } else {
            if (B) mem_write8(addr, (uint8_t)cpu->r[rd]);
            else mem_write32(addr, cpu->r[rd]);
        }
        return 0;
    }

    /* Format 10: Load/Store halfword */
    if ((inst >> 12) == 8) {
        int L = (inst >> 11) & 1;
        int offset = ((inst >> 6) & 0x1F) * 2;
        int rb = (inst >> 3) & 7;
        int rd = inst & 7;
        uint32_t addr = cpu->r[rb] + offset;
        if (L) cpu->r[rd] = mem_read16(addr);
        else mem_write16(addr, (uint16_t)cpu->r[rd]);
        return 0;
    }

    /* Format 11: SP-relative load/store */
    if ((inst >> 12) == 9) {
        int L = (inst >> 11) & 1;
        int rd = (inst >> 8) & 7;
        uint32_t offset = (inst & 0xFF) * 4;
        uint32_t addr = cpu->r[13] + offset;
        if (L) cpu->r[rd] = mem_read32(addr);
        else mem_write32(addr, cpu->r[rd]);
        return 0;
    }

    /* Format 12: Load address (ADD Rd, PC/SP, #imm) */
    if ((inst >> 12) == 0xA) {
        int SP = (inst >> 11) & 1;
        int rd = (inst >> 8) & 7;
        uint32_t offset = (inst & 0xFF) * 4;
        cpu->r[rd] = (SP ? cpu->r[13] : ((pc + 4) & ~3u)) + offset;
        return 0;
    }

    /* Format 13: Add offset to SP */
    if ((inst & 0xFF00) == 0xB000) {
        int sign = (inst >> 7) & 1;
        uint32_t offset = (inst & 0x7F) * 4;
        if (sign) cpu->r[13] -= offset;
        else cpu->r[13] += offset;
        return 0;
    }

    /* Format 14: PUSH/POP */
    if ((inst & 0xF600) == 0xB400) {
        int L = (inst >> 11) & 1;
        int R = (inst >> 8) & 1;  /* LR (push) or PC (pop) */
        uint8_t reg_list = inst & 0xFF;

        if (L) {
            /* POP */
            uint32_t addr = cpu->r[13];
            for (int i = 0; i < 8; i++) {
                if (reg_list & (1 << i)) {
                    cpu->r[i] = mem_read32(addr);
                    addr += 4;
                }
            }
            if (R) {
                uint32_t val = mem_read32(addr);
                addr += 4;
                if ((val & ~1u) == SENTINEL_LR) {
                    cpu->halted = 1;
                    cpu->r[13] = addr;
                    return 0;
                }
                if (val & 1) {
                    cpu->cpsr |= CPSR_T;
                    cpu->r[15] = val & ~1u;
                } else {
                    cpu->cpsr &= ~CPSR_T;
                    cpu->r[15] = val & ~3u;
                }
            }
            cpu->r[13] = addr;
        } else {
            /* PUSH */
            int count = 0;
            for (int i = 0; i < 8; i++) if (reg_list & (1 << i)) count++;
            if (R) count++;
            uint32_t addr = cpu->r[13] - count * 4;
            cpu->r[13] = addr;
            for (int i = 0; i < 8; i++) {
                if (reg_list & (1 << i)) {
                    mem_write32(addr, cpu->r[i]);
                    addr += 4;
                }
            }
            if (R) {
                mem_write32(addr, cpu->r[14]);
            }
        }
        return 0;
    }

    /* Format 15: Multiple load/store (LDMIA/STMIA) */
    if ((inst >> 12) == 0xC) {
        int L = (inst >> 11) & 1;
        int rb = (inst >> 8) & 7;
        uint8_t reg_list = inst & 0xFF;
        uint32_t addr = cpu->r[rb];
        for (int i = 0; i < 8; i++) {
            if (!(reg_list & (1 << i))) continue;
            if (L) cpu->r[i] = mem_read32(addr);
            else mem_write32(addr, cpu->r[i]);
            addr += 4;
        }
        cpu->r[rb] = addr;
        return 0;
    }

    /* Format 16: Conditional branch */
    if ((inst >> 12) == 0xD) {
        int cond_val = (inst >> 8) & 0xF;
        if (cond_val == 0xF) {
            /* SWI */
            uint32_t swi_num = inst & 0xFF;
            if (g_log_count < MAX_LOG_LINES) {
                g_log_count++;
                fprintf(stderr, "[arm-interp] Thumb SWI 0x%02X at PC=0x%08X\n",
                        (unsigned)swi_num, pc);
            }
            return 0;
        }
        if (cond_val == 0xE) return 0; /* Undefined */
        if (check_cond(cpu->cpsr, cond_val)) {
            int32_t offset = (int32_t)(int8_t)(inst & 0xFF) * 2;
            cpu->r[15] = pc + 4 + (uint32_t)offset;
        }
        return 0;
    }

    /* Format 18: Unconditional branch */
    if ((inst >> 11) == 0x1C) {
        int32_t offset = (int32_t)(inst << 21) >> 20;
        cpu->r[15] = pc + 4 + (uint32_t)offset;
        return 0;
    }

    /* Format 19: Long branch with link (BL - two-instruction sequence) */
    if ((inst >> 11) == 0x1E) {
        /* First instruction: LR = PC + 4 + (offset_hi << 12) */
        int32_t offset_hi = (int32_t)(inst << 21) >> 9;  /* sign-extend 11 bits, shift left 12 */
        cpu->r[14] = pc + 4 + (uint32_t)offset_hi;
        return 0;
    }
    if ((inst >> 11) == 0x1F) {
        /* Second instruction: temp = next_inst_addr | 1; PC = LR + (offset_lo << 1); LR = temp */
        uint32_t offset_lo = (inst & 0x7FF) << 1;
        uint32_t target = cpu->r[14] + offset_lo;
        cpu->r[14] = (pc + 2) | 1;

        /* Try native call */
        if (try_native_call(cpu, target & ~1u)) {
            return 0;
        }
        cpu->r[15] = target;
        return 0;
    }
    /* BLX suffix (Thumb → ARM) */
    if ((inst >> 11) == 0x1D) {
        uint32_t offset_lo = (inst & 0x7FF) << 1;
        uint32_t target = (cpu->r[14] + offset_lo) & ~3u;
        cpu->r[14] = (pc + 2) | 1;
        if (try_native_call(cpu, target)) return 0;
        cpu->cpsr &= ~CPSR_T;
        cpu->r[15] = target;
        return 0;
    }

    /* Unhandled */
    if (g_log_count < MAX_LOG_LINES) {
        g_log_count++;
        fprintf(stderr, "[arm-interp] unhandled Thumb inst 0x%04X at PC=0x%08X\n",
                inst, pc);
    }
    return -1;
}

/* ---- Step function ---- */
static int arm_step(ArmCpu *cpu) {
    if (cpu->halted) return 1;
    cpu->cycles++;

    if (cpu->cpsr & CPSR_T) {
        return exec_thumb(cpu);
    } else {
        return exec_arm(cpu);
    }
}

/* ---- Public API ---- */
uint32_t arm_interp_call(uint32_t nds_addr, uint32_t arg0, uint32_t arg1,
                         uint32_t arg2, uint32_t arg3)
{
    if (!g_enabled) return 0;
    if (!nds_arm9_ram_is_mapped()) return 0;

    /* Check for native function first */
    void *fn = host_fnptr_lookup(nds_addr & ~1u);
    if (fn) {
        return ((native_fn_t)fn)(arg0, arg1, arg2, arg3);
    }

    /* Verify the target address is in mapped RAM */
    if (!is_ram_addr(nds_addr & ~1u)) {
        if (g_log_count < MAX_LOG_LINES) {
            g_log_count++;
            fprintf(stderr, "[arm-interp] call to unmapped addr 0x%08X\n", nds_addr);
        }
        return 0;
    }

    g_call_count++;
    if (g_call_count <= 50) {
        fprintf(stderr, "[arm-interp] interpreting call#%d to 0x%08X (args: 0x%X, 0x%X, 0x%X, 0x%X)\n",
                g_call_count, nds_addr, arg0, arg1, arg2, arg3);
    }

    /* Always log gameplay tick (overlay 0) calls */
    static int s_ov0_tick_count = 0;
    int is_ov0_tick = (nds_addr == 0x0206C34Cu);
    if (is_ov0_tick) {
        s_ov0_tick_count++;
        /* Read phase byte at obj+0x10 and a few key fields */
        uint8_t phase = *(volatile uint8_t *)(uintptr_t)(arg0 + 0x10);
        uint16_t flags = *(volatile uint16_t *)(uintptr_t)(arg0 + 0x12);
        uint32_t field_44 = *(volatile uint32_t *)(uintptr_t)(arg0 + 0x44);
        if (s_ov0_tick_count <= 20) {
            fprintf(stderr, "[OV0-TICK] #%d entry: obj=0x%08X phase=%u flags=0x%04X "
                    "field44=0x%08X (call#%d)\n",
                    s_ov0_tick_count, arg0, phase, flags, field_44, g_call_count);
            fflush(stderr);
        }
    }

    /* Enable verbose trace for call #20 (a mid-run title tick) */
    static int s_trace_done = 0;
    if (!s_trace_done && g_call_count >= 15 && nds_addr == 0x02077444u) {
        g_verbose_trace = 1;
        s_trace_done = 1;
        fprintf(stderr, "[trace] === VERBOSE TRACE START (call#%d to 0x%08X) ===\n",
                g_call_count, nds_addr);
        fflush(stderr);
    }

    ArmCpu cpu;
    memset(&cpu, 0, sizeof(cpu));
    cpu.r[0] = arg0;
    cpu.r[1] = arg1;
    cpu.r[2] = arg2;
    cpu.r[3] = arg3;
    /* The real scene-queue dispatch (FUN_0202a33c) keeps the current node
     * in R5 and does `MOV R0, R5; BLX Rn` — so R5 == R0 at tick entry.
     * Several tick functions (e.g. title tick → FUN_02065B5C) rely on R5
     * being the node/scene-obj pointer without saving it themselves. */
    cpu.r[5] = arg0;
    cpu.r[13] = INTERP_STACK_BASE;  /* SP */
    cpu.r[14] = SENTINEL_LR;        /* LR = sentinel (return detection) */

    if (nds_addr & 1) {
        cpu.cpsr = CPSR_T;  /* Thumb mode */
        cpu.r[15] = nds_addr & ~1u;
    } else {
        cpu.cpsr = 0;       /* ARM mode */
        cpu.r[15] = nds_addr & ~3u;
    }

    int max_cycles = MAX_CYCLES_PER_CALL;
    /* Small-range loop detection: if the PC stays within a narrow range
     * (< 256 bytes) for 1000+ instructions, we're stuck in a busy-wait
     * loop (e.g., polling a sprite table or IO register).  Low threshold
     * keeps any loop counter corruption minimal (~143 increments). */
    uint32_t loop_pc_min = cpu.r[15];
    uint32_t loop_pc_max = cpu.r[15];
    int loop_window_count = 0;
    while (!cpu.halted && max_cycles > 0) {
        uint32_t cur_pc = cpu.r[15];
        if (cur_pc >= loop_pc_min && cur_pc <= loop_pc_max + 64) {
            if (cur_pc < loop_pc_min) loop_pc_min = cur_pc;
            if (cur_pc > loop_pc_max) loop_pc_max = cur_pc;
            loop_window_count++;
            if (loop_window_count > 1000 && (loop_pc_max - loop_pc_min) < 256) {
                if (g_log_count < MAX_LOG_LINES) {
                    g_log_count++;
                    fprintf(stderr, "[arm-interp] tight loop at PC=0x%08X..0x%08X (%d iters, breaking)\n",
                            loop_pc_min, loop_pc_max, loop_window_count);
                }
                break;
            }
        } else {
            loop_pc_min = cur_pc;
            loop_pc_max = cur_pc;
            loop_window_count = 0;
        }
        int err = arm_step(&cpu);
        if (err) {
            fprintf(stderr, "[arm-interp] step error %d at PC=0x%08X\n", err, cpu.r[15]);
            break;
        }
        max_cycles--;
    }

    if (max_cycles <= 0 && g_log_count < MAX_LOG_LINES) {
        g_log_count++;
        fprintf(stderr, "[arm-interp] call to 0x%08X hit cycle limit (%llu cycles), stuck at PC=0x%08X\n",
                nds_addr, (unsigned long long)cpu.cycles, cpu.r[15]);
    }

    if (g_call_count <= 50) {
        fprintf(stderr, "[arm-interp] call#%d to 0x%08X finished: %llu cycles, r0=0x%X\n",
                g_call_count, nds_addr, (unsigned long long)cpu.cycles, cpu.r[0]);
    }
    if (is_ov0_tick && s_ov0_tick_count <= 20) {
        fprintf(stderr, "[OV0-TICK] #%d done: %llu cycles, r0=0x%X, max_left=%d\n",
                s_ov0_tick_count, (unsigned long long)cpu.cycles, cpu.r[0], max_cycles);
        fflush(stderr);
    }
    if (g_verbose_trace && nds_addr == 0x02077444u) {
        fprintf(stderr, "[trace] === VERBOSE TRACE END (%llu cycles) ===\n",
                (unsigned long long)cpu.cycles);
        g_verbose_trace = 0;
    }
    g_total_cycles += cpu.cycles;
    return cpu.r[0];
}
