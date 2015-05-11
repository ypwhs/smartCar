#include "gpio.h"
#include "common.h"
#include "uart.h"


void PendSV_Handler(void)
{
    printf("PendSV_Handler\r\n");
}

void SVC_Handler(void)
{
    printf("SVC_Handler\r\n");
}

 
int main(void)
{
    DelayInit();
    GPIO_QuickInit(HW_GPIOE, 6, kGPIO_Mode_OPP);
    UART_QuickInit(UART0_RX_PD06_TX_PD07, 115200);
    
    printf("PendSV & SVC IT test!\r\n");
    /* PendSV */
    SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
    /* SVC */
    SCB->SHCSR |= SCB_SHCSR_SVCALLPENDED_Msk;
    while(1)
    {
        /* …¡À∏–°µ∆ */
        GPIO_ToggleBit(HW_GPIOE, 6);
        DelayMs(500);
    }
}


