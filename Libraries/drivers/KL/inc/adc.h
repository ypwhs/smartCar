/**
  ******************************************************************************
  * @file    adc.h
  * @author  YANDLD
  * @version V2.5
  * @date    2014.3.25
  * @brief   www.beyondcore.net   http://upcmcu.taobao.com 
  * @note    此文件为芯片ADC模块的底层功能函数
  ******************************************************************************
  */
#ifndef __CH_LIB_ADC_H__
#define __CH_LIB_ADC_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

//!< ADC外设模块号
#define HW_ADC0  (0) //ADC0模块


#define ADC0_SE1A_PE16      (0x00086020U)
#define ADC0_SE5A_PE17      (0x00286220U)
#define ADC0_SE2A_PE18      (0x00106420U)
#define ADC0_SE6A_PE19      (0x00306620U)
#define ADC0_SE0A_PE20      (0x00006820U)
#define ADC0_SE4A_PE21      (0x00206A20U)
#define ADC0_SE3A_PE22      (0x00186C20U)
#define ADC0_SE7A_PE23      (0x00386E20U)
#define ADC0_SE4B_PE29      (0x01207A20U)
#define ADC0_SE23A_PE30     (0x00B87C20U)
#define ADC0_SE8A_PB00      (0x00404008U)
#define ADC0_SE9A_PB01      (0x00484208U)
#define ADC0_SE12A_PB02     (0x00604408U)
#define ADC0_SE13A_PB03     (0x00684608U)
#define ADC0_SE14A_PC00     (0x00704010U)
#define ADC0_SE15A_PC01     (0x00784210U)
#define ADC0_SE11A_PC02     (0x00584410U)
#define ADC0_SE5B_PD01      (0x01284218U)
#define ADC0_SE6B_PD05      (0x01304A18U)
#define ADC0_SE7B_PD06      (0x01384C18U)

//!< ADC 转换时钟分频因子,控制转换速度
typedef enum
{
    kADC_ClockDiv1,  
    kADC_ClockDiv2,
    kADC_ClockDiv4,
    kADC_ClockDiv8,
}ADC_ClockDiv_Type;

//!< ADC转换精度定义
typedef enum
{
    kADC_SingleDiff8or9 = 0,    /* 单端8位或 差分9位精度 */
    kADC_SingleDiff12or13 = 1,  /* 单端12位或 差分13位精度 */
    kADC_SingleDiff10or11 = 2,  
    kADC_SingleDIff16 = 3,
}ADC_ResolutionMode_Type;

//!< ADC 触发方式定义
typedef enum
{
    kADC_TriggerSoftware,  //软件触发
    kADC_TriggerHardware,  //硬件触发
}ADC_TriggerSelect_Type;

//!< 是否连续转换
typedef enum
{
    kADC_ContinueConversionEnable,   //开启连续转换
    kADC_ContinueConversionDisable,  //关闭连续转换
}ADC_ContinueMode_Type;

//!< 单端ADC还是查分ADC
typedef enum
{
    kADC_Single,         //单端模式
    kADC_Differential,   //差分模式
}ADC_SingleOrDiffMode_Type;

//!< ADC 通道触发器复用选择(每个ADC通道有2个ADC转换触发器 为MuxA 和 MuxB. MuxB 只能硬件触发)
#define kADC_MuxA                (0x00)
#define kADC_MuxB                (0x01)
//!< ADC 通道复用选择(SEA 还是SEB) 硬件通道选择
#define kADC_ChlMuxA             (0x00)
#define kADC_ChlMuxB             (0x01)

//!< 模拟电压参考源
typedef enum
{
    kADC_VoltageVREF,  /* 使用外部引脚VREFH VREFL 作为参考电源 */
    kADC_VoltageVALT,  /* 可选的第二电压参考源 不同的型号可能连接到不同的模块上 K60DN512Vxxx 连接到 VREF模块 */
}ADC_VoltageRef_Type;

//!< 硬件平均
typedef enum
{
    kADC_HardwareAverageDisable,  //关闭硬件平均
    kADC_HardwareAverage_4,       //4均值
    kADC_HardwareAverage_8,       //8均值
    kADC_HardwareAverage_16,
    kADC_HardwareAverage_32,      //32均值
}ADC_HardwareAveMode_Type;

//!< ADC中断及DMA配置选择
typedef enum
{
    kADC_IT_EOF,            //ADC 转换完成中断
    kADC_DMA_EOF,           //ADC DMA 完成中断
}ADC_ITDMAConfig_Type;

//!< ADC 回调函数定义
typedef void (*ADC_CallBackType)(void);

//!< ADC 初始化结构
typedef struct
{
    uint32_t                    instance;                   //模块号
    ADC_TriggerSelect_Type      triggerMode;                //触发模式 软件触发 或 硬件触发
    ADC_ClockDiv_Type           clockDiv;                   //ADC时钟分频
    ADC_ResolutionMode_Type     resolutionMode;             //分频率选择 8 10 12 16位精度等
    ADC_SingleOrDiffMode_Type   singleOrDiffMode;           //单端 还是 差分输入
    ADC_ContinueMode_Type       continueMode;               //是否启动连续转换
    ADC_HardwareAveMode_Type    hardwareAveMode;            //硬件平均功能选择
    ADC_VoltageRef_Type         vref;                       //模拟电压参考源
}ADC_InitTypeDef;


//!< API functions
void ADC_CallbackInstall(uint32_t instance, ADC_CallBackType AppCBFun);
void ADC_Init(ADC_InitTypeDef* ADC_InitStruct);
uint8_t ADC_QuickInit(uint32_t MAP, ADC_ResolutionMode_Type resolutionMode);
void ADC_ITDMAConfig(uint32_t instance, uint32_t mux, ADC_ITDMAConfig_Type config, bool status);
int32_t ADC_QuickReadValue(uint32_t MAP);
int32_t ADC_ReadValue(uint32_t instance, uint32_t mux);
void ADC_StartConversion(uint32_t instance, uint32_t chl, uint32_t mux);
uint8_t ADC_IsConversionCompleted(uint32_t instance, uint32_t mux);
void ADC_ChlMuxConfig(uint32_t instance, uint32_t mux);


#ifdef __cplusplus
}
#endif



#endif
