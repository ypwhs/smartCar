#include "trans.h"
#include <string.h>
#include "dma.h"
#include "nrf24l01.h"
#include "board.h"
#include "uart.h"
#include "common.h"


struct 
{
    uint32_t dmaChl;
    uint32_t uartInstance;
}trans_hander;

static uint32_t DMA_UART_TxRequestSourceTable[] = 
{
    UART0_TRAN_DMAREQ,
    UART1_TRAN_DMAREQ,
    UART2_TRAN_DMAREQ,
};

static uint32_t DMA_UART_DataRegisterAddrTable[] = 
{
    (uint32_t)&UART0->D,
    (uint32_t)&UART1->D,
    (uint32_t)&UART2->D,
};

//安装回调函数

int trans_init(uint8_t dmaChl, uint32_t uartInstance)
{
    DMA_InitTypeDef DMA_InitStruct1= {0};
    DMA_InitStruct1.chl = dmaChl;
    DMA_InitStruct1.chlTriggerSource = DMA_UART_TxRequestSourceTable[uartInstance];
    DMA_InitStruct1.triggerSourceMode = kDMA_TriggerSource_Normal;
    DMA_InitStruct1.majorLoopCnt = 0;
    DMA_InitStruct1.minorLoopByteCnt = 1;
    DMA_InitStruct1.sAddr = NULL;
    DMA_InitStruct1.sLastAddrAdj = 0;
    DMA_InitStruct1.sAddrOffset = 1;
    DMA_InitStruct1.sDataWidth = kDMA_DataWidthBit_8;
    DMA_InitStruct1.sMod = kDMA_ModuloDisable;
        
    DMA_InitStruct1.dAddr = DMA_UART_DataRegisterAddrTable[uartInstance];
    DMA_InitStruct1.dLastAddrAdj = 0;
    DMA_InitStruct1.dAddrOffset = 0;
    DMA_InitStruct1.dDataWidth = kDMA_DataWidthBit_8;
    DMA_InitStruct1.dMod = kDMA_ModuloDisable;
    DMA_Init(&DMA_InitStruct1);
    UART_ITDMAConfig(uartInstance, kUART_DMA_Tx, true);
    
    trans_hander.dmaChl = dmaChl;
    trans_hander.uartInstance = uartInstance;
    return 0;
}

int trans_start_send_data(uint8_t* buf, uint32_t size)
{
    if(DMA_IsMajorLoopComplete(trans_hander.dmaChl) == 0)
    {
        DMA_SetSourceAddress(trans_hander.dmaChl, (uint32_t)buf);
        DMA_SetMajorLoopCounter(trans_hander.dmaChl, size);
        DMA_EnableRequest(trans_hander.dmaChl); 
        return 0;
    }
    return 1;
}

int trans_is_transmission_complete(void)
{
    if(DMA_IsMajorLoopComplete(trans_hander.dmaChl) == 0)
    {
        return 0;
    }        
    return 1;
}


