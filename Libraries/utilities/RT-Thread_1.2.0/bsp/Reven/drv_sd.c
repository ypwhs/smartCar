#include <rthw.h>
#include <rtthread.h>
#include "sd.h"


static struct rt_device sd_device;
static  rt_mutex_t mutex;


static rt_err_t rt_sd_init (rt_device_t dev)
{
    mutex = rt_mutex_create("_mu", RT_IPC_FLAG_FIFO);
    return RT_EOK;
}

static rt_err_t rt_sd_open(rt_device_t dev, rt_uint16_t oflag)
{
    int r;
    rt_mutex_take(mutex, RT_WAITING_FOREVER);
    if(SD_QuickInit(10*1000*1000))
    {
        return RT_EIO;
    }
    rt_mutex_release(mutex);
    if(r)
    {
        dev->open_flag = RT_DEVICE_OFLAG_CLOSE;
        return RT_ERROR;
    }
    dev->open_flag = RT_DEVICE_OFLAG_OPEN;
	return RT_EOK;
}

static rt_err_t rt_sd_close(rt_device_t dev)
{
	return RT_EOK;
}

rt_err_t rt_sd_indicate(rt_device_t dev, rt_size_t size)
{
    rt_kprintf("I need indicate\r\n"); 
    return RT_EOK;
}

static rt_size_t rt_sd_read(rt_device_t dev, rt_off_t pos, void* buffer, rt_size_t size)
{
    int r;
    if(rt_mutex_take(mutex, RT_WAITING_NO))
    {
        return 0;
    }
    r = SD_ReadMultiBlock(pos, (rt_uint8_t *)buffer, size);
    if(r)
    {
        rt_kprintf("sd_read error!%d\r\n", r);
        dev->open_flag = RT_DEVICE_OFLAG_CLOSE;
        return 0;
    }
    rt_mutex_release(mutex);
	return size;
}

static rt_size_t rt_sd_write (rt_device_t dev, rt_off_t pos, const void* buffer, rt_size_t size)
{
    int r;
    if(rt_mutex_take(mutex, RT_WAITING_NO))
    {
        return 0;
    }
    r = SD_WriteMultiBlock(pos, (rt_uint8_t *)buffer, size);
    if(r)
    {
        rt_kprintf("sd_write error!%d\r\n", r);
        dev->open_flag = RT_DEVICE_OFLAG_CLOSE;
        return 0;
    }
    rt_mutex_release(mutex);
    return size;
}

static rt_err_t rt_sd_control(rt_device_t dev, rt_uint8_t cmd, void *args)
{
    uint32_t size;
    struct rt_device_blk_geometry geometry;
    rt_memset(&geometry, 0, sizeof(geometry));
	switch (cmd)
	{
	case RT_DEVICE_CTRL_BLK_GETGEOME:
		geometry.block_size = 512;
		geometry.bytes_per_sector = 512;
        size = SD_GetSizeInMB();
        
		geometry.sector_count = size*1024*2;
		rt_memcpy(args, &geometry, sizeof(struct rt_device_blk_geometry));
		break;
	default: 
		break;
	}
	return RT_EOK;

}

rt_err_t rt_sd_txcomplete(rt_device_t dev, void *buffer)
{
	return RT_EOK;
}

void rt_hw_sd_init(void)
{

	sd_device.type 		= RT_Device_Class_Block;
	sd_device.rx_indicate = rt_sd_indicate;
	sd_device.tx_complete = RT_NULL;
	sd_device.init 		= rt_sd_init;
	sd_device.open		= rt_sd_open;
	sd_device.close		= rt_sd_close;
	sd_device.read 		= rt_sd_read;
	sd_device.write     = rt_sd_write;
	sd_device.control 	= rt_sd_control;
	sd_device.user_data	= RT_NULL;

    rt_device_register(&sd_device, "sd0", RT_DEVICE_FLAG_RDWR  | RT_DEVICE_FLAG_REMOVABLE|RT_DEVICE_FLAG_STANDALONE);
}

    
INIT_DEVICE_EXPORT(rt_hw_sd_init);
