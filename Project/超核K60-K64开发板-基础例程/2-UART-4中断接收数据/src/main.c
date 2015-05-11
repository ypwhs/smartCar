#include "gpio.h"
#include "common.h"
#include "uart.h"

/*
     实验名称：UART采用中断方式接收数据
     实验平台：渡鸦开发板
     板载芯片：MK60DN512ZVQ10
 实验效果：给开发板发送什么数据，开发板将接收到的数据转发回去
           同时，开发板上的小灯闪烁
*/

/* 串口接收中断回调函数 
   在函数中写中断想要做的事情
*/
static void UART_RX_ISR(uint16_t byteReceived)
{
    /* 将接收到的数据发送回去 */
    UART_WriteByte(HW_UART0, byteReceived);
}

 
int main(void)
{
    DelayInit();
    GPIO_QuickInit(HW_GPIOE, 6, kGPIO_Mode_OPP);
    
    UART_QuickInit(UART0_RX_PD06_TX_PD07 , 115200);
    
    printf("type any character which will echo...\r\n");
    
    /*  配置UART 中断配置 打开接收中断 安装中断回调函数 */
    UART_CallbackRxInstall(HW_UART0, UART_RX_ISR);
    /* 打开串口接收中断功能 IT 就是中断的意思*/
    UART_ITDMAConfig(HW_UART0, kUART_IT_Rx, true);
    
    while(1)
    {
        GPIO_ToggleBit(HW_GPIOE, 6);
        DelayMs(500);
    }
}


