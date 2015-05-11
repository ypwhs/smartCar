#include "gpio.h"
#include "common.h"
#include "uart.h"
#include "pit.h"

/* CH Kinetis固件库 V2.50 版本 */
/* 修改主频 请修改 CMSIS标准文件 system_MKxxxx.c 中的 CLOCK_SETUP 宏 */

/*
     实验名称：PIT定时器中断
     实验平台：渡鸦开发板
     板载芯片：MK60DN512ZVQ10
 实验效果：使用PIT模块的0号定时器产生1s的周期中断，在PIT0中断函数中
     串口发送数据和小灯亮灭变化
     每隔1s小灯闪烁一次，串口发送数据
*/

/* PIT0中断服务函数 */
//此函数中编写用户中断需要做的事情
static void PIT_ISR(void)
{
    static uint32_t i;
    //中断中所做的事情
    printf("enter PIT interrupt! %d\r\n", i++); 
    /* 闪烁小灯 */
    GPIO_ToggleBit(HW_GPIOE, 6);
}

int main(void)
{
    DelayInit();
    
    GPIO_QuickInit(HW_GPIOE, 6, kGPIO_Mode_OPP); /* LED */
    
    UART_QuickInit(UART0_RX_PD06_TX_PD07, 115200);
    
    /* 初始化PIT模块 */
    PIT_InitTypeDef PIT_InitStruct1;  //申请结构体变量
    PIT_InitStruct1.chl = HW_PIT_CH0; /* 使用0号定时器 */
    PIT_InitStruct1.timeInUs = 1000*1000; /* 定时周期1S */
    PIT_Init(&PIT_InitStruct1); //pit模块初始化
    /* 注册PIT 中断回调函数 */
    PIT_CallbackInstall(HW_PIT_CH0, PIT_ISR); //0号定时器的中断处理
    /* 开启PIT0定时器中断 */
    PIT_ITDMAConfig(HW_PIT_CH0, kPIT_IT_TOF, true);
    
    while(1)
    {
        /* 等待系统中断 */
    }
}


