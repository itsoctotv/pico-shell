#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/flash.h"
#include "hardware/sync.h"
#include "lfs.h"
#include "fs.h"

// --- HARD-CODED FLASH LAYOUT ---
// 1MB for the Program (0x000000 to 0x0FFFFF)
// 3MB for the Filesystem (0x100000 to 0x3FFFFF)

#define FS_OFFSET (1 * 1024 * 1024)         // Start at 1MB mark
#define FS_SIZE   (3 * 1024 * 1024)         // Use exactly 3MB
#define LOOKAHEAD_SIZE 16
#define CACHE_SIZE     256

// LittleFS handle and config
struct lfs_config cfg;
lfs_t lfs;              
char ENV_CURRENT_PATH[128] = "/"; 
// --- LittleFS Hardware Abstraction Functions ---

int pico_read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size) {
    // Read directly from the Memory Mapped Flash
    uint32_t addr = XIP_BASE + FS_OFFSET + (block * c->block_size) + off;
    memcpy(buffer, (void *)addr, size);
    return LFS_ERR_OK;
}

int pico_prog(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size) {
    // Write requires the relative offset from Flash start
    uint32_t addr = FS_OFFSET + (block * c->block_size) + off;
    uint32_t ints = save_and_disable_interrupts();
    flash_range_program(addr, buffer, size);
    restore_interrupts(ints);
    return LFS_ERR_OK;
}

int pico_erase(const struct lfs_config *c, lfs_block_t block) {
    uint32_t addr = FS_OFFSET + (block * c->block_size);
    uint32_t ints = save_and_disable_interrupts();
    flash_range_erase(addr, c->block_size);
    restore_interrupts(ints);
    return LFS_ERR_OK;
}

int pico_sync(const struct lfs_config *c) {
    return LFS_ERR_OK;
}


int initFS() {
    //  LittleFS configuration
    cfg.read  = pico_read;
    cfg.prog  = pico_prog;
    cfg.erase = pico_erase;
    cfg.sync  = pico_sync;

    cfg.read_size      = 1;
    cfg.prog_size      = FLASH_PAGE_SIZE;   // 256 bytes
    cfg.block_size     = FLASH_SECTOR_SIZE; // 4096 bytes
    cfg.block_count    = FS_SIZE / FLASH_SECTOR_SIZE;
    cfg.cache_size     = CACHE_SIZE;
    cfg.lookahead_size = LOOKAHEAD_SIZE;
    cfg.block_cycles   = 500; // Wear leveling


	
    // Mount the filesystem
    printf("Mounting LittleFS at 0x%X...\n", FS_OFFSET);
    int err = lfs_mount(&lfs, &cfg);

    // Format if necessary (first run or corruption)
    if (err) {
        printf("Mount failed (%d). Formatting new filesystem...\n", err);
        lfs_format(&lfs, &cfg);
        lfs_mount(&lfs, &cfg);
    }
	

    printf("Filesystem initalized.\n");
    

}
