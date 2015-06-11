#include <rthw.h>
#include <rtthread.h>
#include <stdint.h>
#include "rtt_drv.h"
#include "board.h"
#include "chlib_k.h"

static struct rt_device led_device;
static  uint32_t led_port_tab[] = BOARD_LED_GPIO_BASES;
static  uint32_t led_pin_tab[] = BOARD_LED_PIN_BASES;
    
static rt_err_t rt_led_control(rt_device_t dev, rt_uint8_t cmd, void *args)
{
    RT_ASSERT(dev != RT_NULL);
    rt_uint32_t led = *(rt_uint32_t*)args;
    switch (cmd)
    {
        case RT_DEVICE_CTRL_LED_ON:
            GPIO_WriteBit(led_port_tab[led], led_pin_tab[led], 1);
            break;
        case RT_DEVICE_CTRL_LED_OFF:
            GPIO_WriteBit(led_port_tab[led], led_pin_tab[led], 0);
            break;
        case RT_DEVICE_CTRL_LED_TOGGLE:
            GPIO_ToggleBit(led_port_tab[led], led_pin_tab[led]); 
            break;
        case RT_DEVICE_GET_LED_NUM:
            *(rt_uint32_t*)args =  ARRAY_SIZE(led_port_tab);
            break;
    }
    return RT_EOK;
}


int rt_hw_led_init(const char *name)
{

	led_device.type             = RT_Device_Class_Miscellaneous;
	led_device.rx_indicate      = RT_NULL;
	led_device.tx_complete      = RT_NULL;
	led_device.init             = RT_NULL;
	led_device.open             = RT_NULL;
	led_device.close            = RT_NULL;
	led_device.read             = RT_NULL;
	led_device.write            = RT_NULL;
	led_device.control          = rt_led_control;
	led_device.user_data        = RT_NULL;
	
    int i,led_num;

    led_num = ARRAY_SIZE(led_port_tab);
    for(i=0; i<led_num; i++)
    {
        GPIO_QuickInit(led_port_tab[i], led_pin_tab[i], kGPIO_Mode_OPP);
    }

    return rt_device_register(&led_device, name, RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_STANDALONE | RT_DEVICE_FLAG_REMOVABLE);
}

void rt_hw_led_init2(void)
{
    rt_hw_led_init("led");
}
    
INIT_DEVICE_EXPORT(rt_hw_led_init2);

#ifdef FINSH_USING_MSH
#include <finsh.h>
#include <rtdevice.h>


static int led(int argc, char** argv)
{
    int led, val;
    rt_device_t dev;
    led = strtoul(argv[1],0,0);
    val = strtoul(argv[2],0,0);
    if(led > ARRAY_SIZE(led_port_tab))
    {
        return -2;
    }
	dev = rt_device_find("led");
	if (dev == RT_NULL)
	{
		rt_kprintf("no device found!\n");
		return -1;
	}
    rt_led_control(dev, val, &led);
    return 0;
}

MSH_CMD_EXPORT(led, led. eg:led 1 1)

#endif

