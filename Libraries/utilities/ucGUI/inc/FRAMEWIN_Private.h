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
File        : FRAMEWIN_Private.h
Purpose     : FRAMEWIN private header file
--------------------END-OF-HEADER-------------------------------------
*/

#ifndef FRAMEWIN_PRIVATE_H
#define FRAMEWIN_PRIVATE_H

#include "WM.h"
#include "FRAMEWIN.h"
#include "WIDGET.h"
#include "GUI_HOOK.h"

#if GUI_WINSUPPORT

/*********************************************************************
*
*         Object definition
*
**********************************************************************
*/

typedef struct {
  const GUI_FONT GUI_UNI_PTR * pFont;
  GUI_COLOR                    aBarColor[2];
  GUI_COLOR                    aTextColor[2];
  GUI_COLOR                    ClientColor;
  I16                          TitleHeight;
  I16                          BorderSize;
  I16                          IBorderSize;
  I16                          TextAlign;
} FRAMEWIN_PROPS;

typedef struct {
  WIDGET Widget;
  FRAMEWIN_PROPS Props;
  WM_CALLBACK* cb;
  WM_HWIN hClient;
  WM_HWIN hMenu;
  WM_HWIN hText;
  GUI_RECT rRestore;
  U16 Flags;
  WM_HWIN hFocussedChild;          /* Handle to focussed child .. default none (0) */
  WM_DIALOG_STATUS* pDialogStatus;
  GUI_HOOK* pFirstHook;
  #if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
    int DebugId;
  #endif  
} FRAMEWIN_Obj;

typedef struct {
  I16 TitleHeight;
  I16 MenuHeight;
  GUI_RECT rClient;
  GUI_RECT rTitleText;
} POSITIONS;

/*********************************************************************
*
*       Macros for internal use
*
**********************************************************************
*/
#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
  FRAMEWIN_Obj * FRAMEWIN_h2p(FRAMEWIN_Handle h);
  #define FRAMEWIN_H2P(h) FRAMEWIN_h2p(h)
  #define FRAMEWIN_INIT_ID(p) p->DebugId = FRAMEWIN_ID
#else
  #define FRAMEWIN_H2P(h) (FRAMEWIN_Obj *)GUI_ALLOC_h2p(h)
  #define FRAMEWIN_INIT_ID(p)
#endif

/*********************************************************************
*
*        public data (internal defaults)
*
**********************************************************************
*/

extern FRAMEWIN_PROPS FRAMEWIN__DefaultProps;

/*********************************************************************
*
*        public functions (internal)
*
**********************************************************************
*/

void            FRAMEWIN__CalcPositions   (FRAMEWIN_Obj* pObj, POSITIONS* pPos);
int             FRAMEWIN__CalcTitleHeight (FRAMEWIN_Obj* pObj);
void            FRAMEWIN__UpdatePositions (FRAMEWIN_Obj* pObj);
void            FRAMEWIN__UpdateButtons   (FRAMEWIN_Obj* pObj, int OldHeight);

/*********************************************************************
*
*        public functions
*
**********************************************************************
*/

const GUI_FONT GUI_UNI_PTR * FRAMEWIN_GetFont          (FRAMEWIN_Handle hObj);
int             FRAMEWIN_GetTitleHeight   (FRAMEWIN_Handle hObj);
void            FRAMEWIN_MinButtonSetState(WM_HWIN hButton, int State);
void            FRAMEWIN_MaxButtonSetState(WM_HWIN hButton, int State);

#endif   /* GUI_WINSUPPORT */
#endif   /* FRAMEWIN_H */
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
