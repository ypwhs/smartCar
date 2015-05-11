#include "gpio.h"
#include "pin.h"
#include "finsh.h"


static void _pin_mode(struct rt_device *device, rt_base_t pin, rt_base_t mode)
{
    
}

static void _pin_write(struct rt_device *device, rt_base_t pin, rt_base_t value)
{
    
}

static int _pin_read(struct rt_device *device, rt_base_t pin)
{
    
}
    
static const struct rt_pin_ops _ops =
{
    _pin_mode,
    _pin_write,
    _pin_read,
};

int rt_hw_pin_init(void)
{    
    return rt_device_pin_register("pa", &_ops, RT_NULL);
}

INIT_BOARD_EXPORT(rt_hw_pin_init);






