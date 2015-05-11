#include <rtthread.h>
#include <rthw.h>

#include "gpio.h"
#include "gui.h"
#include "board.h"
#include "drv_led.h"


void led_thread_entry(void* parameter)
{
    int i, led_num;
    rt_device_t dev;
    dev = rt_device_find("led");
    if(!dev)
    {
        return;
    }
    rt_device_control(dev, RT_DEVICE_GET_LED_NUM, &led_num);
    rt_kprintf("led num:%d\r\n", led_num);
    
    while(1)
    {
        for(i=0;i<led_num;i++)
        {
            rt_device_control(dev, RT_DEVICE_CTRL_LED_TOGGLE, &i);
            rt_thread_delay(20);
        }
    }
}


