/**
  ******************************************************************************
  * @file    veep.c
  * @author  YANDLD
  * @version V2.5
  * @date    2015.3.26
  * @brief   www.beyondcore.net   http://upcmcu.taobao.com 
  * @note    virtual EEPORM implemntation
  ******************************************************************************
  */
#include <string.h>

#include "virtual_eep.h"
#include "flash.h"


extern int Image$$ER_IROM1$$RO$$Limit;

#ifndef RT_ALIGN
#define RT_ALIGN(size, align)           (((size) + (align) - 1) & ~((align) - 1))
#endif

    
static uint32_t StartAddr;
    
void veep_init(void)
{
    uint32_t sector_size;
    
    sector_size = FLASH_GetSectorSize();
    StartAddr = RT_ALIGN(((uint32_t)&Image$$ER_IROM1$$RO$$Limit + sector_size), sector_size);
}

void veep_write(uint8_t* buf, uint32_t len)
{
    __disable_irq();
    FLASH_EraseSector(StartAddr);
    FLASH_WriteSector(StartAddr, buf, len);
    __enable_irq();
}

void veep_read(uint8_t *buf, uint32_t len)
{
    memcpy(buf, (void*)StartAddr, len);
}



