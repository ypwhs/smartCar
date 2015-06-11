/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2013        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control module to the FatFs module with a defined API.        */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
#include "sd.h"
/* Definitions of physical drive number for each media */
#define SD_CARD 0
#define MMC		1
#define USB		2


/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber (0..) */
)
{
//	DSTATUS stat;
//	int result;

	switch (pdrv)
    {
        case SD_CARD :
            SD_QuickInit(12*1000*1000);
            return RES_OK;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber (0..) */
)
{
//	DSTATUS stat;
//	int result;

	switch (pdrv)
    {
        case SD_CARD :
            return RES_OK;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	UINT count		/* Number of sectors to read (1..128) */
)
{
	DRESULT res;
//	int result;

	switch (pdrv)
    {
        case SD_CARD :
            SD_ReadMultiBlock(sector, buff, count);
            res = RES_OK;
		return res;

	}
	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _USE_WRITE
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber (0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	UINT count			/* Number of sectors to write (1..128) */
)
{
	DRESULT res;
//	int result;

	switch (pdrv)
    {
	case SD_CARD :
		// translate the arguments here
        SD_WriteMultiBlock(sector, buff, count);
        res = RES_OK;
		return res;

	}
	return RES_PARERR;
}
#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;
//	int result;

	switch (pdrv)
    {
        case SD_CARD :
            switch(cmd)
            {
                case CTRL_SYNC:
                    res = RES_OK;
                    break;	 
                case GET_SECTOR_SIZE:
                    *(WORD*)buff = 512;
                    res = RES_OK;
                    break;	 
                case GET_BLOCK_SIZE:
                    *(WORD*)buff = 8;
                    res = RES_OK;
                    break;	 
                case GET_SECTOR_COUNT:
                    *(DWORD*)buff = SD_GetSizeInMB()*1024*2;
                    res = RES_OK;
                    break;
                default:
                    res = RES_PARERR;
                    break;
            }
		return res;
	}
	return RES_PARERR;
}
#endif

//获得时间
//User defined function to give a current time to fatfs module      */
//31-25: Year(0-127 org.1980), 24-21: Month(1-12), 20-16: Day(1-31) */                                                                                                                                                                                                                                          
//15-11: Hour(0-23), 10-5: Minute(0-59), 4-0: Second(0-29 *2) */                                                                                                                                                                                                                                                
DWORD get_fattime (void)
{				 
	return 0;
}		
