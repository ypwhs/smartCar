#include "shell.h"
#include "gpio.h"
#include "board.h"

int DoRESET(int argc, char * const argv[])
{
    SystemSoftReset();
    return 0;
}

SHELL_EXPORT_CMD(DoRESET, reset , reset test)
    

