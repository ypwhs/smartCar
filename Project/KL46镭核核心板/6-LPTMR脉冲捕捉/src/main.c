#include "gpio.h"
#include "common.h"
#include "uart.h"
#include "pit.h"
#include "tpm.h"
#include "lptmr.h"


int main(void)
{
    int value;
    DelayInit();

    GPIO_QuickInit(HW_GPIOD, 7, kGPIO_Mode_OPP);
    GPIO_QuickInit(HW_GPIOE, 6, kGPIO_Mode_OPP);
    
    UART_QuickInit(UART0_RX_PA01_TX_PA02, 115200);
    
    printf("LPTMR test\r\n");
    printf("PWM will generated from PC04, LPTMR will measure pulse on PC05\r\n");
    printf("please connect PC04&PC05\r\n");
    
    /* 边沿对其方式产生PWM波 */
    TPM_PWM_QuickInit(TPM0_CH3_PC04, kPWM_EdgeAligned, 50);
    
    /* 占空比 50% */
    TPM_PWM_ChangeDuty(HW_TPM0, kTPM_IT_CH3, 5000);
    
    LPTMR_PC_QuickInit(LPTMR_ALT2_PC05);
    
    while(1)
    {
        value = LPTMR_PC_ReadCounter();
        printf("lptmr:%dHz\r\n", value);
        LPTMR_ClearCounter();
        DelayMs(1000);
    }
}


