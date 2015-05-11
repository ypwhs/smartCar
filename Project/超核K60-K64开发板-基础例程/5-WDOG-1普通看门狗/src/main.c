#include "gpio.h"
#include "common.h"
#include "uart.h"
#include "wdog.h"

/* CH Kinetis固件库 V2.50 版本 */
/* 修改主频 请使用 CMSIS标准文件 startup_MKxxxx.c 中的 CLOCK_SETUP 宏 */

/*
     实验名称：WDOG普通看门狗
     实验平台：渡鸦开发板
     板载芯片：MK60DN512ZVQ10
 实验效果：芯片复位后LED灯闪烁一次，如果按键KEY2按一次，将喂狗一次
       芯片将不复位，如果时间超了2s或者不按按键，看门狗将超时，芯片将
       复位。对于按键采用扫描的方式进行动作识别。
*/

int main(void)
{
    DelayInit();
    
    GPIO_QuickInit(HW_GPIOE, 6, kGPIO_Mode_OPP); /* LED */
    GPIO_QuickInit(HW_GPIOE, 26, kGPIO_Mode_IPU); /* KEY */
    
    UART_QuickInit(UART0_RX_PD06_TX_PD07, 115200);
    
    /* 初始化看门狗 */
    WDOG_InitTypeDef WDOG_InitStruct1 = {0};
    WDOG_InitStruct1.mode = kWDOG_Mode_Normal;  //设置看门狗处于正常工作模式
    WDOG_InitStruct1.timeOutInMs = 2000; /* 时限 2000MS : 2000MS 内没有喂狗则复位 */
    WDOG_Init(&WDOG_InitStruct1);
    
    printf("WDOG test start!\r\n");
    printf("press KEY1 to feed dog within 2S or system with reset!\r\n");
    
    /* 点亮LED 然后熄灭  指示系统运行从新上电运行 */
    GPIO_WriteBit(HW_GPIOE, 6, 0);
    DelayMs(200);
    GPIO_WriteBit(HW_GPIOE, 6, 1);
    while(1)
    {
        if(GPIO_ReadBit(HW_GPIOE, 26) == 0) /* 按键被按下 */
        {
            /* 喂狗 防止复位 */
            printf("wdog feed! we have 2s\r\n");
            WDOG_Refresh(); //喂狗
            DelayMs(100);
        }
        DelayMs(10);
    }
}


