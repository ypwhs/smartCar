/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2014  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.26 - Graphical user interface for embedded applications **
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
File        : GUIDEMO.h
Purpose     : Configuration file of GUIDemo
----------------------------------------------------------------------
*/

#ifndef GUIDEMO_H
#define GUIDEMO_H

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

#ifdef _RTE_
#include "RTE_Components.h"
#endif
#include "GUI.h"

#if GUI_WINSUPPORT
  #include "WM.h"

  #include "CHECKBOX.h"
  #include "FRAMEWIN.h"
  #include "PROGBAR.h"
  #include "TEXT.h"
  #include "BUTTON.h"
  #include "SLIDER.h"
  #include "HEADER.h"
  #include "GRAPH.h"
  #include "ICONVIEW.h"
  #include "LISTVIEW.h"
  #include "TREEVIEW.h"
#else
  #include <stdlib.h>  // Definition of NULL
#endif

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define CONTROL_SIZE_X    80
#define CONTROL_SIZE_Y    61
#define INFO_SIZE_Y       65
#define BUTTON_SIZE_X     32
#define BUTTON_SIZE_Y     20
#define PROGBAR_SIZE_X    66
#define PROGBAR_SIZE_Y    12
#define TEXT_SIZE_X       69
#define TEXT_SIZE_Y       7
#define SHOW_PROGBAR_AT   100
#define GUI_ID_HALT       (GUI_ID_USER + 0)
#define GUI_ID_NEXT       (GUI_ID_USER + 1)
#define BK_COLOR_0        0xFF5555
#define BK_COLOR_1        0x880000
#define NUMBYTES_NEEDED   0x200000UL
#define CIRCLE_RADIUS     100
#define LOGO_DIST_BORDER  5
#define CHAR_READING_TIME 80
#define XSIZE_MIN         320
#define YSIZE_MIN         240

//
// Use an or-combination of the following flags to configure the
// GUIDemo container for the current application.
//
#define GUIDEMO_SHOW_CURSOR  (1 << 0)
#define GUIDEMO_SHOW_INFO    (1 << 1)
#define GUIDEMO_SHOW_CONTROL (1 << 2)

#define SHIFT_RIGHT_16(x)    ((x) / 65536)

/*********************************************************************
*
*       Configuration of modules to be used
*
**********************************************************************
*/


#define   SHOW_GUIDEMO_AATEXT            (1)
#define   SHOW_GUIDEMO_AUTOMOTIVE        (1)
#define   SHOW_GUIDEMO_BARGRAPH          (1)
#define   SHOW_GUIDEMO_BITMAP            (0)
#define   SHOW_GUIDEMO_COLORBAR          (1)
#define   SHOW_GUIDEMO_CURSOR            (1)
#define   SHOW_GUIDEMO_FADING            (0)
#define   SHOW_GUIDEMO_GRAPH             (1)
#define   SHOW_GUIDEMO_ICONVIEW          (0)
#define   SHOW_GUIDEMO_IMAGEFLOW         (0)
#define   SHOW_GUIDEMO_LISTVIEW          (0)
#define   SHOW_GUIDEMO_RADIALMENU        (0)
#define   SHOW_GUIDEMO_SKINNING          (0)
#define   SHOW_GUIDEMO_SPEED             (0)
#define   SHOW_GUIDEMO_SPEEDOMETER       (0)
#define   SHOW_GUIDEMO_TRANSPARENTDIALOG (0)
#ifdef    RTE_Graphics_Demo_Treeview
#define   SHOW_GUIDEMO_TREEVIEW          (0)
#endif
#ifdef    RTE_Graphics_Demo_VScreen
#define   SHOW_GUIDEMO_VSCREEN           (0)
#endif
#ifdef    RTE_Graphics_Demo_WashingMachine
#define   SHOW_GUIDEMO_WASHINGMACHINE    (0)
#endif
#ifdef    RTE_Graphics_Demo_ZoomAndRotate
#define   SHOW_GUIDEMO_ZOOMANDROTATE     (0)
#endif
#ifndef   SHOW_GUIDEMO_AATEXT
  #define SHOW_GUIDEMO_AATEXT            (0)
#endif
#ifndef   SHOW_GUIDEMO_AUTOMOTIVE
  #define SHOW_GUIDEMO_AUTOMOTIVE        (0)
#endif
#ifndef   SHOW_GUIDEMO_BARGRAPH
  #define SHOW_GUIDEMO_BARGRAPH          (0)
#endif
#ifndef   SHOW_GUIDEMO_BITMAP
  #define SHOW_GUIDEMO_BITMAP            (0)
#endif
#ifndef   SHOW_GUIDEMO_COLORBAR
  #define SHOW_GUIDEMO_COLORBAR          (0)
#endif
#ifndef   SHOW_GUIDEMO_CURSOR
  #define SHOW_GUIDEMO_CURSOR            (1)
#endif
#ifndef   SHOW_GUIDEMO_FADING
  #define SHOW_GUIDEMO_FADING            (0)
#endif
#ifndef   SHOW_GUIDEMO_GRAPH
  #define SHOW_GUIDEMO_GRAPH             (0)
#endif
#ifndef   SHOW_GUIDEMO_ICONVIEW
  #define SHOW_GUIDEMO_ICONVIEW          (0)
#endif
#ifndef   SHOW_GUIDEMO_IMAGEFLOW
  #define SHOW_GUIDEMO_IMAGEFLOW         (0)
#endif
#ifndef   SHOW_GUIDEMO_LISTVIEW
  #define SHOW_GUIDEMO_LISTVIEW          (0)
#endif
#ifndef   SHOW_GUIDEMO_RADIALMENU
  #define SHOW_GUIDEMO_RADIALMENU        (0)
#endif
#ifndef   SHOW_GUIDEMO_SKINNING
  #define SHOW_GUIDEMO_SKINNING          (0)
#endif
#ifndef   SHOW_GUIDEMO_SPEED
  #define SHOW_GUIDEMO_SPEED             (0)
#endif
#ifndef   SHOW_GUIDEMO_SPEEDOMETER
  #define SHOW_GUIDEMO_SPEEDOMETER       (0)
#endif

  #define SHOW_GUIDEMO_TRANSPARENTDIALOG (0)
  #define SHOW_GUIDEMO_TREEVIEW          (0)
  #define SHOW_GUIDEMO_VSCREEN           (0)
  #define SHOW_GUIDEMO_WASHINGMACHINE    (0)
  #define SHOW_GUIDEMO_ZOOMANDROTATE     (0)


/*********************************************************************
*
*       Configuration macros
*
**********************************************************************
*/
#ifndef   GUIDEMO_SHOW_SPRITES
  #define GUIDEMO_SHOW_SPRITES    (1)
#endif
#ifndef   GUIDEMO_USE_VNC
  #define GUIDEMO_USE_VNC         (0)
#endif
#ifndef   GUIDEMO_USE_AUTO_BK
  #define GUIDEMO_USE_AUTO_BK     (1)
#endif
#ifndef   GUIDEMO_SUPPORT_TOUCH
  #define GUIDEMO_SUPPORT_TOUCH   (1)
#endif
#ifndef   GUIDEMO_SUPPORT_CURSOR
  #define GUIDEMO_SUPPORT_CURSOR  (1)
#endif

#ifndef   GUIDEMO_CF_SHOW_SPRITES
  #define GUIDEMO_CF_SHOW_SPRITES   (GUIDEMO_SHOW_SPRITES                   <<  0)
#endif
#ifndef   GUIDEMO_CF_USE_VNC
  #define GUIDEMO_CF_USE_VNC        (GUIDEMO_USE_VNC                        <<  1)
#endif
#ifndef   GUIDEMO_CF_USE_AUTO_BK
  #define GUIDEMO_CF_USE_AUTO_BK    (GUIDEMO_USE_AUTO_BK                    <<  2)
#endif
#ifndef   GUIDEMO_CF_SUPPORT_TOUCH
  #define GUIDEMO_CF_SUPPORT_TOUCH  (GUI_WINSUPPORT ? GUIDEMO_SUPPORT_TOUCH <<  3 : 0)
#endif

/*********************************************************************
*
*       GUIDEMO_CONFIG
*/
typedef struct GUIDEMO_CONFIG {
  void (* * apFunc)(void);
  int       NumDemos;
  U16       Flags;
  #if GUIDEMO_USE_VNC
    int  (* pGUI_VNC_X_StartServer)(int LayerIndex, int ServerIndex);
  #endif
} GUIDEMO_CONFIG;

#if (GUI_WINSUPPORT == 0)
  #define GUIDEMO_NotifyStartNext  GUIDEMO_ClearHalt
  #define GUIDEMO_Delay            GUI_Delay
#else
  void    GUIDEMO_NotifyStartNext  (void);
  void    GUIDEMO_Delay            (int Time);
#endif

/*********************************************************************
*
*       Internal functions
*
**********************************************************************
*/
void      GUIDEMO_AddIntToString   (char * pText, U32 Number);
void      GUIDEMO_AddStringToString(char * pText, const char * pAdd);
int       GUIDEMO_CheckCancel      (void);
int       GUIDEMO_CheckCancelDelay (int Delay);
void      GUIDEMO_ClearHalt        (void);
void      GUIDEMO_ClearText        (char * pText);
void      GUIDEMO_Config           (GUIDEMO_CONFIG * pConfig);
void      GUIDEMO_ConfigureDemo    (char * pTitle, char * pDescription, unsigned Flags);
void      GUIDEMO_DispHint         (char * pHint);
void      GUIDEMO_DispTitle        (char * pTitle);
void      GUIDEMO_DrawBk           (void);
U16       GUIDEMO_GetConfFlag      (U16 Flag);
int       GUIDEMO_GetTime          (void);
int       GUIDEMO_GetTitleSizeY    (void);
void      GUIDEMO_HideCursor       (void);
void      GUIDEMO_Intro            (void);
void      GUIDEMO_Main             (void);
GUI_COLOR GUIDEMO_MixColors        (GUI_COLOR Color0, GUI_COLOR Color1, U8 Intens);
void      GUIDEMO_SetInfoText      (const char * pInfo);
int       GUIDEMO_ShiftRight       (int Value, U8 NumBits);
void      GUIDEMO_ShowCursor       (void);
void      GUIDEMO_Wait             (int TimeWait);

/*********************************************************************
*
*       Demo modules
*
**********************************************************************
*/
void GUIDEMO_AntialiasedText  (void);
void GUIDEMO_Automotive       (void);
void GUIDEMO_BarGraph         (void);
void GUIDEMO_Bitmap           (void);
void GUIDEMO_ColorBar         (void);
void GUIDEMO_Cursor           (void);
void GUIDEMO_Fading           (void);
void GUIDEMO_Graph            (void);
void GUIDEMO_IconView         (void);
void GUIDEMO_ImageFlow        (void);
void GUIDEMO_Listview         (void);
void GUIDEMO_RadialMenu       (void);
void GUIDEMO_Skinning         (void);
void GUIDEMO_Speed            (void);
void GUIDEMO_Speedometer      (void);
void GUIDEMO_TransparentDialog(void);
void GUIDEMO_Treeview         (void);
void GUIDEMO_VScreen          (void);
void GUIDEMO_WashingMachine   (void);
void GUIDEMO_ZoomAndRotate    (void);

/*********************************************************************
*
*       Externs
*
**********************************************************************
*/
extern GUI_CONST_STORAGE GUI_BITMAP bmSeggerLogo;
extern GUI_CONST_STORAGE GUI_BITMAP bmSeggerLogo70x35;

extern GUI_CONST_STORAGE GUI_FONT   GUI_FontRounded16;
extern GUI_CONST_STORAGE GUI_FONT   GUI_FontRounded22;
extern GUI_CONST_STORAGE GUI_FONT   GUI_FontSouvenir18;
extern GUI_CONST_STORAGE GUI_FONT   GUI_FontD6x8;
extern GUI_CONST_STORAGE GUI_FONT   GUI_FontAA2_32;
extern GUI_CONST_STORAGE GUI_FONT   GUI_FontAA4_32;


#if defined(__cplusplus)
  }
#endif

#endif // Avoid multiple inclusion

/*************************** End of file ****************************/
