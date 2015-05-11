/**
  ******************************************************************************
  * @file    w25qxx.h
  * @author  YANDLD
  * @version V2.5
  * @date    2013.12.25
  * @brief   www.beyondcore.net   http://upcmcu.taobao.com 
  ******************************************************************************
  */
  
#ifndef __W25QXX_H__
#define __W25QXX_H__

#include "spi.h"

//基本概念
// page :256byte 写入的最小单位 并且必须保证 写入前全部是0xFF
// sectoer: 4KBbyte
// block:   64KBbyte


#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x)	(sizeof(x) / sizeof((x)[0]))
#endif

struct w25qxx_attr_t
{
    const char* name;
    uint32_t size;
    uint16_t id;
    uint32_t page_size;
    uint32_t sector_size;
    uint32_t block_size;
};

//!< API functions
int w25qxx_init(uint32_t instance, uint32_t cs);
int w25qxx_write(uint32_t addr, uint8_t *buf, uint32_t len);
int w25qxx_read(uint32_t addr, uint8_t *buf, uint32_t len);
int w25qxx_write_page(uint32_t addr, uint8_t *buf, uint32_t len);
int w25qxx_erase_sector(uint32_t addr);
int w25qxx_erase_chip(void);
int w25qxx_get_attr(struct w25qxx_attr_t* attr);

#endif 


