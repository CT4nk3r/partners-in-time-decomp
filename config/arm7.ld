/* ARM7 Linker Script for Mario & Luigi: Partners in Time
 *
 * Nintendo DS ARM7 Memory Map:
 *   0x037F8000 - 0x0380FFFF : ARM7 WRAM (64KB, though typically loaded at 0x037F8000)
 *   0x03800000 - 0x0380FFFF : Shared WRAM (can be mapped to ARM7 or ARM9)
 *   0x0380FFE0 - 0x0380FFFF : ARM7 stack area
 *
 * The ARM7 handles sound, Wi-Fi, touchscreen, and microphone.
 */

OUTPUT_FORMAT("elf32-littlearm")
OUTPUT_ARCH(arm)

MEMORY
{
    /* ARM7 WRAM */
    arm7_wram (rwx) : ORIGIN = 0x037F8000, LENGTH = 96K
}

SECTIONS
{
    .text : {
        __text_start = .;
        *(.text)
        *(.text.*)
        *(.stub)
        *(.gnu.linkonce.t.*)
        __text_end = .;
    } > arm7_wram

    .glue : {
        *(.glue_7)
        *(.glue_7t)
    } > arm7_wram

    .rodata : {
        *(.rodata)
        *(.rodata.*)
        *(.gnu.linkonce.r.*)
    } > arm7_wram

    .data : {
        __data_start = .;
        *(.data)
        *(.data.*)
        *(.gnu.linkonce.d.*)
        __data_end = .;
    } > arm7_wram

    .bss (NOLOAD) : {
        __bss_start = .;
        *(.bss)
        *(.bss.*)
        *(.gnu.linkonce.b.*)
        *(COMMON)
        __bss_end = .;
    } > arm7_wram

    /DISCARD/ : {
        *(.comment)
        *(.debug*)
        *(.ARM.attributes)
        *(.note.GNU-stack)
    }
}
