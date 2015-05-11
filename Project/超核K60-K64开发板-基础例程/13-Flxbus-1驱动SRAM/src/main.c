#include "gpio.h"
#include "common.h"
#include "uart.h"
#include "flexbus.h"

#include "sram.h" 
/* CH Kinetis固件库 V2.50 版本 */
/* 修改主频 请修改 CMSIS标准文件 system_MKxxxx.c 中的 CLOCK_SETUP 宏 */

/*
     实验名称：Flexbus驱动ARAM
     实验平台：渡鸦开发板
     板载芯片：MK60DN512ZVQ10
 实验效果：测试外挂的SRAM工作情况  
       具体的SRAM应用请参见sram.c文件
*/
int main(void)
{
    DelayInit();
    GPIO_QuickInit(HW_GPIOE, 6, kGPIO_Mode_OPP);
    UART_QuickInit(UART0_RX_PD06_TX_PD07, 115200);
    
    printf("Flexbus SRAM test\r\n");
    
    /* 初始化外部SRAM */
    SRAM_Init();
    /* SRAM 自测 */
    if(!SRAM_SelfTest())
    {
        printf("sram test ok!\r\n");
    }
    else
    {
        printf("sram test failed!\r\n");
    }
    while(1)
    {
        GPIO_ToggleBit(HW_GPIOE, 6);
        DelayMs(500);
    }
}


