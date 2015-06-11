#include "shell.h"
#include "gpio.h"
#include "board.h"
#include "sram.h"

int DoSRAM(int argc, char * const argv[])
{
    uint32_t i, err_cnt;
    SRAM_Init();
    err_cnt = SRAM_SelfTest();
    if(err_cnt)
    {
        shell_printf("SRAM ERROR CNT:%d\r\n", err_cnt);
    }
    else
    {
        shell_printf("SRAM TEST PASSED\r\n");
    }
    return 0;
}

SHELL_EXPORT_CMD(DoSRAM, sram , sram test)
