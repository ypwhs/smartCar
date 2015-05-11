#include "gpio.h"
#include "common.h"
#include "uart.h"

 void UART_ISR(uint16_t data)
 {
     printf("Received Char:%d %c\r\n", data, data);
 }

int main(void)
{
    uint32_t clock;
    DelayInit();

    GPIO_QuickInit(HW_GPIOD, 7, kGPIO_Mode_OPP);
    GPIO_QuickInit(HW_GPIOE, 6, kGPIO_Mode_OPP);
    
    UART_QuickInit(UART0_RX_PA01_TX_PA02, 115200);
    
    printf("HelloWorld\r\n");
    
    CLOCK_GetClockFrequency(kCoreClock, &clock);
    printf("core clock:%dHz\r\n", clock);
    CLOCK_GetClockFrequency(kBusClock, &clock);
    printf("bus clock:%dHz\r\n", clock);
    
    while(1)
    {
        GPIO_ToggleBit(HW_GPIOD, 7);
        GPIO_ToggleBit(HW_GPIOE, 6);
        DelayMs(50);
    }
}


