#include "gpio.h"
#include "common.h"
#include "uart.h"
#include "dma.h"

#include <string.h>
/* CH Kinetis固件库 V2.50 版本 */
/* 修改主频 请修改 CMSIS标准文件 system_MKxxxx.c 中的 CLOCK_SETUP 宏 */

/*
     实验名称：DMA内存拷贝
     实验平台：渡鸦开发板
     板载芯片：MK60DN512ZVQ10
 实验效果：上电后芯片执行dma数据传输，数据传输宽度8位，共传输64字节
        如果DMA功能正常的话，开发板上的小灯将闪烁
*/
static uint8_t SourceBuffer[64]; //源地址
static uint8_t DestBuffer[64];   //目标地址

void DMA_ISR(void)
{
    uint32_t i;
    printf("DMA INT \r\n");
    for(i=0;i<sizeof(DestBuffer);i++)
    {
        printf("DestBuf[%d]:0x%X\r\n", i, DestBuffer[i]);
    }
    printf("DMA_sAddr:%x\r\n", DMA_GetSourceAddress(HW_DMA_CH0));
    printf("DMA_dAddr:%x\r\n", DMA_GetDestAddress(HW_DMA_CH0));
    printf("DMA_sAddr - SouceAddr:%d\r\n", DMA_GetSourceAddress(HW_DMA_CH0) - (uint32_t)SourceBuffer);
    printf("DMA_dAddr - DestAddr:%d\r\n", DMA_GetDestAddress(HW_DMA_CH0) - (uint32_t)DestBuffer);
    printf("DMA memcpy test succ!\r\n");
}

int main(void)
{
    uint32_t i;
    DelayInit();
    GPIO_QuickInit(HW_GPIOE, 6, kGPIO_Mode_OPP);
    UART_QuickInit(UART0_RX_PD06_TX_PD07, 115200);
    
    printf("DMA memcpy test\r\n");
    
    /* 一次触发后 执行 minorLoopByteCnt 完成后 叫做一次Minor Loop */
    /* 完成 majorLoopCnt个 Loop 循环执行后 一次 DMA Major Loop 结束 */
    
    /* 写入测试数据 并且将被测试数据清空*/
    for(i = 0; i < sizeof(SourceBuffer); i++)
    {
        SourceBuffer[i] = i;
    }
    memset(DestBuffer, 0, sizeof(DestBuffer));       //清空数组中的数据
    printf("SouceAddr:%p\r\n", SourceBuffer);
    printf("DestAddr :%p\r\n", DestBuffer);
    DMA_InitTypeDef DMA_InitStruct1 = {0};
    DMA_InitStruct1.chl = HW_DMA_CH0;                                           /* 通道号 */
    DMA_InitStruct1.chlTriggerSource = MUX1_DMAREQ;                             /* Always Enabled 传输 */
    DMA_InitStruct1.triggerSourceMode = kDMA_TriggerSource_Normal;              /* 普通模式 不是周期触发模式 */
    DMA_InitStruct1.minorLoopByteCnt = sizeof(SourceBuffer);
    DMA_InitStruct1.majorLoopCnt = 1;
    
    DMA_InitStruct1.sAddr = (uint32_t)SourceBuffer;                             /*源地址 */
    DMA_InitStruct1.sLastAddrAdj = 0;                                           /* 所有Major Loop执行完后 sAddr偏移量 */
    DMA_InitStruct1.sAddrOffset = 1;                                            /* 每次读取sAddr后的地址偏移 */
    DMA_InitStruct1.sDataWidth = kDMA_DataWidthBit_8;                           /* 每次传输的位宽 */
    DMA_InitStruct1.sMod = kDMA_ModuloDisable;                                  /* 禁止Modulo 模式 */
    
    DMA_InitStruct1.dAddr = (uint32_t)DestBuffer;
    DMA_InitStruct1.dLastAddrAdj = 0;
    DMA_InitStruct1.dAddrOffset = 1;
    DMA_InitStruct1.dDataWidth = kDMA_DataWidthBit_8;
    DMA_InitStruct1.dMod = kDMA_ModuloDisable;
    /* 初始化 并配置中断 */
    DMA_Init(&DMA_InitStruct1);
    DMA_CallbackInstall(HW_DMA_CH0, DMA_ISR);
    DMA_ITConfig(HW_DMA_CH0, kDMA_IT_Major, true);
    DMA_EnableRequest(HW_DMA_CH0);/* 启动DMA传输 */
    while(1)
    {
        GPIO_ToggleBit(HW_GPIOE, 6);
        DelayMs(500);
    }
}


