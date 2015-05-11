/**
  ******************************************************************************
  * @file    gpio.h
  * @author  YANDLD
  * @version V2.5
  * @date    2014.3.24
  * @brief   www.beyondcore.net   http://upcmcu.taobao.com 
  ******************************************************************************
  */
#ifndef __CH_LIB_KL_GPIO_H__
#define __CH_LIB_KL_GPIO_H__

#ifdef __cplusplus
 extern "C" {
#endif
	 
#include "common.h"
#include <stdint.h>

/* GPIO端口定义 */
#define HW_GPIOA  (0x00U)   //PTA引脚端口定义
#define HW_GPIOB  (0x01U)   //PTB引脚端口定义
#define HW_GPIOC  (0x02U)   //PTC引脚端口定义
#define HW_GPIOD  (0x03U)   //PTD引脚端口定义
#define HW_GPIOE  (0x04U)   //PTE引脚端口定义
#define HW_GPIOF  (0x05U)   //PTF引脚端口定义


/* 端口复用选择 根据手册 Signal Multiplexing and Signal Descriptions 章节选择复用*/
typedef enum
{
    kPinAlt0,  //0功能复用
    kPinAlt1,  //1功能复用
    kPinAlt2,  //2功能复用
    kPinAlt3,  //3功能复用
    kPinAlt4,  //4功能复用
    kPinAlt5,  //5功能复用
    kPinAlt6,  //6功能复用
    kPinAlt7,  //7功能复用
}PORT_PinMux_Type;

/* 端口上下拉配置 ，电阻阻值约为20K*/
typedef enum
{
    kPullDisabled,  //关闭上下拉电阻功能
    kPullUp,        //开启上拉电阻功能
    kPullDown,      //开启下拉电阻功能
}PORT_Pull_Type;

/* GPIO端口模式配置 */
typedef enum
{
    kGPIO_Mode_IFT = 0x00,       //浮空输入
    kGPIO_Mode_IPD = 0x01,       //下拉输入
    kGPIO_Mode_IPU = 0x02,       //上拉输入
    kGPIO_Mode_OPP = 0x04,       //推挽输出
}GPIO_Mode_Type;

/* 端口输入输出模式选择 */
typedef enum
{
    kInput,                  //引脚输入模式
    kOutput,                 //引脚输出模式
}GPIO_PinConfig_Type;

/* 端口中断及DMA配置选择 */
typedef enum
{
    kGPIO_DMA_RisingEdge,	      //上升沿触发DMA
    kGPIO_DMA_FallingEdge,        //下降沿触发DMA
    kGPIO_DMA_RisingFallingEdge,  //上升沿和下降沿触发DMA
    kGPIO_IT_Low,                 //低电平出发中断
    kGPIO_IT_RisingEdge,          //上升沿触发中断
    kGPIO_IT_FallingEdge,         //下降沿触发中断
    kGPIO_IT_RisingFallingEdge,   //上升沿和下降沿触发中断
    kGPIO_IT_High,                //高电平触发中断
}GPIO_ITDMAConfig_Type;

/* 端口初始化结构体 */
typedef struct
{
    uint8_t                instance;    //引脚端口HW_GPIOA~HW_GPIOF
    GPIO_Mode_Type         mode;        //工作模式
    uint32_t               pinx;        //引脚号0~31
}GPIO_InitTypeDef;

/* 端口中断回调函数定义 */
typedef void (*GPIO_CallBackType)(uint32_t pinxArray);

/* 参数检查宏 */
#define IS_GPIO_ALL_INSTANCE(INSTANCE)  (INSTANCE < ARRAY_SIZE(GPIO_InstanceTable))
#define IS_PORT_ALL_INSTANCE(INSTANCE)  (INSTANCE < ARRAY_SIZE(PORT_InstanceTable))
#define IS_GPIO_ALL_PIN(PIN)  (PIN < 32)



//!< API functions
void GPIO_Init(GPIO_InitTypeDef * GPIO_InitStruct);
uint8_t GPIO_QuickInit(uint32_t instance, uint32_t pinx, GPIO_Mode_Type mode);
void GPIO_WriteBit(uint32_t instance, uint8_t pinIndex, uint8_t data);
uint8_t GPIO_ReadBit(uint32_t instance, uint8_t pinIndex);
void GPIO_ToggleBit(uint32_t instance, uint8_t pinIndex);
void GPIO_ITDMAConfig(uint32_t instance, uint8_t pinIndex, GPIO_ITDMAConfig_Type config, bool status);
void GPIO_CallbackInstall(uint32_t instance, GPIO_CallBackType AppCBFun);
//!< low level functions
void PORT_PinPullConfig(uint32_t instance, uint8_t pinIndex, PORT_Pull_Type pull);
void GPIO_PinConfig(uint32_t instance, uint8_t pinIndex, GPIO_PinConfig_Type mode);
void PORT_PinMuxConfig(uint32_t instance, uint8_t pinIndex, PORT_PinMux_Type pinMux);
uint32_t GPIO_ReadPort(uint32_t instance);
void GPIO_WritePort(uint32_t instance, uint32_t data);
    
#ifdef __cplusplus
}
#endif

#endif


