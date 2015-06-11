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
File        : GUI_FontIntern.h
Purpose     : Internal decalrations used in font files
---------------------------END-OF-HEADER------------------------------

Attention : Do not modify this file ! If you do, you will not
            be able do update to a later GUI version !

*/


#ifndef  GUI_FONTINTERN_H    /* Guard against multiple inclusion */
#define  GUI_FONTINTERN_H

#include "GUI.h"

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif


extern GUI_CONST_STORAGE GUI_FONT_PROP GUI_Font8ASCII_Prop;
extern GUI_CONST_STORAGE GUI_FONT_PROP GUI_Font10S_ASCII_FontProp1;
extern GUI_CONST_STORAGE GUI_FONT_PROP GUI_Font10ASCIIProp1;
extern GUI_CONST_STORAGE GUI_FONT_PROP GUI_Font16ASCIIProp1;
extern GUI_CONST_STORAGE GUI_FONT_PROP GUI_Font16_1_FontProp1;

extern GUI_CONST_STORAGE GUI_FONT_PROP GUI_Font20_ASCII_Prop1;
extern GUI_CONST_STORAGE GUI_CHARINFO GUI_Font20_ASCII_CharInfo[95];
extern GUI_CONST_STORAGE GUI_FONT_PROP GUI_Font20B_ASCII_Prop1;
extern GUI_CONST_STORAGE GUI_CHARINFO GUI_Font20B_ASCII_CharInfo[95];

extern GUI_CONST_STORAGE GUI_FONT_PROP FontComic24BProp_ASCII;
extern GUI_CONST_STORAGE GUI_CHARINFO GUI_Font24B_ASCII_CharInfo[95];
extern GUI_CONST_STORAGE GUI_CHARINFO GUI_Font24_ASCII_CharInfo[95];
extern GUI_CONST_STORAGE GUI_CHARINFO GUI_Font32B_ASCII_CharInfo[95];
extern GUI_CONST_STORAGE GUI_CHARINFO GUI_Font32_ASCII_CharInfo[95];
extern GUI_CONST_STORAGE GUI_CHARINFO GUI_CharInfoF8x13_ASCII[96];
extern GUI_CONST_STORAGE GUI_CHARINFO GUI_CharInfoF8x15B_ASCII[96];
extern GUI_CONST_STORAGE GUI_CHARINFO CharInfo18B_ASCII[97];
extern GUI_CONST_STORAGE GUI_CHARINFO CharInfo24B_ASCII[96];
extern GUI_CONST_STORAGE GUI_FONT_PROP GUI_Font13ASCII_Prop1;
extern GUI_CONST_STORAGE GUI_FONT_PROP GUI_Font13B_ASCII_Prop1;
extern GUI_CONST_STORAGE GUI_CHARINFO GUI_Font13H_ASCII_CharInfo[95];
extern GUI_CONST_STORAGE GUI_CHARINFO GUI_Font13HB_ASCII_CharInfo[95];
extern GUI_CONST_STORAGE GUI_CHARINFO GUI_Font16_HK_CharInfo[169];
extern GUI_CONST_STORAGE GUI_CHARINFO GUI_Font16B_ASCII_CharInfo[95];


#if defined(__cplusplus)
  }
#endif


#endif   /* Guard against multiple inclusion */

/*************************** End of file ****************************/
