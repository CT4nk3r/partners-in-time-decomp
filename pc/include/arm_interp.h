/*
 * arm_interp.h — Minimal ARMv5TE interpreter for NDS overlay code.
 *
 * Executes ARM/Thumb instructions from the NDS-mapped memory region
 * (0x02000000+) so un-decompiled overlay functions can run on PC.
 * When a BL targets an address with a registered native (decompiled)
 * function, the interpreter calls that function directly.
 */
#ifndef ARM_INTERP_H
#define ARM_INTERP_H

#include <stdint.h>

typedef struct {
    uint32_t r[16];       /* r0-r15 (r13=SP, r14=LR, r15=PC) */
    uint32_t cpsr;        /* Current Program Status Register */
    uint64_t cycles;      /* instruction count */
    int      halted;      /* nonzero = stopped */
} ArmCpu;

/* CPSR flag bit positions */
#define CPSR_N  (1u << 31)
#define CPSR_Z  (1u << 30)
#define CPSR_C  (1u << 29)
#define CPSR_V  (1u << 28)
#define CPSR_T  (1u <<  5)  /* Thumb mode */

/* Call an NDS function via the interpreter.
 * Sets up r0-r3 from args, runs until return, returns r0.
 * If the address has a registered native function, calls it directly. */
uint32_t arm_interp_call(uint32_t nds_addr,
                         uint32_t arg0, uint32_t arg1,
                         uint32_t arg2, uint32_t arg3);

/* Enable/disable interpreter (default: enabled) */
void arm_interp_set_enabled(int enabled);
int  arm_interp_is_enabled(void);

/* Stats */
uint64_t arm_interp_total_cycles(void);
int      arm_interp_call_count(void);

#endif /* ARM_INTERP_H */
