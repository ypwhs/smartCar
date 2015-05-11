#include "DIALOG.h"
#include <dfs_posix.h>
#include <stdbool.h>
#include "gui_appdef.h"


const char *FileNameTbl[] = {"JPEG", "JPG", "TXT", "C", "CPP", "LOG", "PNG", "MD", "INI", "DOC", "BMP", "AVI", "WMV", "BIN", "HEX"};



GUI_FileType GUI_AppGetFileType(const char * fileName)
{
    rt_uint8_t i;

    const char *p;
    p = rt_strstr(fileName, ".");
    for(i = 0; i < kFileNameMax; i++)
    {
        if(!rt_strcasecmp(p + 1, FileNameTbl[i]))
        {
            return i;
        }
    }
    return i;
}




