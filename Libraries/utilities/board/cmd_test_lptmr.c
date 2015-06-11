#include <stdlib.h>
#include "shell.h"
#include "lptmr.h"
#include "gpio.h"
//#include "tpm.h"
#include "board.h"

static void LPTMR_ISR(void)
{
    printf("%s\r\n", __func__);
}

int DoLPTMR(int argc, char * const argv[])
{
    printf("your sel is:%d\r\n", strtoul(argv[1],0,0));
    if(strtoul(argv[1],0,0) == 0)
    {
        printf("LPTMR TC test\r\n");
        LPTMR_TC_InitTypeDef LPTMR_TC_InitStruct1;
        LPTMR_TC_InitStruct1.timeInMs = 500;
    
        LPTMR_TC_Init(&LPTMR_TC_InitStruct1);
        LPTMR_ITDMAConfig(kLPTMR_IT_TOF, true);
        LPTMR_CallbackInstall(LPTMR_ISR);
        return 0;
    }
    if(strtoul(argv[1],0,0) == 1)
    {
        uint32_t val;
        printf("LPTMR PC test\r\n");
       // TPM_PWM_QuickInit(BOARD_FTM_MAP, kPWM_EdgeAligned, 1000);
        LPTMR_PC_QuickInit(BOARD_LPTMR_MAP);
        while(1)
        {
            val = LPTMR_PC_ReadCounter();
            printf("val:%d\r", val);
        }
    }
    return 0;
}

SHELL_EXPORT_CMD(DoLPTMR, lptmr , lptmr 0 lptmr 1)

