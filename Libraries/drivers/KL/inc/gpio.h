#ifndef __CH_LIB_KL_GPIO_H__
#define __CH_LIB_KL_GPIO_H__

#ifdef __cplusplus
 extern "C" {
#endif
	 
#include "common.h"
#include <stdint.h>

/* GPIO instance define */
#define HW_GPIOA  (0x00U)
#define HW_GPIOB  (0x01U)
#define HW_GPIOC  (0x02U)
#define HW_GPIOD  (0x03U)
#define HW_GPIOE  (0x04U)
#define HW_GPIOF  (0x05U)


/* PinMux */
typedef enum
{
    kPinAlt0,
    kPinAlt1,
    kPinAlt2,
    kPinAlt3,
    kPinAlt4,
    kPinAlt5,
    kPinAlt6,
    kPinAlt7,
}PORT_PinMux_Type;

typedef enum
{
    kPullDisabled,
    kPullUp,
    kPullDown,
}PORT_Pull_Type;


typedef enum
{
    kGPIO_Mode_IFT = 0x00,       /* Input floating  */
    kGPIO_Mode_IPD = 0x01,       /* Pull down input */
    kGPIO_Mode_IPU = 0x02,       /* Pull up input    */
    kGPIO_Mode_OPP = 0x04,       /* Push pull output */
}GPIO_Mode_Type;

/* 端口输入输出模式选择 */
typedef enum
{
    kInput,
    kOutput,
}GPIO_PinConfig_Type;

/*!< Interrupts and DMA */
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


typedef struct
{
    uint8_t                instance;    //引脚端口HW_GPIOA~HW_GPIOF
    GPIO_Mode_Type         mode;        //工作模式
    uint32_t               pinx;        //引脚号0~31
}GPIO_InitTypeDef;

/* 端口中断回调函数定义 */
typedef void (*GPIO_CallBackType)(uint32_t pinxArray);


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


