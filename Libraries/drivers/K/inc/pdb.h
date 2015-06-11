/**
  ******************************************************************************
  * @file    pdb.h
  * @author  YANDLD
  * @version V2.5
  * @date    2014.3.24
  * @brief   www.beyondcore.net   http://upcmcu.taobao.com 
  ******************************************************************************
  */
#ifndef __CH_LIB_PDB_H__
#define __CH_LIB_PDB_H__

#ifdef __cplusplus
 extern "C" {
#endif
	 
#include "common.h"
#include <stdint.h>
     
    
#define HW_PDB_CH0
#define HW_PDB_CH1
     
#define HW_PDB_TRIG_CH0
#define HW_PDB_TRIG_CH1
     
//!< PIT CallBack Type
typedef void (*PDB_CallBackType)(void);

     
/* PDB 触发源 参考Chip configuration章节 */
typedef enum
{
    kPDB_TriggerSrc0,
    kPDB_TriggerSrc1,
    kPDB_TriggerSrc2,
    kPDB_TriggerSrc3,
    kPDB_TriggerSrc4,
    kPDB_TriggerSrc5,
    kPDB_TriggerSrc6,
    kPDB_TriggerSrc7,
    kPDB_TriggerSrc8,
    kPDB_TriggerSrc9, 
    kPDB_TriggerSrc10,
    kPDB_TriggerSrc11,
    kPDB_TriggerSrc12,
    kPDB_TriggerSrc13,
    kPDB_TriggerSrc14,
    kPDB_SoftwareTrigger, 
}PDB_TriggerSrc_Type;

typedef enum
{
	kPWM_EdgeAligned,           //边沿对齐 最常用
	kPWM_Combine,               //组合模式
    kPWM_Complementary,         //互补模式 类似组合模式 但是Chl(n) 和 Chl(n+1) 是互补输出
}FTM_PWM_Mode_Type;
     
typedef struct
{
    uint32_t                srcClock;           // 时钟源频率
    PDB_TriggerSrc_Type     inputTrigSource;    // 输入输入触发源
    bool                    isContinuesMode;    //是否是连续触发模式
    uint32_t                timeInUs;
}PDB_InitTypeDef;

/*!< 中断及DMA配置 */
typedef enum
{
    kPDB_IT_CF,
    kPDB_DMA_CF,
}PDB_ITDMAConfig_Type;


//!< API functions
void PDB_QuickInit(PDB_TriggerSrc_Type triggerSrc, uint32_t timeInUs);
void PDB_Init(PDB_InitTypeDef * PDB_InitStruct);
void PDB_SoftwareTrigger(void);
void PDB_ITDMAConfig(PDB_ITDMAConfig_Type config, bool status);
void PDB_CallbackInstall(PDB_CallBackType AppCBFun);
uint32_t PDB_GetMODValue(void);
//!< ADC trigger function
void PDB_SetADCPreTrigger(uint32_t adcInstance, uint32_t adcMux, uint32_t dlyValue, bool status);
void PDB_SetBackToBackMode(uint32_t adcInstance, uint32_t adcMux, bool status);


#ifdef __cplusplus
}
#endif

#endif

