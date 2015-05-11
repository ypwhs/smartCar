#include "gpio.h"
#include "common.h"
#include "uart.h"
#include "cpuidy.h"

#include "swd.h"

#include <stdio.h>
#include <string.h>
#include "flash_kinetis.h"

#include "target_image.h"


uint8_t buf[4*1024];


static int ShowID(void)
{
    uint32_t id;
    SWJ_ReadDP(DP_IDCODE, &id);
    printf("DP-IDR:0x%X\r\n", id);
    
    SWJ_ReadAP(0x000000FC, &id);
    printf("AHB_AP_IDR:0x%X\r\n", id);

    printf("MEMID:0x%X\r\n", TFlashGetMemID());
    printf("SDID:0x%X\r\n", TFlashGetSDID());
    
    return 0;
}


extern const TARGET_FLASH flash;
    
int main(void)
{   
    uint32_t i, err;
    
    DelayInit();
    GPIO_QuickInit(HW_GPIOC, 10, kGPIO_Mode_OPP);
    
    #ifdef MK22F25612
    UART_QuickInit(UART0_RX_PB16_TX_PB17, 115200);
    #elif  MK20D5
    UART_QuickInit(UART1_RX_PC03_TX_PC04, 115200);
    #endif
    
    printf("program for Manley\r\n");

    swd_io_init();

    SWJ_InitDebug();
    
    SWJ_SetTargetState(RESET_HOLD);
    
    ShowID();
    for(i=0;i<sizeof(buf);i++)
    {
        buf[i] = i & 0xFF;
    }
    
    SWJ_WriteMem(0x20000000, buf, sizeof(buf));
    memset(buf, 0, sizeof(buf));
    SWJ_ReadMem(0x20000000, buf, sizeof(buf));

    for(i=0;i<sizeof(buf);i++)
    {
        if(buf[i] != (i & 0xFF))
        {
            printf("error buf[%d]:%d should be:%d\r\n", i, buf[i], i);
        }
    }
    
    printf("mem teset complete\r\n");
    
    
    SWJ_SetTargetState(RESET_PROGRAM);
    
    err =  TFlash_UnlockSequence();
    printf("TFlash_UnlockSequence %d\r\n", err);
    
    
    
    SWJ_SetTargetState(RESET_PROGRAM);
    
    err = TFlash_Init(&flash);
    printf("TFlash_Init %d\r\n", err);

    err = target_flash_program_page(&flash, 0x00000200, (uint8_t*)flash.image, 512);
    printf("target_flash_program_page %d\r\n", err);
    
    while(1)
    {
        GPIO_ToggleBit(HW_GPIOC, 10);
        DelayMs(500);
    }
}



