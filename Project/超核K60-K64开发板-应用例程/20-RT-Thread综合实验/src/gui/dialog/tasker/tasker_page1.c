#include "gui_appdef.h"
#include "drv_ksz8041.h"
#include "tasker.h"

void cpu_usage_get(rt_uint8_t *major, rt_uint8_t *minor);

#define ID_FRAMEWIN_0  (GUI_ID_USER + 0x00)

#define ID_TEXT_0  (GUI_ID_TEXT0 + 0x00)
#define ID_TEXT_1  (GUI_ID_TEXT0 + 0x01)
#define ID_TEXT_2  (GUI_ID_TEXT0 + 0x02)
#define ID_TEXT_3  (GUI_ID_TEXT0 + 0x03)

#define ID_PROGBAR_0  (GUI_ID_PROGBAR0 + 0x00)
#define ID_PROGBAR_1  (GUI_ID_PROGBAR0 + 0x01)
#define ID_PROGBAR_2  (GUI_ID_PROGBAR0 + 0x02)

#define ID_GRAPH_0  (GUI_ID_GRAPH0 + 0x00)
#define ID_GRAPH_1  (GUI_ID_GRAPH0 + 0x01)
#define ID_GRAPH_2  (GUI_ID_GRAPH0 + 0x02)
#define ID_GRAPH_3  (GUI_ID_GRAPH0 + 0x03)

const GUI_WIDGET_CREATE_INFO _aDialogCreate1[] =
{
  { WINDOW_CreateIndirect, "Dialog", ID_FRAMEWIN_0, 0, 0, 250, 270, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Text",   ID_TEXT_0, 5, 5, 82, 17, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Memory", ID_TEXT_1, 6, 91, 90, 20, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Networking", ID_TEXT_2, 6, 180, 90, 20, 0, 0x64, 0 },
  
  { PROGBAR_CreateIndirect, "P0", ID_PROGBAR_0, 8, 21, 50, 63, 1, 0x0, 0 },
  { GRAPH_CreateIndirect, "G0", ID_GRAPH_0, 80, 21, 135, 65, 0, 0x0, 0 },

  { PROGBAR_CreateIndirect, "P1", ID_PROGBAR_1, 8, 111, 50, 63, 1, 0x0, 0 },
  { GRAPH_CreateIndirect, "G1", ID_GRAPH_1, 80, 110, 135, 65, 0, 0x0, 0 },
  
  { GRAPH_CreateIndirect, "G2", ID_GRAPH_2, 10 , 200, 100, 65, 0, 0x0, 0 },
  { GRAPH_CreateIndirect, "G3", ID_GRAPH_3, 120, 200, 100, 65, 0, 0x0, 0 },
};

static void _cbDialog(WM_MESSAGE * pMsg)
{
    WM_HWIN hItem;
    int     NCode;
    int     Id;
    char * buffer = rt_malloc(64);
    static GRAPH_DATA_Handle  _ahData[4]; 
    rt_uint8_t major, minor;
    rt_uint32_t  total, used, max_used;
    switch (pMsg->MsgId)
    {
        case WM_TIMER:
            cpu_usage_get(&major, &minor);
            rt_memory_info(&total, &used, &max_used);
            
            sprintf(buffer, "CPU :%d.%2d%%", major, minor);
            TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_0), buffer);
            PROGBAR_SetValue(WM_GetDialogItem(pMsg->hWin, ID_PROGBAR_0), major);
        
            sprintf(buffer, "Used mem:%d%%", used*100/total);
            TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_1), buffer);
            PROGBAR_SetValue(WM_GetDialogItem(pMsg->hWin, ID_PROGBAR_1), used*100/total);

            GRAPH_DATA_YT_AddValue(_ahData[0], major);
            GRAPH_DATA_YT_AddValue(_ahData[1], used*100/total);
        
            rt_device_t dev;
            enet_phy_data phy_data;
            dev = rt_device_find("e0");
            if(dev)
            {
                dev->control(dev, NIOCTL_GET_PHY_DATA, &phy_data);
                GRAPH_DATA_YT_AddValue(_ahData[2], phy_data.tx_fcnt);
                GRAPH_DATA_YT_AddValue(_ahData[3], phy_data.rx_fcnt);
            }
            WM_RestartTimer(pMsg->Data.v, 20);
            break;
        case WM_INIT_DIALOG:
            WM_CreateTimer(WM_GetClientWindow(pMsg->hWin), 0, 300, 0);
        
            hItem = WM_GetDialogItem(pMsg->hWin, ID_PROGBAR_0);
            PROGBAR_SetBarColor(hItem, 1, GUI_GREEN);
            PROGBAR_SetBarColor(hItem, 0, GUI_RED);
        
            hItem = WM_GetDialogItem(pMsg->hWin, ID_PROGBAR_1);
            PROGBAR_SetBarColor(hItem, 1, GUI_GREEN);
            PROGBAR_SetBarColor(hItem, 0, GUI_RED);
        
            // GRAPH
            _ahData[0] = GRAPH_DATA_YT_Create(GUI_DARKGREEN, 500, 0, 0);
            _ahData[1] = GRAPH_DATA_YT_Create(GUI_BLUE, 500, 0, 0);
            _ahData[2] = GRAPH_DATA_YT_Create(GUI_YELLOW, 500, 0, 0);
            _ahData[3] = GRAPH_DATA_YT_Create(GUI_BROWN, 500, 0, 0);
        
            WM_HWIN hGRAPH[4];
            int i;  
            hGRAPH[0] = WM_GetDialogItem(pMsg->hWin, ID_GRAPH_0);
            hGRAPH[1] = WM_GetDialogItem(pMsg->hWin, ID_GRAPH_1);
            hGRAPH[2] = WM_GetDialogItem(pMsg->hWin, ID_GRAPH_2);
            hGRAPH[3] = WM_GetDialogItem(pMsg->hWin, ID_GRAPH_3);
            for(i=0;i<GUI_COUNTOF(hGRAPH);i++)
            {
                GRAPH_SetBorder(hGRAPH[i], 1, 1, 1, 1);
                GRAPH_SetGridVis(hGRAPH[i], 1);
                GRAPH_SetGridDistX(hGRAPH[i], 10);
                GRAPH_SetGridDistY(hGRAPH[i], 10); 
                GRAPH_AttachData(hGRAPH[i], _ahData[i]);                
            }
        break;
        default:
            WM_DefaultProc(pMsg);
        break;
    }
    rt_free(buffer);
}

WM_HWIN _TaskerAddPage1(WM_HWIN hWin)
{
    return GUI_CreateDialogBox(_aDialogCreate1, GUI_COUNTOF(_aDialogCreate1), _cbDialog, WM_UNATTACHED, 0, 0);
}



