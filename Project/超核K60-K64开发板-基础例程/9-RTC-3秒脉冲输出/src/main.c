#include "gpio.h"
#include "common.h"
#include "uart.h"
#include "rtc.h"

void RTC_ISR(void)
{
    printf("RTC INT\r\n");
}

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
    printf("RTC clk out test, second pulse in on PE26\r\n");
    
    RTC_QuickInit();
    if(RTC_IsTimeValid() != 0)
    {
        RTC_SetDateTime(&td);
    }
    /* 开启中断 */
    RTC_CallbackInstall(RTC_ISR);
    RTC_ITDMAConfig(kRTC_IT_TimeAlarm, true);
    
    /* RTC_CLKOUT 输出 */
    SIM->SOPT2 &= ~SIM_SOPT2_RTCCLKOUTSEL_MASK;
    PORT_PinMuxConfig(HW_GPIOE, 26, kPinAlt6);
    
    while(1)
    {
        RTC_GetDateTime(&td);
        printf("%d-%d-%d %d:%d:%d\r\n", td.year, td.month, td.day, td.hour, td.minute, td.second);
        GPIO_ToggleBit(HW_GPIOE, 6);
        DelayMs(1000);
    }
}


