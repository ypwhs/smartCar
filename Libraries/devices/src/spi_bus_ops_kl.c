/**
  ******************************************************************************
  * @file    spi_bus_ops.c
  * @author  YANDLD
  * @version V2.5
  * @date    2013.12.25
  * @brief   www.beyondcore.net   http://upcmcu.taobao.com 
  * @note    kinetis spi interface for spi_abstraction layer
  ******************************************************************************
  */
#include "spi_abstraction.h"
#include "spi.h"
#include "gpio.h"
#include "board.h"


static int kinetis_spi_configure(spi_device_t device, struct spi_config *configuration)
{
   return 0; 
}

static int kinetis_spi_write(spi_device_t device, uint8_t *buf, uint32_t len, bool cs_return_inactive)
{
    GPIO_WriteBit(BOARD_SPI_CS_PORT, BOARD_SPI_CS_PIN, 0);
    SPI_ReadWriteByte(device->bus->instance, *buf++);
    if(cs_return_inactive)
    {
        GPIO_WriteBit(BOARD_SPI_CS_PORT, BOARD_SPI_CS_PIN, 1);
    }
    else
    {
        GPIO_WriteBit(BOARD_SPI_CS_PORT, BOARD_SPI_CS_PIN, 0);
    }
    return SPI_EOK;
}

static int kinetis_spi_read(spi_device_t device, uint8_t *buf, uint32_t len, bool cs_return_inactive)
{
    uint16_t dummy = 0xFFFF;
    GPIO_WriteBit(BOARD_SPI_CS_PORT, BOARD_SPI_CS_PIN, 0);
    *buf++ = SPI_ReadWriteByte(device->bus->instance, dummy);
    if(cs_return_inactive)
    {
        GPIO_WriteBit(BOARD_SPI_CS_PORT, BOARD_SPI_CS_PIN, 1);
    }
    else
    {
        GPIO_WriteBit(BOARD_SPI_CS_PORT, BOARD_SPI_CS_PIN, 0);
    }
    return SPI_EOK;
}


/* spi bus layer register struct */
const static struct spi_ops kinetis_spi_ops = 
{
    kinetis_spi_configure,
    kinetis_spi_read,
    kinetis_spi_write,
};

int kinetis_spi_bus_init(struct spi_bus* bus, uint32_t instance)
{
    /* init hardware with defalt settings */
    SPI_InitTypeDef SPI_InitStruct1;
    SPI_InitStruct1.baudrate = 10000*1000;
    SPI_InitStruct1.frameFormat = kSPI_CPOL0_CPHA0;
    SPI_InitStruct1.dataSize = 8;
    SPI_InitStruct1.instance = instance;
    SPI_InitStruct1.mode = kSPI_Master;
    SPI_InitStruct1.bitOrder = kSPI_MSBFirst;
    SPI_Init(&SPI_InitStruct1);
    GPIO_QuickInit(BOARD_SPI_CS_PORT, BOARD_SPI_CS_PIN, kGPIO_Mode_OPP);
    /* register bus */
    bus->instance = instance;
    return spi_bus_register(bus, &kinetis_spi_ops);
}
