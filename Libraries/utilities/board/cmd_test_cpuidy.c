#include "shell.h"
#include "cpuidy.h"
#include "board.h"


int GoCPUIDY(int argc, char * const argv[])
{
    uint32_t uid[4];
    printf("cpu idy test\r\n");
    printf("%s %dpin\r\n", CPUIDY_GetFamID(), CPUIDY_GetPinCount());
    printf("PFlash:%dKB\r\n", CPUIDY_GetMemSize(kPFlashSizeInKB));
    printf("DFlash:%dKB\r\n", CPUIDY_GetMemSize(kDFlashSizeInKB));
    printf("FlexNVM:%dKB\r\n", CPUIDY_GetMemSize(kFlexNVMSizeInKB));
    printf("EEPROM:%dbyte\r\n", CPUIDY_GetMemSize(kEEPROMSizeInByte));
    printf("RAM:%dKB\r\n", CPUIDY_GetMemSize(kRAMSizeInKB));
    CPUIDY_GetUID(uid);
    printf("UID: 0x%08X 0x%08X 0x%08X 0x%08X\r\n",uid[0], uid[1], uid[2], uid[3]);
    return 0;
}

SHELL_EXPORT_CMD(GoCPUIDY, cpuidy , cpuidy test)


