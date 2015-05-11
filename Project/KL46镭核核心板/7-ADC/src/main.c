#include "gpio.h"
#include "common.h"
#include "uart.h"
#include "pit.h"
#include "tpm.h"
#include "lptmr.h"
#include "adc.h"


int main(void)
{
    int value;
    DelayInit();

    GPIO_QuickInit(HW_GPIOD, 7, kGPIO_Mode_OPP);
    GPIO_QuickInit(HW_GPIOE, 6, kGPIO_Mode_OPP);
    
    UART_QuickInit(UART0_RX_PA01_TX_PA02, 115200);
    
    printf("ADC test\r\n");
    
    ADC_QuickInit(ADC0_SE5B_PD01, kADC_SingleDiff12or13);

    while(1)
    {
        value = ADC_QuickReadValue(ADC0_SE5B_PD01);
        printf("ADC:%d\r\n", value);
        DelayMs(1000);
    }
}


