#include <rthw.h>
#include <rtthread.h>
#include <stdint.h>
#include "rtt_drv.h"
#include "ftm.h"

static struct rt_device beep_device;
static uint32_t ftm_instance;
static rt_err_t rt_beep_control(rt_device_t dev, rt_uint8_t cmd, void *args)
{
    RT_ASSERT(dev != RT_NULL);
    static int freq = 10000;
    switch (cmd)
    {
        case RT_DEVICE_CTRL_BEEP_START:
            FTM_PWM_ChangeDuty(ftm_instance, HW_FTM_CH3, 3000);
            break;
        case RT_DEVICE_CTRL_BEEP_STOP:
            FTM_PWM_ChangeDuty(ftm_instance, HW_FTM_CH3, 0);
            break;
        case RT_DEVICE_CTRL_SET_BEEP_FRQ:
            freq = *(int*)args;
            FTM_PWM_QuickInit(FTM0_CH3_PA06, kPWM_EdgeAligned, freq);
            break;
    }
    return RT_EOK;
}

static rt_err_t rt_beep_open(rt_device_t dev, rt_uint16_t oflag)
{
    if (dev->rx_indicate != RT_NULL)
    {
        /* Open Interrupt */
    }
    ftm_instance = FTM_PWM_QuickInit(FTM0_CH3_PA06, kPWM_EdgeAligned, 10000);
    FTM_PWM_ChangeDuty(ftm_instance, HW_FTM_CH3, 0);
    return RT_EOK;
}


int rt_hw_beep_init(void)
{

	beep_device.type 		= RT_Device_Class_Miscellaneous;
	beep_device.rx_indicate = RT_NULL;
	beep_device.tx_complete = RT_NULL;
	beep_device.init 		= RT_NULL;
	beep_device.open		= rt_beep_open;
	beep_device.close		= RT_NULL;
	beep_device.read 		= RT_NULL;
	beep_device.write       = RT_NULL;
	beep_device.control 	= rt_beep_control;
	beep_device.user_data	= RT_NULL;
	
    return rt_device_register(&beep_device, "beep", RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_STANDALONE | RT_DEVICE_FLAG_REMOVABLE);
}

INIT_BOARD_EXPORT(rt_hw_beep_init);

#ifdef FINSH_USING_MSH
#include <finsh.h>
#include <rtdevice.h>


static struct rt_device *dev_beep;
static struct rt_timer timer;

static void timer_timeout(void* parameter)
{
    rt_device_control(dev_beep, RT_DEVICE_CTRL_BEEP_STOP, RT_NULL);
    rt_timer_detach(&timer);
}

void beep(int argc, char** argv)
{
    int time, freq;
    time = strtoul(argv[1],0,0);
    freq = strtoul(argv[2],0,0);
    if(!time) time = 1;
	dev_beep = rt_device_find("beep");
	if (dev_beep == RT_NULL)
	{
		rt_kprintf("no device found!\n");
		return;
	}
    rt_device_open(dev_beep, 0);
    rt_timer_init(&timer, "bt", timer_timeout, RT_NULL, time/(1000/RT_TICK_PER_SECOND), RT_TIMER_FLAG_ONE_SHOT);
    rt_timer_start(&timer);
    rt_device_control(dev_beep, RT_DEVICE_CTRL_SET_BEEP_FRQ, &freq);
    rt_device_control(dev_beep, RT_DEVICE_CTRL_BEEP_START, RT_NULL);
}

MSH_CMD_EXPORT(beep, beep. eg:beep <time> <freq>)

#endif
