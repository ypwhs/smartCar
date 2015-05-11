/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2013  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.22 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The software has been licensed to  ARM LIMITED whose registered office
is situated at  110 Fulbourn Road,  Cambridge CB1 9NJ,  England solely
for  the  purposes  of  creating  libraries  for  ARM7, ARM9, Cortex-M
series,  and   Cortex-R4   processor-based  devices,  sublicensed  and
distributed as part of the  MDK-ARM  Professional  under the terms and
conditions  of  the   End  User  License  supplied  with  the  MDK-ARM
Professional. 
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : GUIConf.c
Purpose     : Display controller initialization
---------------------------END-OF-HEADER------------------------------
*/

#include "GUI.h"
#include <rtthread.h>
#include "sram.h"

#define GUI_NUMBYTES  (1024*32)
#define GUI_BLOCKSIZE 4

static void GUI_Log(const char *s)
{
    rt_kprintf("GUI error:%s\r\n", s);
}

void GUI_X_Config(void)
{
    U32 * aMemory = (U32*)rt_malloc(GUI_NUMBYTES);
    if(aMemory == RT_NULL)
    {
        rt_kprintf("not enough mem for GUI!\r\n");
        return;
    }
    GUI_ALLOC_AssignMemory(aMemory, GUI_NUMBYTES);
    GUI_SetOnErrorFunc(GUI_Log);
    GUITASK_SetMaxTask(5);
    GUI_ALLOC_SetAvBlockSize(GUI_BLOCKSIZE);
    GUI_SetDefaultFont(GUI_FONT_6X8);

}

/*************************** End of file ****************************/
