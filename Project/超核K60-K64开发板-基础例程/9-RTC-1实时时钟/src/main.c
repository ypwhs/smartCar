#include "gpio.h"
#include "common.h"
#include "uart.h"
#include "rtc.h"

/*
     实验名称: RTC实时时钟
     实验平台：渡鸦开发板
     板载芯片：MK60DN512ZVQ10
 实验效果：通过串口发送时钟时间   
*/
int main(void)
{
    DelayInit();
    GPIO_QuickInit(HW_GPIOE, 6, kGPIO_Mode_OPP);
    UART_QuickInit(UART0_RX_PD06_TX_PD07, 115200);
    
    /* 声明一个时间结构 */
    RTC_DateTime_Type td = {0};
    td.day = 20;
    td.hour = 23;
    td.minute = 59;
    td.second = 50;
    td.year = 2014;
    td.month = 11;
    printf("RTC test\r\n");
    RTC_QuickInit();
    /* 当时间无效(从来未执行过RTC时) 初始化时间 */
    if(RTC_IsTimeValid())
    {
        RTC_SetDateTime(&td);
    }
    while(1)
    {
        /* 获得时间 */
        RTC_GetDateTime(&td);//获得时间
        printf("%d-%d-%d %d:%d:%d\r\n", td.year, td.month, td.day, td.hour, td.minute, td.second);
        GPIO_ToggleBit(HW_GPIOE, 6);
        DelayMs(1000);
    }
}


