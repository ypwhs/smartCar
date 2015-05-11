#ifndef __RT_HW_SPI_H__
#define __RT_HW_SPI_H__

#include <stdint.h>

struct kinetis_spi_cs
{
    uint32_t ch;
};

void rt_hw_spi_init(void);

#endif
