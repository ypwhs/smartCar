#include "gpio.h"
#include "common.h"
#include "uart.h"
/* CH Kinetis固件库 V2.50 版本 */
/* 修改主频 请修改 CMSIS标准文件 system_MKxxxx.c 中的 CLOCK_SETUP 宏 */
 
/*
     实验名称：485总线
     实验平台：渡鸦开发板
     板载芯片：MK60DN512ZVQ10
 实验效果：类似与串口，通过开发板上的485通信接口通信  
        485通信采用3.3v的MAX3485或SP3485，自收发电路设计
*/
/* 发送一串字符 */
static void UART_SendString(uint32_t instance, char * str)
{
    while(*str != '\0')
    {
        UART_WriteByte(instance, *str++);
    }
}

int main(void)
{
    DelayInit();
    GPIO_QuickInit(HW_GPIOE, 6, kGPIO_Mode_OPP);
    /* 调试串口 */
    UART_QuickInit(UART0_RX_PD06_TX_PD07, 115200);
    /* 485 和 普通的TTL 串口 只是后级硬件不同 对于MCU来说，都是串口 */
    UART_QuickInit(UART5_RX_PE09_TX_PE08, 115200);
    printf("485 test! please connnect other 485 device\r\n");
    
    /* 向485总线发送一串字符 */
    UART_SendString(HW_UART5, "485 device test\r\n");
    uint16_t ch;
    while(1)
    {
        /* 将485总线的数据发回去 */
        if(UART_ReadByte(HW_UART5, &ch) == 0)
        {
            UART_WriteByte(HW_UART5, ch);
        }
        GPIO_ToggleBit(HW_GPIOE, 6);
    }
}


