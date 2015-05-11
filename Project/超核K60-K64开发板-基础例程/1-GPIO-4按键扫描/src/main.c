#include "gpio.h"
#include "common.h"

/* CH Kinetis固件库 V2.50 版本 */
/* 修改主频 请使用 CMSIS标准文件 startup_MKxxxx.c 中的 CLOCK_SETUP 宏 */

/*
     实验名称：GPIO按键扫描
     实验平台：渡鸦开发板
     板载芯片：MK60DN512ZVQ10
 实验效果：使用按键KEY1控制LED1小灯的状态，按键按下会改变小灯的亮灭
*/
//首先定义位操作的宏定义
#define KEY1  PEin(26)  //定义PTE端口的26引脚为输入
#define LED1  PEout(6)  //定义PTE端口的6引脚输出控制

/* 按键返回状态 0未按 1按下 */
#define NO_KEY          (0x00)
#define KEY_SINGLE      (0x01)
/* 按键返回值 */
static uint8_t gRetValue;

/* 状态机表 */
typedef enum
{
    kKEY_Idle,          /*空闲态 */
    kKEY_Debounce,      /*确认与消抖态 */
    kKEY_Confirm,       /*确认按键状态*/
}KEY_Status;

/* 按键扫描程序 */
static void KEY_Scan(void)
{
    static KEY_Status status = kKEY_Idle;
    switch(status)
    {
        case kKEY_Idle:
            gRetValue = NO_KEY;
            if(KEY1 == 0) /* 如果按键被按下 进入确认与消抖态 */
            {
                status = kKEY_Debounce;
            }
            break;
        case kKEY_Debounce: /* 确认与消抖态 */
            if(KEY1 == 0)
            {
                status = kKEY_Confirm;
            }
            else
            {
                status = kKEY_Idle;
                gRetValue = NO_KEY;
            }
            break;
        case kKEY_Confirm: /* 确认态 */
            if(KEY1 == 1) /* 按键松开 */
            {
                gRetValue = KEY_SINGLE;
                status = kKEY_Idle;
            }
            break;
        default:
            break;
    }
}

int main(void)
{
    /* 初始化 Delay */
    DelayInit();
    /* 将GPIO设置为输入模式 芯片内部自动配置上拉电阻 */
    GPIO_QuickInit(HW_GPIOE, 26, kGPIO_Mode_IPU);
    /* 设置为输出 */
    GPIO_QuickInit(HW_GPIOE,  6, kGPIO_Mode_OPP);
    while(1)
    {
        KEY_Scan(); //调用按键扫描程序
        DelayMs(10);
        if(gRetValue == KEY_SINGLE) //按键按下，小灯反正
        {
            LED1 = !LED1;
        }
    }
}


