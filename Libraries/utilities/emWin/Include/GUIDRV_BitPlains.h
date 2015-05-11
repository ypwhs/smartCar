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
File        : GUIDRV_Bitplains.h
Purpose     : Interface definition for GUIDRV_Bitplains driver
---------------------------END-OF-HEADER------------------------------
*/

#ifndef GUIDRV_BITPLAINS_H
#define GUIDRV_BITPLAINS_H

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

/*********************************************************************
*
*       Configuration structure
*/
typedef struct {
  int Mirror;
} CONFIG_BITPLAINS;

typedef struct {
  U8 * apVRAM[8];
} CONFIG_VRAM_BITPLAINS;

/*********************************************************************
*
*       Display drivers
*/
//
// Addresses
//
extern const GUI_DEVICE_API GUIDRV_Win_API;

extern const GUI_DEVICE_API GUIDRV_BitPlains_API;

//
// Macros to be used in configuration files
//
#if defined(WIN32) && !defined(LCD_SIMCONTROLLER)

  #define GUIDRV_BITPLAINS        &GUIDRV_Win_API

#else

  #define GUIDRV_BITPLAINS        &GUIDRV_BitPlains_API

#endif

/*********************************************************************
*
*       Public routines
*/
void GUIDRV_BitPlains_Config(GUI_DEVICE * pDevice, CONFIG_BITPLAINS * pConfig);

#if defined(__cplusplus)
}
#endif

#endif /* GUIDRV_BITPLAINS_H */

/*************************** End of file ****************************/
