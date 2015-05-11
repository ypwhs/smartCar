#include <rthw.h>
#include <rtthread.h>
#include "flash.h"

#if defined(MK60D10)
#define FLASH_SIZE          (512*1024)
#elif defined(MK64F12)
#define FLASH_SIZE          (1024*1024)
#endif

extern int Image$$ER_IROM1$$RO$$Limit;

#define FLASH_OFFSET        ((uint32_t)&Image$$ER_IROM1$$RO$$Limit)   /* UDisk start offset */
#define UDISK_SIZE          (FLASH_SIZE - FLASH_OFFSET)


static struct rt_device dflash_device;
static int SectorCnt;

static rt_err_t rt_dflash_init (rt_device_t dev)
{
    FLASH_Init();
    
    SectorCnt = FLASH_GetSectorSize();
    rt_kprintf("dflash sector size:%d 0ffset:0x%X\r\n", SectorCnt, FLASH_OFFSET);
    
    return RT_EOK;
}

static rt_err_t rt_dflash_open(rt_device_t dev, rt_uint16_t oflag)
{
	return RT_EOK;
}

static rt_err_t rt_dflash_close(rt_device_t dev)
{
	return RT_EOK;
}

static rt_err_t rt_dflash_indicate(rt_device_t dev, rt_size_t size)
{
    //rt_kprintf("I need indicate\r\n"); 
    return RT_EOK;
}

static rt_size_t rt_dflash_read(rt_device_t dev, rt_off_t pos, void* buffer, rt_size_t size)
{
    
    int r;
    uint8_t *p;
    uint8_t i;

    p = (uint8_t*)(FLASH_OFFSET + pos * SectorCnt);
    //rt_kprintf("r pos:%d size:%d\r\n", pos, size);
    rt_memcpy(buffer, p, size * SectorCnt);

	return size;
}

static rt_size_t rt_dflash_write (rt_device_t dev, rt_off_t pos, const void* buffer, rt_size_t size)
{
    int i;
    uint8_t *p;
    
    p = (uint8_t*)(FLASH_OFFSET + pos * SectorCnt);
    //rt_kprintf("w pos:%d size:%d\r\n", pos, size);
    for(i=0;i<size;i++)
    {
        __disable_irq();
        FLASH_EraseSector((uint32_t)p);
        FLASH_WriteSector((uint32_t)p, buffer, SectorCnt);
        __enable_irq();
        p += SectorCnt;
    }
        
    return size;
}

static rt_err_t rt_dflash_control(rt_device_t dev, rt_uint8_t cmd, void *args)
{
    struct rt_device_blk_geometry geometry;
    rt_memset(&geometry, 0, sizeof(geometry));
	switch (cmd)
	{
        case RT_DEVICE_CTRL_BLK_GETGEOME:
            geometry.block_size = SectorCnt;
            geometry.bytes_per_sector = SectorCnt;
        
            geometry.sector_count = UDISK_SIZE/SectorCnt;
            rt_memcpy(args, &geometry, sizeof(struct rt_device_blk_geometry));
//            rt_kprintf("size:%d\r\n", geometry.sector_count);
		break;
	default: 
		break;
	}
	return RT_EOK;

}

rt_err_t rt_dflash_txcomplete(rt_device_t dev, void *buffer)
{
	return RT_EOK;
}

void rt_hw_dflash_init(void)
{

	dflash_device.type 		= RT_Device_Class_Block;
	dflash_device.rx_indicate = rt_dflash_indicate;
	dflash_device.tx_complete = RT_NULL;
	dflash_device.init 		= rt_dflash_init;
	dflash_device.open		= rt_dflash_open;
	dflash_device.close		= rt_dflash_close;
	dflash_device.read 		= rt_dflash_read;
	dflash_device.write     = rt_dflash_write;
	dflash_device.control 	= rt_dflash_control;
	dflash_device.user_data	= RT_NULL;

    rt_device_register(&dflash_device, "dflash0", RT_DEVICE_FLAG_RDWR  | RT_DEVICE_FLAG_REMOVABLE|RT_DEVICE_FLAG_STANDALONE);
}

    
INIT_DEVICE_EXPORT(rt_hw_dflash_init);
