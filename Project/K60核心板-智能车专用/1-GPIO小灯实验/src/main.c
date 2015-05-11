#include "gpio.h"
#include "common.h"
/* CH Kinetis固件库 V2.50 版本 */
/* 修改主频 请修改 CMSIS标准文件 system_MKxxxx.c 中的 CLOCK_SETUP 宏 */

/* GPIO 工作模式 */
/*
 *         @arg kGPIO_Mode_IFT :悬空输入
 *         @arg kGPIO_Mode_IPD :下拉输入
 *         @arg kGPIO_Mode_IPU :上拉输入
 *         @arg kGPIO_Mode_OOD :开漏输出 如果不清楚这2种输出的区别请 百度..
 *         @arg kGPIO_Mode_OPP :推挽输出
 */

 
int main(void)
{
    DelayInit();
    GPIO_QuickInit(HW_GPIOD, 10, kGPIO_Mode_OPP);
    GPIO_WriteBit(HW_GPIOD, 10, 0);
    
    while(1)
    {
        PDout(10) = !PDout(10);
        DelayMs(500);
    }
}


