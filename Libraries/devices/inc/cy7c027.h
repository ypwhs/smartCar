/**
  ******************************************************************************
  * @file    sram.h
  * @author  YANDLD
  * @version V2.5
  * @date    2014.3.24
  * @brief   www.beyondcore.net   http://upcmcu.taobao.com 
  ******************************************************************************
  */
#ifndef __SRAM_H__
#define __SRAM_H__
#include "flexbus.h"

/* SRAM »ùµØÖ· */
#define SRAM_ADDRESS_BASE   (0x60000000)
/* SRAM Size */
#define SRAM_SIZE            (32*1024)
#define SRAM_START_ADDRESS	(volatile uint8_t *)(SRAM_ADDRESS_BASE)

//!< API functions
void SRAM_Init(void);
uint32_t SRAM_SelfTest(void);

#endif

