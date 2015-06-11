/**
  ******************************************************************************
  * @file    dma.h
  * @author  YANDLD
  * @version V2.5
  * @date    2014.3.26
  * @brief   www.beyondcore.net   http://upcmcu.taobao.com 
  ******************************************************************************
  */
#ifndef __CH_LIB_KL_DMA_H__
#define __CH_LIB_KL_DMA_H__

#include <stdint.h>
#include <stdbool.h>

//!< DMA 触发源选择
#define DISABLE_DMAREQ          0
#define UART0_REV_DMAREQ        2
#define UART0_TRAN_DMAREQ       3
#define UART1_REV_DMAREQ        4
#define UART1_TRAN_DMAREQ       5
#define UART2_REV_DMAREQ        6
#define UART2_TRAN_DMAREQ       7
#define I2S0_REV_DMAREQ         14
#define I2S0_TRAN_DMAREQ        15
#define SPI0_REV_DMAREQ         16
#define SPI0_TRAN_DMAREQ        17
#define SPI1_REV_DMAREQ         18
#define SPI1_TRAN_DMAREQ        19
#define I2C0_DMAREQ             22
#define I2C1_DMAREQ             23
#define TPM0_CH0_DMAREQ         24
#define TPM0_CH1_DMAREQ         25
#define TPM0_CH2_DMAREQ         26
#define TPM0_CH3_DMAREQ         27
#define TPM0_CH4_DMAREQ         28
#define TPM1_CH0_DMAREQ         32
#define TPM1_CH1_DMAREQ         33
#define TPM2_CH0_DMAREQ         34
#define TPM2_CH1_DMAREQ         35
#define ADC0_DMAREQ             40
#define CMP0_DMAREQ             42
#define DAC0_DMAREQ             45
#define PORTA_DMAREQ            49
#define PORTC_DMAREQ            51
#define PORTD_DMAREQ            52
#define MUX0_DMAREQ             60
#define MUX1_DMAREQ             61
#define MUX2_DMAREQ             62
#define MUX3_DMAREQ             63


//!< DMA通道
#define HW_DMA_CH0    (0x00)  //DMA0通道
#define HW_DMA_CH1    (0x01)  //DMA1通道
#define HW_DMA_CH2    (0x02)  //DMA2通道
#define HW_DMA_CH3    (0x03)  //DMA3通道

//!< 触发模式选择
typedef enum
{
    kDMA_TriggerSource_Normal,    //正常模式
    kDMA_TriggerSource_Periodic,  //周期触发模式
}DMA_TriggerSource_Type;

//!< DMA搬运数据端口位宽设置
typedef enum
{
    kDMA_DataWidthBit_8 = 1,   /* 8 bit data width */
    kDMA_DataWidthBit_16 = 2,  /* 16 bit data width */
    kDMA_DataWidthBit_32 = 0,  /* 32 bit data width*/
}DMA_DataWidthBit_Type; 

//!< DMA Moduluo 设置
typedef enum 
{
    kDMA_ModuloDisable = 0x0U,
    kDMA_Modulo16bytes = 0x1U,
    kDMA_Modulo32bytes = 0x2U,
    kDMA_Modulo64bytes = 0x3U,
    kDMA_Modulo128bytes = 0x4U,
    kDMA_Modulo256bytes = 0x5U,
    kDMA_Modulo512bytes = 0x6U,
    kDMA_Modulo1Kbytes = 0x7U,
    kDMA_Modulo2Kbytes = 0x8U,
    kDMA_Modulo4Kbytes = 0x9U,
    kDMA_Modulo8Kbytes = 0xAU,
    kDMA_Modulo16Kbytes = 0xBU,
    kDMA_Modulo32Kbytes = 0xCU,
    kDMA_Modulo64Kbytes = 0xDU,
    kDMA_Modulo128Kbytes = 0xEU,
    kDMA_Modulo256Kbytes = 0xFU,
} DMA_Modulo_Type;

//!< 初始化结构体
typedef struct 
{
    uint8_t                     chl;                                /* DMA通道号 */       
    uint8_t                     chlTriggerSource;                   /* DMA触发源选择 */
    uint16_t                    transferByteCnt;
    DMA_TriggerSource_Type      triggerSourceMode;                  /* 触发模式选择 */
    /* 源地址配置 */
    bool                        sAddrIsInc;
    uint32_t                    sAddr;                      /* 数据源地址 */
    DMA_DataWidthBit_Type       sDataWidth;                 /* 数据源地址数据宽度 8 16 32 */
    DMA_Modulo_Type             sMod;                       /* Modulo 设置 参见 AN2898 */
    /* 目标地址属性配置 */
    bool                        dAddrIsInc;                
    uint32_t                    dAddr;                      
    DMA_DataWidthBit_Type       dDataWidth;                          
    DMA_Modulo_Type             dMod;                       
}DMA_InitTypeDef;


//!< interrupt select
typedef enum
{
    kDMA_IT_Major,              //传输完成中断开启
}DMA_ITConfig_Type;  

//!< Callback Type
typedef void (*DMA_CallBackType)(void);

//!< API functions
void DMA_Init(DMA_InitTypeDef *DMA_InitStruct);
/* get vars and status */
uint32_t DMA_GetDestAddress(uint8_t ch);
uint32_t DMA_GetSourceAddress(uint8_t ch);
uint32_t DMA_GetTransferByteCnt(uint8_t chl);
void DMA_SetTransferByteCnt(uint8_t chl, uint32_t val);
/* control */
void DMA_SetDestAddress(uint8_t chl, uint32_t address);
void DMA_SetSourceAddress(uint8_t chl, uint32_t address);
void DMA_EnableRequest(uint8_t chl);
void DMA_DisableRequest(uint8_t chl);
void DMA_EnableAutoDisableRequest(uint8_t chl , bool flag);
void DMA_EnableCycleSteal(uint8_t chl, bool flag);
void DMA_CancelTransfer(uint8_t chl);
/* IT funtions */
void DMA_ITConfig(uint8_t chl, DMA_ITConfig_Type config, bool status);
void DMA_CallbackInstall(uint8_t chl, DMA_CallBackType AppCBFun);



#endif


