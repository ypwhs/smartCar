/**
  ******************************************************************************
  * @file    hmc5883.h
  * @author  YANDLD
  * @version V2.5
  * @date    2013.12.25
  * @brief   www.beyondcore.net   http://upcmcu.taobao.com 
  ******************************************************************************
  */
#ifndef __HMC5883_H__
#define __HMC5883_H__

#include <stdint.h>


#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x)	(sizeof(x) / sizeof((x)[0]))
#endif

//!< API functions
int hmc5883_init(uint32_t instance);
int hmc5883_read_data(int16_t* x, int16_t* y, int16_t* z);


#endif


