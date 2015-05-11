/**
  ******************************************************************************
  * @file    uart.h
  * @author  YANDLD
  * @version V2.5
  * @date    2014.3.25
  * @brief   www.beyondcore.net   http://upcmcu.taobao.com 
  ******************************************************************************
  */
#ifndef __CH_LIB_UART_H__
#define __CH_LIB_UART_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
	 
#ifdef UART_USE_STDIO
#define UART_printf(fmt,args...)    printf (fmt ,##args)
#endif
     
     
/*!< LPUART 硬件模块号 */
#define HW_LPUART0  (0x00U)

/*!< parity 选择 */
typedef enum
{
    kUART_ParityDisabled = 0x0,  // 校验位禁止
    kUART_ParityEven     = 0x2,  // 1位 奇校验 
    kUART_ParityOdd      = 0x3,  // 1位 偶校验 
} UART_ParityMode_Type;

/*!< 每帧数据位个数 */
typedef enum 
{
    kUART_8BitsPerChar  = 0,   // 8-bit 数据 不包括校验位 
    kUART_9BitsPerChar  = 1,   // 9-bit 数据 不包括校验位 
} UART_BitPerChar_Type;

/*!< UART 回调函数声明 */
typedef void (*LPUART_CallBackTxType)(void* pram);

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
    UART_ParityMode_Type    parityMode;     // UART 校验位 
    UART_BitPerChar_Type    bitPerChar;     // UART 每一帧含多少位数据 
}UART_InitTypeDef;
     
/*!< API functions */

uint8_t UART_QuickInit(uint32_t MAP, uint32_t baudrate);
void UART_Init(UART_InitTypeDef * UART_InitStruct);
int UART_printf(const char *format,...);
uint8_t UART_ReadByte(uint32_t instance, uint16_t *ch);
void UART_WriteByte(uint32_t instance, char ch);
void UART_SelectDebugInstance(uint32_t instance);


#ifdef __cplusplus
}
#endif

#endif

