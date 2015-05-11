
#include "DIALOG.h"
#include <rtthread.h>
#include "drv_led.h"

#define ID_FRAMEWIN_0  (GUI_ID_USER + 0x13)
#define ID_BUTTON_0  (GUI_ID_USER + 0x14)
#define ID_SLIDER_0  (GUI_ID_USER + 0x15)
#define ID_RADIO_0  (GUI_ID_USER + 0x16)
#define ID_BUTTON_1  (GUI_ID_USER + 0x17)
#define ID_TEXT_0  (GUI_ID_USER + 0x18)
#define ID_BUTTON_2  (GUI_ID_USER + 0x19)
#define ID_BUTTON_3  (GUI_ID_USER + 0x1A)
#define ID_BUTTON_4  (GUI_ID_USER + 0x1B)

#define LED_CONTROL     (0)
#define LED_WATERLAMP   (1)

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "Framewin1", ID_FRAMEWIN_0, 0, 0, 240, 320, 0, 0x64, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_0, 18, 72, 80, 25, 0, 0x0, 0 },
  { SLIDER_CreateIndirect, "Slider", ID_SLIDER_0, 17, 229, 187, 23, 0, 0x0, 0 },
  { RADIO_CreateIndirect, "Radio", ID_RADIO_0, 18, 12, 97, 41, 0, 0x1402, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_1, 123, 72, 80, 25, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_0, 18, 208, 80, 20, 0, 0x64, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_2, 18, 124, 80, 25, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_3, 124, 124, 80, 25, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_4, 144, 276, 80, 20, 0, 0x0, 0 },

};

static int _gMode;
static int _gFreq;
static rt_device_t dev_led;

static void _LedInit(void)
{
    dev_led = rt_device_find("led");
}

static void _cbDialog(WM_MESSAGE * pMsg) {
    WM_HWIN hItem;
    int     NCode;
    int     Id;
    int led;
    static int i;
    int led_num;
    
    switch (pMsg->MsgId)
    {
        case WM_TIMER:

            rt_device_control(dev_led, RT_DEVICE_GET_LED_NUM, &led_num);
            rt_device_control(dev_led, RT_DEVICE_CTRL_LED_TOGGLE, &i);
            i++; i%= led_num;
            if(_gMode == LED_WATERLAMP)
            {
                WM_RestartTimer(pMsg->Data.v, _gFreq);
            }
            break;
      case WM_INIT_DIALOG:

        _gMode = 0;
        _LedInit();

        hItem = pMsg->hWin;
        FRAMEWIN_SetText(hItem, "led");
      
        hItem = WM_GetDialogItem(pMsg->hWin, ID_RADIO_0);
        RADIO_SetText(hItem, "control", 0);
        RADIO_SetText(hItem, "Water lamp", 1);
        RADIO_SetFont(hItem, GUI_FONT_13B_1);
      
        hItem = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_0);
        SLIDER_SetRange(hItem, 50, 1000);
      
        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
        BUTTON_SetText(hItem, "led1");

        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
        BUTTON_SetText(hItem, "led2");

        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
        TEXT_SetText(hItem, "freuqency");

        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
        BUTTON_SetText(hItem, "led3");

        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_3);
        BUTTON_SetText(hItem, "led4");

        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_4);
        BUTTON_SetText(hItem, "Cancel");
        break;
      case WM_NOTIFY_PARENT:
        Id    = WM_GetId(pMsg->hWinSrc);
        NCode = pMsg->Data.v;
        switch(Id) {
        case ID_SLIDER_0: // Notifications sent by 'Slider'
            switch(NCode)
            {
                case WM_NOTIFICATION_VALUE_CHANGED:
                    _gFreq = SLIDER_GetValue(WM_GetDialogItem(pMsg->hWin, ID_SLIDER_0));
                break;
            }
            break;
        case ID_RADIO_0:
          switch(NCode) {
          case WM_NOTIFICATION_CLICKED:
            break;
          case WM_NOTIFICATION_RELEASED:
            break;
          case WM_NOTIFICATION_VALUE_CHANGED:
              _gMode = RADIO_GetValue(WM_GetDialogItem(pMsg->hWin, ID_RADIO_0));
              if(_gMode == LED_WATERLAMP)
              {
                  WM_CreateTimer(WM_GetClientWindow(pMsg->hWin), 0, 200, 0);
              }
              
            break;
          }
          break;
          
        case ID_BUTTON_0:
          switch(NCode) {
          case WM_NOTIFICATION_CLICKED:
                if(_gMode == LED_CONTROL)
                {
                    led = 0;
                    rt_device_control(dev_led, RT_DEVICE_CTRL_LED_TOGGLE, &led); 
                }
                break;
          }
          break;

        case ID_BUTTON_1:
          switch(NCode) {
          case WM_NOTIFICATION_CLICKED:
                if(_gMode == LED_CONTROL)
                {
                    led = 1;
                    rt_device_control(dev_led, RT_DEVICE_CTRL_LED_TOGGLE, &led); 
                }
            break;
          }
          break;
        case ID_BUTTON_2:
          switch(NCode) {
          case WM_NOTIFICATION_CLICKED:
                if(_gMode == LED_CONTROL)
                {
                    led = 2;
                    rt_device_control(dev_led, RT_DEVICE_CTRL_LED_TOGGLE, &led); 
                }
            break;
          }
          break;
        case ID_BUTTON_3:
          switch(NCode) {
          case WM_NOTIFICATION_CLICKED:
                if(_gMode == LED_CONTROL)
                {
                    led = 3;
                    rt_device_control(dev_led, RT_DEVICE_CTRL_LED_TOGGLE, &led); 
                }
            break;
          }
          break;
        case ID_BUTTON_4: // Notifications sent by 'Button'
          switch(NCode) {
          case WM_NOTIFICATION_RELEASED:
                    GUI_EndDialog(pMsg->hWin, 0);
            break;

          }
          break;
        }
        break;

  default:
    WM_DefaultProc(pMsg);
    break;
  }
}


WM_HWIN GUI_AppDispLED(void)
{
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return hWin;
}

