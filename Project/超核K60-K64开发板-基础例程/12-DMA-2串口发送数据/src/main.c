#include "gpio.h"
#include "common.h"
#include "uart.h"
#include "dma.h"
#include "sram.h"
/* CH Kinetis固件库 V2.50 版本 */
/* 修改主频 请修改 CMSIS标准文件 system_MKxxxx.c 中的 CLOCK_SETUP 宏 */

/*
     实验名称：DMA串口发送数据
     实验平台：渡鸦开发板
     板载芯片：MK60DN512ZVQ10
 实验效果：使用串口发送数据，仅仅是使用了DMA功能，处理速度更快
*/

static const void* UART_DataPortAddrTable[] = 
{
    (void*)&UART0->D,
    (void*)&UART1->D,
    (void*)&UART2->D,
    (void*)&UART3->D,
    (void*)&UART4->D,
    (void*)&UART5->D,    
};

static const uint32_t UART_SendDMATriggerSourceTable[] = 
{
    UART0_TRAN_DMAREQ,
    UART1_TRAN_DMAREQ,
    UART2_TRAN_DMAREQ,
    UART3_TRAN_DMAREQ,
    UART4_TRAN_DMAREQ,
    UART5_TRAN_DMAREQ,
};

/* DMA 发送函数 */
static uint32_t UART_SendWithDMA(uint32_t dmaChl, const uint8_t *buf, uint32_t size)
{
    DMA_SetSourceAddress(dmaChl, (uint32_t)buf);
    DMA_SetMajorLoopCounter(dmaChl, size);
    /* 启动传输 */
    DMA_EnableRequest(dmaChl);
    return 0;
}

static void UART_DMASendConfig(uint32_t uartInstnace, uint8_t dmaChl)
{
    DMA_InitTypeDef DMA_InitStruct1 = {0};
    DMA_InitStruct1.chl = dmaChl;
    DMA_InitStruct1.chlTriggerSource = UART_SendDMATriggerSourceTable[uartInstnace];
    DMA_InitStruct1.triggerSourceMode = kDMA_TriggerSource_Normal;
    DMA_InitStruct1.minorLoopByteCnt = 1;
    DMA_InitStruct1.majorLoopCnt = 0;
    
    DMA_InitStruct1.sAddr = NULL;
    DMA_InitStruct1.sLastAddrAdj = 0; 
    DMA_InitStruct1.sAddrOffset = 1;
    DMA_InitStruct1.sDataWidth = kDMA_DataWidthBit_8;
    DMA_InitStruct1.sMod = kDMA_ModuloDisable;
    
    DMA_InitStruct1.dAddr = (uint32_t)UART_DataPortAddrTable[uartInstnace]; 
    DMA_InitStruct1.dLastAddrAdj = 0;
    DMA_InitStruct1.dAddrOffset = 0;
    DMA_InitStruct1.dDataWidth = kDMA_DataWidthBit_8;
    DMA_InitStruct1.dMod = kDMA_ModuloDisable;
    DMA_Init(&DMA_InitStruct1);
}

int main(void)
{
    static const char String1[] = "This string is send via DMA\r\n";
    DelayInit();
    GPIO_QuickInit(HW_GPIOE, 6, kGPIO_Mode_OPP);
    UART_QuickInit(UART0_RX_PD06_TX_PD07, 115200);
    printf("DMA UART transmit test\r\n");
    /* 打开UART0 DMA发送使能 */
    UART_ITDMAConfig(HW_UART0, kUART_DMA_Tx, true);
    UART_DMASendConfig(HW_UART0, HW_DMA_CH2);
    
    while(1)
    {
        /* 通过串口使用dma功能实现数据发送 */
        UART_SendWithDMA(HW_DMA_CH2, (const uint8_t*)String1, sizeof(String1));
        /* 等待DMA传输结束 */
        while(DMA_IsMajorLoopComplete(HW_DMA_CH2));
        GPIO_ToggleBit(HW_GPIOE, 6);
        DelayMs(200);
    }
}


