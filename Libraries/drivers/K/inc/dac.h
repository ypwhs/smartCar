/**
  ******************************************************************************
  * @file    dac.h
  * @author  YANDLD
  * @version V2.5
  * @date    2014.3.26
  * @brief   www.beyondcore.net   http://upcmcu.taobao.com 
  ******************************************************************************
  */
  
#ifndef __CH_LIB_DAC_H__
#define __CH_LIB_DAC_H__

#include <stdint.h>


#define HW_DAC0     (0x00)  //DAC0模块
#define HW_DAC1     (0x01)  //DAC1模块


//!< DAC 回调函数定义
typedef void (*DAC_CallBackType)(void);


typedef enum
{
    kDAC_TriggerSoftware,  //软件触发
    kDAC_TriggerHardware,  //硬件触发
}DAC_TriggerSelect_Type;

/*DAC 发送缓冲模式 选择 */
typedef enum
{
    kDAC_Buffer_Disable,    //关闭DAC缓存
    kDAC_Buffer_Normal,     //开启DAC缓存
    kDAC_Buffer_Swing,      //dac摇摆模式
    kDAC_Buffer_OneTimeScan,//DAC一次浏览
}DAC_BufferMode_Type;

typedef enum
{
    kDAC_Reference_1,       /* 参考电压源 VREF_OUT */
    kDAC_Reference_2,       /* 参考电压源 VDDA */
}DAC_ReferenceSelect_Type;

typedef struct
{
    uint32_t                    instance;      //DAC模块号0~1
    DAC_TriggerSelect_Type      triggerMode;   //DAC触发模式
    DAC_BufferMode_Type         bufferMode;    //缓存类型
    DAC_ReferenceSelect_Type    referenceMode; //参考源选择
}DAC_InitTypeDef;


//!< DAC中断及DMA配置选择
typedef enum
{
    kDAC_DMA_Disable,
    kDAC_IT_Disable,
    kDAC_IT_BufferPointer_WaterMark,
    kDAC_IT_BufferPointer_TopFlag,      /* when read pointer reach 0 */
    kDAC_IT_BufferPointer_BottomFlag,   /* when read pointer reach upper limit */
    kDAC_DMA_BufferPointer_WaterMark,
    kDAC_DMA_BufferPointer_TopFlag,    
    kDAC_DMA_BufferPointer_BottomFlag, 
}DAC_ITDMAConfig_Type;
 

typedef enum
{
    kDAC_WaterMark_1Word,
    kDAC_WaterMark_2Word,
    kDAC_WaterMark_3Word,
    kDAC_WaterMark_4Word,
}DAC_WaterMarkMode_Type;

 //!< API functions
void DAC_Init(DAC_InitTypeDef* DAC_InitStruct);
void DAC_CallbackInstall(uint8_t instance, DAC_CallBackType AppCBFun);
void DAC_ITDMAConfig(uint32_t instance, DAC_ITDMAConfig_Type config);
uint32_t DAC_GetBufferReadPointer(uint32_t instance);
void DAC_SetBufferReadPointer(uint32_t instance, uint32_t value);
void DAC_SetBufferUpperLimit(uint32_t instance, uint32_t value);
void DAC_ITDMAConfig(uint32_t instance, DAC_ITDMAConfig_Type config);
void DAC_SetBufferValue(uint32_t instance, uint16_t* buf, uint8_t len);
void DAC_SetWaterMark(uint32_t instance, DAC_WaterMarkMode_Type value);
void DAC_SoftwareStartConversion(uint32_t instance);

#endif



