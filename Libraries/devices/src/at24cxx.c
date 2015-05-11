/**
  ******************************************************************************
  * @file    at24cxx.c
  * @author  YANDLD
  * @version V2.5
  * @date    2013.12.25
  * @brief   www.beyondcore.net   http://upcmcu.taobao.com 
  ******************************************************************************
  */

#include <string.h>
#include "at24cxx.h"
#include "i2c.h"

/**
  ******************************************************************************
  * @supported chip    AT24C01
  *                    AT24C02
  *                    AT24C04
  *                    AT24C08
  *                    AT24C16
  ******************************************************************************
  */

struct at24cxx
{
    const char* name;
    uint32_t size;      /* total size */
    uint16_t page;      /* page size */
    uint8_t  addr;      /* base addr */
};

struct at24cxx_device 
{
    uint8_t     addr;
    uint8_t     page_size;
    uint32_t    total_size;
    uint32_t    instance;
    void        *user_data;
};

static struct at24cxx_device at24cxx_dev;

static const struct at24cxx at24cxx_tab[] = 
{
    {"at24c01",    128, 8,  0x50},
    {"at24c02",    256, 8,  0x50},
    {"at24c04",    512, 16, 0x50},
    {"at24c08",   1024, 16, 0x50},
    {"at24c16",   2048, 16, 0x50},
};

int at24cxx_init(uint32_t instance)
{
    uint32_t i;
    
    at24cxx_dev.instance = instance;
    
    for(i=0;i<ARRAY_SIZE(at24cxx_tab);i++)
    {
        if(!I2C_Probe(instance, at24cxx_tab[i].addr))
        {
            at24cxx_dev.page_size = 8;
            at24cxx_dev.total_size = 256;
            at24cxx_dev.addr = at24cxx_tab[i].addr;
            return 0;
        }
    }
    return 1;
}


int at24cxx_read(uint32_t addr, uint8_t *buf, uint32_t len)
{
    uint8_t chip_addr;
    
    chip_addr = (addr/256) + at24cxx_dev.addr;
    return I2C_BurstRead(at24cxx_dev.instance, chip_addr, addr%256, 1, buf, len);
}

static int at24cxx_write_page(uint32_t addr, uint8_t *buf, uint32_t len)
{
    uint8_t chip_addr;
    
    chip_addr = (addr/256) + at24cxx_dev.addr;
    return I2C_BurstWrite(at24cxx_dev.instance, chip_addr, addr%256, 1, buf, len);
}

int at24cxx_write(uint32_t addr, uint8_t *buf, uint32_t len)
{
    int ret;
	uint32_t secpos;
	uint32_t secoff;
	uint16_t Byteremian;
	uint32_t pageremain;
    uint32_t page_size = at24cxx_dev.page_size;
	secpos = addr/page_size;
	secoff = addr%page_size;
	pageremain = page_size - secoff;
    /* check vailidiation */
    if((addr + len ) > at24cxx_dev.total_size)
    {
        return 1;
    }
    ret = at24cxx_write_page(addr, buf, len);
    if(ret)
    {
        return ret;
    }
    while(I2C_Probe(at24cxx_dev.instance, at24cxx_dev.addr));
	addr += pageremain;
	secpos++;
	Byteremian = len - pageremain;
	while(Byteremian != 0)
	{
		if(Byteremian <= page_size)
		{
			 pageremain = Byteremian;
		}
		else
		{
			pageremain = page_size;
		}
        ret = at24cxx_write_page(secpos*page_size, buf, pageremain);
        if(ret)
        {
            return ret;
        }
        while(I2C_Probe(at24cxx_dev.instance, at24cxx_dev.addr));
        secpos++;
        buf += pageremain;
        Byteremian -= pageremain;
	}
    return 0;
}


int at24cxx_self_test(void)
{
    int ret;
    uint8_t buf[128],buf1[128],buf2[128];
    ret = at24cxx_read(0, buf, sizeof(buf));
    if(ret)
    {
        return ret;
    }
    memset(buf1,'Y',sizeof(buf1));
    ret = at24cxx_write(0, buf1, sizeof(buf1));
    if(ret)
    {
        return ret;
    }
    ret = at24cxx_read(0, buf2, sizeof(buf2));
    if(ret)
    {
        return ret;
    }
    ret = at24cxx_write(0, buf, sizeof(buf));
    if(ret)
    {
        return ret;
    }
    if(!memcmp(buf1, buf2, sizeof(buf1)))
    {
        return 0;
    }
    return 1;
}







