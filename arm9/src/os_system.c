/**
 * OS System Functions — IRQ, DMA, IPC, Timers
 *
 * Interrupt initialization, DMA channel command wrappers,
 * inter-processor communication dispatch, timer callbacks,
 * priority management, and spin-wait loops.
 *
 * DMA wrappers all dispatch through dma_command (FUN_0203bda8).
 * IPC functions dispatch through function pointers for ARM9↔ARM7.
 *
 * Decompiled from Ghidra pseudocode.
 */

#include "types.h"

// ============================================================
// Static data pointers
// ============================================================

static vu32 *sIpcInitFlag;      // 0x0203ad10
static vu32 *sIpcBuffers;       // 0x0203ad14
static vu32 *sDmaChannelPtr;    // 0x0203afb8
static vu32 *sIrqControlPtr;    // 0x0203b18c
static vu16 *sIrqStateTable;    // 0x0203b190

// IPC function pointer tables
static void (**sIpcHandlerFn)(u32, u32, u32, u32);  // 0x0203c22c
static void (**sIpcSendFn)(u32, u32, u32, u32);     // 0x0203c244
static void (**sIpcSyncFn)(u32);                     // 0x0203c2e0
static vu32 *sIpcSyncParam;                          // 0x0203c2e4
static void (**sIpcAsyncFn)(u32);                    // 0x0203c2f4
static vu32 *sIpcAsyncParam;                         // 0x0203c2f8

// Timer dispatch
static void (**sTimerFn)(u32, u32, u32);  // 0x0203f390
static vu32 *sTimerParam;                 // 0x0203f394

// Priority/task management
static vu32 *sPriorityBase;     // 0x0203cc18
static vu32 *sWaitFlag1;        // 0x02041d94
static vu32 *sWaitFlag2;        // 0x02041dfc

// ============================================================
// External declarations
// ============================================================

extern void irq_enable(void);
extern void irq_setup(void);
extern void ipc_reset_cmd(void);
extern void enable_ipc_interrupt(int mask);
extern int  power_request_fifo(int cmd, int param, int zero);
extern void panic_check1(void);
extern void panic_check2(void);
extern void dma_channel_enable(int channel);
extern void dma_channel_reset(int zero);
extern void dma_command(int cmd, u32 p1, u32 p2, u32 p3, u32 p4);
extern u32  dma_aux_param(u32 p1, u32 p4, u32 p5);
extern int  get_priority_used1(void);
extern int  get_priority_used2(void);

// ============================================================
// IRQ / Exception initialization
// ============================================================

// FUN_0203a598 @ 0x0203a598 (20 bytes) — Initialize interrupts and system
void os_irq_init(void)
{
    irq_enable();
    irq_setup();
}

// ============================================================
// IPC (Inter-Processor Communication) setup
// ============================================================

// FUN_0203acc0 @ 0x0203acc0 (80 bytes) — Initialize IPC system (once)
void os_ipc_init(void)
{
    if (*sIpcInitFlag == 0) {
        *sIpcInitFlag = 1;
        ipc_reset_cmd();
        *sIpcBuffers = 0;
        *(sIpcBuffers + 1) = 0;
        enable_ipc_interrupt(0x10);
    }
}

// ============================================================
// Power / FIFO wait
// ============================================================

// FUN_0203af48 @ 0x0203af48 (48 bytes) — Wait for power FIFO completion
void os_power_fifo_wait(int device_id)
{
    int result;
    do {
        result = power_request_fifo(0xC, device_id << 8, 0);
    } while (result != 0);
}

// Forward declaration
static void os_panic_shutdown(void);

// FUN_0203af78 @ 0x0203af78 (64 bytes) — Handle power state change
void os_handle_power_state(u32 param1, u32 param2)
{
    if ((param2 & 0x7F00) != 0x1000) {
        os_panic_shutdown();
        return;
    }
    *sDmaChannelPtr = 1;
}

// FUN_0203aeb4 @ 0x0203aeb4 (20 bytes) — Infinite panic/shutdown loop
static void os_panic_shutdown(void)
{
    for (;;) {
        panic_check1();
        panic_check2();
    }
}

// ============================================================
// IRQ state management
// ============================================================

// FUN_0203b160 @ 0x0203b160 (44 bytes) — Initialize 9 IRQ state entries
void os_irq_state_init(void)
{
    int i;

    *sIrqControlPtr = 0;
    for (i = 0; i < 9; i++) {
        sIrqStateTable[i] = 0;
    }
}

// ============================================================
// DMA channel wrappers (all dispatch through dma_command)
// ============================================================

// FUN_0203bd84 @ 0x0203bd84 (36 bytes) — Initialize DMA channels 3 and 0
void dma_channels_init(void)
{
    dma_channel_enable(3);
    dma_channel_reset(0);
}

// FUN_0203be00 @ 0x0203be00 (44 bytes) — DMA command 0x16 (22)
void dma_cmd_22(u32 param1)
{
    dma_command(0x16, param1, 0, 0, 0);
}

// FUN_0203be2c @ 0x0203be2c (52 bytes) — DMA command 7 with combined parameters
void dma_cmd_7(u32 param1, u32 param2, u32 param3, u32 param4, int param5)
{
    dma_command(7, param1 | ((u32)param5 << 24), param2, param3, param4);
}

// FUN_0203be60 @ 0x0203be60 (52 bytes) — DMA command 6
void dma_cmd_6(u32 param1, u32 param2, u32 param3, u32 param4)
{
    dma_command(6, param1, param2, param3, param4);
}

// FUN_0203be94 @ 0x0203be94 (52 bytes) — DMA command 0x19 (25)
void dma_cmd_25(u32 param1, u32 param2, u32 param3, u32 param4)
{
    dma_command(0x19, param1, param2, param3, param4);
}

// FUN_0203bec8 @ 0x0203bec8 (44 bytes) — DMA command 0x20 (32)
void dma_cmd_32(u32 param1, u32 param2)
{
    dma_command(0x20, param1, param2, 0, 0);
}

// FUN_0203bef4 @ 0x0203bef4 (44 bytes) — DMA command 0x1F (31)
void dma_cmd_31(u32 param1, u32 param2)
{
    dma_command(0x1F, param1, param2, 0, 0);
}

// FUN_0203bf20 @ 0x0203bf20 (44 bytes) — DMA command 0x1E (30)
void dma_cmd_30(u32 param1, u32 param2)
{
    dma_command(0x1E, param1, param2, 0, 0);
}

// FUN_0203bfa4 @ 0x0203bfa4 (44 bytes) — DMA command 0x15 (21)
void dma_cmd_21(u32 param1, u32 param2)
{
    dma_command(0x15, param1, param2, 0, 0);
}

// FUN_0203bfd0 @ 0x0203bfd0 (52 bytes) — DMA command 0x14 (20)
void dma_cmd_20(u32 param1, u32 param2, u32 param3)
{
    dma_command(0x14, param1, param2, param3, 0);
}

// FUN_0203c004 @ 0x0203c004 (44 bytes) — DMA command 0x1B (27)
void dma_cmd_27(u32 param1, u32 param2)
{
    dma_command(0x1B, param1, param2, 0, 0);
}

// FUN_0203c030 @ 0x0203c030 (44 bytes) — DMA command 0x1A (26)
void dma_cmd_26(u32 param1, u32 param2)
{
    dma_command(0x1A, param1, param2, 0, 0);
}

// FUN_0203c05c @ 0x0203c05c (68 bytes) — DMA command 0x12 (18) with aux param
void dma_cmd_18(u32 param1, u32 param2, u32 param3, u32 param4, u32 param5)
{
    u32 aux = dma_aux_param(param1, param4, param5);
    dma_command(0x12, param1, param2, param3, aux);
}

// FUN_0203c0a0 @ 0x0203c0a0 (76 bytes) — DMA command 0x11 (17) with bit-packed flags
void dma_cmd_17(int f1, int f2, u32 param3, u32 param4, int f3, int f4, int f5)
{
    u32 flags = ((u32)f1 << 31) | ((u32)f2 << 30) | ((u32)f3 << 29) |
                ((u32)f4 << 28) | ((u32)f5 << 27);
    dma_command(0x11, param3, param4, flags, 0);
}

// ============================================================
// IPC function pointer dispatch
// ============================================================

// FUN_0203c218 @ 0x0203c218 (20 bytes) — IPC send message
void os_ipc_send(u32 param1, u32 param2)
{
    (*sIpcHandlerFn)(param1, 4, param2, 1);
}

// FUN_0203c230 @ 0x0203c230 (20 bytes) — IPC receive message
void os_ipc_recv(u32 param1, u32 param2)
{
    (*sIpcSendFn)(param1, 6, param2, 2);
}

// FUN_0203c2d4 @ 0x0203c2d4 (12 bytes) — Synchronous IPC call
void os_ipc_sync(void)
{
    (*sIpcSyncFn)(*sIpcSyncParam);
}

// FUN_0203c2e8 @ 0x0203c2e8 (12 bytes) — Asynchronous IPC call
void os_ipc_async(void)
{
    (*sIpcAsyncFn)(*sIpcAsyncParam);
}

// ============================================================
// Timer dispatch
// ============================================================

// FUN_0203f378 @ 0x0203f378 (24 bytes) — Timer dispatch via function pointer
void os_timer_dispatch(u32 param1, u32 param2)
{
    (*sTimerFn)(*sTimerParam, param1, param2);
}

// ============================================================
// Priority / task management
// ============================================================

// FUN_0203cbfc @ 0x0203cbfc (28 bytes) — Increment priority counter for task
void os_priority_increment(int task_id)
{
    u32 offset = (u32)task_id * 0xC + *sPriorityBase;
    *(u8 *)(offset + 8) = *(u8 *)(offset + 8) + 1;
}

// FUN_0203c4b8 @ 0x0203c4b8 (32 bytes) — Calculate free priority space
int os_get_free_priority(void)
{
    int used1 = get_priority_used1();
    int used2 = get_priority_used2();
    return (0x100 - used1) - used2;
}

// ============================================================
// Device / memory configuration
// ============================================================

// FUN_0203e328 @ 0x0203e328 (32 bytes) — Configure device memory pointers
void os_config_device_ptrs(u32 device, u32 ptr2, u32 ptr3)
{
    if (ptr3 == 0) {
        ptr2 = 0;
    } else if (ptr2 == 0) {
        ptr3 = 0;
    }
    *(u32 *)(device + 0x48) = ptr2;
    *(u32 *)(device + 0x4C) = ptr3;
}

// ============================================================
// CPU delay / spin-wait
// ============================================================

// FUN_0203aea8 @ 0x0203aea8 (12 bytes) — Spin-wait decrement loop (delay)
void os_delay_spin(u32 cycles)
{
    while (cycles >= 4) {
        cycles -= 4;
    }
}

// FUN_02041d80 @ 0x02041d80 (20 bytes) — Spin-wait for flag to clear
void os_wait_flag_clear(void)
{
    while (*sWaitFlag1 != 0) {
        /* busy-wait */
    }
}

// FUN_02041de8 @ 0x02041de8 (20 bytes) — Spin-wait for flag to change
void os_wait_flag_set(void)
{
    while (*sWaitFlag2 == 1) {
        /* busy-wait */
    }
}
