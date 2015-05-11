/**
  ******************************************************************************
  * @file    iflash.h
  * @author  YANDLD
  * @version V2.5
  * @date    2014.3.25
  * @brief   design for manley
  ******************************************************************************
  */
#ifndef __CH_LIB_IFLASH_H__
#define __CH_LIB_IFLASH_H__

#include <stdint.h>





//!< API declare
void FLASH_Init(void);
uint32_t FLASH_GetSectorSize(void);
uint8_t FLASH_WriteSector(uint32_t addr, const uint8_t *buf, uint32_t len);
uint8_t FLASH_EraseSector(uint32_t addr);

#endif

