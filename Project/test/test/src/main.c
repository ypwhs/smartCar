#include "gpio.h"
#include "common.h"
#include "uart.h"
#include "dma.h"
#include "ftm.h"
#include "uart.h"
#include "lptmr.h"

#define offset 77;
void turn(int angel){
    angel += offset;
    int pwm = (int)((angel/90.0 + 0.5) * 500);  //90度是1.5ms
    printf("turn:%d\r\n", pwm);
    FTM_PWM_ChangeDuty(HW_FTM1, HW_FTM_CH0, pwm);
}

#define DRIVER_PWM_WIDTH 100
void initDriver(){
    printf("initPWM\r\n");

    for(int i=0;i<0;i++)
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
    PCout(0)=1;
    //使能INH

    FTM_PWM_QuickInit(FTM1_CH0_PA12, kPWM_EdgeAligned, 50);     //设置FTM，边沿对齐模式
    turn(0);
    //初始化舵机

    FTM_PWM_QuickInit(FTM0_CH0_PC01, kPWM_EdgeAligned, DRIVER_PWM_WIDTH);
    FTM_PWM_QuickInit(FTM0_CH1_PC02, kPWM_EdgeAligned, DRIVER_PWM_WIDTH);
    FTM_PWM_QuickInit(FTM0_CH2_PC03, kPWM_EdgeAligned, DRIVER_PWM_WIDTH);
    FTM_PWM_QuickInit(FTM0_CH3_PC04, kPWM_EdgeAligned, DRIVER_PWM_WIDTH);
    //初始化电机PWM输出
}

void setLeftSpeed(int spd){
    if(spd>0){
        FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH1, spd);
        FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH0, 0);
    }else
    {
        FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH1, 0);
        FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH0, -spd);
    }
}

void setRightSpeed(int spd){
    if(spd>0){
        FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH2, spd);
        FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH3, 0);
    }else
    {
        FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH2, 0);
        FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH3, -spd);
    }
}

void delay(){
    DelayMs(500);
}


#define spd 1000
//串口接收中断

void UART_RX_ISR(uint16_t byteRec){
    PCout(3)=!PCout(3);
    switch(byteRec){
    case 'w':
        setLeftSpeed(spd);
        setRightSpeed(spd);
        break;
    case 's':
        setLeftSpeed(-spd);
        setRightSpeed(-spd);
        break;
    case 'a':
        turn(-30);
        break;
    case 'd':
        turn(30);
        break;
    case 'x':
        turn(0);
        break;
    default:
        setLeftSpeed(0);
        setRightSpeed(0);
        break;
    }
    printf("%c", byteRec);
}

void initUART(){
    UART_QuickInit(UART0_RX_PB16_TX_PB17, 115200);
    /*
    UART0_RX_PB16_TX_PB17
    UART4_RX_PC14_TX_PC15
    */

    /* 注册中断回调函数 */
    UART_CallbackRxInstall(HW_UART0, UART_RX_ISR);

    /* 开启UART Rx中断 */
    UART_ITDMAConfig(HW_UART0, kUART_IT_Rx, true);
}

int main(void)
{

    DelayInit();
    /* 打印串口及小灯 */

    GPIO_QuickInit(HW_GPIOD, 10, kGPIO_Mode_OPP);

    initUART();
    initDriver();

    while(1)
    {
        PDout(10)=!PDout(10);
        DelayMs(100);
    }
}
