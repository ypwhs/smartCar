#include "shell.h"
#include "gpio.h"
#include "board.h"
#include "sd.h"
#include "ff.h"

__align(4) static uint8_t sd_buffer[512];

int DoSD(int argc, char * const argv[])
{
    uint32_t i;
    printf("SD test\r\n");
    printf("please insert SD card...\r\n");
    SD_QuickInit(20*1000*1000);
    printf("SD size:%dMB\r\n", SD_GetSizeInMB());
    printf("FATFS test\r\n");
    printf("please insert SD card...\r\n");
    FRESULT rc;
    FATFS fs_sd;
    FIL fil;
    FATFS *fs;
    fs = &fs_sd;

    return 0;
}

SHELL_EXPORT_CMD(DoSD, sd , sd card test)
    
