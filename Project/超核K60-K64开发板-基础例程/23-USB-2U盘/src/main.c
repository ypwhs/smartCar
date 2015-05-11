#include "chlib_k.h"

#define UDISK_SIZE          (400*1024)
#define FLASH_OFFSET        (32*1024)   /* UDisk start offset */

U8 BlockBuf[4096];

/* init */
void usbd_msc_init ()
{
  USBD_MSC_MemorySize = UDISK_SIZE;
  USBD_MSC_BlockSize  = FLASH_GetSectorSize();
  USBD_MSC_BlockGroup = 1;
  USBD_MSC_BlockCount = USBD_MSC_MemorySize / USBD_MSC_BlockSize;
  USBD_MSC_BlockBuf   = BlockBuf;

  USBD_MSC_MediaReady = __TRUE;

}

/* read */
void usbd_msc_read_sect (U32 block, U8 *buf, U32 num_of_blocks)
{
    uint8_t *p;
    uint8_t i;
    
    p = (uint8_t*)(FLASH_OFFSET + block * USBD_MSC_BlockSize);
    
    if (USBD_MSC_MediaReady)
    {
        for(i=0;i<num_of_blocks;i++)
        {
            memcpy(buf, p, num_of_blocks * USBD_MSC_BlockSize);
            p += USBD_MSC_BlockSize;
        }
    }
}

/* write */
void usbd_msc_write_sect (U32 block, U8 *buf, U32 num_of_blocks)
{
    int i,j;
    uint8_t *p;
    
    p = (uint8_t*)(FLASH_OFFSET + block * USBD_MSC_BlockSize);
    
    if (USBD_MSC_MediaReady)
    {
        for(i=0;i<num_of_blocks;i++)
        {
            FLASH_EraseSector((uint32_t)p);
            FLASH_WriteSector((uint32_t)p, buf, FLASH_GetSectorSize());
        }
    }
}


int main(void)
{
    DelayInit();
    GPIO_QuickInit(HW_GPIOE, 6, kGPIO_Mode_OPP);
    UART_QuickInit(UART0_RX_PD06_TX_PD07, 115200);
    printf("usbd hid msc(udisk) demo\r\n");

    usbd_init();                          /* USB Device Initialization          */
    usbd_connect(__TRUE);                 /* USB Device Connect                 */
    while (!usbd_configured ());          /* Wait for device to configure        */
    
    printf("usbd configure complete\r\n");
    
    while(1)
    {
        DelayMs(500);
    }
}


