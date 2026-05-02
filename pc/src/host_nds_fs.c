/*
 * host_nds_fs.c — HOST_PORT NDS filesystem implementation
 *
 * Implements the NitroSDK FS primitive functions by reading directly from
 * the memory-mapped NDS ROM.  This replaces the async DMA/card-I/O machinery
 * that the original NitroSDK uses, making all FS operations synchronous.
 *
 * The game code (sdk_fs.c, gx_util.c, os_core_a.c, map_control.c) calls
 * these primitives expecting them to fill/read FSFile-compatible buffers.
 * We maintain a side table keyed by buffer address to track file state
 * without needing to perfectly match the NitroSDK struct layout.
 */
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "nds_rom.h"

/* ================================================================== */
/*  NDS ROM directory table (parsed from FNT at init time)            */
/* ================================================================== */

#define MAX_DIRS 64

static struct {
    uint32_t first_file_id;
    uint16_t file_count;  /* not strictly needed but useful for bounds check */
} g_fnt_dirs[MAX_DIRS];
static int g_fnt_dir_count = 0;

/* FAT entries: (rom_start, rom_end) for each file */
#define MAX_FAT_ENTRIES 128
static struct { uint32_t start, end; } g_fat[MAX_FAT_ENTRIES];
static int g_fat_count = 0;

/* ================================================================== */
/*  Host file handle table                                            */
/* ================================================================== */

#define MAX_OPEN_FILES 32

typedef struct {
    uintptr_t buf_addr;    /* address of the game's FSFile buffer (key) */
    uint32_t  fat_id;      /* FAT entry this file refers to */
    uint32_t  rom_start;   /* ROM offset of file data start */
    uint32_t  rom_end;     /* ROM offset of file data end */
    uint32_t  position;    /* current read offset (relative to rom_start) */
    int       open;        /* 1 if this slot is in use */
} HostFSFile;

static HostFSFile g_open_files[MAX_OPEN_FILES];

/* ================================================================== */
/*  Mounted archive table (NARC sub-filesystems)                      */
/* ================================================================== */

#define MAX_ARCHIVES     8
#define MAX_SUB_FILES  512

typedef struct {
    uintptr_t ctx_addr;     /* address of the game's archive struct */
    uint32_t  data_offset;  /* ROM offset of the GMIF/data section */
    uint32_t  sub_count;    /* number of sub-files */
    struct { uint32_t start, end; } sub_fat[MAX_SUB_FILES];
    int       mounted;
} MountedArchive;

static MountedArchive g_archives[MAX_ARCHIVES];

/* ================================================================== */
/*  Scratch buffer for FUN_0203fa08's copy-to-(buf+4) pattern         */
/* ================================================================== */

static uint8_t g_scratch_buf[1024 * 1024]; /* 1 MB scratch for file reads */

/* ================================================================== */
/*  Init: parse FAT/FNT from the ROM                                  */
/* ================================================================== */

static uint32_t rd32(const uint8_t *p) {
    return (uint32_t)p[0] | ((uint32_t)p[1] << 8) |
           ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
}
static uint16_t rd16(const uint8_t *p) {
    return (uint16_t)p[0] | ((uint16_t)p[1] << 8);
}

void host_nds_fs_init(void)
{
    const uint8_t *rom = rom_data();
    if (!rom || rom_size() < 0x200) {
        fprintf(stderr, "[host_fs] No ROM loaded — FS disabled\n");
        return;
    }

    /* Parse FAT */
    uint32_t fat_off = rd32(rom + 0x48);
    uint32_t fat_sz  = rd32(rom + 0x4C);
    g_fat_count = (int)(fat_sz / 8);
    if (g_fat_count > MAX_FAT_ENTRIES) g_fat_count = MAX_FAT_ENTRIES;

    for (int i = 0; i < g_fat_count; i++) {
        const uint8_t *e = rom + fat_off + i * 8;
        g_fat[i].start = rd32(e);
        g_fat[i].end   = rd32(e + 4);
    }

    /* Parse FNT directory table */
    uint32_t fnt_off = rd32(rom + 0x40);
    const uint8_t *fnt = rom + fnt_off;

    /* Root entry: sub_table_offset(4) + first_file_id(2) + num_dirs(2) */
    g_fnt_dir_count = rd16(fnt + 6);
    if (g_fnt_dir_count > MAX_DIRS) g_fnt_dir_count = MAX_DIRS;

    for (int d = 0; d < g_fnt_dir_count; d++) {
        const uint8_t *de = fnt + d * 8;
        g_fnt_dirs[d].first_file_id = rd16(de + 4);

        /* Count files by walking the subtable */
        uint32_t sub_off = rd32(de);
        const uint8_t *sub = fnt + sub_off;
        uint16_t count = 0;
        while (*sub != 0) {
            uint8_t b = *sub++;
            if (b <= 0x7F) {
                sub += b;  /* skip filename */
                count++;
            } else {
                sub += (b - 0x80) + 2; /* skip dirname + dir_id */
            }
        }
        g_fnt_dirs[d].file_count = count;
    }

    /* Clear file and archive tables */
    memset(g_open_files, 0, sizeof(g_open_files));
    memset(g_archives,   0, sizeof(g_archives));

    fprintf(stderr, "[host_fs] Initialized: %d FAT entries, %d directories\n",
            g_fat_count, g_fnt_dir_count);
}

/* ================================================================== */
/*  Internal helpers                                                   */
/* ================================================================== */

static HostFSFile *find_file_by_buf(uintptr_t addr)
{
    for (int i = 0; i < MAX_OPEN_FILES; i++) {
        if (g_open_files[i].open && g_open_files[i].buf_addr == addr)
            return &g_open_files[i];
    }
    return NULL;
}

static HostFSFile *alloc_file(uintptr_t addr)
{
    /* Reuse existing entry for same address */
    HostFSFile *f = find_file_by_buf(addr);
    if (f) return f;
    /* Find free slot */
    for (int i = 0; i < MAX_OPEN_FILES; i++) {
        if (!g_open_files[i].open) {
            g_open_files[i].buf_addr = addr;
            g_open_files[i].open = 1;
            return &g_open_files[i];
        }
    }
    fprintf(stderr, "[host_fs] WARNING: no free file slots\n");
    return NULL;
}

static void free_file(uintptr_t addr)
{
    HostFSFile *f = find_file_by_buf(addr);
    if (f) f->open = 0;
}

static MountedArchive *find_archive_by_ctx(uintptr_t ctx)
{
    for (int i = 0; i < MAX_ARCHIVES; i++) {
        if (g_archives[i].mounted && g_archives[i].ctx_addr == ctx)
            return &g_archives[i];
    }
    return NULL;
}

/* Resolve (dir_id, file_pos) → FAT index */
static int resolve_fat_id(uint32_t dir_id, uint32_t file_pos)
{
    if ((int)dir_id < g_fnt_dir_count) {
        int fid = (int)g_fnt_dirs[dir_id].first_file_id + (int)file_pos;
        if (fid >= 0 && fid < g_fat_count)
            return fid;
    }
    /* Fallback: treat file_pos as direct FAT index */
    if ((int)file_pos >= 0 && (int)file_pos < g_fat_count)
        return (int)file_pos;
    return -1;
}

/* Parse NARC header and populate a MountedArchive */
static int parse_narc(uint32_t rom_off, MountedArchive *arc)
{
    const uint8_t *rom = rom_data();
    if (!rom) return 0;

    const uint8_t *p = rom + rom_off;

    /* NARC header: "NARC" 0xFFFE0001 fileSize headerSize */
    if (memcmp(p, "NARC", 4) != 0) {
        /* Not a NARC — might be raw data. Treat as single-file archive */
        fprintf(stderr, "[host_fs] Not a NARC at 0x%X (magic: %.4s) — treating as raw\n",
                rom_off, p);
        arc->data_offset = rom_off;
        arc->sub_count = 1;
        /* Find FAT entry that contains this offset */
        for (int i = 0; i < g_fat_count; i++) {
            if (g_fat[i].start == rom_off) {
                arc->sub_fat[0].start = 0;
                arc->sub_fat[0].end   = g_fat[i].end - g_fat[i].start;
                break;
            }
        }
        return 1;
    }

    uint32_t narc_size = rd32(p + 8);
    uint16_t hdr_size  = rd16(p + 12);
    (void)narc_size;

    /* BTAF section (sub-FAT) */
    const uint8_t *btaf = p + hdr_size;
    if (memcmp(btaf, "BTAF", 4) != 0) {
        fprintf(stderr, "[host_fs] NARC missing BTAF at 0x%X\n", rom_off);
        return 0;
    }
    uint32_t btaf_size  = rd32(btaf + 4);
    uint32_t sub_count  = rd32(btaf + 8);
    (void)btaf_size;

    if (sub_count > MAX_SUB_FILES) sub_count = MAX_SUB_FILES;
    arc->sub_count = sub_count;

    for (uint32_t i = 0; i < sub_count; i++) {
        arc->sub_fat[i].start = rd32(btaf + 12 + i * 8);
        arc->sub_fat[i].end   = rd32(btaf + 12 + i * 8 + 4);
    }

    /* BTNF section (skip) */
    const uint8_t *btnf = btaf + rd32(btaf + 4);
    /* GMIF/FIMG section (file data) */
    const uint8_t *gmif = btnf + rd32(btnf + 4);
    if (memcmp(gmif, "GMIF", 4) != 0 && memcmp(gmif, "FIMG", 4) != 0) {
        fprintf(stderr, "[host_fs] NARC missing GMIF/FIMG at 0x%X\n", rom_off);
        return 0;
    }
    arc->data_offset = (uint32_t)(gmif + 8 - rom); /* skip GMIF header (8 bytes) */

    fprintf(stderr, "[host_fs] Mounted NARC at 0x%X: %u sub-files, data at 0x%X\n",
            rom_off, sub_count, arc->data_offset);
    return 1;
}

/* ================================================================== */
/*  FS primitive implementations                                       */
/* ================================================================== */

/*
 * FUN_0203fb40 — FS_OpenFileFast
 *
 * Opens a file by (dir_id, file_pos). Fills the 48-byte FSFile buffer.
 * Returns 1 on success, 0 on failure.
 *
 * Called as: FUN_0203fb40(buf, dir_id, file_pos)
 */
int FUN_0203fb40(uintptr_t buf, uint32_t dir_id, uint32_t file_pos)
{
    if (!rom_data() || g_fat_count == 0) return 0;

    int fid = resolve_fat_id(dir_id, file_pos);
    if (fid < 0) {
        fprintf(stderr, "[host_fs] OpenFileFast: cannot resolve dir=%u pos=%u\n",
                dir_id, file_pos);
        return 0;
    }

    uint32_t rom_start = g_fat[fid].start;
    uint32_t rom_end   = g_fat[fid].end;

    /* Track in our side table */
    HostFSFile *f = alloc_file(buf);
    if (!f) return 0;
    f->fat_id    = (uint32_t)fid;
    f->rom_start = rom_start;
    f->rom_end   = rom_end;
    f->position  = 0;

    /* Fill the game's FSFile buffer at known offsets:
     * +0x04: scratch buffer pointer (used by FUN_0203fa08 as copy dst)
     * +0x08: source identifier (0 = ROM root)
     * +0x0C: flags (0x41 = active + completed)
     * +0x14: result (0 = success)
     * +0x20: file ID
     * +0x24: data top (ROM start)
     * +0x28: data bottom (ROM end)
     * +0x2C: position (0)
     * +0x30: remaining (size)
     */
    uint8_t *b = (uint8_t *)buf;
    memset(b, 0, 48);
    *(uint32_t *)(b + 0x04) = (uint32_t)(uintptr_t)g_scratch_buf;
    *(uint32_t *)(b + 0x0C) = 0x41; /* active + completed */
    *(uint32_t *)(b + 0x14) = 0;    /* success */
    *(uint32_t *)(b + 0x20) = (uint32_t)fid;
    *(uint32_t *)(b + 0x24) = rom_start;
    *(uint32_t *)(b + 0x28) = rom_end;
    *(uint32_t *)(b + 0x2C) = 0;
    *(uint32_t *)(b + 0x30) = rom_end - rom_start;

    fprintf(stderr, "[host_fs] Opened FAT[%d] (dir=%u,pos=%u): 0x%X..0x%X (%u bytes)\n",
            fid, dir_id, file_pos, rom_start, rom_end, rom_end - rom_start);
    return 1;
}

/*
 * FUN_0203fd64 — get file image bounds
 *
 * Writes (top, bottom) ROM offsets to out[0] and out[4].
 * Called as: FUN_0203fd64(&local_58, fsfile)
 */
void FUN_0203fd64(uint32_t *out, uintptr_t buf)
{
    uint8_t *b = (uint8_t *)buf;
    out[0] = *(uint32_t *)(b + 0x24);  /* top */
    out[1] = *(uint32_t *)(b + 0x28);  /* bottom */
}

/*
 * FUN_0203fd8c — seek/reset file position
 *
 * Resets the file handle's read position. Some callers use this to
 * "activate" the file after opening.
 */
void FUN_0203fd8c(uintptr_t buf)
{
    HostFSFile *f = find_file_by_buf(buf);
    if (f) f->position = 0;

    uint8_t *b = (uint8_t *)buf;
    *(uint32_t *)(b + 0x2C) = 0; /* reset position */
}

/*
 * FUN_0203fdd0 — get file data size
 *
 * Returns the size of the file's data region.
 */
int FUN_0203fdd0(uintptr_t buf)
{
    uint8_t *b = (uint8_t *)buf;
    uint32_t top = *(uint32_t *)(b + 0x24);
    uint32_t bot = *(uint32_t *)(b + 0x28);
    return (int)(bot - top);
}

/*
 * FUN_0203f2a0 — initialize a read context (FSFile-like, 68+ bytes)
 *
 * Zero-initializes the context and marks it ready for use.
 */
void FUN_0203f2a0(uint32_t *ctx)
{
    memset(ctx, 0, 68);
    /* Set minimal flags: not active yet */
}

/*
 * FUN_0203efa0 — setup a read window on a context
 *
 * Associates the context with a ROM region [start..end).
 * Returns 1 on success.
 *
 * Called as: FUN_0203efa0(ctx, start_offset, end_offset)
 */
int FUN_0203efa0(uintptr_t ctx, uint32_t start, uint32_t end)
{
    if (!rom_data()) return 0;
    if (start >= end || end > (uint32_t)rom_size()) {
        fprintf(stderr, "[host_fs] ReadWindow: bad range 0x%X..0x%X (rom_size=0x%zX)\n",
                start, end, rom_size());
        return 0;
    }

    uint8_t *c = (uint8_t *)ctx;
    *(uint32_t *)(c + 0x0C) = 0x51; /* active + completed + read-ok */
    *(uint32_t *)(c + 0x24) = start;
    *(uint32_t *)(c + 0x28) = end;
    *(uint32_t *)(c + 0x2C) = 0;    /* position = 0 */

    /* Also track in our side table */
    HostFSFile *f = alloc_file(ctx);
    if (f) {
        f->rom_start = start;
        f->rom_end   = end;
        f->position  = 0;
    }
    return 1;
}

/*
 * FUN_0203edcc — copy bytes from read context to destination
 *
 * Reads `size` bytes from the current position in the context's ROM
 * window and copies them to `dst`. Returns bytes actually copied.
 *
 * Called as: FUN_0203edcc(ctx, dst, size)
 */
int FUN_0203edcc(uintptr_t ctx, uintptr_t dst, int size)
{
    if (size <= 0 || !rom_data() || !dst) return 0;

    uint8_t *c = (uint8_t *)ctx;
    uint32_t start = *(uint32_t *)(c + 0x24);
    uint32_t end   = *(uint32_t *)(c + 0x28);
    uint32_t pos   = *(uint32_t *)(c + 0x2C);

    uint32_t available = (start + pos < end) ? (end - start - pos) : 0;
    uint32_t to_copy = ((uint32_t)size < available) ? (uint32_t)size : available;

    if (to_copy > 0) {
        memcpy((void *)dst, rom_data() + start + pos, to_copy);
        *(uint32_t *)(c + 0x2C) = pos + to_copy;

        /* Update side table too */
        HostFSFile *f = find_file_by_buf(ctx);
        if (f) f->position += to_copy;
    }

    return (int)to_copy;
}

/*
 * FUN_0203ef08 — close/release a read context
 *
 * Marks the context as inactive and frees the side-table entry.
 */
void FUN_0203ef08(uintptr_t ctx)
{
    uint8_t *c = (uint8_t *)ctx;
    *(uint32_t *)(c + 0x0C) = 0; /* clear flags */
    free_file(ctx);
}

/*
 * FUN_0203f864 — close an opened file
 *
 * Similar to FUN_0203ef08 but for FSFile buffers opened by FUN_0203fb40.
 */
void FUN_0203f864(uintptr_t buf)
{
    uint8_t *b = (uint8_t *)buf;
    *(uint32_t *)(b + 0x0C) = 0;
    free_file(buf);
}

/*
 * FUN_0203faa8 — mount an opened file as a NARC archive
 *
 * Parses the NARC header from the file described by `file_buf`
 * and registers the archive in the `archive_ctx` struct.
 * Returns 1 on success.
 *
 * Called as: FUN_0203faa8(file_buf, archive_ctx)
 */
int FUN_0203faa8(uintptr_t file_buf, uintptr_t archive_ctx)
{
    HostFSFile *f = find_file_by_buf(file_buf);
    if (!f) {
        /* Try from the buffer itself */
        uint8_t *b = (uint8_t *)file_buf;
        uint32_t start = *(uint32_t *)(b + 0x24);
        uint32_t end   = *(uint32_t *)(b + 0x28);
        if (start >= end) return 0;

        /* Find free archive slot */
        for (int i = 0; i < MAX_ARCHIVES; i++) {
            if (!g_archives[i].mounted) {
                g_archives[i].ctx_addr = archive_ctx;
                g_archives[i].mounted  = 1;
                return parse_narc(start, &g_archives[i]);
            }
        }
        return 0;
    }

    /* Find free archive slot */
    for (int i = 0; i < MAX_ARCHIVES; i++) {
        if (!g_archives[i].mounted) {
            g_archives[i].ctx_addr = archive_ctx;
            g_archives[i].mounted  = 1;
            return parse_narc(f->rom_start, &g_archives[i]);
        }
    }
    fprintf(stderr, "[host_fs] WARNING: no free archive slots\n");
    return 0;
}

/*
 * FUN_0203ed60 — seek within a bound file context
 *
 * Sets the read position of the context to `offset`. The context
 * is typically embedded in a game object at some fixed offset.
 *
 * Called as: FUN_0203ed60(ctx, absolute_rom_offset, flags)
 */
void FUN_0203ed60(uintptr_t ctx, uint32_t offset, uint32_t flags)
{
    (void)flags;
    uint8_t *c = (uint8_t *)ctx;

    /* The offset is typically an absolute ROM address.
     * Store it so FUN_0203edcc can read from there. */
    *(uint32_t *)(c + 0x24) = offset;  /* source start */
    *(uint32_t *)(c + 0x28) = (uint32_t)rom_size(); /* source end = entire ROM */
    *(uint32_t *)(c + 0x2C) = 0;       /* position = 0 */
    *(uint32_t *)(c + 0x0C) = 0x51;    /* active */

    /* Side table tracking */
    HostFSFile *f = alloc_file(ctx);
    if (f) {
        f->rom_start = offset;
        f->rom_end   = (uint32_t)rom_size();
        f->position  = 0;
    }
}

/*
 * FUN_0203ef50 — post-open setup
 *
 * Called after FUN_0203fb40 in some code paths. On NDS this would
 * configure DMA transfer parameters. We just mark the context active.
 */
int FUN_0203ef50(uintptr_t ctx, uint32_t param)
{
    (void)param;
    uint8_t *c = (uint8_t *)ctx;
    *(uint32_t *)(c + 0x0C) |= 0x10; /* mark transfer configured */
    return 1;
}

/*
 * FUN_0203f2c8 — flush/sync archives
 *
 * Called to ensure all pending archive operations complete.
 * On host, everything is synchronous, so this is a no-op.
 */
void FUN_0203f2c8(void)
{
    /* No-op: all operations are synchronous on host */
}

/*
 * FUN_0203e328 — clear/reset FS state flags
 */
void FUN_0203e328(uintptr_t ctx)
{
    uint8_t *c = (uint8_t *)ctx;
    uint16_t flags = *(uint16_t *)c;
    flags &= ~0x40;
    *(uint16_t *)c = flags;
    /* The original calls FUN_0203e5a0 then checks result.
     * On host, we just clear the flag and return. */
}

/*
 * FUN_0203f740 — verify file table entry
 *
 * Checks that the FSFile buffer contains valid data.
 * Returns 1 if valid.
 */
int FUN_0203f740(uintptr_t buf)
{
    uint8_t *b = (uint8_t *)buf;
    uint32_t top = *(uint32_t *)(b + 0x24);
    uint32_t bot = *(uint32_t *)(b + 0x28);
    return (top < bot && bot <= (uint32_t)rom_size()) ? 1 : 0;
}

/*
 * FUN_0203f75c — extended file verification / prepare for archive read
 *
 * Verifies the file and prepares it for archive mounting.
 * Returns 1 if the file is a valid archive.
 */
int FUN_0203f75c(uintptr_t buf)
{
    /* For HOST_PORT, we trust the ROM data */
    return FUN_0203f740(buf);
}

/*
 * FUN_0203f398 — release timer / FS sync resource
 *
 * On NDS this releases a timer used for card access timing.
 * No-op on host.
 */
void FUN_0203f398(void)
{
    /* No-op */
}

/*
 * FUN_0203eddc — variant of edcc / read helper
 *
 * 12-byte function, likely a thin wrapper.
 */
int FUN_0203eddc(void)
{
    return 0;
}

/* ================================================================== */
/*  HOST_PORT overrides for sdk_fs.c functions with int-as-pointer     */
/*  issues that crash on 64-bit hosts.                                 */
/* ================================================================== */

/*
 * FUN_0203fa08 — write file data (HOST_PORT version)
 *
 * The original in sdk_fs.c takes `int param_1` which truncates 64-bit
 * pointers on the host. This replacement uses uintptr_t and reads
 * directly from the ROM via the side table.
 */
uint32_t FUN_0203fa08(uintptr_t fsfile)
{
    HostFSFile *f = find_file_by_buf(fsfile);
    if (!f || !rom_data()) {
        fprintf(stderr, "[host_fs] FUN_0203fa08: no open file for buf %p\n", (void *)fsfile);
        return 0;
    }

    uint32_t size = f->rom_end - f->rom_start;
    if (size > sizeof(g_scratch_buf)) {
        fprintf(stderr, "[host_fs] FUN_0203fa08: file too large (%u > %u)\n",
                size, (uint32_t)sizeof(g_scratch_buf));
        size = sizeof(g_scratch_buf);
    }

    memcpy(g_scratch_buf, rom_data() + f->rom_start, size);

    /* Also set the scratch pointer in the buffer so callers that read +4 get it */
    uint8_t *b = (uint8_t *)fsfile;
    *(uintptr_t *)(b + 0x04) = (uintptr_t)g_scratch_buf;

    fprintf(stderr, "[host_fs] FUN_0203fa08: read %u bytes from ROM 0x%X into scratch\n",
            size, f->rom_start);
    return 1;
}

/*
 * FUN_0203f6e4 — verify and finalize file (HOST_PORT version)
 *
 * The original in sdk_fs.c passes auStack_30 through (int) cast which
 * truncates the stack pointer on 64-bit. This replacement uses proper
 * uintptr_t throughout.
 */
uint32_t FUN_0203f6e4(uint32_t dir_id, uint32_t file_pos)
{
    uint8_t fsfile[64]; /* slightly oversize for safety */
    memset(fsfile, 0, sizeof(fsfile));

    int rc = FUN_0203fb40((uintptr_t)fsfile, dir_id, file_pos);
    if (!rc) return 0;

    rc = FUN_0203fa08((uintptr_t)fsfile);
    if (!rc) return 0;

    FUN_0203f864((uintptr_t)fsfile);
    return 1;
}

/*
 * FUN_0203fc54 — read file table entry (HOST_PORT version)
 *
 * The original takes `int param_1` as a pointer and casts stack buffers
 * through (int). This replacement uses proper pointer-width types.
 *
 * param_1 = destination FSFile buffer (pointer)
 * param_2 = archive selector (0=root, nonzero=archive)
 * param_3 = sub-file index within archive
 * param_4, param_5, param_6 = archive source bounds
 * param_7, param_8 = alternate bounds when param_2 != 0
 */
uint32_t FUN_0203fc54(uintptr_t param_1, intptr_t param_2, intptr_t param_3,
                       uint32_t param_4, intptr_t param_5, uint32_t param_6,
                       intptr_t param_7, uint32_t param_8)
{
    /* Select which bounds to use */
    if (param_2 != 0) {
        param_5 = param_7;
        param_6 = param_8;
    }

    if (param_6 <= (uint32_t)(param_3 * 0x20)) {
        return 0;
    }

    /* The function reads a 32-byte FAT entry from (param_5 + param_3*0x20)
     * which is a ROM offset. On HOST_PORT we read directly from ROM. */
    uint32_t entry_off = (uint32_t)param_5 + (uint32_t)param_3 * 0x20;
    if (!rom_data() || entry_off + 0x20 > rom_size()) {
        fprintf(stderr, "[host_fs] FUN_0203fc54: entry offset 0x%X out of range\n", entry_off);
        return 0;
    }

    /* Copy the 32-byte entry into param_1 */
    memcpy((void *)param_1, rom_data() + entry_off, 0x20);

    /* Set file_id at +0x20 */
    *(intptr_t *)(param_1 + 0x20) = param_2;

    /* Get top/bottom from the entry we just read (offsets +0x08 and +0x0C in entry) */
    uint8_t *entry = (uint8_t *)param_1;
    uint32_t data_start = rd32(entry + 0x08);
    uint32_t data_end   = rd32(entry + 0x0C);

    /* Store file bounds */
    *(uint32_t *)(param_1 + 0x24) = data_start;
    *(uint32_t *)(param_1 + 0x28) = data_end - data_start;

    fprintf(stderr, "[host_fs] FUN_0203fc54: read entry[%d] from 0x%X, data=0x%X..0x%X\n",
            (int)param_3, entry_off, data_start, data_end);
    return 1;
}
