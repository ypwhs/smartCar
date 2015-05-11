#include "gpio.h"
#include "common.h"
#include "uart.h"
#include "pit.h"
/* CH Kinetis固件库 V2.50 版本 */
/* 修改主频 请修改 CMSIS标准文件 system_MKxxxx.c 中的 CLOCK_SETUP 宏 */

/*
     实验名称：PIT演示NVIC功能
     实验平台：渡鸦开发板
     板载芯片：MK60DN512ZVQ10
 实验效果：修改PIT的中断优先级
   说明：修改优先级是根据keil自带的函数实现的，在core-cm4.h文件
*/

/* 每1秒触发一次 */
static void PIT0_ISR(void)
{
    printf("PIT0 IT enter!\r\n");
    DelayMs(500);
    printf("PIT0 IT exit!\r\n");
}

/* 每200MS 触发一次 */
static void PIT1_ISR(void)
{
    printf("PIT1 IT!\r\n");
}

int main(void)
{
    DelayInit();
    GPIO_QuickInit(HW_GPIOE, 6, kGPIO_Mode_OPP);
    UART_QuickInit(UART0_RX_PD06_TX_PD07, 115200);
    //设置优先级分组
    NVIC_SetPriorityGrouping(NVIC_PriorityGroup_2);
    NVIC_SetPriority(PIT0_IRQn, NVIC_EncodePriority(NVIC_PriorityGroup_2, 2, 2)); /* PIT0中断，抢占2 子优先级1 */
    NVIC_SetPriority(PIT1_IRQn, NVIC_EncodePriority(NVIC_PriorityGroup_2, 1, 2)); /* PIT1中断，抢占2 子优先级2 */
    //中断优先级修改完毕

    PIT_QuickInit(HW_PIT_CH0, 1000*1000);
    PIT_CallbackInstall(HW_PIT_CH0, PIT0_ISR);
    PIT_ITDMAConfig(HW_PIT_CH0, kPIT_IT_TOF, true);
    
    PIT_QuickInit(HW_PIT_CH1, 200*1000);
    PIT_CallbackInstall(HW_PIT_CH1, PIT1_ISR);
    PIT_ITDMAConfig(HW_PIT_CH1, kPIT_IT_TOF, true);
    
    /* 打印优先级 */
    uint32_t PreemptPriority, SubPriority;
    NVIC_DecodePriority(NVIC_GetPriority(PIT0_IRQn), NVIC_PriorityGroup_2, &PreemptPriority, &SubPriority);
    printf("NVIC PIT0_IRQn PreemptPriority:%d SubPriority:%d\r\n", PreemptPriority, SubPriority);

    NVIC_DecodePriority(NVIC_GetPriority(PIT1_IRQn), NVIC_PriorityGroup_2, &PreemptPriority, &SubPriority);
    printf("NVIC PIT1_IRQn PreemptPriority:%d SubPriority:%d\r\n", PreemptPriority, SubPriority);
    
    while(1)
    {
        GPIO_ToggleBit(HW_GPIOE, 6);
        DelayMs(500);
    }
}
