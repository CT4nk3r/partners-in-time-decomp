#ifndef HOST_NDS_FS_H
#define HOST_NDS_FS_H

/*
 * host_nds_fs.h — HOST_PORT NDS filesystem layer
 *
 * Provides synchronous file I/O by reading directly from the memory-mapped
 * NDS ROM.  Call host_nds_fs_init() after rom_load() to parse the FAT/FNT.
 */

/* Initialize the FS layer by parsing FAT/FNT from the loaded ROM */
void host_nds_fs_init(void);

#endif /* HOST_NDS_FS_H */
