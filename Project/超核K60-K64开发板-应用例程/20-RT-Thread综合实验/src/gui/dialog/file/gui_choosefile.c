
#include "gui_appdef.h"


#define _MAX_PATH 256
static char fullpath[_MAX_PATH];


static int _GetData(CHOOSEFILE_INFO * pInfo)
{
    int         r;
    static DIR *dir;
    r = 1;
    struct dirent* dirent;
    struct stat s;
    
    switch (pInfo->Cmd)
    {
        case CHOOSEFILE_FINDFIRST:
                dir = opendir(pInfo->pRoot);
                pInfo->pName = RT_NULL;
                pInfo->pAttrib = RT_NULL;
                pInfo->SizeH = 0;
                pInfo->SizeL = 0;
                return 0;
        case CHOOSEFILE_FINDNEXT:
                if(dir == RT_NULL)
                {
                    //rt_kprintf("opendir error\r\n");
                    r = 1;
                    break;
                }
                dirent = readdir(dir);
                if(dirent == RT_NULL)
                {
                    //rt_kprintf("readdir error\r\n");
                    closedir(dir);
                    r = 1;
                    break;
                }
                
                /* build full path for each file */
                rt_sprintf(fullpath, "%s/%s", pInfo->pRoot, dirent->d_name);
                rt_memset(&s, 0, sizeof(struct stat));
                stat(fullpath, &s);
                if (s.st_mode & DFS_S_IFDIR)
                {
                    pInfo->Flags = CHOOSEFILE_FLAG_DIRECTORY;
                }
                else
                {
                    pInfo->Flags = 0;
                }
                r = 0;
            break;
    }
    
    if(r == 0)
    {
        pInfo->pName = dirent->d_name;
        
        if(s.st_mode & DFS_S_IFDIR)
        {
         //   pInfo->pExt = "";
            pInfo->pAttrib = "DIR";
        }
        else
        {
          //  pInfo->pExt = rt_strstr(fullpath, ".") + 1;
            pInfo->pAttrib = "R";
        }
        pInfo->SizeL   = s.st_size;
//        pInfo->SizeH   = s.st_size/0xFFFF;
        
    }
    return r;
}


const char *GUI_AooDispChooseFile(void)
{
    int r;
    WM_HWIN hWin;
    static CHOOSEFILE_INFO Info;
    
    char              acDir[32]    = "/";
    char const      * apDrives[1]         = { acDir };
    
    rt_memset(&Info, 0, sizeof(CHOOSEFILE_INFO));
    Info.pfGetData = _GetData;
    Info.pMask     = "*.*";
    CHOOSEFILE_SetDelim('/');
    rt_enter_critical();
    hWin = CHOOSEFILE_Create(WM_HBKWIN, 0, 0, LCD_GetXSize(), LCD_GetYSize(), apDrives, GUI_COUNTOF(apDrives), 0, "File Dialog", 0, &Info);
    rt_exit_critical();
    WM_MakeModal(hWin);
    //FRAMEWIN_SetMoveable(hWin, 1);
    r = GUI_ExecCreatedDialog(hWin);
    if(!r)
    {
        gui_msg_t msg;
        switch(GUI_AppGetFileType(Info.pRoot))
        {
            case kJPEG:
            case kBMP:
            case kJPG:
                
                msg.cmd = 2;
                msg.exec = GUI_AppDispImage;
                msg.parameter = Info.pRoot;
                rt_mq_send(guimq, &msg, sizeof(msg));
                break;
            case kTXT:
            case kINI:
            case kC:  
            case kLOG:                
            case kMD:
                msg.cmd = 2;
                msg.exec = GUI_AppNotepad;
                msg.parameter = Info.pRoot;
                rt_mq_send(guimq, &msg, sizeof(msg));
            default:     
                GUI_MessageBox(Info.pRoot, "File", GUI_MESSAGEBOX_CF_MODAL);
                break;
        }
    }
    return Info.pRoot;
}


