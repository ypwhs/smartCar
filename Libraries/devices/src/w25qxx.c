/**
  ******************************************************************************
  * @file    w25qxx.c
  * @author  YANDLD
  * @version V2.5
  * @date    2013.12.25
  * @brief   www.beyondcore.net   http://upcmcu.taobao.com 
  ******************************************************************************
  */

#include <string.h>

#include "spi.h"
#include "w25qxx.h"

//芯片WQ25指令
#define W25X_WriteEnable		0x06 
#define W25X_WriteDisable		0x04 
#define W25X_ReadStatusReg		0x05 
#define W25X_ReadStatusReg2		0x35 
#define W25X_WriteStatusReg		0x01 
#define W25X_ReadData			0x03 
#define W25X_FastReadData		0x0B 
#define W25X_FastReadDual		0x3B 
#define W25X_PageProgram		0x02 
#define W25X_BlockErase			0xD8 
#define W25X_SectorErase		0x20 
#define W25X_ChipErase			0xC7 
#define W25X_PowerDown			0xB9 
#define W25X_ReleasePowerDown	0xAB 
#define W25X_DeviceID			0xAB 
#define W25X_ManufactDeviceID	0x90 
#define W25X_JedecDeviceID		0x9F 


#ifndef USE_USER_MEM
uint8_t SPI_FLASH_BUFFER[4096];	
#endif

#define W25QXX_DEBUG		0
#if ( W25QXX_DEBUG == 1 )
#define W25QXX_TRACE	printf
#else
#define W25QXX_TRACE(...)
#endif

//W25芯片信息
static const struct w25qxx_attr_t w25qxx_tbl[] = 
{
    {"W25Q10",    128*1024, 0xEF10, 256, 4096, (64*1024)},
    {"W25Q20",    256*1024, 0xEF11, 256, 4096, (64*1024)},
    {"W25Q40",    512*1024, 0xEF12, 256, 4096, (64*1024)},
    {"W25Q80",   1024*1024, 0xEF13, 256, 4096, (64*1024)},
    {"W25Q16",   2048*1024, 0xEF14, 256, 4096, (64*1024)},
    {"W25Q32",   4096*1024, 0xEF15, 256, 4096, (64*1024)},
    {"W25Q64",   8192*1024, 0xEF16, 256, 4096, (64*1024)},
    {"W25Q128", 16384*1024, 0xEF17, 256, 4096, (64*1024)}, 
};

struct w25qxx_device 
{
    const char              *name;
    uint32_t                spi_instance;
    uint8_t                 spi_cs;
    struct w25qxx_attr_t    attr;
    void                    *user_data;
};

static struct w25qxx_device w25_dev;

static inline uint8_t spi_xfer(uint8_t data, SPI_PCS_Type csStatus)
{
    return (uint8_t)SPI_ReadWriteByte(w25_dev.spi_instance , HW_CTAR0, (uint8_t)data, w25_dev.spi_cs, csStatus);
}


//芯片上电
static int w25qxx_power_up(void)
{
    volatile uint32_t i;
    spi_xfer(W25X_ReleasePowerDown, kSPI_PCS_ReturnInactive);
    
    /* delay 3us */
    for(i=0;i<1000*5;i++);
    return 0;
}

//芯片读
static uint8_t w25qxx_read_sr2(void)
{
    uint8_t sr;
    
    spi_xfer(W25X_ReadStatusReg2, kSPI_PCS_KeepAsserted);
    sr = spi_xfer(0x00, kSPI_PCS_ReturnInactive);
    
    return sr;
}

static uint8_t w25qxx_read_sr(void)
{
    uint8_t sr;
    
    spi_xfer(W25X_ReadStatusReg, kSPI_PCS_KeepAsserted);
    sr = spi_xfer(0x00, kSPI_PCS_ReturnInactive);
    
    return sr;
}

//芯片写使能
static int w25qxx_write_enable(void)
{
    spi_xfer(W25X_WriteEnable, kSPI_PCS_ReturnInactive);
    return 0;
}

//芯片写
static int w25qxx_write_sr(uint8_t value)
{
    w25qxx_write_enable();
    spi_xfer(W25X_WriteStatusReg, kSPI_PCS_KeepAsserted);
    spi_xfer(value, kSPI_PCS_ReturnInactive);
    return 0;
}

//芯片探测
static int w25qxx_probe(void)
{
    uint32_t i;
    uint16_t id;
    uint8_t buf[2];

    /* read id */
    spi_xfer(W25X_ManufactDeviceID, kSPI_PCS_KeepAsserted);
    spi_xfer(0, kSPI_PCS_KeepAsserted);
    spi_xfer(0, kSPI_PCS_KeepAsserted);
    spi_xfer(0, kSPI_PCS_KeepAsserted);
    buf[0] = spi_xfer(0, kSPI_PCS_KeepAsserted);
    buf[1] = spi_xfer(0, kSPI_PCS_ReturnInactive);
    id = ((buf[0]<<8) + buf[1]);
    W25QXX_TRACE("ID:0x%X\r\n", id);
    //see if we find a match
    for(i = 0; i< ARRAY_SIZE(w25qxx_tbl);i++)
    {
        if(w25qxx_tbl[i].id == id)
        {
            /* find a match */
            w25_dev.attr = w25qxx_tbl[i];
            w25qxx_power_up();
            buf[0] = w25qxx_read_sr();
            W25QXX_TRACE("SR:0x%X\r\n", buf[0]);
            buf[0] = w25qxx_read_sr2();
            W25QXX_TRACE("SR2:0x%X\r\n", buf[0]);
            // enable full access to all memory regin, something like unlock chip.
            w25qxx_write_sr(0x00);
            return 0; 
        }
    }
    return 1;
}

int w25qxx_get_attr(struct w25qxx_attr_t* attr)
{
    memcpy(attr, &w25_dev.attr, sizeof(struct w25qxx_attr_t));
    return 0;
}

//读取数据
int w25qxx_read(uint32_t addr, uint8_t *buf, uint32_t len)
{
    /* send addr */
    spi_xfer(W25X_ReadData, kSPI_PCS_KeepAsserted);
    spi_xfer((uint8_t)((addr)>>16), kSPI_PCS_KeepAsserted);
    spi_xfer((uint8_t)((addr)>>8), kSPI_PCS_KeepAsserted);
    spi_xfer((uint8_t)addr, kSPI_PCS_KeepAsserted);
   
    while(len--)
    {
        if(len)
            *buf++ = spi_xfer(0x00, kSPI_PCS_KeepAsserted);
        else
            *buf++ = spi_xfer(0x00, kSPI_PCS_ReturnInactive);
    }
    return 0;
}

//写一页数据
int w25qxx_write_page(uint32_t addr, uint8_t *buf, uint32_t len)
{
    w25qxx_write_enable();

    /* send addr */
    spi_xfer(W25X_PageProgram, kSPI_PCS_KeepAsserted);
    spi_xfer((uint8_t)((addr)>>16), kSPI_PCS_KeepAsserted);
    spi_xfer((uint8_t)((addr)>>8), kSPI_PCS_KeepAsserted);
    spi_xfer((uint8_t)addr, kSPI_PCS_KeepAsserted);
    
    while(len--)
    {
        if(len)
            spi_xfer(*buf, kSPI_PCS_KeepAsserted);
        else
            spi_xfer(*buf, kSPI_PCS_ReturnInactive);
        buf++;
    }
    
    /* wait busy */
    while((w25qxx_read_sr() & 0x01) == 0x01);
    return 0;
}

//写数据
static int w25qxx_write_no_check(uint32_t addr, uint8_t *buf, uint32_t len)  
{ 			 		 
	uint16_t pageremain;	   
    W25QXX_TRACE("w25qxx - write_no_check: addr:%d len:%d\r\n", addr, len);
    
	pageremain = w25_dev.attr.page_size-(addr%w25_dev.attr.page_size); //单页剩余的字节数		 	    
	if(len <= pageremain) pageremain = len;//不大于256个字节
	while(1)
	{	   
        if(w25qxx_write_page(addr, buf, pageremain))
        {
            return 1;
        }
		if(len == pageremain)break;//写入结束了
	 	else //NumByteToWrite>pageremain
		{
			buf += pageremain;
			addr += pageremain;	
			len -= pageremain;			  //减去已经写入了的字节数
			if(len > 256)pageremain=256; //一次可以写入256个字节
			else pageremain = len; 	  //不够256个字节了
		}
	}
    return 0;
} 

//擦除扇区
int w25qxx_erase_sector(uint32_t addr)
{
    
    addr /= w25_dev.attr.sector_size;
    addr *= w25_dev.attr.sector_size; //round addr to N x W25X_SECTOR_SIZE
    
    w25qxx_write_enable();
    while((w25qxx_read_sr() & 0x01) == 0x01);
    
    spi_xfer(W25X_SectorErase, kSPI_PCS_KeepAsserted);
    spi_xfer((uint8_t)((addr)>>16), kSPI_PCS_KeepAsserted);
    spi_xfer((uint8_t)((addr)>>8), kSPI_PCS_KeepAsserted);
    spi_xfer((uint8_t)addr, kSPI_PCS_ReturnInactive);
    
    while((w25qxx_read_sr() & 0x01) == 0x01);
    return 0;
}

//擦除整个芯片
int w25qxx_erase_chip(void)
{
    w25qxx_write_enable();
    while((w25qxx_read_sr() & 0x01) == 0x01);
    
    spi_xfer(W25X_ChipErase, kSPI_PCS_ReturnInactive);

    while((w25qxx_read_sr() & 0x01) == 0x01);
    return 0;
}

//向芯片写数据
int w25qxx_write(uint32_t addr, uint8_t *buf, uint32_t len)  
{ 
	uint32_t secpos;
	uint16_t secoff;
	uint16_t secremain;	   
 	uint16_t i;    
	uint8_t * mem_pool;
    mem_pool = SPI_FLASH_BUFFER;
 	secpos = addr/4096;//扇区地址  
	secoff = addr%4096;//在扇区内的偏移
	secremain = 4096-secoff;//扇区剩余空间大小   

 	if(len <= secremain)secremain = len;//不大于4096个字节
	while(1) 
	{	
        w25qxx_read(secpos*4096, mem_pool, 4096); //读出整个扇区的内容
		for(i = 0; i < secremain; i++)//校验数据
		{
			if(mem_pool[secoff+i]!=0XFF)break;//需要擦除  	  
		}
		if(i<secremain)//需要擦除
		{
            w25qxx_erase_sector(secpos*4096); //擦除这个扇区
			for(i=0;i<secremain;i++)	   //复制原来的数据
			{
				mem_pool[i+secoff]=buf[i];	  
			}
            w25qxx_write_no_check(secpos*4096, mem_pool,4096); //写入整个扇区 
		}else 
        {
            W25QXX_TRACE("no need to erase -addr:%d\r\n", addr);
            w25qxx_write_no_check(addr, buf, secremain);//写已经擦除了的,直接写入扇区剩余区间. 
        }			   
		if(len == secremain)break;//写入结束了
		else//写入未结束
		{
			secpos++;//扇区地址增1
			secoff=0;//偏移位置为0 	 

		   	buf += secremain;  //指针偏移
			addr += secremain;//写地址偏移	   
		   	len -= secremain;				//字节数递减
			if(len > 4096) secremain = 4096;	//下一个扇区还是写不完
			else secremain = len;			//下一个扇区可以写完了
		}	 
	}
    return 0;
}

//芯片初始化
int w25qxx_init(uint32_t instance, uint32_t cs)
{
    w25_dev.spi_instance = instance;
    w25_dev.spi_cs = cs; 
    SPI_CTARConfig(instance, HW_CTAR0, kSPI_CPOL0_CPHA0, 8, kSPI_MSB, 30*1000*1000);
    
    return w25qxx_probe();
}

