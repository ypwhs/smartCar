#include <rthw.h>
#include <rtthread.h>
#include "rtc.h"
#include <time.h>
#include <rtdevice.h>
#include <drivers/rtc.h>


static struct rt_device rtc;

static rt_err_t rt_rtc_open(rt_device_t dev, rt_uint16_t oflag)
{
    if (dev->rx_indicate != RT_NULL)
    {
        /* Open Interrupt */
    }
    return RT_EOK;
}


static rt_err_t rt_rtc_control(rt_device_t dev, rt_uint8_t cmd, void *args)
{
    uint32_t val;
    RT_ASSERT(dev != RT_NULL);
    switch (cmd)
    {
        case RT_DEVICE_CTRL_RTC_GET_TIME:
            val = RTC_GetTSR();
            *(time_t*)args = *(time_t*)&val;
            break;

        case RT_DEVICE_CTRL_RTC_SET_TIME:
            RTC_SetTSR(*(uint32_t*)args);
            break;
    }
    return RT_EOK;
}

int rt_hw_rtc_init(void)
{
    rtc.type	= RT_Device_Class_RTC;
    RTC_QuickInit();

    /* register rtc device */
    rtc.init 	= RT_NULL;
    rtc.open 	= rt_rtc_open;
    rtc.close	= RT_NULL;
    rtc.read 	= RT_NULL;
    rtc.write	= RT_NULL;
    rtc.control = rt_rtc_control;

    /* no private */
    rtc.user_data = RT_NULL;
    rt_device_register(&rtc, "rtc", RT_DEVICE_FLAG_RDWR);
    
    /* provent ignore linking from lib */
    extern void list_date(void);
    list_date();
    return 0;
}

    
INIT_DEVICE_EXPORT(rt_hw_rtc_init);

