/**
  ******************************************************************************
  * @file    vref.h
  * @author  YANDLD
  * @version V2.5
  * @date    2014.3.26
  * @brief   www.beyondcore.net   http://upcmcu.taobao.com 
  ******************************************************************************
  */
  
#ifndef __CH_LIB_VREF_H__
#define __CH_LIB_VREF_H__

#include <stdint.h>

/* VREF 输出模式选择 */
typedef enum
{
    kVREF_BufferMode_Lv0,  /* Bandgap on only, for stabilization and startup */     
    kVREF_BufferMode_Lv1,  /* High power buffer mode enabled */
    kVREF_BufferMode_Lv2,  /* Low-power buffer mode enabled */
}VREF_BufferMode_Type;

/* VREF init struct */
typedef struct
{
    uint32_t                    instance;
    VREF_BufferMode_Type        bufferMode;
}VREF_InitTypeDef;

//!< API functions
void VREF_Init(VREF_InitTypeDef *VREF_InitStruct);
void VREF_DeInit(void);
void VREF_QuickInit(void);
void VREF_SetTrimValue(uint32_t val);

#endif

