#ifndef __RT_KSZ8041_H__
#define __RT_KSZ8041_H__

#include <stdint.h>
#include <rtthread.h>
//!< API functions

typedef struct
{
    rt_uint32_t tx_fcnt;
    rt_uint32_t tx_dcnt;
    rt_uint32_t rx_fcnt;
    rt_uint32_t rx_dcnt;
}enet_phy_data;

#define NIOCTL_GET_PHY_DATA     (0x05)

#endif



