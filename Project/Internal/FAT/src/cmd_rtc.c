#include "shell.h"
#include "rtc.h"
#include "common.h"
#include "systick.h"

void RTC_ISR(void)
{
    printf("RTC INT\r\n");
}

int CMD_RTC(int argc, char * const argv[])
{
    printf("RTC TEST\r\n");
    
    RTC_DateTime_Type ts;

    ts.year = 2014;
    ts.month = 3;
    ts.day = 20;
    ts.hour = 0;
    ts.minute = 0;
    ts.second = 15;
    RTC_QuickInit();
    //RTC_ITDMAConfig(kRTC_IT_TimeOverflow);
    //RTC_CallbackInstall(RTC_ISR);
    while(1)
    {
        RTC_GetDateTime(&ts);
        printf("%d-%d-%d %d:%d:%d\r\n", ts.year, ts.month, ts.day, ts.hour, ts.minute, ts.second);
        DelayMs(1000);
    }
}




const cmd_tbl_t CommandFun_RTC = 
{
    .name = "RTC",
    .maxargs = 4,
    .repeatable = 1,
    .cmd = CMD_RTC,
    .usage = "RTC",
    .complete = NULL,
    .help = "RTC"
};
