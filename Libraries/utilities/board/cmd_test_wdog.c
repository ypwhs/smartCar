#include "shell.h"
#include "wdog.h"
#include "common.h"
#include "systick.h"

void WDOG_ISR(void)
{
    shell_printf("%s\r\n", __func__);
}

int DoWDOG(int argc, char * const argv[])
{
    int timeout = 0;
    printf("WDOG TEST\r\n");
    printf("WDOG RESET COUNTER:%d\r\n", WDOG_GetResetCounter());
    WDOG_InitTypeDef WDOG_InitStruct1;
    WDOG_InitStruct1.mode = kWDOG_Mode_Normal;
    WDOG_InitStruct1.timeOutInMs = 500;
    WDOG_InitStruct1.windowInMs = 1800;
    NVIC_EnableIRQ(Watchdog_IRQn);
    WDOG_Init(&WDOG_InitStruct1);
    //貌似进不去中断
    WDOG_CallbackInstall(WDOG_ISR);
    WDOG_ITDMAConfig(ENABLE);
    
    while(1)
    {
        timeout++;
        if(timeout < 5)
        {
            printf("wodg reflash\r\n");
            WDOG_Refresh();
        }
        else
        {
            printf("stop reflash!\r\n");
        }
        DelayMs(400);
    }
}


SHELL_EXPORT_CMD(DoWDOG, wdog , wdog test)

