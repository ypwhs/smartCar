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
File        : GUIDEMO_ColorBar.c
Purpose     : Draws color bars
----------------------------------------------------------------------
*/

#include "GUIDEMO.h"

#if (SHOW_GUIDEMO_COLORBAR)

/*********************************************************************
*
*       Define
*
**********************************************************************
*/
#define TIME_PAUSE  500
#define TIME_STEP   500
#define TIME_RUN    ((TIME_PAUSE + TIME_STEP) * 6)
#define NUM_COLORS  8

/*********************************************************************
*
*       GUIDEMO_ColorBar
*/
void GUIDEMO_ColorBar(void) {
  GUI_COLOR ColorStartBlack;
  GUI_COLOR ColorStartWhite;
  char      acTitle[]        = "Color bar";
  char      acDesc[]         = "emWin features an integrated\ncolor management which automatically finds\nthe best available color for any logical color";
  char      acText[80]       = { 0 };
  int       BitsPerPixel;
  int       NumColors;
  int       TimeStart;
  int       ScreenX0;
  int       ScreenY0;
  int       FontY0;
  int       Index;
  int       xSize;
  int       ySize;
  int       yStep;
  int       Time;
  int       Dir;

  xSize    = LCD_GetXSize();
  ySize    = LCD_GetYSize();
  ScreenX0 = 60;
  ScreenY0 = 60;
  yStep    = (ySize - ScreenY0  * 2) / (NUM_COLORS * 2);
  if (yStep < 10) {
    yStep = 10;
  }
  GUIDEMO_ConfigureDemo(acTitle, acDesc, GUIDEMO_SHOW_CURSOR | GUIDEMO_SHOW_CONTROL);
  GUIDEMO_DrawBk();
  //
  // Heading
  //
	GUI_SetColor(GUI_WHITE);
	GUI_SetFont(&GUI_FontRounded22);
	GUI_DispStringHCenterAt("Color bars", xSize / 2, 12);
  GUI_SetFont(&GUI_FontSouvenir18);
  //
  // Colors
  //
  FontY0 = ScreenY0 + ((yStep * 2 - GUI_GetFontDistY()) / 2);
  GUI_DispStringAt("Red",     1, FontY0);
  GUI_DispStringAt("Green",   1, FontY0 + yStep *  2);
  GUI_DispStringAt("Blue",    1, FontY0 + yStep *  4);
  GUI_DispStringAt("Grey",    1, FontY0 + yStep *  6);
  GUI_DispStringAt("Yellow",  1, FontY0 + yStep *  8);
  GUI_DispStringAt("Cyan",    1, FontY0 + yStep * 10);
  GUI_DispStringAt("Magenta", 1, FontY0 + yStep * 12);
  GUI_SetFont(&GUI_Font8_ASCII);
  //
  // LCD Controller
  //
  #ifdef LCD_CONTROLLER
    GUIDEMO_AddStringToString(acText, "LCD Controller: ");
    GUIDEMO_AddStringToString(acText, LCD_CONTROLLER);
    GUI_DispStringAt         (acText, 12, ySize - 45);
    GUIDEMO_ClearText        (acText);
  #endif
  //
  // BPP and number of colors
  //
  BitsPerPixel = LCD_GetBitsPerPixel();
  GUIDEMO_AddIntToString   (acText, BitsPerPixel);
  GUIDEMO_AddStringToString(acText, " bpp");
  NumColors = LCD_GetDevCap(LCD_DEVCAP_NUMCOLORS);
  if (NumColors) {
    GUIDEMO_AddStringToString(acText, ", ");
    GUIDEMO_AddIntToString   (acText, NumColors);
    GUIDEMO_AddStringToString(acText, " colors");
  }
  GUI_DispStringAt(acText, 12, ySize - 25);
  //
  // Gradients
  //
  TimeStart = GUIDEMO_GetTime();
  while (((GUIDEMO_GetTime() - TimeStart) < TIME_RUN) && (GUIDEMO_CheckCancel() == 0)) {
    Time  = (GUIDEMO_GetTime() - TimeStart) % ((TIME_PAUSE + TIME_STEP) << 1);
    Dir   = Time / (TIME_PAUSE + TIME_STEP);
    Time -= Dir * (TIME_PAUSE + TIME_STEP);
    GUI_Exec();
    if (Time > TIME_PAUSE) {
      continue;
    }
    Index           = ((Time * 0xFF) / TIME_STEP) ^ (Dir * 0xFF);
    ColorStartBlack = 0x000000 + 0x010101 * Index;
    ColorStartWhite = 0xFFFFFF - ColorStartBlack;
    GUI_DrawGradientH(ScreenX0, ScreenY0 + yStep *  0, xSize - ScreenX0, (ScreenY0 + yStep *  1) - 1, GUI_RED,     ColorStartBlack);
    GUI_DrawGradientH(ScreenX0, ScreenY0 + yStep *  1, xSize - ScreenX0, (ScreenY0 + yStep *  2) - 1, GUI_RED,     ColorStartWhite);
    GUI_DrawGradientH(ScreenX0, ScreenY0 + yStep *  2, xSize - ScreenX0, (ScreenY0 + yStep *  3) - 1, GUI_GREEN,   ColorStartBlack);
    GUI_DrawGradientH(ScreenX0, ScreenY0 + yStep *  3, xSize - ScreenX0, (ScreenY0 + yStep *  4) - 1, GUI_GREEN,   ColorStartWhite);
    GUI_DrawGradientH(ScreenX0, ScreenY0 + yStep *  4, xSize - ScreenX0, (ScreenY0 + yStep *  5) - 1, GUI_BLUE,    ColorStartBlack);
    GUI_DrawGradientH(ScreenX0, ScreenY0 + yStep *  5, xSize - ScreenX0, (ScreenY0 + yStep *  6) - 1, GUI_BLUE,    ColorStartWhite);
    GUI_DrawGradientH(ScreenX0, ScreenY0 + yStep *  6, xSize - ScreenX0, (ScreenY0 + yStep *  7) - 1, GUI_GRAY,    ColorStartBlack);
    GUI_DrawGradientH(ScreenX0, ScreenY0 + yStep *  7, xSize - ScreenX0, (ScreenY0 + yStep *  8) - 1, GUI_GRAY,    ColorStartWhite);
    GUI_DrawGradientH(ScreenX0, ScreenY0 + yStep *  8, xSize - ScreenX0, (ScreenY0 + yStep *  9) - 1, GUI_YELLOW,  ColorStartBlack);
    GUI_DrawGradientH(ScreenX0, ScreenY0 + yStep *  9, xSize - ScreenX0, (ScreenY0 + yStep * 10) - 1, GUI_YELLOW,  ColorStartWhite);
    GUI_DrawGradientH(ScreenX0, ScreenY0 + yStep * 10, xSize - ScreenX0, (ScreenY0 + yStep * 11) - 1, GUI_CYAN,    ColorStartBlack);
    GUI_DrawGradientH(ScreenX0, ScreenY0 + yStep * 11, xSize - ScreenX0, (ScreenY0 + yStep * 12) - 1, GUI_CYAN,    ColorStartWhite);
    GUI_DrawGradientH(ScreenX0, ScreenY0 + yStep * 12, xSize - ScreenX0, (ScreenY0 + yStep * 13) - 1, GUI_MAGENTA, ColorStartBlack);
    GUI_DrawGradientH(ScreenX0, ScreenY0 + yStep * 13, xSize - ScreenX0, (ScreenY0 + yStep * 14) - 1, GUI_MAGENTA, ColorStartWhite);
  }
}

#else

void GUIDEMO_ColorBar_C(void);
void GUIDEMO_ColorBar_C(void) {}

#endif  // SHOW_GUIDEMO_COLORBAR

/*************************** End of file ****************************/
