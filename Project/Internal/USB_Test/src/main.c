#include "gpio.h"
#include "common.h"
#include "uart.h"
#include "dma.h"
#include "usb.h"
#include "usb_hid.h"



int main(void)
{
    uint8_t buf[4];
    DelayInit();
    GPIO_QuickInit(HW_GPIOE, 6, kGPIO_Mode_OPP);
    
    #ifdef FRDM
    UART_QuickInit(UART0_RX_PB16_TX_PB17, 115200);
    #else
    UART_QuickInit(UART0_RX_PD06_TX_PD07, 115200);
    #endif
    
    printf("HelloWorld\r\n");
    if(USB_ClockInit())
    {
        printf("USB  Init failed, clock must be 96M or 48M\r\n");
        while(1);
    }
    USB_Init();
    NVIC_EnableIRQ(USB0_IRQn);
    USBD_HID_Init();
    
      S8 report[4] = {0, 0, 0, 0};
      
      report[0] = 1;
      report[1] = 1;
      report[2] = 1;
      report[3] = 1;
    while(1)
    {
        USB_HID_SetMouse(1,0,0,0);
        HID_Proc();
    }
}


