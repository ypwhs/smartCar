#include "gpio.h"
#include "common.h"

/* CH Kinetis固件库 V2.50 版本 */
/* 修改主频 请使用 CMSIS标准文件 startup_MKxxxx.c 中的 CLOCK_SETUP 宏 */

/*
     实验名称：GPIO位带操作
     实验平台：渡鸦开发板
     板载芯片：MK60DN512ZVQ10
 实验效果：通过使用位带操作控制开发板上的4颗小灯亮灭 
*/
//首先定义位操作的宏定义
#define LED1  PEout(6)  //定义PTE端口的6引脚输出控制
#define LED2  PEout(7)  //定义PTE端口的7引脚输出控制
#define LED3  PEout(11) //定义PTE端口的11引脚输出控制
#define LED4  PEout(12) //定义PTE端口的12引脚输出控制

int main(void)
{
    /* 初始化 Delay */
    DelayInit();

    /* 设置4个小灯的控制引脚为开漏输出 */
    GPIO_QuickInit(HW_GPIOE,  6, kGPIO_Mode_OPP);
    GPIO_QuickInit(HW_GPIOE,  7, kGPIO_Mode_OPP); 
    GPIO_QuickInit(HW_GPIOE, 11, kGPIO_Mode_OPP); 
    GPIO_QuickInit(HW_GPIOE, 12, kGPIO_Mode_OPP); 
    while(1)
    {
        /* 流水灯 */
        DelayMs(100);
        LED1 = !LED1; //改变输出状态，原先为高电平现在输出低电平，或者相反。
        DelayMs(100);
        LED2 = !LED2;
        DelayMs(100);
        LED3 = !LED3;
        DelayMs(100);
        LED4 = !LED4;
    }
}


