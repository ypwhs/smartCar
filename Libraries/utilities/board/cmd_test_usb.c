#include "shell.h"
#include "gpio.h"
#include "board.h"
#include "usb_hid.h"
#include "usb.h"


void USB_App_Callback(uint8_t controller_ID, uint8_t event_type, void* val)
{
    if((event_type == USB_APP_BUS_RESET) || (event_type == USB_APP_CONFIG_CHANGED))
    {
        
    }
    else if(event_type == USB_APP_ENUM_COMPLETE)
    {   /* if enumeration is complete set mouse_init
           so that application can start */
        printf("USB_APP_ENUM_COMPLETE\r\n");
    }
    
}
uint_8 USB_App_Param_Callback(
      uint_8 request,        /* [IN] request type */
      uint_16 value,         /* [IN] report type and ID */
      uint_16 wIndex,		 /* [IN] interface*/
      uint_8_ptr* data,      /* [OUT] pointer to the data */
      USB_PACKET_SIZE* size  /* [OUT] size of the transfer */
)
{
    
    printf("request:%d\r\n", request);
    return 0;
}


int DoUSB(int argc, char * const argv[])
{
    uint8_t buf[4];
    USB_Init();

    NVIC_EnableIRQ(USB0_IRQn);
    USB_Class_HID_Init(0, USB_App_Callback, NULL, USB_App_Param_Callback);
                            

    while(1)
    {
        USB_Class_HID_Periodic_Task();
        
        /* send USB data offset:1*/
        buf[2] = 1;
        buf[1] = 0x00;
        buf[0] = 0x00;
        USB_Class_HID_Send_Data(0, HID_ENDPOINT, buf, 4);
                                        
    }
    return 0;
}

SHELL_EXPORT_CMD(DoUSB, usb , usb test)


