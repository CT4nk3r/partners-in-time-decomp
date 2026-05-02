/*
 * host_display_data_init.c - HOST_PORT only.
 *
 * Owns 64-bit-safe definitions of the display_system.c .data slots
 * (DAT_02019730..0x0201977c), declared via the g_disp_DAT_xxx names
 * that display_system.c is rewired to under HOST_PORT.
 *
 * The host_undefined_stubs.c versions are 4-byte uint32_t and would
 * truncate any pointer we try to write into them; full-width pointer
 * globals here are read 8 bytes at a time (matching the int * / short *
 * declarations the decompiled code uses).
 *
 * Backing buffers are seeded from the user's own arm9.bin via the
 * generated header so the lookup tables FUN_020192f8 indexes contain
 * the genuine ROM bytes, not zero.
 */
#include "types.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "host_data_init_generated.h"

extern u32 *game_state_host_get_current_slot(void);

/* Mutable shadow buffers seeded from ROM bytes. */
static unsigned char s_tbl_02019734[sizeof(HOSTDATA_DISP_TBL_02019734_BYTES)];
static unsigned char s_tbl_02019738[sizeof(HOSTDATA_DISP_TBL_02019738_BYTES)];
static unsigned char s_tbl_0201973c[sizeof(HOSTDATA_DISP_TBL_0201973C_BYTES)];
static unsigned char s_tbl_02019740[sizeof(HOSTDATA_DISP_TBL_02019740_BYTES)];
static unsigned char s_tbl_02019748[sizeof(HOSTDATA_DISP_TBL_02019748_BYTES)];
static unsigned char s_tbl_0201974c[sizeof(HOSTDATA_DISP_TBL_0201974C_BYTES)];
static unsigned char s_tbl_02019750[sizeof(HOSTDATA_DISP_TBL_02019750_BYTES)];
static unsigned char s_tbl_02019754[sizeof(HOSTDATA_DISP_TBL_02019754_BYTES)];
static unsigned char s_tbl_02019758[sizeof(HOSTDATA_DISP_TBL_02019758_BYTES)];
static unsigned char s_tbl_0201975c[sizeof(HOSTDATA_DISP_TBL_0201975C_BYTES)];
static unsigned char s_tbl_02019760[sizeof(HOSTDATA_DISP_TBL_02019760_BYTES)];
static unsigned char s_tbl_02019764[sizeof(HOSTDATA_DISP_TBL_02019764_BYTES)];
static unsigned char s_tbl_0201976c[sizeof(HOSTDATA_DISP_TBL_0201976C_BYTES)];
static unsigned char s_tbl_02019770[sizeof(HOSTDATA_DISP_TBL_02019770_BYTES)];
static unsigned char s_tbl_02019774[sizeof(HOSTDATA_DISP_TBL_02019774_BYTES)];
static unsigned char s_tbl_02019778[sizeof(HOSTDATA_DISP_TBL_02019778_BYTES)];

/* Full-width definitions matching the extern types in display_system.c
 * under HOST_PORT (pointer / intptr_t — 8 bytes on 64-bit host). */
int      *g_disp_DAT_02019730;
short    *g_disp_DAT_02019734;
intptr_t  g_disp_DAT_02019738;
intptr_t  g_disp_DAT_0201973c;
intptr_t  g_disp_DAT_02019740;
intptr_t  g_disp_DAT_02019744;
short    *g_disp_DAT_02019748;
intptr_t  g_disp_DAT_0201974c;
intptr_t  g_disp_DAT_02019750;
intptr_t  g_disp_DAT_02019754;
short    *g_disp_DAT_02019758;
intptr_t  g_disp_DAT_0201975c;
intptr_t  g_disp_DAT_02019760;
intptr_t  g_disp_DAT_02019764;
intptr_t  g_disp_DAT_02019768;
short    *g_disp_DAT_0201976c;
intptr_t  g_disp_DAT_02019770;
intptr_t  g_disp_DAT_02019774;
intptr_t  g_disp_DAT_02019778;

void host_display_data_init_install(void)
{
    /* DAT_02019730 aliases the sGameState->current word - same as
     * DAT_02005D28. The decomp dereferences *DAT_02019730 to get the
     * clGameMain instance pointer. */
    g_disp_DAT_02019730 = (int *)game_state_host_get_current_slot();

    /* Scalar offsets (used as +offset added to *DAT_02019730). */
    g_disp_DAT_02019744 = (intptr_t)HOSTDATA_DAT_02019744;  /* 0x41C */
    g_disp_DAT_02019768 = (intptr_t)HOSTDATA_DAT_02019768;  /* 0x464 */

#define INSTALL_TBL_PTR(addr_lower, src, var)                                  \
    do {                                                                       \
        memcpy(s_tbl_##addr_lower, HOSTDATA_DISP_TBL_##src##_BYTES,            \
               sizeof(s_tbl_##addr_lower));                                    \
        var = (void *)s_tbl_##addr_lower;                                      \
    } while (0)

#define INSTALL_TBL_INTPTR(addr_lower, src, var)                               \
    do {                                                                       \
        memcpy(s_tbl_##addr_lower, HOSTDATA_DISP_TBL_##src##_BYTES,            \
               sizeof(s_tbl_##addr_lower));                                    \
        var = (intptr_t)(uintptr_t)s_tbl_##addr_lower;                         \
    } while (0)

    INSTALL_TBL_PTR   (02019734, 02019734, g_disp_DAT_02019734);
    INSTALL_TBL_INTPTR(02019738, 02019738, g_disp_DAT_02019738);
    INSTALL_TBL_INTPTR(0201973c, 0201973C, g_disp_DAT_0201973c);
    INSTALL_TBL_INTPTR(02019740, 02019740, g_disp_DAT_02019740);
    INSTALL_TBL_PTR   (02019748, 02019748, g_disp_DAT_02019748);
    INSTALL_TBL_INTPTR(0201974c, 0201974C, g_disp_DAT_0201974c);
    INSTALL_TBL_INTPTR(02019750, 02019750, g_disp_DAT_02019750);
    INSTALL_TBL_INTPTR(02019754, 02019754, g_disp_DAT_02019754);
    INSTALL_TBL_PTR   (02019758, 02019758, g_disp_DAT_02019758);
    INSTALL_TBL_INTPTR(0201975c, 0201975C, g_disp_DAT_0201975c);
    INSTALL_TBL_INTPTR(02019760, 02019760, g_disp_DAT_02019760);
    INSTALL_TBL_INTPTR(02019764, 02019764, g_disp_DAT_02019764);
    INSTALL_TBL_PTR   (0201976c, 0201976C, g_disp_DAT_0201976c);
    INSTALL_TBL_INTPTR(02019770, 02019770, g_disp_DAT_02019770);
    INSTALL_TBL_INTPTR(02019774, 02019774, g_disp_DAT_02019774);
    INSTALL_TBL_INTPTR(02019778, 02019778, g_disp_DAT_02019778);

    fprintf(stderr,
            "[HOST-DISP] installed: slot=%p off44=0x%lx off68=0x%lx tbl734=%p\n",
            (void *)g_disp_DAT_02019730,
            (long)g_disp_DAT_02019744,
            (long)g_disp_DAT_02019768,
            (void *)g_disp_DAT_02019734);
}
