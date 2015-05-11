/*
*********************************************************************************************************
*                                                uC/GUI
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2002, SEGGER Microcontroller Systeme GmbH
*
*              µC/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed
*              in any way. We appreciate your understanding and fairness.
*
----------------------------------------------------------------------
File        : LCD_IncludeDriver.h
Purpose     : Glue code for including the configured driver
---------------------------END-OF-HEADER------------------------------
*/

#ifndef LCD_INCLUDEDRIVER_H
#define LCD_INCLUDEDRIVER_H


/*********************************************************************
*
*      Some error checking
*
**********************************************************************
*/
#ifndef LCD_CONTROLLER
  #error LCD_CONTROLLER not defined !
#endif

/*********************************************************************
*
*           Color conversion routines for layer <n>
*
**********************************************************************
*/

#include "LCD_L0_Generic.c"

/*********************************************************************
*
*           Driver for layer <n>
*
**********************************************************************
*/

#if   !defined(WIN32) | defined(LCD_SIMCONTROLLER)
  #if   (LCD_CONTROLLER   == -2)
    #include "LCDNull.c"  /* Empty driver, no output to LCD */
  #elif (LCD_CONTROLLER   == -1)
    #include "LCDDummy.c" /* Empty driver, no output to LCD */
  #elif (LCD_CONTROLLER   == 0)
    #if   ((LCD_BITSPERPIXEL == 1) | (LCD_BITSPERPIXEL == 2))
      #include "LCDMem.c"
    #elif ((LCD_BITSPERPIXEL == 3) | (LCD_BITSPERPIXEL == 6))
      #include "LCDMemC.c"
    #else
      #error LCD Config error !
    #endif
  #elif (LCD_CONTROLLER / 100 == 4)
    #include "LCDColorOnMono.c"
  #elif (LCD_CONTROLLER / 100 == 5)
    #include "LCD501.c"
  #elif (LCD_CONTROLLER / 100 == 7)
    #include "LCD07X1.c"
  #elif (LCD_CONTROLLER / 100 == 13)
    #include "LCDLin.c"
  #elif (LCD_CONTROLLER / 100 == 15)
    #include "LCDPage1bpp.c"
    #include "LCDPage1bpp_1.c"
  #elif (LCD_CONTROLLER / 100 == 16)
    #include "LCD159A.c"
  #elif (LCD_CONTROLLER / 100 == 17)
    #include "LCD15E05.c"
  #elif (LCD_CONTROLLER / 100 == 18)
    #include "LCD1611.c"
  #elif (LCD_CONTROLLER / 100 == 32)
    #include "LCDLin32.c"
  #elif (LCD_CONTROLLER / 100 == 69)
    #include "LCDSLin.c"
  #elif (LCD_CONTROLLER / 100 == 87)
    #include "LCDFujitsu.c"
  #elif (LCD_CONTROLLER / 100 == 63)
    #include "LCD6331.c"
  #elif (LCD_CONTROLLER / 100 == 664)
    #include "LCD6642X.c"
  #else
    #error LCD Config error !
  #endif
#else
  #include "LCDWin.c"
#endif

#endif /* LCD_INCLUDEDRIVER_H */
