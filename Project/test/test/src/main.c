#include "gpio.h"
#include "common.h"
#include "uart.h"
#include "dma.h"
#include "ftm.h"
#include "uart.h"

void turn(int angel){
    int pwm = (int)((angel/180.0 + 1) * 500);  //90度是1.5ms，0度是1ms，180度是2ms
    FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH3, pwm);
}
#define DRIVER_PWM_WIDTH 5000
void initDriver(){
    for(int i=0;i<8;i++)
        GPIO_QuickInit(HW_GPIOC, i, kGPIO_Mode_OPP);
    //初始化所有引脚为输出，其中
    /*
        C0----INH
        C1----OUT1
        C2----OUT2
        C3----OUT3
        C4----OUT4
        C5----LED1
        C6----LED2
        C7----LED3
    */
    FTM_PWM_QuickInit(FTM0_CH3_PA06, kPWM_EdgeAligned, 50);     //设置频率50HZ，20ms脉宽，边沿对齐模式
    FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH3, 750);              //设置初始占空比750/10000*100% = 7.5% = 1.5ms
    //初始化舵机

    FTM_PWM_QuickInit(FTM0_CH0_PC01, kPWM_EdgeAligned, DRIVER_PWM_WIDTH);
    FTM_PWM_QuickInit(FTM0_CH1_PC02, kPWM_EdgeAligned, DRIVER_PWM_WIDTH);
    FTM_PWM_QuickInit(FTM0_CH2_PC03, kPWM_EdgeAligned, DRIVER_PWM_WIDTH);
    FTM_PWM_QuickInit(FTM0_CH3_PC04, kPWM_EdgeAligned, DRIVER_PWM_WIDTH);
    //初始化电机PWM输出
}

void setLeftSpeed(int spd){
    if(spd>0){
        FTM_PWM_ChangeDuty(HW_FTM0, FTM0_CH2_PC03, spd);
        FTM_PWM_ChangeDuty(HW_FTM0, FTM0_CH3_PC04, 0);
    }else{
        FTM_PWM_ChangeDuty(HW_FTM0, FTM0_CH2_PC03, 0);
        FTM_PWM_ChangeDuty(HW_FTM0, FTM0_CH3_PC04, -spd);
    }
}

void setRightSpeed(int spd){
    if(spd>0){
        FTM_PWM_ChangeDuty(HW_FTM0, FTM0_CH0_PC01, spd);
        FTM_PWM_ChangeDuty(HW_FTM0, FTM0_CH1_PC02, 0);
    }else{
        FTM_PWM_ChangeDuty(HW_FTM0, FTM0_CH0_PC01, 0);
        FTM_PWM_ChangeDuty(HW_FTM0, FTM0_CH1_PC02, -spd);
    }
}

int main(void)
{

    DelayInit();
    /* 打印串口及小灯 */

    GPIO_QuickInit(HW_GPIOC, 3, kGPIO_Mode_OPP);
    UART_QuickInit(UART0_RX_PB16_TX_PB17, 115200);
    initDriver();
    while(1)
    {
        setRightSpeed(1000);
        DelayMs(1000);
        setRightSpeed(0);
        DelayMs(1000);
        setLeftSpeed(1000);
        DelayMs(1000);
        setLeftSpeed(0);
        DelayMs(1000);
    }
}
