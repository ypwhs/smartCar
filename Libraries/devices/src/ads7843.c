/**
  ******************************************************************************
  * @file    ads7843.c
  * @author  YANDLD
  * @version V2.5
  * @date    2013.12.25
  * @brief   www.beyondcore.net   http://upcmcu.taobao.com 
  * @note    此文件为触控芯片2046的驱动文件
  ******************************************************************************
  */

#include "spi.h"
#include "ads7843.h"

#define ADS7843_CMD_READ_X   (0xD0)
#define ADS7843_CMD_READ_Y   (0X90)

struct ads7843_device 
{
    uint32_t    spi_instance;
    uint8_t     spi_cs;
    void        *user_data;
};

static struct ads7843_device ads_dev;

static uint8_t spi_xfer(uint8_t data, SPI_PCS_Type csStatus)
{
    return (uint8_t)SPI_ReadWriteByte(ads_dev.spi_instance , HW_CTAR0, (uint8_t)data, ads_dev.spi_cs, csStatus);
}

//读X坐标的数据
uint32_t ads7843_readX(int* value)
{
    uint8_t buf[2];
    spi_xfer(ADS7843_CMD_READ_X, kSPI_PCS_KeepAsserted);
    buf[0] = spi_xfer(0x00, kSPI_PCS_KeepAsserted);
    buf[1] = spi_xfer(0x00, kSPI_PCS_ReturnInactive);

    *value = ((buf[0]<<8) + buf[1])>>4; //12bit mode
    return 0;
}

//读Y坐标的数据
uint32_t ads7843_readY(int* value)
{
    uint8_t buf[2];
    spi_xfer(ADS7843_CMD_READ_Y, kSPI_PCS_KeepAsserted);
    buf[0] = spi_xfer(0x00, kSPI_PCS_KeepAsserted);
    buf[1] = spi_xfer(0x00, kSPI_PCS_ReturnInactive);

    *value = ((buf[0]<<8) + buf[1])>>4; //12bit mode
    return 0;
}

//触控芯片初始化
int ads7843_init(int instance, int cs)
{
    ads_dev.spi_instance = instance;
    ads_dev.spi_cs = cs; 
    SPI_CTARConfig(instance, HW_CTAR0, kSPI_CPOL0_CPHA0, 8, kSPI_MSB, 2*1000*1000);
    return 0;
}

