#include "shell.h"
#include "gpio.h"
#include "pdb.h"

void PDB_ISR(void)
{
    printf("PDB ISR enter\r\n");
}

static int DoPDB(int argc, char * const argv[])
{
    uint32_t clock;
    CLOCK_GetClockFrequency(kBusClock, &clock);
    PDB_InitTypeDef PDB_InitStruct1;
    PDB_InitStruct1.inputTrigSource = 15;
    PDB_InitStruct1.isContinuesMode = true;
    PDB_InitStruct1.srcClock = clock;
    PDB_Init(&PDB_InitStruct1);
    
    PDB_CallbackInstall(PDB_ISR);
    PDB_ITDMAConfig(kPDB_IT_CF, true);
    
    PDB_SoftwareTrigger();
}

SHELL_EXPORT_CMD(DoPDB, pdb , pdb test)
    

