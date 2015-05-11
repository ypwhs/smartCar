#include "gpio.h"
#include "common.h"
#include "systick.h"

/* CH Kinetis固件库 V2.50 版本 */
/* 修改主频 请使用 CMSIS标准文件 startup_MKxxxx.c 中的 CLOCK_SETUP 宏 */

/*
     实验名称：Systick实验
     实验平台：渡鸦开发板
     板载芯片：MK60DN512ZVQ10
 实验效果：LED1等周期性的闪烁，时间间隔为100ms
*/
#define LED1  PEout(6) //定义PTE端口的6引脚输出控制

int main(void)
{

    /* 设置为输出 */
    GPIO_QuickInit(HW_GPIOE, 6, kGPIO_Mode_OPP);
    /*初始化配置systick模块*/
    DelayInit();
    
    while(1)
    {
        LED1 = !LED1;
        DelayMs(100); //使用systick实现延时功能
    }
}


