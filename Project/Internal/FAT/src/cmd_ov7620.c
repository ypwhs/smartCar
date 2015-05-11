#include "shell.h"
#include "gpio.h"
#include "common.h"
#include "board.h"
#include "ov7620.h"
#include "ili9320.h"

#include "ftm.h"
#include "shell.h"
#include "gpio.h"
#include "board.h"
#include <stdlib.h>
#include <stdio.h>
//RGB »ìÉ«

uint16_t RGB2COLOR(uint8_t RR,uint8_t GG,uint8_t BB)
{
  return (((RR/8)<<11)+((GG/8)<<6)+BB/8); 
}


static void OV7620_ISR(void)
{
    uint32_t i,j;
    static uint32_t cnt;
    printf("cnt:%d\r\n", cnt++);
    for(i=0;i<OV7620_H;i++)
    {
        for(j=0;j<OV7620_W;j++)
        {
            ili9320_write_pixel(OV7620_H - i, OV7620_W- j, RGB2COLOR(CCDBuffer[i][j], CCDBuffer[i][j], CCDBuffer[i][j]));
        }
    }
}

int CMD_BOV7620(int argc, char * const * argv)
{
    printf("OV7620 TEST\r\n");
    printf("INIT FLEXBUS FIRST\r\n");
    SIM->CLKDIV1 &= ~SIM_CLKDIV1_OUTDIV3_MASK;
    SIM->CLKDIV1 |= SIM_CLKDIV1_OUTDIV3(2);
    ili9320_init();
    OV7620_CallbackInstall(OV7620_ISR);
    OV7620_Init();
    return 0;
}

const cmd_tbl_t CommandFun_OV7620 = 
{
    .name = "OV7620",
    .maxargs = 4,
    .repeatable = 1,
    .cmd = CMD_BOV7620,
    .usage = "OV7620",
    .complete = NULL,
    .help = "OV7620"
};
