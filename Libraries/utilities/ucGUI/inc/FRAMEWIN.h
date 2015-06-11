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
File        : FRAMEWIN.h
Purpose     : Frame window include
--------------------END-OF-HEADER-------------------------------------
*/

#ifndef FRAMEWIN_H
#define FRAMEWIN_H

#include "WM.h"
#if GUI_WINSUPPORT
#include "DIALOG_Intern.h"      /* Req. for Create indirect data structure */

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

/************************************************************
*
*       Defines
*
*************************************************************
*/
/************************************************************
*
*       Configuration
*/
#ifndef   FRAMEWIN_ALLOW_DRAG_ON_FRAME
  #define FRAMEWIN_ALLOW_DRAG_ON_FRAME 1
#endif

/************************************************************
*
*       Create / Status flags
*/
#define FRAMEWIN_CF_ACTIVE     (1<<3)
#define FRAMEWIN_CF_MOVEABLE   (1<<4)
#define FRAMEWIN_CF_TITLEVIS   (1<<5)
#define FRAMEWIN_CF_MINIMIZED  (1<<6)
#define FRAMEWIN_CF_MAXIMIZED  (1<<7)

#define FRAMEWIN_SF_ACTIVE     FRAMEWIN_CF_ACTIVE
#define FRAMEWIN_SF_MOVEABLE   FRAMEWIN_CF_MOVEABLE
#define FRAMEWIN_SF_TITLEVIS   FRAMEWIN_CF_TITLEVIS
#define FRAMEWIN_SF_MINIMIZED  FRAMEWIN_CF_MINIMIZED
#define FRAMEWIN_SF_MAXIMIZED  FRAMEWIN_CF_MAXIMIZED

/************************************************************
*
*       BUTTON Flags
*/

#define FRAMEWIN_BUTTON_RIGHT   (1<<0)
#define FRAMEWIN_BUTTON_LEFT    (1<<1)

/************************************************************
*                                                           *
*                         Types                             *
*                                                           *
*************************************************************
*/

typedef WM_HMEM FRAMEWIN_Handle;

/*********************************************************************
*
*       Create functions
*
**********************************************************************
*/
FRAMEWIN_Handle FRAMEWIN_Create        (const char* pTitle, WM_CALLBACK* cb, int Flags, int x0, int y0, int xsize, int ysize);
FRAMEWIN_Handle FRAMEWIN_CreateAsChild (int x0, int y0, int xsize, int ysize, WM_HWIN hParent, const char* pText, WM_CALLBACK* cb, int Flags);
FRAMEWIN_Handle FRAMEWIN_CreateIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK* cb);
FRAMEWIN_Handle FRAMEWIN_CreateEx      (int x0, int y0, int xsize, int ysize, WM_HWIN hParent,
                                        int WinFlags, int ExFlags, int Id, const char* pTitle, WM_CALLBACK* cb);

/*********************************************************************
*
*       The callback ...
*
* Do not call it directly ! It is only to be used from within an
* overwritten callback.
*/
void FRAMEWIN_Callback(WM_MESSAGE * pMsg);

/*********************************************************************
*
*       Standard member functions
*
**********************************************************************
*/

#define FRAMEWIN_EnableMemdev(hObj)  WM_EnableMemdev(hObj)
#define FRAMEWIN_DisableMemdev(hObj) WM_DisableMemdev(hObj)
#define FRAMEWIN_Delete(hObj)        WM_DeleteWindow(hObj)
#define FRAMEWIN_Paint(hObj)         WM_Paint(hObj)
#define FRAMEWIN_Invalidate(hObj)    WM_InvalidateWindow(hObj)

WM_HWIN FRAMEWIN_AddButton     (FRAMEWIN_Handle hObj, int Flags, int Off, int Id);
WM_HWIN FRAMEWIN_AddCloseButton(FRAMEWIN_Handle hObj, int Flags, int Off);
WM_HWIN FRAMEWIN_AddMaxButton  (FRAMEWIN_Handle hObj, int Flags, int Off);
void    FRAMEWIN_AddMenu       (FRAMEWIN_Handle hObj, WM_HWIN hMenu);
WM_HWIN FRAMEWIN_AddMinButton  (FRAMEWIN_Handle hObj, int Flags, int Off);

void    FRAMEWIN_Minimize      (FRAMEWIN_Handle hObj);
void    FRAMEWIN_Maximize      (FRAMEWIN_Handle hObj);
void    FRAMEWIN_Restore       (FRAMEWIN_Handle hObj);

/*********************************************************************
*
*       Member functions: Set Properties
*
**********************************************************************
*/
void FRAMEWIN_SetActive      (FRAMEWIN_Handle hObj, int State);
void FRAMEWIN_SetBarColor    (FRAMEWIN_Handle hObj, unsigned Index, GUI_COLOR Color);
void FRAMEWIN_SetBorderSize  (FRAMEWIN_Handle hObj, unsigned Size);
void FRAMEWIN_SetClientColor (FRAMEWIN_Handle hObj, GUI_COLOR Color);
void FRAMEWIN_SetFont        (FRAMEWIN_Handle hObj, const GUI_FONT GUI_UNI_PTR * pFont);
void FRAMEWIN_SetMoveable    (FRAMEWIN_Handle hObj, int State);
void FRAMEWIN_SetResizeable  (FRAMEWIN_Handle hObj, int State);
void FRAMEWIN_SetText        (FRAMEWIN_Handle hObj, const char* s);
void FRAMEWIN_SetTextAlign   (FRAMEWIN_Handle hObj, int Align);
void FRAMEWIN_SetTextColor   (FRAMEWIN_Handle hObj, GUI_COLOR Color);
void FRAMEWIN_SetTextColorEx (FRAMEWIN_Handle hObj, unsigned Index, GUI_COLOR Color);
void FRAMEWIN_SetTitleVis    (FRAMEWIN_Handle hObj, int Show);
int  FRAMEWIN_SetTitleHeight (FRAMEWIN_Handle hObj, int Height);

/*********************************************************************
*
*       Member functions: Get Properties
*
**********************************************************************
*/
int FRAMEWIN_GetTitleHeight (FRAMEWIN_Handle hObj);
int FRAMEWIN_GetBorderSize  (FRAMEWIN_Handle hObj);
int FRAMEWIN_IsMinimized    (FRAMEWIN_Handle hObj);
int FRAMEWIN_IsMaximized    (FRAMEWIN_Handle hObj);

/*********************************************************************
*
*       Global functions
*
**********************************************************************
*/
GUI_COLOR       FRAMEWIN_GetDefaultBarColor   (unsigned Index);
int             FRAMEWIN_GetDefaultBorderSize (void);
int             FRAMEWIN_GetDefaultTitleHeight(void);
GUI_COLOR       FRAMEWIN_GetDefaultClientColor(void);
const GUI_FONT GUI_UNI_PTR * FRAMEWIN_GetDefaultFont       (void);
GUI_COLOR       FRAMEWIN_GetDefaultTextColor  (unsigned Index);
void            FRAMEWIN_SetDefaultBarColor   (unsigned Index, GUI_COLOR Color);
void            FRAMEWIN_SetDefaultBorderSize (int DefaultBorderSize);
void            FRAMEWIN_SetDefaultTitleHeight(int DefaultTitleHeight);
void            FRAMEWIN_SetDefaultClientColor(GUI_COLOR Color);
void            FRAMEWIN_SetDefaultFont       (const GUI_FONT GUI_UNI_PTR * pFont);
int             FRAMEWIN_SetDefaultTextAlign  (int TextAlign);
void            FRAMEWIN_SetDefaultTextColor  (unsigned Index, GUI_COLOR Color);

/*********************************************************************
*
*       Macros for compatibility
*
**********************************************************************
*/
#define         FRAMEWIN_SetDefaultCaptionSize(Height)       FRAMEWIN_SetDefaultTitleHeight(Height)
#define         FRAMEWIN_GetDefaultCaptionSize()             FRAMEWIN_GetDefaultTitleHeight()
#define         FRAMEWIN_CreateButton(hObj, Flags, Off, Id)  FRAMEWIN_AddButton(hObj, Flags, Off, Id) 
#define         FRAMEWIN_CreateCloseButton(hObj, Flags, Off) FRAMEWIN_AddCloseButton(hObj, Flags, Off)
#define         FRAMEWIN_CreateMaxButton(hObj, Flags, Off)   FRAMEWIN_AddMaxButton(hObj, Flags, Off)  
#define         FRAMEWIN_CreateMinButton(hObj, Flags, Off)   FRAMEWIN_AddMinButton(hObj, Flags, Off)  

#if defined(__cplusplus)
  }
#endif

#endif
#endif   /* FRAMEWIN_H */
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
