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

#include <math.h>

#include "filter.h"



#define M_PI 3.141592653f


/**
 * @brief  一节低通滤波器系数计算
 * @param  time: 调用时间 delataT
 * @param  f_cut: 截止频率
 * @retval 参数值
 */
float lpf_1st_factor_cal(float time, float f_cut)
{
    return time / (time + 1 / (2 * M_PI * f_cut));
}

/**
 * @brief  一节低通滤波器
 * @param  old_data: 上一次数据
 * @param  new_data: 新数据
 * @param  factor:   滤波系数
 * @retval 结果
 */
float inline lpf_1st(float old_data, float new_data, float factor)
{
	return old_data * (1 - factor) + new_data * factor; 
}




