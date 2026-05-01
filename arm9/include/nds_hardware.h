/**
 * Nintendo DS Hardware Register Definitions (ARM9)
 *
 * Based on GBATEK documentation (public domain hardware documentation).
 * These are memory-mapped I/O register addresses for the NDS ARM9 processor.
 */

#ifndef NDS_HARDWARE_H
#define NDS_HARDWARE_H

#include <stdint.h>

typedef volatile uint8_t  vu8;
typedef volatile uint16_t vu16;
typedef volatile uint32_t vu32;
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef int8_t   s8;
typedef int16_t  s16;
typedef int32_t  s32;

/* Display registers */
#define REG_DISPCNT       (*(vu32*)0x04000000)
#define REG_DISPSTAT      (*(vu16*)0x04000004)
#define REG_VCOUNT        (*(vu16*)0x04000006)

/* BG control */
#define REG_BG0CNT        (*(vu16*)0x04000008)
#define REG_BG1CNT        (*(vu16*)0x0400000A)
#define REG_BG2CNT        (*(vu16*)0x0400000C)
#define REG_BG3CNT        (*(vu16*)0x0400000E)

/* DMA */
#define REG_DMA0SAD       (*(vu32*)0x040000B0)
#define REG_DMA0DAD       (*(vu32*)0x040000B4)
#define REG_DMA0CNT       (*(vu32*)0x040000B8)
#define REG_DMA1SAD       (*(vu32*)0x040000BC)
#define REG_DMA1DAD       (*(vu32*)0x040000C0)
#define REG_DMA1CNT       (*(vu32*)0x040000C4)
#define REG_DMA2SAD       (*(vu32*)0x040000C8)
#define REG_DMA2DAD       (*(vu32*)0x040000CC)
#define REG_DMA2CNT       (*(vu32*)0x040000D0)
#define REG_DMA3SAD       (*(vu32*)0x040000D4)
#define REG_DMA3DAD       (*(vu32*)0x040000D8)
#define REG_DMA3CNT       (*(vu32*)0x040000DC)

/* Timers */
#define REG_TM0CNT_L      (*(vu16*)0x04000100)
#define REG_TM0CNT_H      (*(vu16*)0x04000102)
#define REG_TM1CNT_L      (*(vu16*)0x04000104)
#define REG_TM1CNT_H      (*(vu16*)0x04000106)
#define REG_TM2CNT_L      (*(vu16*)0x04000108)
#define REG_TM2CNT_H      (*(vu16*)0x0400010A)
#define REG_TM3CNT_L      (*(vu16*)0x0400010C)
#define REG_TM3CNT_H      (*(vu16*)0x0400010E)

/* Input */
#define REG_KEYINPUT      (*(vu16*)0x04000130)
#define REG_KEYCNT        (*(vu16*)0x04000132)

/* IPC / Inter-processor communication */
#define REG_IPCSYNC       (*(vu16*)0x04000180)
#define REG_IPCFIFOCNT    (*(vu16*)0x04000184)
#define REG_IPCFIFOSEND   (*(vu32*)0x04000188)
#define REG_IPCFIFORECV   (*(vu32*)0x04100000)

/* Interrupt registers */
#define REG_IME           (*(vu32*)0x04000208)
#define REG_IE            (*(vu32*)0x04000210)
#define REG_IF            (*(vu32*)0x04000214)

/* VRAM banks */
#define REG_VRAMCNT_A     (*(vu8*)0x04000240)
#define REG_VRAMCNT_B     (*(vu8*)0x04000241)
#define REG_VRAMCNT_C     (*(vu8*)0x04000242)
#define REG_VRAMCNT_D     (*(vu8*)0x04000243)
#define REG_VRAMCNT_E     (*(vu8*)0x04000244)
#define REG_VRAMCNT_F     (*(vu8*)0x04000245)
#define REG_VRAMCNT_G     (*(vu8*)0x04000246)
#define REG_WRAMCNT       (*(vu8*)0x04000247)
#define REG_VRAMCNT_H     (*(vu8*)0x04000248)
#define REG_VRAMCNT_I     (*(vu8*)0x04000249)

/* Memory addresses */
#define MAIN_RAM_START    0x02000000
#define MAIN_RAM_SIZE     0x00400000
#define VRAM_A            0x06800000
#define VRAM_B            0x06820000
#define VRAM_C            0x06840000
#define VRAM_D            0x06860000
#define OAM               0x07000000
#define PALETTE_RAM       0x05000000

#endif /* NDS_HARDWARE_H */
