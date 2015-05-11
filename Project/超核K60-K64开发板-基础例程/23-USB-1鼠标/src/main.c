#include "chlib_k.h"


/*
 *  Get Mouse Input Report -> MouseInReport
 *    Parameters:      report:
 *                               Byte0.0: 1st Button (Left)
 *                               Byte0.1: 2nd Button (Right)
 *                               Byte0.2: 3rd Button
 *                               Byte1:   Relative X Pos
 *                               Byte2:   Relative Y Pos
 *                               Byte3:   Relative Wheel Pos
 *                     size:     report size
 *    Return Value:    None
 */
void GetMouseInReport (S8 *report, U32 size)
{

    report[0] = 0;
    report[1] = 1; /* little move */
    report[2] = 0;
    report[3] = 0;
}

int usbd_hid_get_report (U8 rtype, U8 rid, U8 *buf, U8 req)
{
    U32 i;
    S8 report[4] = {0, 0, 0, 0};
    switch (rtype)
    {
        case HID_REPORT_INPUT:
        switch (rid)
        {
            case 0:
            switch (req)
            {
                case USBD_HID_REQ_EP_CTRL:
                case USBD_HID_REQ_PERIOD_UPDATE:
                    GetMouseInReport (report, 4);
                    for (i = 0; i < sizeof(report); i++)
                    {
                        *buf++ = (U8)report[i];
                    }
                    return (1);
                case USBD_HID_REQ_EP_INT:
                break;
            }
            break;
        }
    break;
    case HID_REPORT_FEATURE:
      return (1);
  }
  return (0);
}

void usbd_hid_set_report (U8 rtype, U8 rid, U8 *buf, int len, U8 req)
{
    static int i;
    switch (rtype)
    {
        case HID_REPORT_OUTPUT:
            printf("%d  %d\r\n", i++, len);
        break;
        case HID_REPORT_FEATURE:
        break;
    }
}

int main(void)
{
    S8 report[4];
    DelayInit();
    GPIO_QuickInit(HW_GPIOE, 6, kGPIO_Mode_OPP);
    UART_QuickInit(UART0_RX_PD06_TX_PD07, 115200);
    printf("usbd hid mouse demo\r\n");

    usbd_init();                          /* USB Device Initialization          */
    usbd_connect(__TRUE);                 /* USB Device Connect                 */
    while (!usbd_configured ());          /* Wait for device to configure        */
    
    printf("usbd mouse configure complete\r\n");
    
    while(1)
    {
        GetMouseInReport(report, 4);
        usbd_hid_get_report_trigger(0, (U8 *)report, 4);
        DelayMs(10);
    }
}


