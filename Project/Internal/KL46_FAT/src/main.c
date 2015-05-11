#include "gpio.h"
#include "common.h"
#include "uart.h"
#include "shell.h"
#include "board.h"
 
int main(void)
{
    DelayInit();

    UART_QuickInit(BOARD_UART_DEBUG_MAP, 115200);
    printf("HelloWorld!\r\n");
    shell_init();
    while(1)
    {
        shell_main_loop("SHELL>>");
    }
}




