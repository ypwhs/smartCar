#include "shell.h"
#include "gpio.h"
#include "board.h"
#include "rtc.h"

int DoRTC(int argc, char * const argv[])
{
    uint32_t i;
    RTC_DateTime_Type ts;
    
    printf("RTC TEST\r\n");
    
    ts.year = 2014;
    ts.month = 3;
    ts.day = 20;
    ts.hour = 0;
    ts.minute = 0;
    ts.second = 15;
    RTC_QuickInit();
    RTC_GetDateTime(&ts);

    for(i=0;i<5;i++)
    {
        printf("%d-%d-%d %d:%d:%d\r\n", ts.year, ts.month, ts.day, ts.hour, ts.minute, ts.second); 
        DelayMs(1000);
    }
    return 0;
}

SHELL_EXPORT_CMD(DoRTC, rtc , rtc test)



