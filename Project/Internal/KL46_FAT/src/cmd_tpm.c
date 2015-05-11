#include "shell.h"
#include "tpm.h"
#include "common.h"


static void TPM1_ISR(void)
{
    printf("%s\r\n", __func__);
}


static int DoTPM(int argc, char * const argv[])
{
    int i;
    printf("func:%s\r\n", __func__);
   // for(i=0;i<ARRAY_SIZE(TPM_QuickInitTable);i++)
    {
  //      printf("(0x%08XU)\r\n", QuickInitEncode(&TPM_QuickInitTable[i]));
    }
    
    TPM_PWM_QuickInit(TPM0_CH3_PD03, kPWM_EdgeAligned, 20);
    for(i=0;i<10000;i++)
    {
        TPM_PWM_ChangeDuty(HW_TPM0, HW_TPM_CH3, i);
        DelayUs(100);
    }
    for(i=0;i<10000;i++)
    {
        TPM_PWM_ChangeDuty(HW_TPM0, HW_TPM_CH3, i);
        DelayUs(100);
    }
    for(i=0;i<10000;i++)
    {
        TPM_PWM_ChangeDuty(HW_TPM0, HW_TPM_CH3, i);
        DelayUs(100);
    }

    TPM_PWM_QuickInit(TPM1_CH0_PE20, kPWM_EdgeAligned, 30);
    TPM_CallbackInstall(HW_TPM1, TPM1_ISR);
    TPM_ITDMAConfig(HW_TPM1, kTPM_IT_CH0, true);
    
    return 0;
}

SHELL_EXPORT_CMD(DoTPM, tpm , tpm test)
    
