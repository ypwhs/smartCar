#include "shell.h"
#include "lptmr.h"
#include "gpio.h"
#include "tpm.h"

static void LPTMR_ISR(void)
{
    printf("%s\r\n", __func__);
    
}

int DoLPTMR(int argc, char * const argv[])
{
    LPTMR_TC_InitTypeDef LPTMR_TC_InitStruct1;
    LPTMR_TC_InitStruct1.timeInMs = 500;
    
    LPTMR_TC_Init(&LPTMR_TC_InitStruct1);
    LPTMR_ITDMAConfig(kLPTMR_IT_TOF, true);
    LPTMR_CallbackInstall(LPTMR_ISR);
    
    DelayMs(2000);
    printf("begin test LPTMR PC\r\n");
    LPTMR_PC_QuickInit(LPTMR_ALT1_PA19);
    
    return 0;
}

SHELL_EXPORT_CMD(DoLPTMR, lptmr , lptmr test)
    

