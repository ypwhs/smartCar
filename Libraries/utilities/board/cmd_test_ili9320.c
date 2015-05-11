#include "shell.h"
#include "gpio.h"
#include "board.h"
#include "ili9320.h"

int DoILI9320(int argc, char * const argv[])
{
    uint32_t i;
    ili9320_init();
    ili9320_clear(RED);
    return 0;
}

SHELL_EXPORT_CMD(DoILI9320, ili9320 , ili9320 test)



