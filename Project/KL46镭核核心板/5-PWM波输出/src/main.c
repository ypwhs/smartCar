#include "gpio.h"
#include "common.h"
#include "uart.h"
#include "pit.h"
#include "tpm.h"



int main(void)
{
    int duty;
    DelayInit();

    GPIO_QuickInit(HW_GPIOD, 7, kGPIO_Mode_OPP);
    GPIO_QuickInit(HW_GPIOE, 6, kGPIO_Mode_OPP);
    
    UART_QuickInit(UART0_RX_PA01_TX_PA02, 115200);
    
    printf("PWM test, please measure waveform in PC04\r\n");
    
    /* 边沿对其方式产生PWM波 */
    TPM_PWM_QuickInit(TPM0_CH3_PC04, kPWM_EdgeAligned, 1000);
    
    while(1)
    {
        /* 不断改变占空比 */
        duty%=10000;
        TPM_PWM_ChangeDuty(HW_TPM0, kTPM_IT_CH3, duty++);
        DelayMs(1);
    }
}


