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
File        : RADIO.h
Purpose     : RADIO include
--------------------END-OF-HEADER-------------------------------------
*/

#ifndef RADIO_H
#define RADIO_H

#include "WM.h"
#include "DIALOG_Intern.h"      /* Req. for Create indirect data structure */

#if GUI_WINSUPPORT

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

/*********************************************************************
*
*       Bitmap indices
*/
#define RADIO_BI_INACTIV 0
#define RADIO_BI_ACTIV   1
#define RADIO_BI_CHECK   2

/*********************************************************************
*
*            Defaults for public configuration switches
*
**********************************************************************

The following are defaults for config switches which affect the
interface specified in this module
*/

/*********************************************************************
*
*           defines
*
**********************************************************************
*/

#define RADIO_TEXTPOS_RIGHT       0
#define RADIO_TEXTPOS_LEFT        WIDGET_STATE_USER0  /* Not implemented, TBD */

/*********************************************************************
*
*                         Public Types
*
**********************************************************************
*/

typedef WM_HMEM RADIO_Handle;

/*********************************************************************
*
*                 Create functions
*
**********************************************************************
*/

RADIO_Handle RADIO_Create        (int x0, int y0, int xsize, int ysize, WM_HWIN hParent, int Id, int Flags, unsigned Para);
RADIO_Handle RADIO_CreateIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK* cb);
RADIO_Handle RADIO_CreateEx      (int x0, int y0, int xsize, int ysize, WM_HWIN hParent,
                                  int WinFlags, int ExFlags, int Id, int NumItems, int Spacing);

/*********************************************************************
*
*       The callback ...
*
* Do not call it directly ! It is only to be used from within an
* overwritten callback.
*/
void RADIO_Callback(WM_MESSAGE * pMsg);

/*********************************************************************
*
*       Standard member functions
*
**********************************************************************
*/

void      RADIO_SetDefaultFont      (const GUI_FONT GUI_UNI_PTR* pFont);
GUI_COLOR RADIO_SetDefaultFocusColor(GUI_COLOR Color);
void      RADIO_SetDefaultImage     (const GUI_BITMAP * pBitmap, unsigned int Index);
void      RADIO_SetDefaultTextColor (GUI_COLOR TextColor);

const GUI_FONT GUI_UNI_PTR* RADIO_GetDefaultFont      (void);
GUI_COLOR                   RADIO_GetDefaultTextColor (void);

/*********************************************************************
*
*                 Member functions
*
**********************************************************************
*/

void      RADIO_AddValue     (RADIO_Handle hObj, int Add);
void      RADIO_Dec          (RADIO_Handle hObj);
int       RADIO_GetText      (RADIO_Handle hObj, unsigned Index, char * pBuffer, int MaxLen);
void      RADIO_Inc          (RADIO_Handle hObj);
void      RADIO_SetBkColor   (RADIO_Handle hObj, GUI_COLOR Color);
GUI_COLOR RADIO_SetFocusColor(RADIO_Handle hObj, GUI_COLOR Color);
void      RADIO_SetFont      (RADIO_Handle hObj, const GUI_FONT GUI_UNI_PTR* pFont);
void      RADIO_SetGroupId   (RADIO_Handle hObj, U8 GroupId);
void      RADIO_SetImage     (RADIO_Handle hObj, const GUI_BITMAP * pBitmap, unsigned int Index);
void      RADIO_SetText      (RADIO_Handle hObj, const char* pText, unsigned Index);
void      RADIO_SetTextColor (RADIO_Handle hObj, GUI_COLOR Color);
void      RADIO_SetValue     (RADIO_Handle hObj, int v);

/*********************************************************************
*
*                 Query state
*
**********************************************************************
*/
int RADIO_GetValue(RADIO_Handle hObj);

#if defined(__cplusplus)
  }
#endif

#endif   /* if GUI_WINSUPPORT */
#endif   /* RADIO_H */

/************************* end of file ******************************/
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
