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
File        : BUTTON.h
Purpose     : BUTTON public header file (API)
--------------------END-OF-HEADER-------------------------------------
*/

#ifndef BUTTON_H
#define BUTTON_H

#include "WM.h"
#include "DIALOG_Intern.h"      /* Req. for Create indirect data structure */
#include "WIDGET.h"


#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

#if GUI_WINSUPPORT

/************************************************************
*
*       Create flags
*/
/* For compatibility only ! */
#define BUTTON_CF_HIDE   WM_CF_HIDE
#define BUTTON_CF_SHOW   WM_CF_SHOW
#define BUTTON_CF_MEMDEV WM_CF_MEMDEV

/*********************************************************************
*
*       Color indices
*/
#define BUTTON_CI_UNPRESSED 0
#define BUTTON_CI_PRESSED   1
#define BUTTON_CI_DISABLED  2

/*********************************************************************
*
*       Bitmap indices
*/
#define BUTTON_BI_UNPRESSED 0
#define BUTTON_BI_PRESSED   1
#define BUTTON_BI_DISABLED  2

/************************************************************
*
*       Messages
*/

/************************************************************
*
*       States
*/

#define BUTTON_STATE_FOCUS      WIDGET_STATE_FOCUS
#define BUTTON_STATE_PRESSED    WIDGET_STATE_USER0

/************************************************************
*
*       Types
*
*************************************************************
*/
typedef WM_HMEM BUTTON_Handle;

/************************************************************
*
*       Create function(s)

  Note: the parameters to a create function may vary.
         Some widgets may have multiple create functions
*/

BUTTON_Handle BUTTON_Create        (int x0, int y0, int xsize, int ysize, int ID, int Flags);
BUTTON_Handle BUTTON_CreateAsChild (int x0, int y0, int xsize, int ysize, WM_HWIN hParent, int Id, int Flags);
BUTTON_Handle BUTTON_CreateIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK* cb);
BUTTON_Handle BUTTON_CreateEx      (int x0, int y0, int xsize, int ysize, WM_HWIN hParent,
                                    int WinFlags, int ExFlags, int Id);

/*********************************************************************
*
*       Standard member functions
*
**********************************************************************
*/

GUI_COLOR                    BUTTON_GetDefaultBkColor   (unsigned Index);
const GUI_FONT GUI_UNI_PTR * BUTTON_GetDefaultFont      (void);
int                          BUTTON_GetDefaultTextAlign (void);
GUI_COLOR                    BUTTON_GetDefaultTextColor (unsigned Index);
void                         BUTTON_SetDefaultBkColor   (GUI_COLOR Color, unsigned Index);
GUI_COLOR                    BUTTON_SetDefaultFocusColor(GUI_COLOR Color);
void                         BUTTON_SetDefaultFont      (const GUI_FONT GUI_UNI_PTR * pFont);
void                         BUTTON_SetDefaultTextAlign (int Align);
void                         BUTTON_SetDefaultTextColor (GUI_COLOR Color, unsigned Index);

#define BUTTON_EnableMemdev(hObj)  WM_EnableMemdev    (hObj)
#define BUTTON_DisableMemdev(hObj) WM_DisableMemdev   (hObj)
#define BUTTON_Delete(hObj)        WM_DeleteWindow    (hObj)
#define BUTTON_Paint(hObj)         WM_Paint           (hObj)
#define BUTTON_Invalidate(hObj)    WM_InvalidateWindow(hObj)

/*********************************************************************
*
*       The callback ...
*
* Do not call it directly ! It is only to be used from within an
* overwritten callback.
*/
void BUTTON_Callback(WM_MESSAGE *pMsg);

/*********************************************************************
*
*       Member functions
*
**********************************************************************
*/

GUI_COLOR        BUTTON_GetBkColor         (BUTTON_Handle hObj, unsigned int Index);
const GUI_BITMAP GUI_UNI_PTR * BUTTON_GetBitmap(BUTTON_Handle hObj,unsigned int Index);
const GUI_FONT GUI_UNI_PTR * BUTTON_GetFont(BUTTON_Handle hObj);
void             BUTTON_GetText            (BUTTON_Handle hObj, char * pBuffer, int MaxLen);
GUI_COLOR        BUTTON_GetTextColor       (BUTTON_Handle hObj, unsigned int Index);
unsigned         BUTTON_IsPressed          (BUTTON_Handle hObj);
void             BUTTON_SetBitmap          (BUTTON_Handle hObj, unsigned int Index, const GUI_BITMAP * pBitmap);
void             BUTTON_SetBitmapEx        (BUTTON_Handle hObj, unsigned int Index, const GUI_BITMAP * pBitmap, int x, int y);
void             BUTTON_SetBkColor         (BUTTON_Handle hObj, unsigned int Index, GUI_COLOR Color);
void             BUTTON_SetBMP             (BUTTON_Handle hObj, unsigned int Index, const void * pBitmap);
void             BUTTON_SetBMPEx           (BUTTON_Handle hObj, unsigned int Index, const void * pBitmap, int x, int y);
void             BUTTON_SetFont            (BUTTON_Handle hObj, const GUI_FONT GUI_UNI_PTR * pfont);
void             BUTTON_SetState           (BUTTON_Handle hObj, int State);                                    /* Not to be doc. */
void             BUTTON_SetPressed         (BUTTON_Handle hObj, int State);
GUI_COLOR        BUTTON_SetFocusColor      (BUTTON_Handle hObj, GUI_COLOR Color);
void             BUTTON_SetFocussable      (BUTTON_Handle hObj, int State);
void             BUTTON_SetStreamedBitmap  (BUTTON_Handle hObj, unsigned int Index, const GUI_BITMAP_STREAM * pBitmap);
void             BUTTON_SetStreamedBitmapEx(BUTTON_Handle hObj, unsigned int Index, const GUI_BITMAP_STREAM * pBitmap, int x, int y);
void             BUTTON_SetText            (BUTTON_Handle hObj, const char* s);
void             BUTTON_SetTextAlign       (BUTTON_Handle hObj, int Align);
void             BUTTON_SetTextColor       (BUTTON_Handle hObj, unsigned int Index, GUI_COLOR Color);
void             BUTTON_SetSelfDrawEx      (BUTTON_Handle hObj, unsigned int Index, void (*pDraw)(void), int x, int y); /* Not to be doc. */
void             BUTTON_SetSelfDraw        (BUTTON_Handle hObj, unsigned int Index, void (*pDraw)(void));               /* Not to be doc. */

/************************************************************
*
*       States
*/
#define BUTTON_STATE_HASFOCUS 0

#endif   /* BUTTON_H */

#if defined(__cplusplus)
  }
#endif

#endif   /* BUTTON_H */
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
