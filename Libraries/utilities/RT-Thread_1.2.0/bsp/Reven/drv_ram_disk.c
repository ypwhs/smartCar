#include <rthw.h>
#include <rtthread.h>

#include <stdint.h>

#define RAM_DISK_SIZE          (120*1024)
#define RAM_SECTOR_SIZE         (512)
static void* DRAMStartAddr;


static struct rt_device dram_device;

static rt_err_t rt_dram_init (rt_device_t dev)
{
    rt_kprintf("ram disk size:%d\r\n", RAM_DISK_SIZE);
    DRAMStartAddr = rt_malloc(RAM_DISK_SIZE);
    if(!DRAMStartAddr)
    {
        rt_kprintf("no ram space!\r\n");
        return RT_ENOMEM;
    }
    return RT_EOK;
}

static rt_err_t rt_dram_open(rt_device_t dev, rt_uint16_t oflag)
{
	return RT_EOK;
}

static rt_err_t rt_dram_close(rt_device_t dev)
{
	return RT_EOK;
}

static rt_err_t rt_dram_indicate(rt_device_t dev, rt_size_t size)
{
    return RT_EOK;
}

static rt_size_t rt_dram_read(rt_device_t dev, rt_off_t pos, void* buffer, rt_size_t size)
{
    uint8_t *p;
    p = (uint8_t*)((uint32_t)DRAMStartAddr + pos * RAM_SECTOR_SIZE);
    rt_memcpy(buffer, p, size * RAM_SECTOR_SIZE);

	return size;
}

static rt_size_t rt_dram_write (rt_device_t dev, rt_off_t pos, const void* buffer, rt_size_t size)
{
    uint8_t *p;
    p = (uint8_t*)((uint32_t)DRAMStartAddr + pos * RAM_SECTOR_SIZE);
    rt_memcpy(p, buffer, size * RAM_SECTOR_SIZE);

	return size;
}

static rt_err_t rt_dram_control(rt_device_t dev, rt_uint8_t cmd, void *args)
{
    struct rt_device_blk_geometry geometry;
    rt_memset(&geometry, 0, sizeof(geometry));
	switch (cmd)
	{
        case RT_DEVICE_CTRL_BLK_GETGEOME:
            geometry.block_size = RAM_SECTOR_SIZE;
            geometry.bytes_per_sector = RAM_SECTOR_SIZE;
            geometry.sector_count = RAM_DISK_SIZE/RAM_SECTOR_SIZE;
            rt_memcpy(args, &geometry, sizeof(struct rt_device_blk_geometry));
		break;
	default: 
		break;
	}
	return RT_EOK;

}

rt_err_t rt_dram_txcomplete(rt_device_t dev, void *buffer)
{
	return RT_EOK;
}

void rt_hw_dram_init(void)
{

	dram_device.type 		= RT_Device_Class_Block;
	dram_device.rx_indicate = rt_dram_indicate;
	dram_device.tx_complete = RT_NULL;
	dram_device.init 		= rt_dram_init;
	dram_device.open		= rt_dram_open;
	dram_device.close		= rt_dram_close;
	dram_device.read 		= rt_dram_read;
	dram_device.write     = rt_dram_write;
	dram_device.control 	= rt_dram_control;
	dram_device.user_data	= RT_NULL;

    rt_device_register(&dram_device, "dram0", RT_DEVICE_FLAG_RDWR  | RT_DEVICE_FLAG_REMOVABLE|RT_DEVICE_FLAG_STANDALONE);
}

    
INIT_DEVICE_EXPORT(rt_hw_dram_init);
