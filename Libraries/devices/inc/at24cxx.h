/**
  ******************************************************************************
  * @file    at24cxx.h
  * @author  YANDLD
  * @version V2.5
  * @date    2013.12.25
  * @brief   www.beyondcore.net   http://upcmcu.taobao.com 
  ******************************************************************************
  */
#ifndef __24CXX_H__
#define __24CXX_H__

#include <stdint.h>

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x)	(sizeof(x) / sizeof((x)[0]))
#endif

//API funtctions
int at24cxx_init(uint32_t instance);
int at24cxx_read(uint32_t addr, uint8_t *buf, uint32_t len);
int at24cxx_write(uint32_t addr, uint8_t *buf, uint32_t len);
int at24cxx_self_test(void);
int at24cxx_get_size(void);

#endif

