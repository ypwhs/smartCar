

#include "DIALOG.h"
#include "rtthread.h"


#define ID_FRAMEWIN_0 (GUI_ID_USER + 0x00)
#define ID_TEXT_0 (GUI_ID_USER + 0x02)
#define ID_BUTTON_0 (GUI_ID_USER + 0x03)


static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "system", ID_FRAMEWIN_0, 0, 0, 240, 320, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_0, 3, 3, 80, 20, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_0, 149, 280, 80, 20, 0, 0x0, 0 },

};


static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;


switch (pMsg->MsgId)
{
    rt_uint32_t tick;
    rt_uint8_t str_buffer[128];
    case WM_TIMER:
        tick = rt_tick_get();
        rt_sprintf(str_buffer, "run tims:%ds",tick/RT_TICK_PER_SECOND);
        
        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
        WM_RestartTimer(pMsg->Data.v, 1000);
        TEXT_SetText(hItem, (const char*)str_buffer);
        break;
    case WM_INIT_DIALOG:
        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
        BUTTON_SetFont(hItem, GUI_FONT_13_1);
        BUTTON_SetText(hItem, "OK");
        WM_MakeModal(pMsg->hWin);
        
        WM_CreateTimer(WM_GetClientWindow(pMsg->hWin), 0, 1000, 0);
        break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_BUTTON_0: // Notifications sent by 'Button'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        GUI_EndDialog(pMsg->hWin, 0);
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    // USER START (Optionally insert additional code for further Ids)
    // USER END
    }
    break;
  // USER START (Optionally insert additional message handling)
  // USER END
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}



WM_HWIN GUI_AppDispSysInfo(void)
{
  WM_HWIN hWin;
  hWin = GUI_ExecDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return hWin;
}


