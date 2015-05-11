/*
*********************************************************************************************************
*                                             uC/GUI V3.98
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
File        : GUI_Private.h
Purpose     : GUI internal declarations
---------------------------END-OF-HEADER------------------------------

Attention : Do not modify this file ! If you do, you will not
            be able do update to a later GUI version !

*/

#ifndef  GUI_PRIVATE_H
#define  GUI_PRIVATE_H

#include "GUI_Protected.h"
#include "LCD_Private.h"   /* Required because of LCD_PIXELINDEX, which depends on LCDConf */

#if GUI_SUPPORT_DEVICES
  #define LCDDEV_L0_Color2Index         GUI_Context.pDeviceAPI->pfColor2Index
  #define LCDDEV_L0_DrawBitmap          GUI_Context.pDeviceAPI->pfDrawBitmap
  #define LCDDEV_L0_DrawHLine           GUI_Context.pDeviceAPI->pfDrawHLine
  #define LCDDEV_L0_DrawVLine           GUI_Context.pDeviceAPI->pfDrawVLine
  #define LCDDEV_L0_DrawPixel           GUI_Context.pDeviceAPI->pfDrawPixel
  #define LCDDEV_L0_FillRect            GUI_Context.pDeviceAPI->pfFillRect
  #define LCDDEV_L0_GetPixel            GUI_Context.pDeviceAPI->pfGetPixel
  #define LCDDEV_L0_GetRect             GUI_Context.pDeviceAPI->pfGetRect
  #define LCDDEV_L0_GetPixelIndex       GUI_Context.pDeviceAPI->pfGetPixelIndex
  #define LCDDEV_L0_Index2Color         GUI_Context.pDeviceAPI->pfIndex2Color
  #define LCDDEV_L0_SetPixelIndex       GUI_Context.pDeviceAPI->pfSetPixelIndex
  #define LCDDEV_L0_XorPixel            GUI_Context.pDeviceAPI->pfXorPixel
  #define LCDDEV_L0_GetDevFunc          GUI_Context.pDeviceAPI->pfGetDevFunc
#else
  #define LCDDEV_L0_Color2Index         LCD_L0_Color2Index
  #define LCDDEV_L0_DrawBitmap          LCD_L0_DrawBitmap
  #define LCDDEV_L0_DrawHLine           LCD_L0_DrawHLine
  #define LCDDEV_L0_DrawVLine           LCD_L0_DrawVLine
  #define LCDDEV_L0_DrawPixel           LCD_L0_DrawPixel
  #define LCDDEV_L0_FillRect            LCD_L0_FillRect
  #define LCDDEV_L0_GetPixel            LCD_L0_GetPixel
  #define LCDDEV_L0_GetRect             LCD_L0_GetRect
  #define LCDDEV_L0_GetPixelIndex       LCD_L0_GetPixelIndex
  #define LCDDEV_L0_Index2Color         LCD_L0_Index2Color
  #define LCDDEV_L0_SetPixelIndex       LCD_L0_SetPixelIndex
  #define LCDDEV_L0_XorPixel            LCD_L0_XorPixel
  #define LCDDEV_L0_GetDevFunc          LCD_L0_GetDevFunc
#endif

void LCD_L0_DrawBitmap    (int x0, int y0, int xsize, int ysize, 
                           int BitsPerPixel, int BytesPerLine, 
                           const U8 GUI_UNI_PTR * pData, int Diff, const LCD_PIXELINDEX* pTrans);
void LCD_L0_1_DrawBitmap  (int x0, int y0, int xsize, int ysize, 
                           int BitsPerPixel, int BytesPerLine, 
                           const U8 GUI_UNI_PTR * pData, int Diff, const LCD_PIXELINDEX* pTrans);
void LCD_L0_2_DrawBitmap  (int x0, int y0, int xsize, int ysize, 
                           int BitsPerPixel, int BytesPerLine, 
                           const U8 GUI_UNI_PTR * pData, int Diff, const LCD_PIXELINDEX* pTrans);
void LCD_L0_3_DrawBitmap  (int x0, int y0, int xsize, int ysize, 
                           int BitsPerPixel, int BytesPerLine, 
                           const U8 GUI_UNI_PTR * pData, int Diff, const LCD_PIXELINDEX* pTrans);
void LCD_L0_4_DrawBitmap  (int x0, int y0, int xsize, int ysize, 
                           int BitsPerPixel, int BytesPerLine, 
                           const U8 GUI_UNI_PTR * pData, int Diff, const LCD_PIXELINDEX* pTrans);
void LCD_L0_MAG_DrawBitmap(int x0, int y0, int xsize, int ysize, 
                           int BitsPerPixel, int BytesPerLine, 
                           const U8 GUI_UNI_PTR * pData, int Diff, const LCD_PIXELINDEX* pTrans);
void LCD_L0_DELTA_DrawBitmap(int x0, int y0, int xsize, int ysize, 
                           int BitsPerPixel, int BytesPerLine, 
                           const U8 GUI_UNI_PTR * pData, int Diff, const LCD_PIXELINDEX* pTrans);

LCD_PIXELINDEX*  LCD_GetpPalConvTable(const LCD_LOGPALETTE GUI_UNI_PTR *  pLogPal);
LCD_PIXELINDEX*  LCD_GetpPalConvTableUncached(const LCD_LOGPALETTE GUI_UNI_PTR *  pLogPal);


#define GUI_POS_AUTO -4095   /* Position value for auto-pos */

#if GUI_SUPPORT_MEMDEV
  void*  GUI_MEMDEV__XY2PTR  (int x,int y);
  void*  GUI_MEMDEV__XY2PTREx(GUI_MEMDEV* pDev, int x,int y);
  void   GUI_MEMDEV__WriteToActiveAt(GUI_MEMDEV_Handle hMem,int x, int y);
#endif

#endif   /* ifdef GUI_H */

/*************************** End of file ****************************/
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
