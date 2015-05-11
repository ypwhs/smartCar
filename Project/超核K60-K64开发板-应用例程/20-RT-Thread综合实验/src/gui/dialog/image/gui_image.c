#include "gui_appdef.h"


#define GUI_IMAGE_BUF_SIZE   (2048)

#define ID_WINDOW_0         (GUI_ID_USER + 0x00)
#define ID_IMAGE_0          (GUI_ID_USER + 0x04)

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 0, 0, 240, 320, 0, 0x0, 0 },
    { IMAGE_CreateIndirect, "Image", ID_IMAGE_0, 0, 0, 100, 100, 0, IMAGE_CF_MEMDEV | IMAGE_CF_ATTACHED | IMAGE_CF_TILE | IMAGE_CF_ALPHA, 0 },
};

static unsigned char *gImageBuf = RT_NULL;
static const char *gPath;

int _GetData(void * p, const U8 ** ppData, unsigned NumBytes, U32 Off)
{
    int hFile;
    int NumBytesRead;
    hFile = open(gPath, O_RDONLY , 0);
    if(hFile < 0)
    {
        rt_kprintf("open failed\r\n");
        return NumBytes;
    }
    if (NumBytes >GUI_IMAGE_BUF_SIZE) NumBytes = GUI_IMAGE_BUF_SIZE;
    lseek(hFile, Off, SEEK_SET);
    NumBytesRead = read(hFile, gImageBuf, NumBytes);
    close(hFile);
    if(NumBytesRead == -1)
    {
        rt_kprintf("read failed\r\n");
        return NumBytes;
    }
    *ppData = gImageBuf;
    return NumBytesRead;
}


static int _DispImage(WM_HWIN Handle, const char *path)
{
    int hFile;
    hFile = open(path, O_RDONLY , 0);
    if(hFile < 0)
    {
        rt_kprintf("open failed\r\n");
        return RT_EIO;
    }
    close(hFile);

    gImageBuf = rt_malloc(GUI_IMAGE_BUF_SIZE);
    if(!gImageBuf)
    {
        return RT_ENOMEM;
    }
    
    switch(GUI_AppGetFileType(path))
    {
        case kBMP:
            IMAGE_SetBMPEx(Handle, _GetData, (void*)&hFile);
            break;
        case kJPG:
        case kJPEG:
                IMAGE_SetJPEGEx(Handle, _GetData, (void*)&hFile);
            break;
        default:
            rt_kprintf("unknown type:%s\r\n", path);
            break;
    }
    return 0;
}

static void _cbDialog(WM_MESSAGE * pMsg)
{
    WM_HWIN      hItem;
    switch (pMsg->MsgId)
    {
        case WM_INIT_DIALOG:
            hItem = pMsg->hWin;
            WM_MakeModal(hItem);
            WINDOW_SetBkColor(hItem, 0x00EDEDE9);
        
            hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_0);
            _DispImage(hItem, gPath);
            GUI_CURSOR_Hide();
            break;
        case WM_NOTIFY_PARENT:
            GUI_CURSOR_Show();
            GUI_EndDialog(pMsg->hWin, 0);
        default:
            WM_DefaultProc(pMsg);
        break;
    }
}

void GUI_AppDispImage(const char * path)
{
    gPath = path;
    GUI_ExecDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
    return;
}


#ifdef FINSH_USING_MSH
#include "finsh.h"
extern char working_directory[];

int ui_image(int argc, char** argv)
{
    gui_msg_t msg;
    char *fullpath;
    if(argc != 2)
    {
        return -1;
    }
     
    fullpath = dfs_normalize_path(NULL, argv[1]);

    msg.cmd = 2;
    msg.exec = GUI_AppDispImage;
    msg.parameter = (void *)fullpath;
    rt_mq_send(guimq, &msg, sizeof(msg));
    return 0;
}

MSH_CMD_EXPORT(ui_image, eg: ui_image /SF/A.BMP.);
#endif
