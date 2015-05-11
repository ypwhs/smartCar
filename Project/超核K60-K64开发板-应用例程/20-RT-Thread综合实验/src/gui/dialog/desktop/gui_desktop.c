#include "gui_appdef.h"


#define ID_WINDOW_0     (GUI_ID_USER + 0x01)
#define ID_FRAMEWIN_0   (GUI_ID_USER + 0x00)
#define ID_ICONVIEW_0   (GUI_ID_USER + 0x01)
#define ID_BUTTON_0     (GUI_ID_USER + 0x01)
#define ID_BUTTON_1     (GUI_ID_USER + 0x02)
#define ID_BUTTON_2     (GUI_ID_USER + 0x03)
#define ID_BUTTON_3     (GUI_ID_USER + 0x04)
#define ID_BUTTON_4     (GUI_ID_USER + 0x05)
#define ID_BUTTON_5     (GUI_ID_USER + 0x06)
#define ID_BUTTON_6     (GUI_ID_USER + 0x07)
#define ID_BUTTON_7     (GUI_ID_USER + 0x08)
#define ID_BUTTON_8     (GUI_ID_USER + 0x09)
#define ID_BUTTON_9     (GUI_ID_USER + 0x0A)
#define ID_BUTTON_10    (GUI_ID_USER + 0x0B)
#define ID_BUTTON_11    (GUI_ID_USER + 0x0C)
#define ID_BUTTON_12    (GUI_ID_USER + 0x0D)
#define ID_IMAGE_0      (GUI_ID_USER + 0x04)


static UIAppEntry UIApp[] = 
{
    {"about",    "about",     "/SD/SYS/APPS/ABOUT.BMP", RT_NULL, 10, 10, 50, 50,         ID_BUTTON_0, GUI_AppDispLED},
    {"calendar",      "calendar",       "/SD/SYS/APPS/CALENDER.BMP", RT_NULL, 10, 10, 50, 50,         ID_BUTTON_1, GUI_AppDispSysInfo},
    {"clock",        "clock",         "/SD/SYS/APPS/CLOCK.BMP", RT_NULL, 10, 10, 50, 50,         ID_BUTTON_2, GUI_AooDispChooseFile},
    {"settings",       "settings",        "/SD/SYS/APPS/CONFIG.BMP", RT_NULL, 10, 10, 50, 50,         ID_BUTTON_3, GUI_AppDispTime},
    {"network",     "network",  "/SD/SYS/APPS/LAN.BMP", RT_NULL, 10, 10, 50, 50,         ID_BUTTON_4, GUI_ExecCalibrationDialog},
    {"file",      "file",       "/SD/SYS/APPS/FILE.BMP", RT_NULL, 10, 10, 50, 50,         ID_BUTTON_5, GUI_AooDispChooseFile},
    {"music",      "music",       "/SD/SYS/APPS/MUSIC.BMP", RT_NULL, 10, 10, 50, 50,         ID_BUTTON_6, GUI_CreateTaskerDialog},
    {"notepad",     "notepad",       "/SD/SYS/APPS/NOTEPAD.BMP", RT_NULL, 10, 10, 50, 50,         ID_BUTTON_7, GUI_CreateTaskerDialog},
    {"picture",      "picture",       "/SD/SYS/APPS/PICTURE.BMP", RT_NULL, 10, 10, 50, 50,         ID_BUTTON_8, GUI_CreateTaskerDialog},
    {"notepad",      "notepad",       "/SD/SYS/APPS/NOTEPAD.BMP", RT_NULL, 10, 10, 50, 50,         ID_BUTTON_9, GUI_CreateTaskerDialog},
    {"usb",      "usb",       "/SD/SYS/APPS/USB.BMP", RT_NULL, 10, 10, 50, 50,         ID_BUTTON_10, GUI_CreateTaskerDialog},
    {"performence",      "performence",       "/SD/SYS/APPS/PERFOR~1.BMP", RT_NULL, 10, 10, 50, 50,         ID_BUTTON_11, GUI_CreateTaskerDialog},
};


static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 0, 0, 240, 400, 0, 0x0, 0 },
//    { IMAGE_CreateIndirect, "Image", ID_IMAGE_0, 0, 0, 240, 320, 0, IMAGE_CF_MEMDEV | IMAGE_CF_ATTACHED | IMAGE_CF_TILE | IMAGE_CF_ALPHA, 0 },
};



static void _cbDialog(WM_MESSAGE * pMsg) {
    int i;
    WM_HWIN hItem;
    int     NCode;
    int     Id;
    gui_msg_t msg;
    msg.exec = RT_NULL;
    
    switch (pMsg->MsgId)
    {
        case WM_INIT_DIALOG:
        hItem = pMsg->hWin;
        WM_EnableMemdev(hItem);
        //WM_MoveTo(hItem, 0, -30);
        //WM_MOTION_SetMoveable(hItem, WM_CF_MOTION_Y, 1);
        WINDOW_SetBkColor(hItem, GUI_WHITE);
        int fd;
        

        /* load APP */
        for(i=0;i<GUI_COUNTOF(UIApp);i++)
        {
            hItem = BUTTON_CreateAsChild(10, 10, 50, 50, pMsg->hWin, UIApp[i].GUID, WM_CF_SHOW);
            BUTTON_SetText(hItem, UIApp[i].text);
            WM_EnableMemdev(hItem);
            struct stat s;
            stat(UIApp[i].logoPath, &s);
            
            UIApp[i].plogo = rt_malloc(s.st_size);
            if(UIApp[i].plogo)
            {
                fd = open(UIApp[i].logoPath, O_RDONLY , 0);
                if(fd >=0)
                {
                    rt_kprintf("%s", UIApp[i].logoPath);
                    read(fd, UIApp[i].plogo, s.st_size);

                    
                    //WM_MoveTo(hItem, 0+(i%2)*120, (i/2)*120 + 0);
                    
                    WM_SetSize(hItem, GUI_BMP_GetXSize(UIApp[i].plogo), GUI_BMP_GetYSize(UIApp[i].plogo));
                    //xPos+= xSize;
                    //yPos+= ySize;
                    
                    
                    BUTTON_SetBMPEx(hItem, BUTTON_BI_UNPRESSED, UIApp[i].plogo ,0, 0);
                    BUTTON_SetBMPEx(hItem, BUTTON_BI_PRESSED, UIApp[i].plogo ,1, 1);
                    BUTTON_SetTextOffset(hItem, 0, 50);
                    close(fd);
                }
            }
            else
            {
                rt_kprintf("no mem!\r\n");
            }
        }
        
        /* algin Apps */
        int xPos, yPos, xSize, ySize;
        xPos = 0; xSize = 0;
        yPos = 0; ySize = 0;
        for(i=0;i<GUI_COUNTOF(UIApp);i++)
        {
            hItem = WM_GetDialogItem(pMsg->hWin, UIApp[i].GUID);
            xSize = GUI_BMP_GetXSize(UIApp[i].plogo);
            ySize = GUI_BMP_GetYSize(UIApp[i].plogo);
            
            if(xSize == 0 || xSize > 9999)
            {
                xSize = 60;
            }
            if(ySize == 0 || ySize > 9999)
            {
                ySize = 60;
            }
            
            if(abs(LCD_GetXSize() - xPos) >= xSize)
            {
                WM_MoveTo(hItem, xPos, yPos);
                xPos+=xSize;
            }
            else
            {
                xPos = 0;
                yPos+=ySize;
                WM_MoveTo(hItem, xPos, yPos);
                xPos+=xSize; 
            }
        }
    
//        char *p;
//        p = rt_malloc(38399);
//        fd = open("/SD/DESKTOP.JPG",O_RDONLY , 0);
//        read(fd, p, 38399);
//        hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_0);
//        WM_EnableMemdev(hItem);
      //  IMAGE_SetJPEG(hItem, p, 38399);
        break;
    case WM_NOTIFY_PARENT:
        Id = WM_GetId(pMsg->hWinSrc);
        NCode = pMsg->Data.v;
    if(NCode == WM_NOTIFICATION_RELEASED)
    {
        for(i=0;i<GUI_COUNTOF(UIApp);i++)
        {
            if(Id == UIApp[i].GUID)
            {
                msg.exec = UIApp[i].exec;
                break;
            }
        }
        if(msg.exec != RT_NULL)
        {
            rt_mq_send(guimq, &msg, sizeof(msg));
        }
    }
    break;
    default:
        WM_DefaultProc(pMsg);
        break;
    }
}

  
WM_HWIN GUI_CreateDesktopDialog(void)
{
    WM_HWIN  hWin;
    WM_MOTION_Enable(1);

    GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
}



