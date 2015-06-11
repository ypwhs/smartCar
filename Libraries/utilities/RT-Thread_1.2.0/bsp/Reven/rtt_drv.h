#ifndef __RTT_DRV_H__
#define __RTT_DRV_H__

#include <rtdef.h>
#include <stdint.h>


#define NIOCTL_GET_PHY_DATA     (0x05)

#define RT_DEVICE_CTRL_BEEP_START           0x00
#define RT_DEVICE_CTRL_BEEP_STOP            0x01
#define RT_DEVICE_CTRL_SET_BEEP_FRQ         0x02

#define RT_DEVICE_CTRL_LED_ON           0x00
#define RT_DEVICE_CTRL_LED_OFF          0x01
#define RT_DEVICE_CTRL_LED_TOGGLE       0x02
#define RT_DEVICE_GET_LED_NUM           0x03

struct kinetis_spi_cs
{
    uint32_t ch;
};


typedef struct
{
    rt_uint32_t tx_fcnt;
    rt_uint32_t tx_dcnt;
    rt_uint32_t rx_fcnt;
    rt_uint32_t rx_dcnt;
}enet_phy_data;


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


int rt_hw_i2c_bit_ops_bus_init(void);
void rt_hw_uart_init(void);
void rt_hw_spi_init(void);
int rt_hw_spi_bus_init(void);
int rt_hw_rtc_init(void);
rt_err_t at24cxx_init(const char * device_name, const char * i2c_bus_name);
int rt_hw_ksz8041_init(void);


#endif
