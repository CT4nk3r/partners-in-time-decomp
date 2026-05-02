/*
 * host_fnptr_resolver.h — public API for the NDS→host fnptr trampoline.
 */
#ifndef HOST_FNPTR_RESOLVER_H
#define HOST_FNPTR_RESOLVER_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void  host_fnptr_init(void);
void  host_fnptr_register(uint32_t nds_addr, void *host_fn);
void *host_fnptr_lookup(uint32_t nds_addr);
int   host_fnptr_count(void);

void  nds_call_2arg(uint32_t nds_addr, uintptr_t a, uintptr_t b);
void  nds_call_1arg(uint32_t nds_addr, uintptr_t a);

/* Reserved synthetic NDS address range for host-only trampolines. The
 * real ROM never produces code pointers here; we use it for fake nodes
 * whose vtable points at host C functions like host_draw_test_node. */
#define HOST_FNPTR_SYNTHETIC_BASE 0xFA000000u

#ifdef __cplusplus
}
#endif

#endif /* HOST_FNPTR_RESOLVER_H */
