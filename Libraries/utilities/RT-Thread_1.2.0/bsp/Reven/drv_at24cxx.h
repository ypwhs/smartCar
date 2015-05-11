#ifndef I2C_AT24CXX_H_INCLUDED
#define I2C_AT24CXX_H_INCLUDED

#include <rtthread.h>
#include <drivers/i2c.h>
#include <stdint.h>



struct at24cxx_attr
{
    const char* name;
    uint32_t total_size;    /* total size */
    uint16_t page_size;     /* page size */
    uint8_t  chip_addr;     /* base addr */
};

struct at24cxx_device
{
    struct rt_device                parent;
    struct rt_device_blk_geometry   geometry;
    struct rt_i2c_bus_device *      bus;
    struct rt_mutex                 lock;
    struct at24cxx_attr             attr;
};

extern rt_err_t at24cxx_init(const char * device_name,
                            const char * i2c_bus_name);

#endif
