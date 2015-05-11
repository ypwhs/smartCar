#include "gpio.h"
#include "common.h"
#include "uart.h"
/* CH Kinetis固件库 V2.50 版本 */
/* 修改主频 请修改 CMSIS标准文件 system_MKxxxx.c 中的 CLOCK_SETUP 宏 */

/*
     实验名称：NVIC中断优先级
     实验平台：渡鸦开发板
     板载芯片：MK60DN512ZVQ10
 实验效果：连接 PE26 和 PA4 使他们的信号相同 按下KEY1
使用串口观察哪个中断 先发生 由于 PORTA PORTE的中断抢占优先级不同
抢占优先级低的不能打断抢占优先级高的中断 所以 在一个中断完全执行完后
才能出发另外一个等待的中断 
*/

static void PORTE_ISR(uint32_t array)
{
    printf("PORTE IT enter!\r\n");
    DelayMs(1000);
    printf("PORTE IT exit!\r\n");
}

static void PORTA_ISR(uint32_t array)
{
    printf("PORTA IT enter!\r\n");
    DelayMs(1000);
    printf("PORTA IT exit!\r\n");
}

/* 将E26和 A4 跳线连接 然后配置 他们的NVIC优先级 */

int main(void)
{
    DelayInit();
    GPIO_QuickInit(HW_GPIOE, 6, kGPIO_Mode_OPP);
    UART_QuickInit(UART0_RX_PD06_TX_PD07, 115200);
    
    /* 设置PORTE PORTA 中断 */
    GPIO_QuickInit(HW_GPIOE,26, kGPIO_Mode_IPU);
    GPIO_QuickInit(HW_GPIOA, 4, kGPIO_Mode_IPU);
    GPIO_CallbackInstall(HW_GPIOE, PORTE_ISR);
    GPIO_CallbackInstall(HW_GPIOA, PORTA_ISR);
    GPIO_ITDMAConfig(HW_GPIOE, 26, kGPIO_IT_RisingEdge, true);
    GPIO_ITDMAConfig(HW_GPIOA, 4,  kGPIO_IT_RisingEdge, true);
    
    printf("NVIC test connect E26&A04\r\n");
    /* 将系统 中断优先级分组 可以配置 16个 抢占优先级 和16个 子优先级 */
    NVIC_SetPriorityGrouping(NVIC_PriorityGroup_2);  //中断优先级分成2组
    NVIC_SetPriority(PORTE_IRQn, NVIC_EncodePriority(NVIC_PriorityGroup_2, 2, 2)); //设置PTE端口的抢占优先级的子优先级
    NVIC_SetPriority(PORTA_IRQn, NVIC_EncodePriority(NVIC_PriorityGroup_2, 2, 2));
    
    while(1)
    {
        GPIO_ToggleBit(HW_GPIOE, 6);
        DelayMs(500);
    }
}


