#include "gpio.h"
#include "common.h"
#include "uart.h"

 void UART_ISR(uint16_t data)
 {
     printf("Received Char:%d %c\r\n", data, data);
 }

int main(void)
{
    DelayInit();

    GPIO_QuickInit(HW_GPIOD, 7, kGPIO_Mode_OPP);
    GPIO_QuickInit(HW_GPIOE, 6, kGPIO_Mode_OPP);
    
    UART_QuickInit(UART0_RX_PA01_TX_PA02, 115200);
    
    printf("HelloWorld\r\n");
    
    /* 打开串口中断 */
    UART_ITDMAConfig(HW_UART0, kUART_IT_Rx, true);
    UART_CallbackRxInstall(HW_UART0, UART_ISR);
    
    while(1)
    {
        GPIO_ToggleBit(HW_GPIOD, 7);
        GPIO_ToggleBit(HW_GPIOE, 6);
        DelayMs(50);
    }
}


