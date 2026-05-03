@ Common assembly macros for M&L:PiT decompilation
@ Include this in all .s files

.macro thumb_func_start name
    .thumb
    .thumb_func
    .align 2
    .global \name
    .type \name, %function
.endm

.macro arm_func_start name
    .arm
    .align 2
    .global \name
    .type \name, %function
.endm

.macro func_end name
    .size \name, .-\name
.endm

