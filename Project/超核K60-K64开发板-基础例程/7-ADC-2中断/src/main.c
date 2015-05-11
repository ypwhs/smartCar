#include "gpio.h"
#include "common.h"
#include "uart.h"
#include "adc.h"
/* CH Kinetis固件库 V2.50 版本 */
/* 修改主频 请修改 CMSIS标准文件 system_MKxxxx.c 中的 CLOCK_SETUP 宏 */

/*
     实验名称：ADC中断实验
     实验平台：渡鸦开发板
     板载芯片：MK60DN512ZVQ10
 实验效果：使用芯片的DM0引脚进行ad采集，ad采集的通道为19，精度是10位
        ad转换完成后进入中断，在ad中断中获取ad采集数据 
        通过调节开发板上的电位器，可以更改ad采集结果
*/

static int32_t ADC_Value;

//AD中断处理函数
void ADC_ISR(void)
{
    ADC_Value = ADC_ReadValue(HW_ADC0, kADC_MuxA);
}

int main(void)
{
    DelayInit();
    GPIO_QuickInit(HW_GPIOE, 6, kGPIO_Mode_OPP);
    UART_QuickInit(UART0_RX_PD06_TX_PD07, 115200);
    
    printf("ADC test\r\n");
    
    /* 初始化ADC模块 ADC0_SE19_DM0 */
    ADC_InitTypeDef ADC_InitStruct1; //申请一个结构体
    ADC_InitStruct1.instance = HW_ADC0; //使用ADC0模块
    ADC_InitStruct1.clockDiv = kADC_ClockDiv8; /* ADC采样时钟2分频 */
    ADC_InitStruct1.resolutionMode = kADC_SingleDiff10or11; //设置10位或11位精度
    ADC_InitStruct1.triggerMode = kADC_TriggerSoftware; /* 软件触发转换 */
    ADC_InitStruct1.singleOrDiffMode = kADC_Single; /*单端模式 */
    ADC_InitStruct1.continueMode = kADC_ContinueConversionEnable; /* 启动连续转换 转换一次后 自动开始下一次转换*/
    ADC_InitStruct1.hardwareAveMode = kADC_HardwareAverage_32; /*禁止 硬件平均 功能 */
    ADC_InitStruct1.vref = kADC_VoltageVREF;                       /* 使用外部VERFH VREFL 作为模拟电压参考 */
    ADC_Init(&ADC_InitStruct1);
    
    /* 开启转换完成中断配置 */
    ADC_CallbackInstall(HW_ADC0, ADC_ISR);
    ADC_ITDMAConfig(HW_ADC0, kADC_MuxA, kADC_IT_EOF);
    
    /* 初始化对应引脚 */
    /* DM0引脚为专门的模拟引脚 ADC时 无需设置复用  DM0也无法当做普通的数字引脚 */
    
    /* 启动ADC转换 */
    ADC_StartConversion(HW_ADC0, 19, kADC_MuxA);
    
    while(1)
    {
        printf("ADC:%4d\r", ADC_Value);
        GPIO_ToggleBit(HW_GPIOE, 6);
        DelayMs(50);
    }
}


