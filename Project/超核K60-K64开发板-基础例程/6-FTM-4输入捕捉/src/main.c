#include "gpio.h"
#include "common.h"
#include "uart.h"
#include "ftm.h"


 static uint32_t InputCaptureValue;

static void FTM1_ISR(void)
{
    uint32_t clock;
    InputCaptureValue = FTM_GetChlCounter(HW_FTM1, HW_FTM_CH0);
    CLOCK_GetClockFrequency(kBusClock, &clock);
    FTM_SetMoudleCounter(HW_FTM1, 0); /* 复位计数值 */
    InputCaptureValue = (clock/128/InputCaptureValue);  /* 频率 = FTM输入时钟/分频/计数值 */
}


int main(void)
{
    DelayInit();
    UART_QuickInit(UART0_RX_PD06_TX_PD07, 115200);
    GPIO_QuickInit(HW_GPIOE, 6, kGPIO_Mode_OPP); /* LED */

    printf("ftm intput capture test, pwm will be generated on PA06 and input pin: PA08\r\n");
    
    FTM_PWM_QuickInit(FTM0_CH3_PA06, kPWM_EdgeAligned, 200);
    
    /* 配置IC 功能 设置中断 */
    FTM_IC_QuickInit(FTM1_CH0_PA08, kFTM_ClockDiv128);
    FTM_IC_SetTriggerMode(HW_FTM1, HW_FTM_CH0, kFTM_IC_FallingEdge);
    FTM_CallbackInstall(HW_FTM1, FTM1_ISR);
    FTM_ITDMAConfig(HW_FTM1, kFTM_IT_CH0, true);
    
    while(1)
    {
        printf("Frequency:%6dHz\r", InputCaptureValue);
        GPIO_ToggleBit(HW_GPIOE, 6); //控制小灯闪烁
        DelayMs(500);
    }
}


