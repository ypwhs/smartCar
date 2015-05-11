#include "gui_appdef.h"


#define ID_FRAMEWIN_0       (GUI_ID_USER + 0x00)
#define ID_LISTVIEW_0       (GUI_ID_USER + 0x05)

extern struct rt_object_information rt_object_container[];
static  WM_HWIN hParent;

const GUI_WIDGET_CREATE_INFO _aDialogCreate2[] = {
    { WINDOW_CreateIndirect,    "Dialog 1", 0,                   0,   0, 250, 270, FRAMEWIN_CF_MOVEABLE },
    { LISTVIEW_CreateIndirect, "Listview", ID_LISTVIEW_0,        6,  10, 210, 230, 0, 0x0, 0 },
    { BUTTON_CreateIndirect, "Cancel", GUI_ID_BUTTON0,          150, 248, 60, 20, 0, 0x0, 0 },
};

static void _DispThread(WM_HWIN hItem)
{

    struct rt_list_node *list;
    struct rt_list_node *node;
    list = &rt_object_container[RT_Object_Class_Thread].object_list;
    struct rt_thread *thread;
    const char * ppText[4];
    char *sPriority;
    char *sStackSize;
    char *sStatus;
    sPriority = rt_malloc(16);
    sStackSize = rt_malloc(16);
    sStatus = rt_malloc(16);
    for (node = list->next; node != list; node = node->next)
    {
        thread = rt_list_entry(node, struct rt_thread, list);
        if (thread->stat == RT_THREAD_READY)        sStatus = "ready";
        else if (thread->stat == RT_THREAD_SUSPEND) sStatus = "suspend";
        else if (thread->stat == RT_THREAD_INIT)    sStatus = "init";
        else if (thread->stat == RT_THREAD_CLOSE)   sStatus = "close";
        
        sprintf(sPriority, "%d", thread->current_priority);
        sprintf(sStackSize, "%d", thread->stack_size);
        
        ppText[0] = thread->name;
        ppText[1] = sPriority;
        ppText[2] = sStackSize; 
        ppText[3] = sStatus;  
        LISTVIEW_AddRow(hItem, ppText);
    }
    rt_free(sPriority);
    rt_free(sStackSize);
    rt_free(sStatus);
}

void _cbDialog(WM_MESSAGE * pMsg)
{
  WM_HWIN hItem;
  int     NCode;
  int     Id;
    
  switch (pMsg->MsgId) {
    case WM_TIMER:
        hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0);
       // _DispThread(hItem);
        WM_RestartTimer(pMsg->Data.v, 200);
        break;
      
    case WM_INIT_DIALOG:
    WM_CreateTimer(WM_GetClientWindow(pMsg->hWin), 0, 100, 0);
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0);
    LISTVIEW_AddColumn(hItem, 60, "Thread", GUI_TA_HCENTER | GUI_TA_VCENTER);
    LISTVIEW_AddColumn(hItem, 40, "Priority", GUI_TA_HCENTER | GUI_TA_VCENTER);
    LISTVIEW_AddColumn(hItem, 40, "Stack", GUI_TA_HCENTER | GUI_TA_VCENTER);
    LISTVIEW_AddColumn(hItem, 50, "Status", GUI_TA_HCENTER | GUI_TA_VCENTER);
    LISTVIEW_SetGridVis(hItem, 1);
    _DispThread(hItem);
    
    break;
        case WM_NOTIFY_PARENT:
        Id    = WM_GetId(pMsg->hWinSrc);
        NCode = pMsg->Data.v;
        switch(Id) 
        {
            case GUI_ID_BUTTON0: // Notifications sent by 'Button'
            switch(NCode)
            {
                case WM_NOTIFICATION_RELEASED:
                    GUI_EndDialog(pMsg->hWin, 0);
                    GUI_EndDialog(hParent, 0);
                    
                    break;
            }
            break;
        }
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

WM_HWIN _TaskerAddPage2(WM_HWIN hWin)
{
    hParent = hWin;
    return GUI_CreateDialogBox(_aDialogCreate2, GUI_COUNTOF(_aDialogCreate2), _cbDialog, WM_UNATTACHED, 0, 0);
}

