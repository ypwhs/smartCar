#include "gui_appdef.h"


#define ID_FRAMEWIN_0 (GUI_ID_USER + 0x00)
#define ID_EDIT_0 (GUI_ID_USER + 0x07)
#define ID_MENU_0 (GUI_ID_USER + 0x0A)

static int fpic;
static const char *gpath;


static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_0, 0, 0, 240, 320, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "Edit", ID_EDIT_0, 0, 17, 235, 280, 0, 0x64, 0 },
  { MENU_CreateIndirect, "Menu", ID_MENU_0, 0, -1, 233, 19, 0, 0x0, 0 },
};


static int _DispText(WM_HWIN Handle, const char *path)
{
    char *buf;
    struct stat f_stat;
    fpic = open(path, O_RDONLY , 0);
    if(fpic <0)
    {
        rt_kprintf("open file failed\r\n");
        return 1;
    }
    stat(path, &f_stat);
    buf = rt_malloc(512);
    read(fpic, buf, 512);

    EDIT_SetText(Handle, buf);
    close(fpic);
}


static void _cbDialog(WM_MESSAGE * pMsg) {
  MENU_ITEM_DATA   ItemData;
  WM_HWIN          hItem;
  int              NCode;
  int              Id;
  // USER START (Optionally insert additional variables)
  // USER END

  switch (pMsg->MsgId)
    {
  case WM_INIT_DIALOG:
    hItem = pMsg->hWin;
    FRAMEWIN_SetText(hItem, "Notepade");
    WM_MakeModal(hItem);
    FRAMEWIN_AddCloseButton(hItem, FRAMEWIN_BUTTON_RIGHT, 0);
    FRAMEWIN_AddMaxButton(hItem, FRAMEWIN_BUTTON_RIGHT, 0);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0);
    EDIT_SetText(hItem, "");
    SCROLLBAR_CreateAttached(hItem, SCROLLBAR_CF_VERTICAL);
  
    _DispText(hItem, gpath);
  
    hItem = WM_GetDialogItem(pMsg->hWin, ID_MENU_0);
    ItemData.Flags    = 0;
    ItemData.hSubmenu = 0;
    ItemData.Id       = 0;
    ItemData.pText    = "File";
    MENU_AddItem(hItem, &ItemData);
    ItemData.Flags    = 0;
    ItemData.hSubmenu = 0;
    ItemData.Id       = 1;
    ItemData.pText    = "View";
    MENU_AddItem(hItem, &ItemData);
    ItemData.Flags    = 0;
    ItemData.hSubmenu = 0;
    ItemData.Id       = 2;
    ItemData.pText    = "Help";
    MENU_AddItem(hItem, &ItemData);

    
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id)
    {

    case ID_MENU_0: // Notifications sent by 'Menu'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:

        break;
      case WM_NOTIFICATION_RELEASED:

        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
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

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       Createsystem
*/

WM_HWIN GUI_AppNotepad(const char* path)
{
    gpath = path;
    GUI_ExecDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
}


