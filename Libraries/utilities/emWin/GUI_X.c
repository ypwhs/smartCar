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
---Author-Explanation
* 
* 1.00.00 020519 JJL    First release of uC/GUI to uC/OS-II interface
* 
*
* Known problems or limitations with current version
*
*    None.
*
*
* Open issues
*
*    None
*********************************************************************************************************
*/

#include "GUI_Private.H"
#include <stdio.h>
#include <rtthread.h>


GUI_TIMER_TIME GUI_X_GetTime (void) 
{
    return (rt_tick_get()*(1000/RT_TICK_PER_SECOND));
}

void  GUI_X_Delay (int ms) 
{
    rt_thread_delay(ms);
}


/*
*********************************************************************************************************
*                                          GUI_X_ExecIdle()
*********************************************************************************************************
*/
void GUI_X_ExecIdle (void) 
{
    GUI_X_Delay(1);
}


static rt_mutex_t gui_x_mutex = RT_NULL;

void  GUI_X_InitOS (void)
{ 
    gui_x_mutex = rt_mutex_create("gui_x_mutex", RT_IPC_FLAG_FIFO);
    if (gui_x_mutex == RT_NULL)
    {
        rt_kprintf("gui_x_mutex create failed\r\n");
        return;
    }
}

static U32 i;
void GUI_X_Lock (void)
{ 
    //rt_mutex_take(gui_x_mutex, RT_WAITING_FOREVER);
    //rt_kprintf("%d\r\n",i);
    i++;
}


void GUI_X_Unlock (void)
{ 
    i--;
    //rt_mutex_release(gui_x_mutex);
}


U32 GUI_X_GetTaskId (void) 
{
    return 1;
}


static rt_device_t touch_device;

#define SAMP_CNT 4
#define SAMP_CNT_DIV2 2
static int buf[2];
/* ÂË²¨ */
static int ads_filter(int* buf)
{
    int i, j, k, min;
    int temp;
    int tempXY[2][SAMP_CNT];
    rt_uint8_t buf1[2];
    for(i=0; i<SAMP_CNT; i++)
    {
        if(touch_device != RT_NULL)
        {
            touch_device->read(touch_device, 0, buf1, 2);
            tempXY[0][i] = ((buf1[0]<<8) + buf1[1])>>4; //12bit mode
            touch_device->read(touch_device, 1, buf1, 2);
            tempXY[1][i] = ((buf1[0]<<8) + buf1[1])>>4; //12bit mode
        }
    }
    for(k=0; k<2; k++)
    {
        for(i=0; i<SAMP_CNT-1; i++)
        {
            min=i;
            for (j=i+1; j<SAMP_CNT; j++)
            {
                if (tempXY[k][min] > tempXY[k][j]) min=j;
            }
            temp = tempXY[k][i];
            tempXY[k][i] = tempXY[k][min];
            tempXY[k][min] = temp;
        }
        if((tempXY[k][SAMP_CNT_DIV2]-tempXY[k][SAMP_CNT_DIV2-1]) > 5)
        return 1;
        buf[k] = (tempXY[k][SAMP_CNT_DIV2]+tempXY[k][SAMP_CNT_DIV2-1]) / 2;
    }
    return 0;
}


void GUI_TOUCH_X_ActivateX(void)  {}
void GUI_TOUCH_X_ActivateY(void)  {}


int GUI_TOUCH_X_MeasureX(void) 
{
    ads_filter(buf);
    return buf[0];
}


int GUI_TOUCH_X_MeasureY(void) 
{
    return buf[1];
}

void GUI_X_ErrorOut(const char * s)
{
    GUI_DispString(s);
}
void GUI_X_Warn(const char * s)
{
    GUI_DispString(s);
}
void GUI_X_Log(const char * s)
{
    GUI_DispString(s);
}

/*
*********************************************************************************************************
*                                        GUI_X_WaitEvent()
*                                        GUI_X_SignalEvent()
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                      KEYBOARD INTERFACE FUNCTIONS
*
* Purpose: The keyboard routines are required only by some widgets.
*          If widgets are not used, they may be eliminated.
*
* Note(s): If uC/OS-II is used, characters typed into the log window will be placed	in the keyboard buffer. 
*          This is a neat feature which allows you to operate your target system without having to use or 
*          even to have a keyboard connected to it. (useful for demos !)
*********************************************************************************************************
*/

static  void  CheckInit (void) 
{
    GUI_X_Init();
}


void GUI_X_Init (void) 
{
    touch_device = rt_device_find("ads7843");
  //  KeySem = OSSemCreate(0);
}
