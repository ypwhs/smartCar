#include "gpio.h"
#include "common.h"
#include "uart.h"
#include "cpuidy.h"

/* CH Kinetis固件库 V2.50 版本 */
/* 修改主频 请使用 CMSIS标准文件 startup_MKxxxx.c 中的 CLOCK_SETUP 宏 */

/*
     实验名称：UART采用轮询方式接收数据
     实验平台：渡鸦开发板
     板载芯片：MK60DN512ZVQ10
 实验效果：给开发板发送什么数据，开发板将接收到的数据转发回去
*/
 
int main(void)
{
    uint16_t ch;
    DelayInit();
    GPIO_QuickInit(HW_GPIOE, 6, kGPIO_Mode_OPP);
    
    UART_QuickInit(UART0_RX_PD06_TX_PD07, 115200);
    
    /* 打印信息 */
    printf(" type any character whitch will echo your input...\r\n");
    
    while(1)
    {
        /* 不停的查询 串口接收的状态 一旦接收成功 返回0 发送回接收到的数据 实现回环测试*/
        if(UART_ReadByte(HW_UART0, &ch) == 0)
        {
            /****************打印提示信息**************************/
            UART_WriteByte(HW_UART0,'R');
            UART_WriteByte(HW_UART0,'e');
            UART_WriteByte(HW_UART0,'c');
            UART_WriteByte(HW_UART0,'v');
            UART_WriteByte(HW_UART0,':');
            UART_WriteByte(HW_UART0,' ');
            /****************将接收到的字节打印出来****************/
            UART_WriteByte(HW_UART0, ch);
            /***********每次接受的数据打印完之后换行***************/
            UART_WriteByte(HW_UART0,'\r');
            UART_WriteByte(HW_UART0,'\n');				
        }
    }
}


