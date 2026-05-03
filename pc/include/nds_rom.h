#ifndef NDS_ROM_H
#define NDS_ROM_H
#include <stdint.h>
#include <stddef.h>
int            rom_load(const char *path);
const uint8_t *rom_data(void);
size_t         rom_size(void);
int            rom_read_file(int file_id, void *dst, size_t max);
int            rom_read_overlay(int overlay_id);
#endif
