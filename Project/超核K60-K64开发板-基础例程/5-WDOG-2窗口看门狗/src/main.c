#include "gpio.h"
#include "common.h"
#include "uart.h"
#include "wdog.h"

/* CH Kinetis固件库 V2.50 版本 */
/* 修改主频 请使用 CMSIS标准文件 startup_MKxxxx.c 中的 CLOCK_SETUP 宏 */

/*
     实验名称：WDOG窗口看门狗
     实验平台：渡鸦开发板
     板载芯片：MK60DN512ZVQ10
 实验效果：开启看门狗的窗口模式，必须在规定的时间范围内喂狗，否则芯片复位
*/
int main(void)
{
    DelayInit();
    
    GPIO_QuickInit(HW_GPIOE, 6, kGPIO_Mode_OPP); /* LED */
    
    UART_QuickInit(UART0_RX_PD06_TX_PD07, 115200);
    
    /* 初始化看门狗 */
    WDOG_InitTypeDef WDOG_InitStruct1 = {0};
    WDOG_InitStruct1.mode = kWDOG_Mode_Window;   //设置看门狗为窗口模式
    WDOG_InitStruct1.windowInMs = 1000;   /* 开窗时间 设置为窗体模式后 喂狗必须在 看门狗开始计时后 1000 - 2000 MS内完成 多了少了都复位 比普通看门狗严格*/
    WDOG_InitStruct1.timeOutInMs = 2000; /* 时限 2000MS : 2000MS 内没有喂狗则复位 */
    WDOG_Init(&WDOG_InitStruct1);
    
    printf("\r\nSYSTEM RESET!!!!!!!%d\r\n", WDOG_GetResetCounter());
    printf("press any character to feed dog feed, must be in windows time\r\n");
    
    static uint32_t i;
    uint16_t ch;
    while(1)
    {
        if(UART_ReadByte(HW_UART0, &ch) == 0)
        {
            printf("wdog feed succ!\r\n");
            WDOG_Refresh(); //喂狗
            i = 0;
        }
        printf("cnt:i:%d\r", i++);
        DelayMs(100);
        GPIO_ToggleBit(HW_GPIOE, 6);
    }
}


