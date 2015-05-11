#include "gpio.h"
#include "common.h"
#include "systick.h"
#include "uart.h"
//uCOS
#include "includes.h"



//任务堆栈大小
#define TASK_STK_SIZE              (128)
//定义任务优先级
#define APP_LED1_TASK_PRIO         (5)
#define APP_LED0_TASK_PRIO         (7)
//声明任务堆栈
OS_STK  APP_LED1_STK[TASK_STK_SIZE];
OS_STK  APP_LED0_STK[TASK_STK_SIZE];
//LED小灯任务
void AppLED1Task(void *pdata)
{
    pdata = pdata; //防止编译器出错 无实际意义
	while(1)
	{
		printf("LED1 Task!\r\n");
		OSTimeDlyHMSM(0, 0, 0, 500);
	}
}

void AppLED0Task(void *pdata)
{
    pdata = pdata; //防止编译器出错 无实际意义
	while(1)
	{
		printf("LED2 Task!\r\n");
		OSTimeDlyHMSM(0, 0, 0, 500);
	}
}


int main(void)
{
    DelayInit();
    SYSTICK_Init((1000*1000)/OS_TICKS_PER_SEC);
    SYSTICK_ITConfig(true);
    
    UART_QuickInit(UART0_RX_PD06_TX_PD07, 115200);
    
    printf("uCOSII test\r\n");

    OSInit();  //OS初始化
    OSTaskCreate(AppLED1Task,(void *)0,
                &APP_LED1_STK[TASK_STK_SIZE-1],
                APP_LED1_TASK_PRIO); //建立LED1 任务
    OSTaskCreate(AppLED0Task,(void *)0,
                &APP_LED0_STK[TASK_STK_SIZE-1],
                APP_LED0_TASK_PRIO); //建立LED0 任务
    
    SYSTICK_Cmd(true);
    
    /* 控制权交给操作系统 */
    OSStart();
	/* 程序永远不会运行到这 */
    while(1);
}


