#include "gpio.h"
#include "uart.h"
#include "sd.h"
#include "sram.h"

#include "ff.h"
#include "gui.h"
#include <string.h>
#include <stdio.h>

/* 打印操作结果 */
#define ERROR_TRACE(rc)     do {if(rc != 0){printf("fatfs error:%d\r\n", rc);}} while(0)

/* 显示BMP图片 */
static int _DrawBMPFile(char *path)
{
    FRESULT rc;
    uint32_t size;
    FIL fil;
    uint8_t *pFile = (uint8_t*)SRAM_START_ADDRESS;
    UINT br;
    /* 读取文件 */
    rc = f_open(&fil, path, FA_READ);
    ERROR_TRACE(rc);
    if(rc) return 1;
    size = f_size(&fil);
    printf("file size:%d\r\n", size);
    if(size > SRAM_SIZE)
    {
        printf("file size to large\r\n");
        rc = f_close(&fil);
        return 2;
    }
    rc = f_read(&fil, pFile, size, &br);
    if(rc || br != size)
    {
        printf("file data read failed\r\n");
        rc = f_close(&fil);
        return 3;
    }
    printf("Xsize:%d Ysize:%d\r\n", GUI_BMP_GetXSize(pFile), GUI_BMP_GetYSize(pFile));
    GUI_Clear();
    if(GUI_BMP_Draw(pFile, 0, 0))
    {
        GUI_DispString("BMP file display failed!");
    }
    rc = f_close(&fil);
    ERROR_TRACE(rc);
    return 0;
}

/* 扫描磁盘BMP文件 */
static FRESULT scan_files(char* path)
{
    FRESULT res;
    FILINFO fno;
    DIR dir;
    char *fn;   /* This function is assuming non-Unicode cfg. */
#if _USE_LFN
    static char lfn[_MAX_LFN + 1];   /* Buffer to store the LFN */
    fno.lfname = lfn;
    fno.lfsize = sizeof lfn;
#endif
    
    char full_path[64];
    res = f_opendir(&dir, path);                       /* Open the directory */
    if (res == FR_OK)
    {
        for (;;)
        {
            res = f_readdir(&dir, &fno);                   /* Read a directory item */
            if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
            if (fno.fname[0] == '.') continue;             /* Ignore dot entry */
#if _USE_LFN
            fn = *fno.lfname ? fno.lfname : fno.fname;
#else
            fn = fno.fname;
#endif
            if (fno.fattrib & AM_DIR)
            {                    /* It is a directory */
                //printf("%s <DIR>\r\n", fn);
                res = scan_files(fn);
                if (res != FR_OK) break;
            }
            else
            {
                /* It is a file. */
                //printf("File:%s\r\n", fn);
                if(!strncmp((const char *)(uint32_t)fn + strlen(fn)-3 , "BMP", 3))
                {
                    sprintf(full_path, "%s/%s", path, fn);
                    printf("openning file:%s/%s...\r\n", path, fn); 
                    _DrawBMPFile(full_path);
                    DelayMs(500);
                }
            }
        }
        f_closedir(&dir);
    }
    return res;
}

int main(void)
{
    DelayInit();
    GPIO_QuickInit(HW_GPIOE, 6, kGPIO_Mode_OPP);
    SRAM_Init();
    UART_QuickInit(UART0_RX_PD06_TX_PD07, 115200);
    GUI_Init();
    GUI_DispString("BMP file test\r\n");
    GUI_DispString("please insert SD card...\r\n");
    SD_QuickInit(10*1000*1000);
    printf("SD size:%dMB\r\n", SD_GetSizeInMB());
    FATFS fs_sd;
    FATFS *fs = &fs_sd;
    /* 挂载文件系统 */
    f_mount(fs, "0:", 0);
    
    while(1)
    {
        scan_files("0:");
        GPIO_ToggleBit(HW_GPIOE, 6);
    }
}


