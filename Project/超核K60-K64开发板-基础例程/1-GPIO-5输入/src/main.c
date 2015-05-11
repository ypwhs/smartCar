#include "gpio.h"
#include "common.h"

/* CH Kinetis固件库 V2.50 版本 */
/* 修改主频 请使用 CMSIS标准文件 startup_MKxxxx.c 中的 CLOCK_SETUP 宏 */

/* GPIO 工作模式 */
/*
 *         @arg kGPIO_Mode_IFT :悬空输入
 *         @arg kGPIO_Mode_IPD :下拉输入
 *         @arg kGPIO_Mode_IPU :上拉输入
 *         @arg kGPIO_Mode_OOD :开漏输出 如果不清楚这2种输出的区别请 百度..
 *         @arg kGPIO_Mode_OPP :推挽输出
 */
 
 /*
     实验名称：GPIO按键输入
     实验平台：渡鸦开发板
     板载芯片：MK60DN512ZVQ10
 实验效果：控制开发板上的3个普通按键，当按键按下时
        引脚信号为低电平，采用轮询的方式读取按键的状态
        并使用3个小灯的亮灭展示按键的效果
*/
int main(void)
{
    bool val[3];
    /* 初始化Delay */
    DelayInit();
    /* 将GPIO设置为输入模式 芯片内部自动配置上拉电阻 */
    GPIO_QuickInit(HW_GPIOE, 26, kGPIO_Mode_IPU);
    GPIO_QuickInit(HW_GPIOE, 27, kGPIO_Mode_IPU);
    GPIO_QuickInit(HW_GPIOA,  4, kGPIO_Mode_IPU);
    /* 设置为输出 */
    GPIO_QuickInit(HW_GPIOE,  7, kGPIO_Mode_OPP);
    GPIO_QuickInit(HW_GPIOE,  6, kGPIO_Mode_OPP);
    GPIO_QuickInit(HW_GPIOE,  11, kGPIO_Mode_OPP);
    while(1)
    {
        /* 不断读取 引脚电平状态 返回0 或 1*/
        val[0] = GPIO_ReadBit(HW_GPIOE, 26);
        val[1] = GPIO_ReadBit(HW_GPIOE, 27);
        val[2] = GPIO_ReadBit(HW_GPIOA, 4);
        /* 将返回的电平设置到另外一个引脚上 */
        GPIO_WriteBit(HW_GPIOE, 11, val[0]);
        GPIO_WriteBit(HW_GPIOE,  7, val[1]);
        GPIO_WriteBit(HW_GPIOE,  6, val[2]);
    }
}


