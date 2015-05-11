#include "gpio.h"
#include "common.h"

/* CH Kinetis固件库 V2.50 版本 */
/* 修改主频 请使用 CMSIS标准文件 system_MKxxxx.c 中的 CLOCK_SETUP 宏 */

/* GPIO 工作模式 */
/*
 *         @arg kGPIO_Mode_IFT :悬空输入
 *         @arg kGPIO_Mode_IPD :下拉输入
 *         @arg kGPIO_Mode_IPU :上拉输入
 *         @arg kGPIO_Mode_OOD :开漏输出 如果不清楚这2种输出的区别请 百度..
 *         @arg kGPIO_Mode_OPP :推挽输出
 */
 
/*
     实验名称：GPIO蜂鸣器
     实验平台：渡鸦开发板
     板载芯片：MK60DN512ZVQ10
 实验效果：控制PTA端口的6引脚是输出模式
        该引脚将周期性输出高低电平，蜂鸣器发出嗡嗡声 
*/

int main(void)
{
    /* 初始化Delay */
    DelayInit();
    /* 使用简易初始化初始化一个GPIO 设置为推挽输出模式 */
    GPIO_QuickInit(HW_GPIOA, 6, kGPIO_Mode_OPP);
    /* 另外一种初始化方式: 结构体模式的初始化 */
//    GPIO_InitTypeDef GPIO_InitStruct1;
//    GPIO_InitStruct1.instance = HW_GPIOA;
//    GPIO_InitStruct1.mode = kGPIO_Mode_OPP;
//    GPIO_InitStruct1.pinx = 6;
//    GPIO_Init(&GPIO_InitStruct1);
    
    /* 输出低电平 */
    GPIO_WriteBit(HW_GPIOA, 6, 0);
    
    while(1)
    {
        /* 翻转引脚电平 原来是低变成高 原来是高变成低 */
        GPIO_ToggleBit(HW_GPIOA, 6);
        DelayMs(300); //延时 10ms
    }
}


