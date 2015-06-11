/**
  ******************************************************************************
  * @file    filter.h
  * @author  YANDLD
  * @version V2.5
  * @date    2015.3.26
  * @brief   www.beyondcore.net   http://upcmcu.taobao.com 
  * @note    
  ******************************************************************************
  */
#ifndef _FILTER_H_
#define _FILTER_H_

#include <stdint.h>


float lpf_1st_factor_cal(float time, float f_cut);
float lpf_1st(float old_data, float new_data, float factor);

#endif

