#include <rtthread.h>
#include "gpio.h"
#include <rtdevice.h>
#include <drivers/i2c.h>
#include "board.h"

static void (set_sda)(void *data, rt_int32_t state)
{
    GPIO_PinConfig(BOARD_I2C_SDA_PORT, BOARD_I2C_SDA_PIN, kOutput);
    GPIO_WriteBit(BOARD_I2C_SDA_PORT, BOARD_I2C_SDA_PIN, state);
}

static void (set_scl)(void *data, rt_int32_t state)
{
    GPIO_PinConfig(BOARD_I2C_SCL_PORT, BOARD_I2C_SCL_PIN, kOutput);
    GPIO_WriteBit(BOARD_I2C_SCL_PORT, BOARD_I2C_SCL_PIN, state);
}

static rt_int32_t (get_sda)(void *data)
{
    GPIO_PinConfig(BOARD_I2C_SDA_PORT, BOARD_I2C_SDA_PIN, kInput);
    return GPIO_ReadBit(BOARD_I2C_SDA_PORT, BOARD_I2C_SDA_PIN);
}

static rt_int32_t (get_scl)(void *data)
{
    GPIO_PinConfig(BOARD_I2C_SCL_PORT, BOARD_I2C_SCL_PIN, kInput);
    return GPIO_ReadBit(BOARD_I2C_SCL_PORT, BOARD_I2C_SCL_PIN);
}

static void (udelay)(rt_uint32_t us)
{
    volatile int i,j;
    for(i=0;i<us;i++)
    {
        __NOP(); __NOP();
    }
}
    
static struct rt_i2c_bus_device i2c_bus;
static struct rt_i2c_bit_ops bit_ops = 
{
    RT_NULL,
    set_sda,
    set_scl,
    get_sda,
    get_scl,
    udelay,
    1,
    1,
};

int rt_hw_i2c_bit_ops_bus_init(void)
{
    rt_memset((void *)&i2c_bus, 0, sizeof(struct rt_i2c_bus_device));
    i2c_bus.priv = (void *)&bit_ops;
    
    /* init i2c gpio */
    GPIO_WriteBit(BOARD_I2C_SDA_PORT, BOARD_I2C_SDA_PIN, 1);
    GPIO_WriteBit(BOARD_I2C_SCL_PORT, BOARD_I2C_SCL_PIN, 1);
    GPIO_QuickInit(BOARD_I2C_SDA_PORT, BOARD_I2C_SDA_PIN, kGPIO_Mode_OPP);
    GPIO_QuickInit(BOARD_I2C_SCL_PORT, BOARD_I2C_SCL_PIN, kGPIO_Mode_OPP);

    PORT_PinPullConfig(BOARD_I2C_SDA_PORT, BOARD_I2C_SDA_PIN, kPullUp);
    PORT_PinPullConfig(BOARD_I2C_SCL_PORT, BOARD_I2C_SCL_PIN, kPullUp);

    return rt_i2c_bit_add_bus(&i2c_bus, "i2c0");
}


INIT_APP_EXPORT(rt_hw_i2c_bit_ops_bus_init);
