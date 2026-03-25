#ifndef W25Q128_H
#define W25Q128_H

#include "pico/stdlib.h"
#include "hardware/spi.h"

// --- Configuration ---
#define FLASH_SPI_PORT spi0
#define FLASH_PIN_MISO 16
#define FLASH_PIN_CS   17
#define FLASH_PIN_SCK  18
#define FLASH_PIN_MOSI 19

// --- Winbond Commands ---
#define CMD_WREN         0x06
#define CMD_READ         0x03
#define CMD_SECTOR_ERASE 0x20
#define CMD_PAGE_PROG    0x02
#define CMD_RDSR         0x05
#define CMD_JEDEC_ID     0x9F
#define CMD_ERASE_ALL	 0xC7

/**
 * Initialize SPI and GPIO for the Flash Chip
 * Speed: 10MHz (Safe for breadboards)
 */
static inline void flash_init() {
    spi_init(FLASH_SPI_PORT, 10 * 1000 * 1000);
    gpio_set_function(FLASH_PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(FLASH_PIN_SCK,  GPIO_FUNC_SPI);
    gpio_set_function(FLASH_PIN_MOSI, GPIO_FUNC_SPI);
    
    gpio_init(FLASH_PIN_CS);
    gpio_set_dir(FLASH_PIN_CS, GPIO_OUT);
    gpio_put(FLASH_PIN_CS, 1);
}

/**
 * Poll the Status Register until the Busy bit (0) is cleared
 */
static inline void flash_wait_ready() {
    uint8_t cmd = CMD_RDSR;
    uint8_t status;
    do {
        gpio_put(FLASH_PIN_CS, 0);
        spi_write_blocking(FLASH_SPI_PORT, &cmd, 1);
        spi_read_blocking(FLASH_SPI_PORT, 0, &status, 1);
        gpio_put(FLASH_PIN_CS, 1);
    } while (status & 0x01);
}
static inline bool flash_is_busy() {
    uint8_t cmd = CMD_RDSR;
    uint8_t status;
    
    gpio_put(FLASH_PIN_CS, 0);
    spi_write_blocking(FLASH_SPI_PORT, &cmd, 1);
    spi_read_blocking(FLASH_SPI_PORT, 0, &status, 1);
    gpio_put(FLASH_PIN_CS, 1);
    
    return (status & 0x01); // True if busy, False if ready
}
/**
 * Send Write Enable command (Required before Erase/Write)
 */
static inline void flash_write_enable() {
    uint8_t cmd = CMD_WREN;
    gpio_put(FLASH_PIN_CS, 0);
    spi_write_blocking(FLASH_SPI_PORT, &cmd, 1);
    gpio_put(FLASH_PIN_CS, 1);
}

/**
 * Read Data: The "Hard Drive Read"
 * @param addr: 0 to 0xFFFFFF (16MB)
 */
static inline void flash_read(uint32_t addr, uint8_t *buf, size_t len) {
    uint8_t cmdbuf[4] = {
        CMD_READ,
        (uint8_t)(addr >> 16),
        (uint8_t)(addr >> 8),
        (uint8_t)(addr)
    };
    gpio_put(FLASH_PIN_CS, 0);
    spi_write_blocking(FLASH_SPI_PORT, cmdbuf, 4);
    spi_read_blocking(FLASH_SPI_PORT, 0, buf, len);
    gpio_put(FLASH_PIN_CS, 1);
}

/**
 * Erase a 4KB Sector: Required before writing new data
 * @param addr: Must be 4KB aligned (0, 4096, 8192...)
 */
static inline void flash_erase_sector(uint32_t addr) {
    flash_write_enable();
    uint8_t cmdbuf[4] = {
        CMD_SECTOR_ERASE,
        (uint8_t)(addr >> 16),
        (uint8_t)(addr >> 8),
        (uint8_t)(addr)
    };
    gpio_put(FLASH_PIN_CS, 0);
    spi_write_blocking(FLASH_SPI_PORT, cmdbuf, 4);
    gpio_put(FLASH_PIN_CS, 1);
    flash_wait_ready();
}

/**
 * Write a Page: Max 256 bytes per call. 
 * Note: Data can only be written to erased (0xFF) areas.
 */
static inline void flash_write_page(uint32_t addr, const uint8_t *data, size_t len) {
    flash_write_enable();
    uint8_t cmdbuf[4] = {
        CMD_PAGE_PROG,
        (uint8_t)(addr >> 16),
        (uint8_t)(addr >> 8),
        (uint8_t)(addr)
    };
    gpio_put(FLASH_PIN_CS, 0);
    spi_write_blocking(FLASH_SPI_PORT, cmdbuf, 4);
    spi_write_blocking(FLASH_SPI_PORT, data, len);
    gpio_put(FLASH_PIN_CS, 1);
    flash_wait_ready();
}

/**
 * Get JEDEC ID: Use this to verify the chip is connected.
 * W25Q128JV should return: EF 40 18
 */
static inline uint32_t flash_read_id() {
    uint8_t cmd = CMD_JEDEC_ID;
    uint8_t id[3];
    gpio_put(FLASH_PIN_CS, 0);
    spi_write_blocking(FLASH_SPI_PORT, &cmd, 1);
    spi_read_blocking(FLASH_SPI_PORT, 0, id, 3);
    gpio_put(FLASH_PIN_CS, 1);
    return (id[0] << 16) | (id[1] << 8) | id[2];
}
static inline void flash_erase_all() {
    uint8_t cmd = CMD_ERASE_ALL; // 0xC7 is the "Bulk Erase" command
    
    gpio_put(FLASH_PIN_CS, 0); // Pull CS Low
    spi_write_blocking(spi0, &cmd, 1);
    gpio_put(FLASH_PIN_CS, 1); // Pull CS High
}
#endif // W25Q128_H
