#include "chlib_k.h"

#define SECTER_SIZE         (512)

U8 BlockBuf[SECTER_SIZE];


/* init */
void usbd_msc_init (void)
{
    USBD_MSC_MemorySize = SD_GetSizeInMB()*1024*1024;
    USBD_MSC_BlockSize  = SECTER_SIZE;
    USBD_MSC_BlockGroup = 1;
    USBD_MSC_BlockCount = USBD_MSC_MemorySize / USBD_MSC_BlockSize;
    USBD_MSC_BlockBuf   = BlockBuf;
    
    USBD_MSC_MediaReady = __TRUE;
}

/* read */
void usbd_msc_read_sect (U32 block, U8 *buf, U32 num_of_blocks)
{
    if (USBD_MSC_MediaReady)
    {
        SD_ReadMultiBlock(block, buf, num_of_blocks);
    }
}

/* write */
void usbd_msc_write_sect (U32 block, U8 *buf, U32 num_of_blocks)
{
    if (USBD_MSC_MediaReady)
    {
        SD_WriteMultiBlock(block, buf, num_of_blocks);
    }
}


int main(void)
{
    DelayInit();
    GPIO_QuickInit(HW_GPIOE, 6, kGPIO_Mode_OPP);
    UART_QuickInit(UART0_RX_PD06_TX_PD07, 115200);
    printf("usbd hid msc(udisk) demo\r\n");

    
    SD_QuickInit(20*1000*1000);
    
    usbd_init();                          /* USB Device Initialization          */
    usbd_connect(__TRUE);                 /* USB Device Connect                 */
    
    while (!usbd_configured ());          /* Wait for device to configure        */
    
    printf("usbd configure complete\r\n");
    
    while(1)
    {
        DelayMs(500);
    }
}


