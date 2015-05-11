#include "gpio.h"
#include "common.h"
#include "uart.h"
#include "pit.h"

#include "myos.h"

static OS_STK Task1_STK[256];
static OS_STK Task2_STK[256];
static OS_TCB Task1_TCB;
static OS_TCB Task2_TCB;

void PIT_ISR(void)
{
    OSTick();
}

void Task1()
{
    OS_TCB* TCB;
    while(1)
    {
        TCB = OSGetCurTCB();
        printf("%s Runing!!!\r\n", TCB->Name);
        OSTimeDly(10);
    }
}

void Task2()
{
    OS_TCB* TCB;
    while(1)
    {
        TCB = OSGetCurTCB();
        printf("%s Runing!!!\r\n", TCB->Name);
        OSTimeDly(10);
    }
}

void OSTaskSwHook(void)
{
   // printf("OSTaskSwHook\r\n");
}

int main(void)
{
    GPIO_QuickInit(HW_GPIOE, 6, kGPIO_Mode_OPP);
    UART_QuickInit(UART0_RX_PD06_TX_PD07, 115200);
    
    PIT_QuickInit(HW_PIT_CH0, 1000*100);
    PIT_CallbackInstall(HW_PIT_CH0, PIT_ISR);
    PIT_ITDMAConfig(HW_PIT_CH0, kPIT_IT_TOF, true);
    

    OSInit();
    OSTaskCreate("Task1", &Task1_TCB, Task1, &Task1_STK[64], 0);
    OSTaskCreate("Task2", &Task2_TCB, Task2, &Task2_STK[64], 1);
    OSStart();
    
    printf("OS error!\r\n");
    while(1)
    {
    }
}


