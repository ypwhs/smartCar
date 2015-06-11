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
File        : MULTIPAGE.h
Purpose     : MULTIPAGE include
--------------------END-OF-HEADER-------------------------------------
*/

#ifndef MULTIPAGE_H
#define MULTIPAGE_H

#include "WM.h"
#include "DIALOG.h"      /* Req. for Create indirect data structure */

#if GUI_WINSUPPORT

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

/*********************************************************************
*
*       defines
*
**********************************************************************
*/
/*********************************************************************
*
*       Create / Status flags
*/

#define MULTIPAGE_ALIGN_LEFT    (0<<0)
#define MULTIPAGE_ALIGN_RIGHT   (1<<0)
#define MULTIPAGE_ALIGN_TOP     (0<<2)
#define MULTIPAGE_ALIGN_BOTTOM  (1<<2)

/*********************************************************************
*
*       Public Types
*
**********************************************************************
*/

typedef WM_HMEM MULTIPAGE_Handle;

/*********************************************************************
*
*       Create functions
*
**********************************************************************
*/

MULTIPAGE_Handle MULTIPAGE_Create        (int x0, int y0, int xsize, int ysize, WM_HWIN hParent, int Id, int Flags, int SpecialFlags);
MULTIPAGE_Handle MULTIPAGE_CreateIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK* cb);
MULTIPAGE_Handle MULTIPAGE_CreateEx      (int x0, int y0, int xsize, int ysize, WM_HWIN hParent,
                                          int WinFlags, int ExFlags, int Id);

/*********************************************************************
*
*       Member functions
*
**********************************************************************
*/

/* Methods changing properties */
void        MULTIPAGE_AddPage       (MULTIPAGE_Handle hObj, WM_HWIN hWin ,const char* pText);
void        MULTIPAGE_DeletePage    (MULTIPAGE_Handle hObj, unsigned Index, int Delete);
void        MULTIPAGE_SelectPage    (MULTIPAGE_Handle hObj, unsigned Index);
void        MULTIPAGE_EnablePage    (MULTIPAGE_Handle hObj, unsigned Index);
void        MULTIPAGE_DisablePage   (MULTIPAGE_Handle hObj, unsigned Index);
void        MULTIPAGE_SetText       (MULTIPAGE_Handle hObj, const char* pText, unsigned Index);
void        MULTIPAGE_SetBkColor    (MULTIPAGE_Handle hObj, GUI_COLOR Color, unsigned Index);
void        MULTIPAGE_SetTextColor  (MULTIPAGE_Handle hObj, GUI_COLOR Color, unsigned Index);
void        MULTIPAGE_SetFont       (MULTIPAGE_Handle hObj, const GUI_FONT GUI_UNI_PTR * pFont);
void        MULTIPAGE_SetAlign      (MULTIPAGE_Handle hObj, unsigned Align);
int         MULTIPAGE_GetSelection  (MULTIPAGE_Handle hObj);
WM_HWIN     MULTIPAGE_GetWindow     (MULTIPAGE_Handle hObj, unsigned Index);
int         MULTIPAGE_IsPageEnabled (MULTIPAGE_Handle hObj, unsigned Index);

/*********************************************************************
*
*       Get/Set defaults
*
**********************************************************************
*/

unsigned                     MULTIPAGE_GetDefaultAlign(void);
GUI_COLOR                    MULTIPAGE_GetDefaultBkColor(unsigned Index);
const GUI_FONT GUI_UNI_PTR * MULTIPAGE_GetDefaultFont(void);
GUI_COLOR                    MULTIPAGE_GetDefaultTextColor(unsigned Index);

void                         MULTIPAGE_SetDefaultAlign(unsigned Align);
void                         MULTIPAGE_SetDefaultBkColor(GUI_COLOR Color, unsigned Index);
void                         MULTIPAGE_SetDefaultFont(const GUI_FONT GUI_UNI_PTR * pFont);
void                         MULTIPAGE_SetDefaultTextColor(GUI_COLOR Color, unsigned Index);

#if defined(__cplusplus)
  }
#endif

#endif   /* if GUI_WINSUPPORT */
#endif   /* MULTIPAGE_H */
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
