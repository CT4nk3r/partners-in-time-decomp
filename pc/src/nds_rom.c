/*
 * nds_rom.c - NDS ROM loader for PC port
 *
 * Attempts to mmap baserom.nds from the roms/ directory next to the
 * executable. Parses the NDS ROM header to expose ARM9 binary location,
 * FAT/FNT tables, and individual file read. If the ROM is missing, the
 * program still runs (just with no game data).
 */
#include "nds_rom.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#endif

#define NDS_HDR_ARM9_OFFSET    0x020
#define NDS_HDR_ARM9_ENTRY     0x024
#define NDS_HDR_ARM9_RAM_ADDR  0x028
#define NDS_HDR_ARM9_SIZE      0x02C
#define NDS_HDR_FNT_OFFSET     0x040
#define NDS_HDR_FNT_SIZE       0x044
#define NDS_HDR_FAT_OFFSET     0x048
#define NDS_HDR_FAT_SIZE       0x04C

static uint8_t  *g_rom_data  = NULL;
static size_t    g_rom_size  = 0;
static int       g_rom_valid = 0;

#ifdef _WIN32
static HANDLE g_file_handle    = INVALID_HANDLE_VALUE;
static HANDLE g_mapping_handle = NULL;
#endif

static uint32_t read_le32(const uint8_t *p) {
    return (uint32_t)p[0] | ((uint32_t)p[1] << 8) |
           ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
}

int rom_load(const char *path) {
#ifdef _WIN32
    g_file_handle = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ,
                                NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (g_file_handle == INVALID_HANDLE_VALUE) {
        fprintf(stderr,
            "[rom] Place your legal Mario & Luigi: Partners in Time .nds dump at roms/baserom.nds to load game data\n");
        return 0;
    }
    LARGE_INTEGER file_size;
    if (!GetFileSizeEx(g_file_handle, &file_size)) {
        CloseHandle(g_file_handle);
        g_file_handle = INVALID_HANDLE_VALUE;
        return 0;
    }
    g_rom_size = (size_t)file_size.QuadPart;

    g_mapping_handle = CreateFileMappingA(g_file_handle, NULL,
                                          PAGE_READONLY, 0, 0, NULL);
    if (!g_mapping_handle) {
        CloseHandle(g_file_handle);
        g_file_handle = INVALID_HANDLE_VALUE;
        return 0;
    }
    g_rom_data = (uint8_t*)MapViewOfFile(g_mapping_handle,
                                          FILE_MAP_READ, 0, 0, 0);
    if (!g_rom_data) {
        CloseHandle(g_mapping_handle);
        CloseHandle(g_file_handle);
        g_mapping_handle = NULL;
        g_file_handle = INVALID_HANDLE_VALUE;
        return 0;
    }
#else
    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        fprintf(stderr,
            "[rom] Place your legal Mario & Luigi: Partners in Time .nds dump at roms/baserom.nds to load game data\n");
        return 0;
    }
    struct stat st;
    fstat(fd, &st);
    g_rom_size = (size_t)st.st_size;
    g_rom_data = (uint8_t*)mmap(NULL, g_rom_size, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);
    if (g_rom_data == MAP_FAILED) { g_rom_data = NULL; return 0; }
#endif

    if (g_rom_size < 0x200) {
        fprintf(stderr, "[rom] File too small to be a valid NDS ROM\n");
        g_rom_valid = 0;
        return 1;
    }

    g_rom_valid = 1;
    fprintf(stdout, "[rom] Loaded %zu bytes from %s\n", g_rom_size, path);
    uint32_t arm9_off  = read_le32(g_rom_data + NDS_HDR_ARM9_OFFSET);
    uint32_t arm9_size = read_le32(g_rom_data + NDS_HDR_ARM9_SIZE);
    fprintf(stdout, "[rom] ARM9: offset=0x%08X size=0x%08X\n", arm9_off, arm9_size);
    return 1;
}

const uint8_t *rom_data(void) { return g_rom_data; }
size_t         rom_size(void) { return g_rom_size; }

int rom_read_file(int file_id, void *dst, size_t max) {
    if (!g_rom_valid || !g_rom_data || !dst) return -1;

    uint32_t fat_off  = read_le32(g_rom_data + NDS_HDR_FAT_OFFSET);
    uint32_t fat_size = read_le32(g_rom_data + NDS_HDR_FAT_SIZE);
    int num_files = (int)(fat_size / 8);

    if (file_id < 0 || file_id >= num_files) return -1;
    if (fat_off + (uint32_t)(file_id + 1) * 8 > g_rom_size) return -1;

    const uint8_t *fat_entry = g_rom_data + fat_off + file_id * 8;
    uint32_t start = read_le32(fat_entry);
    uint32_t end   = read_le32(fat_entry + 4);

    if (start >= end || end > g_rom_size) return -1;

    size_t file_size = (size_t)(end - start);
    size_t to_copy = (file_size < max) ? file_size : max;
    memcpy(dst, g_rom_data + start, to_copy);
    return (int)to_copy;
}

int rom_read_overlay(int overlay_id) {
    if (!g_rom_valid) return 0;
    (void)overlay_id;
    return 1;
}
