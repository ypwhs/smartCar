#include <stdlib.h>
#include "shell.h"
#include "board.h"
#include "gpio.h"

/*
000 FlexBus CLKOUT
001 Reserved
010 Flash clock
011 LPO clock (1 kHz)
100 MCGIRCLK
101 RTC 32.768kHz clock
110 OSCERCLK0
111 IRC 48 MHz clock
*/

/* KV10 
000 Reserved
001 Reserved
010 Bus clock
011 LPO clock (1 kHz)
100 MCGIRCLK
101 Reserved
110 OSCERCLK
111 Reserved
*/

static const char *ClkNameTable[] = 
{
    "FlexBus CLKOUT",
    "Reserved",
    "Flash clock",
    "LPO clock (1 kHz)",
    "MCGIRCLK",
    "RTC 32.768kHz clock",
    "OSCERCLK0",
    "IRC 48 MHz clock",
};

int DoCLKOUT(int argc, char * const argv[])
{
    int i;
    i = strtoul(argv[1], 0, 0);
    
    printf("please see RM SOPT2 for desc\r\n");
    printf("clkout sel:[%d]:%s\r\n", i, ClkNameTable[i]);
    
    SIM->SOPT2 &= ~SIM_SOPT2_CLKOUTSEL_MASK;
    SIM->SOPT2 |= SIM_SOPT2_CLKOUTSEL(i);
    
    printf("P%c%1d\r\n", BOARD_CLKOUT_PORT+'A', BOARD_CLKOUT_PIN);
    PORT_PinMuxConfig(BOARD_CLKOUT_PORT, BOARD_CLKOUT_PIN, kPinAlt5);

    //MCG->C2 &= ~MCG_C2_EREFS0_MASK;
    //OSC->CR |= OSC_CR_ERCLKEN_MASK;
    return 0;
}

SHELL_EXPORT_CMD(DoCLKOUT, clkout , clkout <sel>)
