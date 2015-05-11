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
File        : DROPDOWN_Private.h
Purpose     : DROPDOWN private header file
--------------------END-OF-HEADER-------------------------------------
*/

#ifndef DROPDOWN_PRIVATE_H
#define DROPDOWN_PRIVATE_H

#include "DROPDOWN.h"
#include "WIDGET.h"
#include "GUI_ARRAY.h"

#if GUI_WINSUPPORT

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define DROPDOWN_SF_AUTOSCROLLBAR DROPDOWN_CF_AUTOSCROLLBAR

/*********************************************************************
*
*       Object definition
*
**********************************************************************
*/
typedef struct {
  const GUI_FONT GUI_UNI_PTR * pFont;
  GUI_COLOR aBackColor[3];
  GUI_COLOR aTextColor[3];
  GUI_COLOR aColor[2];
  GUI_COLOR aScrollbarColor[3];
  I16       TextBorderSize;
  I16       Align;
} DROPDOWN_PROPS;

typedef struct {
  WIDGET  Widget;
  I16     Sel;                        /* current selection */
  I16     ySizeEx;                    /* Drop down size */
  I16     TextHeight;
  GUI_ARRAY Handles;
  WM_SCROLL_STATE ScrollState;
  DROPDOWN_PROPS Props;
  WM_HWIN hListWin;
  U8      Flags;
  U16     ItemSpacing;
  U8      ScrollbarWidth;
  char  IsPressed;
  #if GUI_DEBUG_LEVEL >1
    int DebugId;
  #endif  
} DROPDOWN_Obj;

/*********************************************************************
*
*       Macros for internal use
*
**********************************************************************
*/
#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
  DROPDOWN_Obj * DROPDOWN_h2p(DROPDOWN_Handle h);
  #define DROPDOWN_H2P(h) DROPDOWN_h2p(h)
  #define DROPDOWN_INIT_ID(p) p->DebugId = DROPDOWN_ID
#else
  #define DROPDOWN_H2P(h) (DROPDOWN_Obj *)GUI_ALLOC_h2p(h)
  #define DROPDOWN_INIT_ID(p)
#endif

/*********************************************************************
*
*       Private (module internal) data
*
**********************************************************************
*/

extern DROPDOWN_PROPS DROPDOWN__DefaultProps;

/*********************************************************************
*
*       Private functions
*
**********************************************************************
*/

void DROPDOWN__AdjustHeight(DROPDOWN_Handle hObj, DROPDOWN_Obj* pObj);
int  DROPDOWN__GetNumItems (DROPDOWN_Obj* pObj);

#endif /* GUI_WINSUPPORT */
#endif /* DROPDOWN_PRIVATE_H */
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
