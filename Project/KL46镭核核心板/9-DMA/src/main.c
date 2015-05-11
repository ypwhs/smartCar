#include "gpio.h"
#include "common.h"
#include "uart.h"
#include "pit.h"
#include "tpm.h"
#include "lptmr.h"
#include "dma.h"


uint8_t sData[64];
uint8_t dData[64];
    
void DMA_ISR(void)
{
    int i;
    printf("data result:\r\n");
    for(i=0;i<ARRAY_SIZE(dData);i++)
    {
        printf("0x%02X ", dData[i]);
    }
}


int main(void)
{
    DelayInit();

    GPIO_QuickInit(HW_GPIOD, 7, kGPIO_Mode_OPP);
    GPIO_QuickInit(HW_GPIOE, 6, kGPIO_Mode_OPP);
    
    UART_QuickInit(UART0_RX_PA01_TX_PA02, 115200);
    
    printf("DMA test\r\n");
    int i;

    printf("func:%s\r\n", __func__);
    
    for(i=0;i<ARRAY_SIZE(sData);i++)
    {
        sData[i] = i;
        dData[i] = 0;
    }

    DMA_InitTypeDef DMA_InitStruct1 = {0};
    
    DMA_InitStruct1.chl = HW_DMA_CH0;
    DMA_InitStruct1.chlTriggerSource = MUX0_DMAREQ;
    DMA_InitStruct1.transferByteCnt = sizeof(sData);

    DMA_InitStruct1.sMod = kDMA_ModuloDisable;
    DMA_InitStruct1.sAddrIsInc = true;
    DMA_InitStruct1.sDataWidth = kDMA_DataWidthBit_8;
    DMA_InitStruct1.sAddr = (uint32_t)sData;
    
    DMA_InitStruct1.dMod = kDMA_ModuloDisable;
    DMA_InitStruct1.dAddrIsInc = true;
    DMA_InitStruct1.dDataWidth = kDMA_DataWidthBit_8;
    DMA_InitStruct1.dAddr = (uint32_t)dData;
    
    DMA_Init(&DMA_InitStruct1);
    
    DMA_EnableRequest(HW_DMA_CH0);
    DMA_CallbackInstall(HW_DMA_CH0, DMA_ISR);
    DMA_ITConfig(HW_DMA_CH0, kDMA_IT_Major, true);
    
    while(1)
    {
        DelayMs(1000);
    }
}


