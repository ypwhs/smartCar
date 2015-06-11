/**
  ******************************************************************************
  * @file    bmp180.h
  * @author  YANDLD
  * @version V2.5
  * @date    2013.12.25
  * @brief   www.beyondcore.net   http://upcmcu.taobao.com 
  ******************************************************************************
  */
#ifndef __BMP180_H__
#define __BMP180_H__

#include <stdint.h>


//!< API functions
int bmp180_init(uint32_t instance);
int bmp180_pressure2altitude(int32_t pressure, int32_t *altitude);
int bmp180_conversion_process(int32_t *pressure, int32_t *temperature);
#endif

