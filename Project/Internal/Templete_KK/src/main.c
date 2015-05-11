#include "gpio.h"
#include "common.h"
#include "uart.h"
#include "pit.h"
#include "FlashOS.H"
#include "nfc.h"
#include "MT47H64M16HR.h"





int main(void)
{
    uint32_t i,j;
    
    DelayInit();
    
    GPIO_QuickInit(HW_GPIOA, 11, kGPIO_Mode_OPP);
    UART_QuickInit(UART2_RX_PE17_TX_PE16, 115200);
    
    printf("HelloWorld\r\n");
    
    /* DDR clock is routed from PLL1 */
    MCG->C10 = (uint8_t)0x14;
    MCG->C12 = (uint8_t)0x1EU;
    MCG->C11 = (uint8_t)0x44U;
    
    MT47H64M16HR_Init();
    DDR_SelfTest();

    
    #ifdef NFC
    
    #define PAGE_SIZE 2048
    #define ECC_SIZE  64

    NFC_InitTypeDef NFC_InitStruct;
    NFC_InitStruct.cs = 0;
    NFC_InitStruct.sectorSize = (PAGE_SIZE + ECC_SIZE);
    NFC_InitStruct.width = kNFC_Width16Bit;
    NFC_Init(&NFC_InitStruct);
    NFC_SendResetCmd();
    uint32_t id0, id1;
    NFC_ReadFlashID(0, &id0, &id1);
    printf("id0:0x%X 0x%X\r\n", id0, id1);

    /* erase all */
    for(i=0;i<2048;i++)
    {
        NFC_BlockErase(0, i*64);
    }
    
    for(j = 0 ; j < 2048*64; j++)
    {
        if(!(j%512))
        {
            printf("program sector:%d...", j);
        }
    
    uint32_t *p;
    
    p = (uint32_t*)NFC_GetBufAddr(0);
    
    for(i=0;i<PAGE_SIZE/4;i++)
    {
        p[i] = (uint32_t)&p[i];
    }
    
    NFC_PageProgram(0, 0, j, 0);
    
    p = (uint32_t*)NFC_GetBufAddr(0);
    
    for(i=0;i<PAGE_SIZE/4;i++)
    {
        p[i] = 0;
    }
    
    NFC_PageRead(0, 0, j, 0);
    
    if(!(j%512))
    {
        printf("varify...\r\n");
    }

    for(i=0; i< PAGE_SIZE/4; i++)
    {
        if(p[i] != (uint32_t)&p[i])
        {
            printf("NAND error %d! Read: 0x%08x Expected: 0x%08x\r\n", i, p[i], (uint32_t)&p[i]);
        }
    }
}

    #endif
    while(1)
    {
        GPIO_ToggleBit(HW_GPIOA, 11);
        DelayMs(500);
    }
}


