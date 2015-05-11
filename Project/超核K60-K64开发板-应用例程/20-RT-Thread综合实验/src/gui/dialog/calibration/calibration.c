#include "gui_appdef.h"


#define ID_FRAMEWIN_0  (GUI_ID_USER + 0x00)
#define ID_TEXT_0  (GUI_ID_USER + 0x01)
#define ID_TEXT_1  (GUI_ID_USER + 0x02)
#define ID_BUTTON_0  (GUI_ID_USER + 0x04)
#define ID_BUTTON_1  (GUI_ID_USER + 0x05)
#define ID_TEXT_2  (GUI_ID_USER + 0x06)
#define ID_TEXT_3  (GUI_ID_USER + 0x07)



static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { FRAMEWIN_CreateIndirect,    "Framewin", ID_FRAMEWIN_0, 0, 0, 240, 320, 0, 0x64, 0 },
  { TEXT_CreateIndirect,        "Text", ID_TEXT_0, 18, 104, 194, 109, 0, 0x64, 0 },
  { TEXT_CreateIndirect,        "TOUCH_Calibrate", ID_TEXT_1, 67, 0, 95, 17, 0, 0x64, 0 },
  { BUTTON_CreateIndirect,      "", ID_BUTTON_0, 10, 10, 13, 13, 0, WM_CF_HIDE, 0 },
  { BUTTON_CreateIndirect,      "", ID_BUTTON_1, 215, 300, 13, 13, 0, WM_CF_HIDE, 0 },
  { TEXT_CreateIndirect,        "Press here", ID_TEXT_2, 10, 25, 98, 20, 0, WM_CF_HIDE, 0 },
  { TEXT_CreateIndirect,        "Press here", ID_TEXT_3, 150, 300, 70, 20, 0, WM_CF_HIDE, 0 },
};

static void _WaitForPressedState(int Pressed) {
  GUI_PID_STATE State;

  do {
    GUI_TOUCH_GetState(&State);
    GUI_Delay(1);
    if (State.Pressed == Pressed) {
      int TimeStart = GUI_GetTime();
      do {
        GUI_TOUCH_GetState(&State);
        GUI_Delay(1);
        if (State.Pressed != Pressed) {
          break;
        } else if ((GUI_GetTime() - 50) > TimeStart) {
          return;
        }
      } while (1);
    }
  } while (1);
}

static void _cbDialog(WM_MESSAGE * pMsg)
{

    WM_HWIN hItem;
    switch (pMsg->MsgId)
    {
        case WM_INIT_DIALOG:
            hItem = pMsg->hWin;
            FRAMEWIN_SetTitleVis(hItem, 0);
            GUI_SetFont(&GUI_Font13B_ASCII);
            // 
            hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
            TEXT_SetWrapMode(hItem, GUI_WRAPMODE_CHAR);
            TEXT_SetTextAlign(hItem, GUI_TA_HORIZONTAL|GUI_TA_VCENTER);
            TEXT_SetText(hItem,"This sample shows how a touch screen can be calibrated at run time.\n"
                               "Please press HERE to continue...");

            GUI_CURSOR_Hide();
        default:
            WM_DefaultProc(pMsg);
        break;
    }
}

WM_HWIN GUI_ExecCalibrationDialog(GUI_TouchData* pTouchData)
{
    GUI_TouchData tData;
    static int State;
    
    WM_HWIN hWin;
    State = 0;
    tData.magic = 'T';
    tData.aLogX[0] = 15;
    tData.aLogY[0] = 15;
    tData.aLogX[1] = 220;
    tData.aLogY[1] = 305;
    
    hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);

    while(State <= 3)
    {
        switch(State)
        {
            case 0:
                WM_HideWindow(WM_GetDialogItem(hWin, ID_TEXT_2));
                WM_HideWindow(WM_GetDialogItem(hWin, ID_TEXT_3));
                WM_HideWindow(WM_GetDialogItem(hWin, ID_BUTTON_0));
                WM_HideWindow(WM_GetDialogItem(hWin, ID_BUTTON_1));
                WM_ShowWindow(WM_GetDialogItem(hWin, ID_TEXT_0));
                WM_ShowWindow(WM_GetDialogItem(hWin, ID_TEXT_1));  
                break;
            case 1:
                WM_HideWindow(WM_GetDialogItem(hWin, ID_TEXT_0));
                WM_HideWindow(WM_GetDialogItem(hWin, ID_TEXT_1)); 
                WM_ShowWindow(WM_GetDialogItem(hWin, ID_BUTTON_0));
                WM_ShowWindow(WM_GetDialogItem(hWin, ID_TEXT_2)); 
                break;
            case 2:
                tData.aPhysX[0] = GUI_TOUCH_GetxPhys();
                tData.aPhysY[0] = GUI_TOUCH_GetyPhys();
                WM_HideWindow(WM_GetDialogItem(hWin, ID_BUTTON_0));
                WM_HideWindow(WM_GetDialogItem(hWin, ID_TEXT_2)); 
                WM_ShowWindow(WM_GetDialogItem(hWin, ID_BUTTON_1));
                WM_ShowWindow(WM_GetDialogItem(hWin, ID_TEXT_3));   
                break;
            case 3:
                tData.aPhysX[1] = GUI_TOUCH_GetxPhys();
                tData.aPhysY[1] = GUI_TOUCH_GetyPhys();
                WM_HideWindow(WM_GetDialogItem(hWin, ID_BUTTON_1));
                WM_HideWindow(WM_GetDialogItem(hWin, ID_TEXT_3));
                rt_kprintf("Touch Data:%d %d %d %d", tData.aPhysX[0], tData.aPhysX[1], tData.aPhysY[0], tData.aPhysY[1]);
                GUI_TOUCH_Calibrate(0, tData.aLogX[0], tData.aLogX[1], tData.aPhysX[0], tData.aPhysX[1]); /* Calibrate X-axis */
                GUI_TOUCH_Calibrate(1, tData.aLogY[0], tData.aLogY[1], tData.aPhysY[0], tData.aPhysY[1]); /* Calibrate Y-axis */
                GUI_CURSOR_Show();
                GUI_MessageBox("Calibrate OK", "OK", GUI_MESSAGEBOX_CF_MODAL);
                if(pTouchData != RT_NULL)
                {
                    if(pTouchData->magic == 'T')
                    {
                        rt_memcpy(pTouchData, &tData, sizeof(GUI_TouchData));
                    }
                }
                GUI_EndDialog(hWin, 0);
                return 0;
                break;
            default:
                break;
        }
        GUI_Exec();
        _WaitForPressedState(0);
        _WaitForPressedState(1);
        State++;
    }
    return 1;
}

