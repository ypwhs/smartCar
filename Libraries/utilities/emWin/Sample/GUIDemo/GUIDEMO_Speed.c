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
File        : GUIDEMO_Speed.c
Purpose     : Speed demo
----------------------------------------------------------------------
*/

#include "GUIDEMO.h"

#if (SHOW_GUIDEMO_SPEED)

#include <stdlib.h>  // rand()

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
static const GUI_COLOR _aColor[8] = {
  0x000000,
  0x0000FF,
  0x00FF00,
  0x00FFFF,
  0xFF0000,
  0xFF00FF,
  0xFFFF00,
  0xFFFFFF
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************
*
*       _GetPixelsPerSecond
*/
static U32 _GetPixelsPerSecond(void) {
  GUI_COLOR BkColor;
  GUI_COLOR Color;
  I32       PixelsPerSecond;
  I32       PixelCnt;
  I32       t0;
  I32       t;
  U32       xSize;
  U32       ySize;
  U32       Cnt;
  U32       x0;
  U32       x1;
  U32       y0;
  U32       y1;

  //
  // Find an area which is not obstructed by any windows
  //
  xSize   = LCD_GetXSize();
  ySize   = LCD_GetYSize();
  Cnt     = 0;
  x0      = 0;
  x1      = xSize - 1;
  y0      = 65;
  y1      = ySize - 60 - 1;
  Color   = GUI_GetColor();
  BkColor = GUI_GetBkColor();
  GUI_SetColor(BkColor);
  //
  // Repeat fill as often as possible in 100 ms
  //
  t0 = GUIDEMO_GetTime();
  do {
    GUI_FillRect(x0, y0, x1, y1);
    Cnt++;
    t = GUIDEMO_GetTime();
  } while ((t - (t0 + 100)) <= 0);
  //
  // Compute result
  //
  t -= t0;
  PixelCnt = (x1 - x0 + 1) * (y1 - y0 + 1) * Cnt;
  PixelsPerSecond = PixelCnt / t * 1000;
  GUI_SetColor(Color);
  return PixelsPerSecond;
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       GUIDEMO_Speed
*/
void GUIDEMO_Speed(void) {
  #if GUI_SUPPORT_TOUCH
    GUI_PID_STATE State;
  #endif
  GUI_RECT        ClipRect;
  GUI_RECT        Rect;
  char            acText[40] = { 0 };
  U32             PixelsPerSecond;
  int             aColorIndex[8];
  int             TimeStart;
  int             vySize;
  int             xSize;
  int             ySize;
  int             i;

  GUIDEMO_ConfigureDemo("High speed", "Multi layer clipping\nHighly optimized drivers", 0);
  xSize  = LCD_GetXSize();
  ySize  = LCD_GetYSize();
  vySize = LCD_GetVYSize();
  if (vySize > ySize) {
    ClipRect.x0 = 0;
    ClipRect.y0 = 0;
    ClipRect.x1 = xSize;
    ClipRect.y1 = ySize;
    GUI_SetClipRect(&ClipRect);
  }
  for (i = 0; i< 8; i++) {
    aColorIndex[i] = GUI_Color2Index(_aColor[i]);
  }
  TimeStart = GUIDEMO_GetTime();
  for (i = 0; ((GUIDEMO_GetTime() - TimeStart) < 5000) && (GUIDEMO_CheckCancel() == 0); i++) {
    GUI_SetColorIndex(aColorIndex[i&7]);
    //
    // Calculate random positions
    //
    Rect.x0 = rand() % xSize - xSize / 2;
    Rect.y0 = rand() % ySize - ySize / 2;
    Rect.x1 = Rect.x0 + 20 + rand() % xSize;
    Rect.y1 = Rect.y0 + 20 + rand() % ySize;
    GUI_FillRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1);
    //
    // Clip rectangle to visible area and add the number of pixels (for speed computation)
    //
    if (Rect.x1 >= xSize) {
      Rect.x1 = xSize - 1;
    }
    if (Rect.y1 >= ySize) {
      Rect.y1 = ySize - 1;
    }
    if (Rect.x0 < 0 ) {
      Rect.x0 = 0;
    }
    if (Rect.y1 < 0) {
      Rect.y1 = 0;
    }
    //
    // There is no control window. A simple click on any position has to skip the demo.
    //
    #if GUI_SUPPORT_TOUCH
      GUI_PID_GetState(&State);
      if (State.Pressed) {
        break;
      }
    #endif
  }
  GUIDEMO_NotifyStartNext();
  PixelsPerSecond = _GetPixelsPerSecond();
  GUI_SetClipRect(NULL);
  GUIDEMO_AddStringToString(acText, "Pixels/sec: ");
  GUIDEMO_AddIntToString(acText, PixelsPerSecond);
  GUIDEMO_DrawBk();
  GUI_SetColor(GUI_WHITE);
  GUI_SetTextMode(GUI_TM_TRANS);
  GUI_SetFont(&GUI_FontRounded22);
  GUI_DispStringHCenterAt(acText, xSize / 2, (ySize - GUI_GetFontSizeY()) / 2);
  GUIDEMO_ConfigureDemo(NULL, NULL, GUIDEMO_SHOW_CURSOR | GUIDEMO_SHOW_CONTROL);
  GUIDEMO_Delay(4000);
}

#else

void GUIDEMO_Speed_C(void);
void GUIDEMO_Speed_C(void) {}

#endif  // SHOW_GUIDEMO_SPEED

/*************************** End of file ****************************/
