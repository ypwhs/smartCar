#include "gpio.h"
#include "common.h"
#include "uart.h"
#include "pit.h"

void PIT_ISR(void)
{
    static int i;
    printf("%s %d\r\n", __func__, i++);
}


int main(void)
{
    DelayInit();

    GPIO_QuickInit(HW_GPIOD, 7, kGPIO_Mode_OPP);
    GPIO_QuickInit(HW_GPIOE, 6, kGPIO_Mode_OPP);
    
    UART_QuickInit(UART0_RX_PA01_TX_PA02, 115200);
    
    printf("PIT test\r\n");
    
    /* ¿ªÆôPIT ¶¨Ê±1S */
    PIT_QuickInit(HW_PIT_CH0, 1000*1000);
    PIT_CallbackInstall(HW_PIT_CH0, PIT_ISR);
    PIT_ITDMAConfig(HW_PIT_CH0, kPIT_IT_TOF);
    
    while(1)
    {
        GPIO_ToggleBit(HW_GPIOD, 7);
        GPIO_ToggleBit(HW_GPIOE, 6);
        DelayMs(50);
    }
}


