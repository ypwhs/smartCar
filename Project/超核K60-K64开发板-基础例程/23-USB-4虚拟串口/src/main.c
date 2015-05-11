#include "chlib_k.h"




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
        int32_t ch;
        ch = USBD_CDC_ACM_GetChar();
        if(ch != -1)
        {
            USBD_CDC_ACM_DataSend("USB VCOM test\r\n", 15);
            USBD_CDC_ACM_PutChar(ch);
        }
    }
}


