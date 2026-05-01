    .include "macros.inc"

    .text
    .arm
    .align 2
    .global _start
    .type _start, %function

@ Nintendo DS ARM9 CRT0 (C Runtime Startup)
@ Entry point: 0x02004800
@
@ This is the first code executed by the ARM9 processor.
@ It initializes processor modes, stack pointers, clears BSS,
@ and calls into the Nitro SDK and game initialization.

_start: @ 0x02004800
    @ Disable interrupts (IME = 0)
    mov     ip, #0x04000000
    str     ip, [ip, #0x208]        @ REG_IME = 0

    @ Initialize CP15 coprocessor (cache, TCM, MPU)
    bl      crt0_init_cp15

    @ Set IRQ mode stack
    mov     r0, #0x12               @ IRQ mode
    msr     CPSR_c, r0
    ldr     r0, =__dtcm_top
    add     r0, r0, #0x3FC0
    mov     sp, r0

    @ Set SVC mode stack
    mov     r0, #0x13               @ SVC mode
    msr     CPSR_c, r0
    ldr     r0, =__dtcm_top
    add     r0, r0, #0x3FC0
    sub     r0, r0, #0x40
    mov     sp, r0

    @ Set System mode (normal execution) stack
    ldr     r1, =__irq_stack_size
    mov     r0, #0x1F               @ System mode
    msr     CPSR_fsxc, r0
    sub     sp, r1, #4

    @ Clear DTCM
    mov     r0, #0
    ldr     r1, =__dtcm_top
    mov     r2, #0x4000
    bl      crt0_fill_mem

    @ Clear ITCM
    mov     r0, #0
    ldr     r1, =__itcm_top
    mov     r2, #0x400
    bl      crt0_fill_mem

    @ Fill ITCM with reset vector
    mov     r0, #0x200
    ldr     r1, =__itcm_reset
    mov     r2, #0x400
    bl      crt0_fill_mem

    @ Copy static initializers
    ldr     r1, =__static_init_table
    ldr     r0, [r1, #0x14]
    bl      crt0_copy_sections

    @ Initialize hardware
    bl      crt0_init_hardware

    @ Clear BSS
    ldr     r0, =__static_init_table
    ldr     r1, [r0, #0x0C]        @ BSS start
    ldr     r2, [r0, #0x10]        @ BSS end
    mov     r0, #0
.Lclear_bss:
    cmp     r1, r2
    strcc   r0, [r1], #4
    bcc     .Lclear_bss

    @ Set up IRQ handler pointer
    ldr     r1, =__irq_handler_ptr
    mov     r0, #0
    str     r0, [r1]

    @ Set up DTCM IRQ vector
    ldr     r1, =__dtcm_top
    add     r1, r1, #0x3FC0
    add     r1, r1, #0x3C
    ldr     r0, =__irq_vector
    str     r0, [r1]

    @ Call SDK init functions
    bl      OS_Init                 @ 0x02046DFC - Nitro SDK OS init
    bl      GX_Init                 @ 0x0202976C - Graphics init
    bl      FS_Init                 @ 0x02048618 - File system init

    @ Jump to NitroMain (game entry point)
    ldr     r1, =NitroMain
    ldr     lr, =__reset_vector
    bx      r1

    .size _start, .-_start


@ Simple memory fill: r0 = value, r1 = dest, r2 = size (bytes)
    .global crt0_fill_mem
    .type crt0_fill_mem, %function
crt0_fill_mem:
    add     ip, r1, r2
.Lfill_loop:
    cmp     r1, ip
    stmltia r1!, {r0}
    blt     .Lfill_loop
    bx      lr
    .size crt0_fill_mem, .-crt0_fill_mem
