#include "shell.h"
#include "common.h"
#include "cpuidy.h"

const char* ClockSourceNameTable[] = 
{
	"CoreClock",
	"SystemClock",
	"BusClock",
	"FlashClock"
};

const char* MemNameTable[] = 
{
	"ProramFlashSize",
	"DadaFlashSize",
	"FLexNVMSize",
	"EEPORMSizeInByte",
	"RAMSize",
};


int DoCPUInfo(int argc, char *const argv[])
{
    uint8_t i;
    uint32_t val;
    val = CPUIDY_GetPinCount();
    uint32_t UID[4];
    if(argc == 1)
    {
        shell_printf("%-*s- %s - %dPin\r\n", 16, "CPU:", CPUIDY_GetFamID(), val);
        for(i=0;i< kClockNameCount; i++)
        {
            CLOCK_GetClockFrequency((CLOCK_Source_Type)i,&val);
            shell_printf("%-*s- %dKHZ\r\n", 16, ClockSourceNameTable[i],val/1000);
        }
        for(i=0;i< kMemNameCount; i++)
        {
            val = CPUIDY_GetMemSize((CLOCK_Source_Type)i);
            shell_printf("%-*s- %dKB\r\n", 16, MemNameTable[i],val);
        }
        CPUIDY_GetUID(UID);
        for(i = 0; i < 4; i++)
        {
            shell_printf("UID[%i]:0x%x\r\n", i, UID[i]);
        }
    }
    return 0;
}
 
const cmd_tbl_t CommandFun_CPU = 
{
    .name = "CPU",
    .maxargs = 2,
    .repeatable = 1,
    .cmd = DoCPUInfo,
    .usage = "display CPU clock",
    .complete = NULL,
    .help = "\r\n"

};
