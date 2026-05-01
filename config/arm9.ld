/* ARM9 Linker Script for Mario & Luigi: Partners in Time
 *
 * Nintendo DS ARM9 Memory Map:
 *   0x02000000 - 0x023FFFFF : Main RAM (4MB)
 *   0x027E0000 - 0x027FFFFF : ARM9 reserved (shared WRAM mapped here sometimes)
 *
 * The ARM9 binary is loaded at 0x02000000 by the NDS firmware.
 * Overlays are loaded into main RAM at various addresses.
 */

OUTPUT_FORMAT("elf32-littlearm")
OUTPUT_ARCH(arm)

MEMORY
{
    /* Main RAM - ARM9 loaded at 0x02004000 for this ROM */
    main_ram (rwx) : ORIGIN = 0x02004000, LENGTH = 4M - 0x4000

    /* ITCM - 32KB instruction tightly-coupled memory */
    itcm (rwx) : ORIGIN = 0x01000000, LENGTH = 32K

    /* DTCM - 16KB data tightly-coupled memory */
    dtcm (rw) : ORIGIN = 0x027C0000, LENGTH = 16K
}

SECTIONS
{
    /* Code and read-only data */
    .text : {
        __text_start = .;
        *(.text)
        *(.text.*)
        *(.stub)
        *(.gnu.linkonce.t.*)
        __text_end = .;
    } > main_ram

    /* ARM/Thumb interworking glue */
    .glue : {
        *(.glue_7)
        *(.glue_7t)
    } > main_ram

    /* Read-only data */
    .rodata : {
        *(.rodata)
        *(.rodata.*)
        *(.gnu.linkonce.r.*)
    } > main_ram

    /* Initialized data */
    .data : {
        __data_start = .;
        *(.data)
        *(.data.*)
        *(.gnu.linkonce.d.*)
        __data_end = .;
    } > main_ram

    /* Uninitialized data (BSS) */
    .bss (NOLOAD) : {
        __bss_start = .;
        *(.bss)
        *(.bss.*)
        *(.gnu.linkonce.b.*)
        *(COMMON)
        __bss_end = .;
    } > main_ram

    /* ITCM section */
    .itcm : {
        *(.itcm)
        *(.itcm.*)
    } > itcm

    /* DTCM section */
    .dtcm : {
        *(.dtcm)
        *(.dtcm.*)
    } > dtcm

    /* Discard debug sections */
    /DISCARD/ : {
        *(.comment)
        *(.debug*)
        *(.ARM.attributes)
        *(.note.GNU-stack)
    }
}
