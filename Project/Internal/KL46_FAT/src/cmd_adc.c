#include "shell.h"
#include "adc.h"
#include "common.h"


QuickInit_Type ADC_QuickInitTable[] =
{
    { 0, 4, 0,16, 1, 1, 0}, //ADC0_SE1A_PE16
    { 0, 4, 0,17, 1, 5, 0}, //ADC0_SE5A_PE17
    { 0, 4, 0,18, 1, 2, 0}, //ADC0_SE2A_PE18
    { 0, 4, 0,19, 1, 6, 0}, //ADC0_SE6A_PE19
    { 0, 4, 0,20, 1, 0, 0}, //ADC0_SE0A_PE20
    { 0, 4, 0,21, 1, 4, 0}, //ADC0_SE4A_PE21
    { 0, 4, 0,22, 1, 3, 0}, //ADC0_SE3A_PE22
    { 0, 4, 0,23, 1, 7, 0}, //ADC0_SE7A_PE23
    { 0, 4, 0,29, 1, 4, 1}, //ADC0_SE4B_PE29
    { 0, 4, 0,30, 1,23, 0}, //ADC0_SE23A_PE30
    { 0, 1, 0, 0, 1, 8, 0}, //ADC0_SE8A_PB00
    { 0, 1, 0, 1, 1, 9, 0}, //ADC0_SE9A_PB01
    { 0, 1, 0, 2, 1,12, 0}, //ADC0_SE12A_PB02
    { 0, 1, 0, 3, 1,13, 0}, //ADC0_SE13A_PB03
    { 0, 2, 0, 0, 1,14, 0}, //ADC0_SE14A_PC00
    { 0, 2, 0, 1, 1,15, 0}, //ADC0_SE15A_PC01
    { 0, 2, 0, 2, 1,11, 0}, //ADC0_SE11A_PC02
    { 0, 3, 0, 1, 1, 5, 1}, //ADC0_SE5B_PD01
    { 0, 3, 0, 5, 1, 6, 1}, //ADC0_SE6B_PD05
    { 0, 3, 0, 6, 1, 7, 1}, //ADC0_SE7B_PD06
};

static int DoADC(int argc, char * const argv[])
{
    int i;
    int result;
    printf("%s\r\n", __func__);
    
    for(i=0;i<ARRAY_SIZE(ADC_QuickInitTable);i++)
    {
 //       printf("(0x%08XU)\r\n", QuickInitEncode(&ADC_QuickInitTable[i]));
    }
    
    ADC_QuickInit(ADC0_SE1A_PE16, kADC_SingleDiff12or13);
    
    for(i=0;i<20;i++)
    {
        result = ADC_QuickReadValue(ADC0_SE1A_PE16);
        printf("ADC:%d\r\n", result);
        DelayMs(300);
    }
    return 0;
}

SHELL_EXPORT_CMD(DoADC, adc , adc test)
    

