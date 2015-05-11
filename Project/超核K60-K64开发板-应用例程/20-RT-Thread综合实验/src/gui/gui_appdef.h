#ifndef __GUI_APPDEF_H__
#define __GUI_APPDEF_H__

#include <rtthread.h>
#include <finsh.h>
#include <dfs.h>
#include <dfs_posix.h>

#include <stdint.h>
#include <stdbool.h>

#include    "GUI.H"
#include    "WM.H"
#include    "DIALOG.h"
#include    "BUTTON.h"
#include    "EDIT.h"
#include    "FRAMEWIN.h"

typedef enum
{
    kJPEG,
    kJPG,
    kTXT,
    kC,
    kCPP,
    kLOG,
    kINI,
    kPNG,
    kMD,
    kDOC,
    kBMP,
    kAVI,
    kWMV,
    kBIN,
    kHEX,
    kFileNameMax,
}GUI_FileType;

extern rt_mq_t guimq;

typedef struct
{
    int cmd;
    void (*exec)(void *parameter);
    void *parameter;
}gui_msg_t;

typedef struct
{
    rt_uint8_t magic;
    int aPhysX[2];
    int aPhysY[2];
    int aLogX[2];
    int aLogY[2];
}GUI_TouchData;


typedef struct
{
    const char* name;
    const char* text;
    const char* logoPath;
    U8 * plogo;
    int x0;
    int y0;
    int xSize;
    int ySize;
    U32 GUID;
    void (*exec)(void *parameter);
}UIAppEntry;


//!< API below API should only call in GUI thread
void GUI_AppDispImage(const char * path);
void GUI_AppDispCalender(void);
void GUI_AppDispTime(void);
WM_HWIN GUI_AppNotepad(const char* path);
const char *GUI_AooDispChooseFile(void);
WM_HWIN GUI_AppDispSysInfo(void);
GUI_FileType GUI_AppGetFileType(const char * fileName);
void GUI_AppAutoCalibration(const char* path);
WM_HWIN GUI_CreateTaskerDialog(void);
WM_HWIN GUI_AppDispCalculator(void);
WM_HWIN GUI_ExecCalibrationDialog(GUI_TouchData* pTouchData);
WM_HWIN GUI_CreateDesktopDialog(void);
WM_HWIN GUI_AppDispLED(void);

#endif



