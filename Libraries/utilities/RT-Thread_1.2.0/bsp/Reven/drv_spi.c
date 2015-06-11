#include <rthw.h>
#include <rtthread.h>
#include "chlib_k.h"

#include <rtdevice.h>
#include <drivers/spi.h>

#include "rtt_drv.h"
static struct rt_spi_bus kinetis_spi;

#define  RTT_SPI_DRIVER_DEBUG  0

#if (RTT_SPI_DRIVER_DEBUG == 1)
#define RTT_SPI_DRIVER_TRACE	rt_kprintf
#else
#define RTT_SPI_DRIVER_TRACE(...)
#endif


static rt_err_t configure(struct rt_spi_device* device, struct rt_spi_configuration* configuration)
{
    SPI_InitTypeDef SPI_InitStruct1;
    /* data_width */
    if(configuration->data_width <= 8)
    {
        SPI_InitStruct1.dataSize = 8;
    }
    else if(configuration->data_width <= 16)
    {
        SPI_InitStruct1.dataSize = 16;
    }
    else
    {
        return RT_EIO;
    }
    /* baud */
    if(configuration->max_hz < 100)
    {
        SPI_InitStruct1.baudrate = 1000;
    }
    else if (configuration->max_hz > 47*1000*1000)
    {
        SPI_InitStruct1.baudrate = 47*1000*1000;
    }
    else
    {
        SPI_InitStruct1.baudrate = configuration->max_hz;
    }
    RTT_SPI_DRIVER_TRACE("spi bus baudrate:%d\r\n", SPI_InitStruct1.baudrate);
    /* frame foramt */
    SPI_InitStruct1.frameFormat = (SPI_FrameFormat_Type)(configuration->mode & 0x03);
    /* MSB or LSB */
    if(configuration->mode & RT_SPI_MSB)
    {
        SPI_InitStruct1.bitOrder = kSPI_MSB;
    }
    else
    {
        SPI_InitStruct1.bitOrder = kSPI_LSB;
    }
    SPI_InitStruct1.mode = kSPI_Master;
    SPI_InitStruct1.ctar = HW_CTAR0;
    SPI_InitStruct1.instance = HW_SPI2;
    SPI_Init(&SPI_InitStruct1);
    

    return RT_EOK;

}

static rt_uint32_t xfer(struct rt_spi_device* device, struct rt_spi_message* message)
{
    struct rt_spi_configuration * config = &device->config;
    rt_uint32_t size = message->length;
    const rt_uint8_t * send_ptr = message->send_buf;
    rt_uint8_t * recv_ptr = message->recv_buf;
    struct kinetis_spi_cs * kinetis_spi_cs = device->parent.user_data;
    //rt_kprintf("spi size:%d\r\n", size);
    while(size--)
    {
        rt_uint16_t data = 0xFF;
        if(send_ptr != RT_NULL)
        {
            data = *send_ptr++;
        }
        /* 最后一个 并且是需要释放CS */
        if((size == 0) && (message->cs_release))
        {  
            data = SPI_ReadWriteByte(HW_SPI2, HW_CTAR0, data, kinetis_spi_cs->ch, kSPI_PCS_ReturnInactive);
        }
        else
        {
            data = SPI_ReadWriteByte(HW_SPI2, HW_CTAR0, data, kinetis_spi_cs->ch, kSPI_PCS_KeepAsserted);
        }
        if(recv_ptr != RT_NULL)
        {
            *recv_ptr++ = data;
        }
    }
    return message->length;
}



static struct rt_spi_ops kinetis_spi_ops =
{
    configure,
    xfer
};

int rt_hw_spi_bus_init(void)
{
    kinetis_spi.ops = &kinetis_spi_ops;
    PORT_PinMuxConfig(HW_GPIOD, 14, kPinAlt2); 
    PORT_PinMuxConfig(HW_GPIOD, 13, kPinAlt2); 
    PORT_PinMuxConfig(HW_GPIOD, 12, kPinAlt2); 
    return rt_spi_bus_register(&kinetis_spi, "spi2", &kinetis_spi_ops); 
}

void rt_hw_spi_init(void)
{
    rt_hw_spi_bus_init();
    struct rt_spi_device* spi_21;
    struct kinetis_spi_cs* cs_21;
    spi_21 = rt_malloc(sizeof(struct rt_spi_device));
    cs_21 = rt_malloc(sizeof(struct kinetis_spi_cs));
    
    cs_21->ch = 1;
    PORT_PinMuxConfig(HW_GPIOD, 15, kPinAlt2); //SPI2_PCS1
    rt_spi_bus_attach_device(spi_21, "spi21", "spi2", cs_21);
    
    struct rt_spi_device* spi_20;
    struct kinetis_spi_cs* cs_20;
    spi_20 = rt_malloc(sizeof(struct rt_spi_device));
    cs_20 = rt_malloc(sizeof(struct kinetis_spi_cs));
    cs_20->ch = 0;
    PORT_PinMuxConfig(HW_GPIOD, 11, kPinAlt2); //SPI2_PCS0
    rt_spi_bus_attach_device(spi_20, "spi20", "spi2", cs_20);
}

INIT_BOARD_EXPORT(rt_hw_spi_init);

