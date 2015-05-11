#include "gpio.h"
#include "common.h"
#include "uart.h"

 

int main(void)
{
    uint32_t clock;
    DelayInit();
    /* 初始化一个引脚 设置为推挽输出 */
    GPIO_QuickInit(HW_GPIOD, 7, kGPIO_Mode_OPP);
    GPIO_QuickInit(HW_GPIOE, 6, kGPIO_Mode_OPP);
    
    UART_QuickInit(UART0_RX_PA01_TX_PA02, 115200);
    
    printf("HelloWorld\r\n");
    CLOCK_GetClockFrequency(kCoreClock, &clock);
    printf("CoreClock:%dHz\r\n", clock);
    CLOCK_GetClockFrequency(kBusClock, &clock);
    printf("kBusClock:%dHz\r\n", clock);

    while(1)
    {
        GPIO_ToggleBit(HW_GPIOD, 7);
        GPIO_ToggleBit(HW_GPIOE, 6);
        DelayMs(50);
    }
}


