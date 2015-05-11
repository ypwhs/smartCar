#include "shell.h"
#include "gpio.h"
#include "pdb.h"

void PDB_ISR(void)
{
    static int i;
    printf("%s %d\r\n", __func__, i++);
    if(i == 10)
    {
        printf("Close PDB IT\r\n");
        PDB_ITDMAConfig(kPDB_IT_CF, false);
    }
    
}

static int DoPDB(int argc, char * const argv[])
{
    uint32_t clock;
    CLOCK_GetClockFrequency(kBusClock, &clock);
    PDB_InitTypeDef PDB_InitStruct1;
    PDB_InitStruct1.inputTrigSource = kPDB_SoftwareTrigger;
    PDB_InitStruct1.isContinuesMode = true;
    PDB_InitStruct1.timeInUs = 200*1000;
    PDB_InitStruct1.srcClock = clock;
    PDB_Init(&PDB_InitStruct1);
    
    PDB_CallbackInstall(PDB_ISR);
    PDB_ITDMAConfig(kPDB_IT_CF, true);
    
    PDB_SoftwareTrigger();
    return 0;
}

SHELL_EXPORT_CMD(DoPDB, pdb , pdb test)
