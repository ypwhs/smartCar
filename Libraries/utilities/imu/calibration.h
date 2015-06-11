/**
  ******************************************************************************
  * @file    calibration.h
  * @author  YANDLD
  * @version V2.5
  * @date    2015.3.26
  * @brief   www.beyondcore.net   http://upcmcu.taobao.com 
  * @note    dymalic caliberation impletmentation
  ******************************************************************************
  */

#ifndef _DCALIBERATION_H_
#define _DCALIBERATION_H_

#include <stdint.h>
#include <stdbool.h>

struct dcal_t
{
    int  magic;
    int16_t     m_min[3];
    int16_t     m_max[3];
    int16_t     mo[3];
    float       mg[3];
    int16_t     ao[3];
    int16_t     go[3];
    
    bool        need_update;
};


void dcal_init(struct dcal_t *dc);
void dcal_minput(int16_t *mdata);
void dcal_ginput(int16_t *gdata);
void dcal_output(struct dcal_t *dc);
void dcal_print(struct dcal_t * dc);

#endif

