/**
  ******************************************************************************
  * @file    uart.h
  * @author  YANDLD
  * @version V2.5
  * @date    2014.3.26
  * @brief   www.beyondcore.net   http://upcmcu.taobao.com 
  ******************************************************************************
  */
#ifndef __CH_LIB_KL_UART_H__
#define __CH_LIB_KL_UART_H__

#include <stdint.h>
#include <stdbool.h>

/*!< UART 硬件模块号 */
#define HW_UART0  (0x00U)
#define HW_UART1  (0x01U)
#define HW_UART2  (0x02U)


/*!< UART 快速初始化宏 */                      
#define UART1_RX_PE01_TX_PE00   (0x000080E1U)
#define UART2_RX_PE17_TX_PE16   (0x0000A0E2U)
#define UART0_RX_PE21_TX_PE20   (0x0000A920U)
#define UART2_RX_PE23_TX_PE22   (0x0000AD22U)
#define UART0_RX_PA01_TX_PA02   (0x00008280U)
#define UART0_RX_PA15_TX_PA14   (0x00009CC0U)
#define UART1_RX_PA18_TX_PA19   (0x0000A4C1U)
#define UART0_RX_PB16_TX_PB17   (0x0000A0C8U)
#define UART2_RX_PD02_TX_PD03   (0x000084DAU)
#define UART2_RX_PD04_TX_PD05   (0x000088DAU)
#define UART0_RX_PD06_TX_PD07   (0x00008CD8U)


/*!< 中断及DMA配置 */
typedef enum
{
    kUART_IT_Tx,                // 开启每发送一帧传输完成中断 
    kUART_DMA_Tx,               // 开启每发送一帧传输完成触发DMA 
    kUART_IT_Rx,                // 开启每接收一帧传输完成中断 
    kUART_DMA_Rx,               // 开启每接收一帧传输完成触发DMA 
}UART_ITDMAConfig_Type;

/*!< UART初始化结构 */
typedef struct
{
    uint32_t                srcClock;       // 时钟源频率
    uint8_t                 instance;       // UART 模块号 HW_UART0~HW_UART5
    uint32_t                baudrate;       // UART 波特率 
}UART_InitTypeDef;

/*!< UART 回调函数声明 */
typedef void (*UART_CallBackTxType)(uint16_t * pbyteToSend);
typedef void (*UART_CallBackRxType)(uint16_t byteReceived);

/*!< API functions */
uint8_t UART_QuickInit(uint32_t MAP, uint32_t baudrate);
void UART_Init(UART_InitTypeDef * UART_InitStruct);
int UART_printf(const char *format,...);
uint8_t UART_ReadByte(uint32_t instance, uint16_t *ch);
void UART_WriteByte(uint32_t instance, uint16_t ch);
void UART_SelectDebugInstance(uint32_t instance);

/* Interrupt and DMA functions */
void UART_CallbackTxInstall(uint32_t instance, UART_CallBackTxType AppCBFun);
void UART_CallbackRxInstall(uint32_t instance, UART_CallBackRxType AppCBFun);
void UART_ITDMAConfig(uint32_t instance, UART_ITDMAConfig_Type config, bool status);

#ifdef UART_USE_STDIO
int printf(const char *fmt, ...);
#endif


#endif


