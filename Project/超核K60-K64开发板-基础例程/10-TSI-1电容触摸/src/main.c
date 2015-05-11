#include "gpio.h"
#include "common.h"
#include "uart.h"
#include "tsi.h"
/* CH Kinetis固件库 V2.50 版本 */
/* 修改主频 请修改 CMSIS标准文件 system_MKxxxx.c 中的 CLOCK_SETUP 宏 */

/*
     实验名称：TSI电容触摸
     实验平台：渡鸦开发板
     板载芯片：MK60DN512ZVQ10
 实验效果：开发板下方的4个触控按键，按下时对应的小灯就会改变状态
        说明：触控按键占用了下载器的接口，需要把下载器拔掉，当再次
        下载程序时需要按着复位键下载。
*/



int main(void)
{
    DelayInit();
    
    GPIO_QuickInit(HW_GPIOE, 6, kGPIO_Mode_OPP);
    UART_QuickInit(UART0_RX_PD06_TX_PD07, 115200); /* 为了代码简单 这里直接调用快速初始化了 */
    
    printf("TSI counter test\r\n");
    
    /* 使能1个TSI 触摸引脚 注意 TSI上电需要校准 下载后 必须拔掉 JLINK 然后 复位 */
    TSI_QuickInit(TSI0_CH1_PA00);  //使用PTA0引脚作为触控的1通道

    while(1)
    {
        printf("tsi ch[%d]:%06d\r", 0, TSI_GetCounter(HW_TSI_CH1));
        GPIO_ToggleBit(HW_GPIOE, 6);
        DelayMs(10);
    }
}


