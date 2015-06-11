#include <rtthread.h>
#include "chlib_k.h"
#include "rtt_drv.h"
#include <rtdevice.h>
#include <netif/ethernetif.h>
#include "ksz8041.h"

static struct eth_device device;
static uint8_t gCfgLoca_MAC[] = {0x00, 0xCF, 0x52, 0x35, 0x00, 0x01};
static rt_uint8_t *gTxBuf;
static rt_uint8_t *gRxBuf;



static enet_phy_data phy_data;

void ENET_ISR(void)
{
    rt_interrupt_enter();
    eth_device_ready(&device);
    rt_interrupt_leave();
}


static rt_err_t rt_ksz8041_init(rt_device_t dev)
{
    /* init driver */
    ENET_InitTypeDef ENET_InitStruct1;
    ENET_InitStruct1.pMacAddress = gCfgLoca_MAC;
    ENET_InitStruct1.is10MSpped = false;
    ENET_InitStruct1.isHalfDuplex = false;
    ENET_Init(&ENET_InitStruct1);
    
    PORT_PinMuxConfig(HW_GPIOB, 0, kPinAlt4);
    PORT_PinPullConfig(HW_GPIOB, 0, kPullUp);
    PORT_PinOpenDrainConfig(HW_GPIOB, 0, ENABLE);
    
    PORT_PinMuxConfig(HW_GPIOB, 1, kPinAlt4);
    PORT_PinMuxConfig(HW_GPIOA, 5, kPinAlt4);
    PORT_PinMuxConfig(HW_GPIOA, 12, kPinAlt4);
    PORT_PinMuxConfig(HW_GPIOA, 13, kPinAlt4);
    PORT_PinMuxConfig(HW_GPIOA, 14, kPinAlt4);
    PORT_PinMuxConfig(HW_GPIOA, 15, kPinAlt4);
    PORT_PinMuxConfig(HW_GPIOA, 16, kPinAlt4);
    PORT_PinMuxConfig(HW_GPIOA, 17, kPinAlt4);
    
    ksz8041_init();

//    if(!ksz8041_is_linked())
//    {
//        eth_device_linkchange(&device, false);
//        return RT_EIO;
//    }
    
    ENET_CallbackRxInstall(ENET_ISR);
    ENET_ITDMAConfig(kENET_IT_RXF);
    
    return RT_EOK;
}


static rt_err_t rt_ksz8041_open(rt_device_t dev, rt_uint16_t oflag)
{
    return RT_EOK;
}

static rt_err_t rt_ksz8041_close(rt_device_t dev)
{
    return RT_EOK;
}

static rt_size_t rt_ksz8041_read(rt_device_t dev, rt_off_t pos, void* buffer, rt_size_t size)
{
    return RT_EOK;
}

static rt_size_t rt_ksz8041_write (rt_device_t dev, rt_off_t pos, const void* buffer, rt_size_t size)
{
    return RT_EOK;
}

static rt_err_t rt_ksz8041_control(rt_device_t dev, rt_uint8_t cmd, void *args)
{
    switch (cmd)
    {
    case NIOCTL_GADDR:
        /* get mac address */
        if (args) rt_memcpy(args, gCfgLoca_MAC, 6);
        else return -RT_ERROR;
        break;
    case NIOCTL_GET_PHY_DATA:
        if (args) rt_memcpy(args, &phy_data, sizeof(phy_data));
        break;
    default :
        break;
    }
    return RT_EOK;
}

struct pbuf *rt_ksz8041_rx(rt_device_t dev)
{
    struct pbuf* p;
    rt_uint32_t i;
    rt_uint32_t rx_len;
    
    /* init p pointer */
    p = RT_NULL;
    i = 0;
    rx_len = ENET_MacReceiveData(gRxBuf);
    if(rx_len)
    {
        phy_data.rx_fcnt++;
        phy_data.rx_dcnt+= rx_len;
        p = pbuf_alloc(PBUF_LINK, rx_len, PBUF_RAM);
        if (p != RT_NULL)
        {
            struct pbuf* q;  
            for (q = p; q != RT_NULL; q = q->next)
            {
                rt_memcpy((rt_uint8_t*)q->payload, (rt_uint8_t*)&gRxBuf[i], q->len);
            }
            rx_len = 0;
        }
        else
        {
            return NULL;
        }
    }
    return p;
}

rt_err_t rt_ksz8041_tx( rt_device_t dev, struct pbuf* p)
{

    rt_uint32_t tx_len;
    struct pbuf *q;
    
    tx_len = 0;
    for (q = p; q != RT_NULL; q = q->next)
    {
        rt_memcpy((rt_uint8_t*)&gTxBuf[tx_len], (rt_uint8_t*)q->payload, q->len);
        tx_len += q->len;
    }
    
    /* check if still linked */
    if(!ksz8041_is_linked())
    {
        eth_device_linkchange(&device, false);
        return RT_ERROR;
    }
    else
    {
        eth_device_linkchange(&device, true);
    }
    
    //rt_enter_critical();
    DisableInterrupts();
    ENET_MacSendData(gTxBuf, tx_len);
    EnableInterrupts();
    phy_data.tx_fcnt++;
    phy_data.tx_dcnt+= tx_len;

    //rt_exit_critical();

    return RT_EOK;
}


int rt_hw_ksz8041_init(void)
{
    device.parent.init       = rt_ksz8041_init;
    device.parent.open       = rt_ksz8041_open;
    device.parent.close      = rt_ksz8041_close;
    
    device.parent.read       = rt_ksz8041_read;
    device.parent.write      = rt_ksz8041_write;
    device.parent.control    = rt_ksz8041_control;
    device.parent.user_data    = RT_NULL;

    device.eth_rx     = rt_ksz8041_rx;
    device.eth_tx     = rt_ksz8041_tx;
    
    gTxBuf = rt_malloc(CFG_ENET_BUFFER_SIZE+16);
    gTxBuf = (rt_uint8_t*)(uint32_t)RT_ALIGN((uint32_t)gTxBuf, 16);
    if(!gTxBuf)
    {
        return RT_ENOMEM;
    }
    
    gRxBuf = rt_malloc(CFG_ENET_BUFFER_SIZE+16);
    gRxBuf = (rt_uint8_t*)(uint32_t)RT_ALIGN((uint32_t)gRxBuf, 16);
    if(!gRxBuf)
    {
        return RT_ENOMEM;
    }
    
    eth_device_init(&device, "e0");
    return 0;
}

