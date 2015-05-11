#include "shell.h"
#include "dma.h"
#include "common.h"


void DMA_ISR(void)
{
    printf("func:%s\r\n", __func__);
}


static int DoDMA(int argc, char * const argv[])
{
    int i;
    uint8_t sData[64];
    uint8_t dData[64];
    printf("func:%s\r\n", __func__);
    
    for(i=0;i<ARRAY_SIZE(sData);i++)
    {
        sData[i] = i;
        dData[i] = 0;
    }
    
#if __CORTEX_M == 4
    
    DMA_InitTypeDef DMA_InitStruct1 = {0};
    
    DMA_InitStruct1.chl = HW_DMA_CH0;
    DMA_InitStruct1.chlTriggerSource = MUX0_DMAREQ;
    DMA_InitStruct1.triggerSourceMode = kDMA_TriggerSource_Normal;  
    DMA_InitStruct1.majorLoopCnt = sizeof(dData);
    DMA_InitStruct1.minorLoopByteCnt = 1;
    
    DMA_InitStruct1.sAddr = (uint32_t)sData;
    DMA_InitStruct1.sAddrOffset = 1;
    DMA_InitStruct1.sDataWidth = kDMA_DataWidthBit_8;
    DMA_InitStruct1.sMod = kDMA_ModuloDisable;
    
    DMA_InitStruct1.dAddr = (uint32_t)dData;
    DMA_InitStruct1.dAddrOffset = 1;
    DMA_InitStruct1.dDataWidth = kDMA_DataWidthBit_8;
    DMA_InitStruct1.dMod = kDMA_ModuloDisable;
    
#elif
    
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
 #endif   
    DMA_Init(&DMA_InitStruct1);
    
    DMA_EnableAutoDisableRequest(HW_DMA_CH0, true);
    
    DMA_EnableRequest(HW_DMA_CH0);
    DMA_CallbackInstall(HW_DMA_CH0, DMA_ISR);
    DMA_ITConfig(HW_DMA_CH0, kDMA_IT_Major, true);
    DelayMs(10);
    
    for(i=0;i<ARRAY_SIZE(dData);i++)
    {
        printf("0x%02X ", dData[i]);
    }
    printf("\r\n");
    return 0;
}

SHELL_EXPORT_CMD(DoDMA, dma , dma test)
    

