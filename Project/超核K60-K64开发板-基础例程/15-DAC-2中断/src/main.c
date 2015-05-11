#include "gpio.h"
#include "common.h"
#include "uart.h"
#include "dac.h"
#include "adc.h"
#include "pit.h"

/* CH Kinetis固件库 V2.50 版本 */
/* 修改主频 请修改 CMSIS标准文件 system_MKxxxx.c 中的 CLOCK_SETUP 宏 */
 
/*
     实验名称：DAC实验
     实验平台：渡鸦开发板
     板载芯片：MK60DN512ZVQ10
 实验效果：使用DAC模块产生三角波，采用PIT触发转换
      小灯周期性闪烁，闪烁时间间隔500ms     
*/
void PIT_ISR(void)
{
    uint32_t val;
    /* 不停的触发 DAC 来产生波形 */
    DAC_SoftwareStartConversion(HW_DAC0);
    val = ADC_QuickReadValue(ADC0_SE23);
    printf("ADC:%4d | %0.3fV | pointer:%d\r\n", val, (float)val*3.300/(1<<12), DAC_GetBufferReadPointer(HW_DAC0));
}

void DAC_ISR(void)
{
    printf("DAC INT\r\n");
}

int main(void)
{
    uint32_t i;
    uint16_t value[16];
    DelayInit();
    GPIO_QuickInit(HW_GPIOE, 6, kGPIO_Mode_OPP);
    UART_QuickInit(UART0_RX_PD06_TX_PD07, 115200);
    
    printf("DAC test please measure DAC0_OUT\r\n");
    
    DAC_InitTypeDef DAC_InitStruct = {0};
    DAC_InitStruct.bufferMode = kDAC_Buffer_Swing; /*Buffer摇摆模式 产生三角波 */
    DAC_InitStruct.instance = HW_DAC0;
    DAC_InitStruct.referenceMode = kDAC_Reference_2; /* 使用VDDA作为参考源 */
    DAC_InitStruct.triggerMode = kDAC_TriggerSoftware; /*软件触发 */
    DAC_Init(&DAC_InitStruct);
    /* 设置水位 */
    DAC_SetWaterMark(HW_DAC0, kDAC_WaterMark_1Word);
    /* 开启DAC中断 */
    DAC_CallbackInstall(HW_DAC0, DAC_ISR);
    DAC_ITDMAConfig(HW_DAC0, kDAC_IT_BufferPointer_WaterMark);
    for(i = 0; i < 16; i++)
    {
        value[i] = i*4096/16; //数模转换的数据
    }
    DAC_SetBufferValue(HW_DAC0, value, 16); /*写入DAC buffer 最多写入16个 uint16_t 的转换值 */
    
    /* 开启PIT中断 */
    PIT_QuickInit(HW_PIT_CH0, 200*1000);
    PIT_CallbackInstall(HW_PIT_CH0, PIT_ISR);
    PIT_ITDMAConfig(HW_PIT_CH0, kPIT_IT_TOF, true);
    
    /* 开启一个ADC模块用于测量 DAC的值 */
    ADC_QuickInit(ADC0_SE23, kADC_SingleDiff12or13);
    
    while(1)
    {
        GPIO_ToggleBit(HW_GPIOE, 6);
        DelayMs(500);
    }
}


