#include "shell.h"
#include "board.h"
#include "adc.h"
#include "common.h"

static int DoADC(int argc, char * const argv[])
{
    int i;
    int result;
    
    /* decode MAP */
    uint32_t map = BOARD_ADC_MAP;
    QuickInit_Type type;
    QuickInitDecode(map, &type);
    printf("ADC%d CH%d:\r\n", type.ip_instance, type.channel);
    printf("PIN: P%c%d\r\n", type.io_instance+'A', type.io_offset);
    
    
    ADC_QuickInit(BOARD_ADC_MAP, kADC_SingleDiff12or13);

    for(i=0;i<20;i++)
    {
        result = ADC_QuickReadValue(BOARD_ADC_MAP);
        printf("ADC:%d\r\n", result);
        DelayMs(300);
    }
    return 0;
}

SHELL_EXPORT_CMD(DoADC, adc , adc test)

