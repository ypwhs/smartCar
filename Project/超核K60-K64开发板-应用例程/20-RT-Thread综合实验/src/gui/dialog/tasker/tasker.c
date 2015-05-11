
#include "gui_appdef.h"

#define ID_FRAMEWIN_0  (GUI_ID_USER + 0x00)
#define ID_MULTIPAGE_0  (GUI_ID_USER + 0x04)




WM_HWIN _TaskerAddPage1(WM_HWIN hWin);
WM_HWIN _TaskerAddPage2(WM_HWIN hWin);

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "Task Manager", ID_FRAMEWIN_0, 0, 0, 240, 320, 0, 0x0, 0 },
  { MULTIPAGE_CreateIndirect, "Multipage", ID_MULTIPAGE_0, 0, 2, 232, 306, 0, 0x0, 0 },

};

static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;

  int i;
    switch (pMsg->MsgId)
    {
        case WM_INIT_DIALOG:

        hItem = pMsg->hWin;
        FRAMEWIN_SetFont(hItem, GUI_FONT_13_1);
        FRAMEWIN_AddCloseButton(hItem, FRAMEWIN_BUTTON_RIGHT, 0);
        WM_MakeModal(hItem);
        hItem = WM_GetDialogItem(pMsg->hWin, ID_MULTIPAGE_0);

        WM_HWIN hDialog;
        hDialog = _TaskerAddPage2(pMsg->hWin);
        MULTIPAGE_AddPage(hItem, hDialog, "Thread");
        hDialog = _TaskerAddPage1(pMsg->hWin);
        MULTIPAGE_AddPage(hItem, hDialog, "Performance");

        break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_MULTIPAGE_0: // Notifications sent by 'Multipage'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:

        break;
      case WM_NOTIFICATION_RELEASED:

        break;
      case WM_NOTIFICATION_VALUE_CHANGED:

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



WM_HWIN GUI_CreateTaskerDialog(void)
{
  WM_HWIN hWin;

  hWin = GUI_ExecDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return hWin;
}
