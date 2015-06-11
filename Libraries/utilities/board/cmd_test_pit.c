#include "shell.h"
#include "pit.h"
#include "board.h"

void PIT0_ISR(void)
{
    printf("%s\r\n", __func__);
}


void PIT1_ISR(void)
{
    printf("%s\r\n", __func__);
}

static int DoPIT(int argc, char * const argv[])
{
    PIT_QuickInit(HW_PIT_CH1, 500*1000);
    PIT_QuickInit(HW_PIT_CH0, 1000*1000);
    PIT_CallbackInstall(HW_PIT_CH0, PIT0_ISR);
    PIT_CallbackInstall(HW_PIT_CH1, PIT1_ISR);
    PIT_ITDMAConfig(HW_PIT_CH0, kPIT_IT_TOF, true);
    PIT_ITDMAConfig(HW_PIT_CH1, kPIT_IT_TOF, true);
    return 0;
}

SHELL_EXPORT_CMD(DoPIT, pit , pit test)
    

