#include "gpio.h"
#include "common.h"
#include "uart.h"
#include "adc.h"
#include "vref.h"

int main(void)
{
    DelayInit();
    GPIO_QuickInit(HW_GPIOE, 6, kGPIO_Mode_OPP);
    UART_QuickInit(UART0_RX_PD06_TX_PD07, 115200);
    printf("VREF Test please connect ADC1_SE16 & VERF_OUT\r\n");
    /* 初始化 VREF 输出1.2V基准电压 */
    VREF_QuickInit();
    ADC_QuickInit(ADC1_SE16, kADC_SingleDiff12or13);
    uint32_t val;
    while(1)
    {
        val = ADC_QuickReadValue(ADC1_SE16);
        printf("ADC:%d | %0.3fV  \r", val, (double)val*3.300/(1<<12));
        GPIO_ToggleBit(HW_GPIOE, 6);
        DelayMs(500);
    }
}


