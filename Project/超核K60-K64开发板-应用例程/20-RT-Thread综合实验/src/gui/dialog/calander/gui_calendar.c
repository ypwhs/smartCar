#include "gui_appdef.h"
#include <time.h>


#define ID_FRAMEWIN_0       (GUI_ID_USER + 0x08)
#define ID_BUTTON_0         (GUI_ID_USER + 0x09)
#define ID_BUTTON_1         (GUI_ID_USER + 0x0A)


static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "Calander", ID_FRAMEWIN_0, 0, 1, 160, 200, 0, FRAMEWIN_CF_MOVEABLE, 0 },
  { BUTTON_CreateIndirect, "OK", ID_BUTTON_0, 0, 154, 60, 20, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Back", ID_BUTTON_1, 94, 154, 60, 20, 0, 0x0, 0 },

};


static void _cbDialog(WM_MESSAGE * pMsg)
{
    WM_HWIN hItem;
    int     NCode;
    int     Id;


  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    hItem = pMsg->hWin;
    FRAMEWIN_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    FRAMEWIN_SetFont(hItem, GUI_FONT_13B_1);
    FRAMEWIN_SetMoveable(hItem, 1);
    WM_MakeModal(hItem);

    CALENDAR_SetDefaultSize(CALENDAR_SI_HEADER, 25+4);
    CALENDAR_SetDefaultSize(CALENDAR_SI_CELL_X, 18+4);
    CALENDAR_SetDefaultSize(CALENDAR_SI_CELL_Y, 13+4);
    hItem = CALENDAR_Create(hItem, 2, 19, 2014, 4, 5, 1, GUI_ID_CALENDAR0, WM_CF_SHOW);
    
    time_t now = time(0);
    struct tm* time_stamp;
    time_stamp = localtime(&now);
    CALENDAR_DATE cd;
    cd.Year = time_stamp->tm_year + 1900;
    cd.Month = time_stamp->tm_mon + 1;
    cd.Day = time_stamp->tm_mday;
    CALENDAR_SetDate(hItem, &cd);
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id)
    {
        case ID_BUTTON_0:
            switch(NCode)
            {
                case WM_NOTIFICATION_CLICKED:
                GUI_EndDialog(pMsg->hWin, 0);
                break;
                case WM_NOTIFICATION_RELEASED:
                break;
            }
            break;
            case ID_BUTTON_1:
            switch(NCode)
            {
                case WM_NOTIFICATION_CLICKED:
                GUI_EndDialog(pMsg->hWin, 0);
                break;
            case WM_NOTIFICATION_RELEASED:
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


void GUI_AppDispCalender(void)
{
    GUI_ExecDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
}

