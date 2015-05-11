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

/* BMP180 control values and cmd*/
#define BMP180_T_MEASURE                0x2E /* temperature measurement */
#define BMP180_P0_MEASURE               0x34 /* pressure measurement (OSS=0, 4.5ms) */
#define BMP180_P1_MEASURE               0x74 /* pressure measurement (OSS=1, 7.5ms) */
#define BMP180_P2_MEASURE               0xB4 /* pressure measurement (OSS=2, 13.5ms) */
#define BMP180_P3_MEASURE               0xF4 /* pressure measurement (OSS=3, 25.5ms) */


//!< API functions
int bmp180_init(uint32_t instance);
int bmp180_probe(void);
int bmp180_read_temperature(int32_t * temperature);
int bmp180_start_conversion(uint8_t cmd);
int bmp180_read_pressure(int32_t * pressure);
int bmp180_read_altitude(int32_t *altitude);
int bmp180_pressure2altitude(int32_t pressure, int32_t *altitude);
int is_conversion_busy(void);
#endif

