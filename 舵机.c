#include "gpio.h"
#include "common.h"
#include "uart.h"
#include "ftm.h"

/* CH Kinetis固件库 V2.50 版本 */
/* 修改主频 请修改 CMSIS标准文件 system_MKxxxx.c 中的 CLOCK_SETUP 宏 */


/* 可用的FTM通道有: */
/*
 FTM0_CH4_PB12   // ftm0 模块 4通道 PB12引脚
 FTM0_CH5_PB13   
 FTM0_CH5_PA00   
 FTM0_CH6_PA01   
 FTM0_CH7_PA02   
 FTM0_CH0_PA03
 FTM0_CH1_PA04   
 FTM0_CH2_PA05   
 FTM0_CH3_PA06   
 FTM0_CH4_PA07   
 FTM0_CH0_PC01   
 FTM0_CH1_PC02   
 FTM0_CH2_PC03   
 FTM0_CH3_PC04   
 FTM0_CH4_PD04   
 FTM0_CH5_PD05   
 FTM0_CH6_PD06   
 FTM0_CH7_PD07   
 FTM1_CH0_PB12   
 FTM1_CH1_PB13   
 FTM1_CH0_PA08   
 FTM1_CH1_PA09   
 FTM1_CH0_PA12   
 FTM1_CH1_PA13   
 FTM1_CH0_PB00   
 FTM1_CH1_PB01   
 FTM2_CH0_PA10   
 FTM2_CH1_PA11   
 FTM2_CH0_PB18   
 FTM2_CH1_PB19   
*/

void turn(int angel){
    int pwm = (angel/180.0 + 1) * 500;
    FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH3, pwm);
}


int main(void)
{
    DelayInit();
    
    GPIO_QuickInit(HW_GPIOA, 6, kGPIO_Mode_OPP);
    /* 使用快速初始化 帮助初学者完成必要配置 */
    FTM_PWM_QuickInit(FTM0_CH3_PA06, kPWM_EdgeAligned, 50);
    FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH3, 1000);
    int f = 3;
    int ang = 0;
    int delay = 50;
    f=180*delay/6000;
    while(1)
    {
        if(ang<0|ang>180)f=-f;
        ang+=f;
        turn(ang
             ///3*2+30
                 );
        DelayMs(delay);
    }
}


