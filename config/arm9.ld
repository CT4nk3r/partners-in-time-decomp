/* ARM9 Linker Script for Mario & Luigi: Partners in Time */

OUTPUT_FORMAT("elf32-littlearm")
OUTPUT_ARCH(arm)

/* Auto-generated symbol definitions for FUN_/DAT_ references */
INCLUDE "config/undefined_syms.ld"

MEMORY
{
    main_ram (rwx) : ORIGIN = 0x02004000, LENGTH = 4M - 0x4000
    itcm (rwx)     : ORIGIN = 0x01000000, LENGTH = 32K
    dtcm (rw)      : ORIGIN = 0x027C0000, LENGTH = 16K
}

/* CRT0 system symbols */
__dtcm_top          = ORIGIN(dtcm);
__itcm_top          = ORIGIN(itcm);
__itcm_reset        = ORIGIN(itcm);
__irq_stack_size    = 0x400;
__irq_handler_ptr   = ORIGIN(dtcm) + 0x3FFC;
__irq_vector        = ORIGIN(dtcm) + 0x3FFC;
__reset_vector      = ORIGIN(main_ram);
__static_init_table = ORIGIN(main_ram);

SECTIONS
{
    .text : {
        __text_start = .;
        *crt0.o(.text)
        *(.text)
        *(.text.*)
        *(.stub)
        *(.gnu.linkonce.t.*)
        *(.glue_7)
        *(.glue_7t)
        __text_end = .;
    } > main_ram

    .rodata : {
        *(.rodata)
        *(.rodata.*)
        *(.gnu.linkonce.r.*)
    } > main_ram

    .data : {
        __data_start = .;
        *(.data)
        *(.data.*)
        *(.gnu.linkonce.d.*)
        __data_end = .;
    } > main_ram

    .bss (NOLOAD) : {
        __bss_start = .;
        *(.bss)
        *(.bss.*)
        *(.gnu.linkonce.b.*)
        *(COMMON)
        __bss_end = .;
    } > main_ram

    .itcm : {
        *(.itcm)
        *(.itcm.*)
    } > itcm AT > main_ram

    .dtcm : {
        *(.dtcm)
        *(.dtcm.*)
    } > dtcm AT > main_ram

    /DISCARD/ : {
        *(.comment)
        *(.debug*)
        *(.ARM.attributes)
        *(.note.GNU-stack)
    }
}
