#ifndef __FLASH_KINETIS_H__
#define __FLASH_KINETIS_H__

#include <stdint.h>
#include "swd.h"

typedef struct {

    uint32_t init;
    uint32_t uninit;
    uint32_t erase_chip;
    uint32_t erase_sector;
    uint32_t program_page;

    FLASH_SYSCALL sys_call_param;

    uint32_t program_buffer;
    uint32_t algo_start;
    uint32_t algo_size;
    const uint32_t * image;

    uint32_t ram_to_flash_bytes_to_be_written;

} TARGET_FLASH;


uint32_t TFlashGetMemID(void);
uint32_t TFlashGetSDID(void);
uint8_t TFlash_UnlockSequence(void);
uint8_t TFlash_Init(const TARGET_FLASH* flash);
uint8_t target_flash_program_page(const TARGET_FLASH* flash, uint32_t addr, uint8_t * buf, uint32_t size);
uint8_t target_flash_erase_sector(const TARGET_FLASH* flash, unsigned int sector);
uint8_t target_flash_erase_chip(const TARGET_FLASH* flash);


#endif

