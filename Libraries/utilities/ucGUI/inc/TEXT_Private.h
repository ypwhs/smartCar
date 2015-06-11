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
File        : TEXT.h
Purpose     : TEXT include
--------------------END-OF-HEADER-------------------------------------
*/

#ifndef TEXT_PRIVATE_H
#define TEXT_PRIVATE_H

#include "TEXT.h"
#include "GUI_Protected.h"

#if GUI_WINSUPPORT

/*********************************************************************
*
*       Private config defaults
*
**********************************************************************
*/

#ifndef TEXT_SUPPORT_TRANSPARENCY
  #define TEXT_SUPPORT_TRANSPARENCY WM_SUPPORT_TRANSPARENCY
#endif

/*********************************************************************
*
*       Object definition
*
**********************************************************************
*/
typedef struct {
  const GUI_FONT GUI_UNI_PTR * pFont;
  GUI_COLOR TextColor;
  GUI_COLOR BkColor;
  GUI_WRAPMODE WrapMode;
} TEXT_PROPS;

typedef struct {
  WIDGET Widget;
  TEXT_PROPS Props;
  WM_HMEM hpText;
  I16 Align;
  #if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
    int DebugId;
  #endif  
} TEXT_Obj;

/*********************************************************************
*
*       Macros for internal use
*
**********************************************************************
*/
#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
  TEXT_Obj * TEXT_h2p(TEXT_Handle h);
  #define TEXT_H2P(h) TEXT_h2p(h)
  #define TEXT_INIT_ID(p) p->DebugId = TEXT_ID
#else
  #define TEXT_H2P(h) (TEXT_Obj *)GUI_ALLOC_h2p(h)
  #define TEXT_INIT_ID(p)
#endif

/*********************************************************************
*
*       Module internal data
*
**********************************************************************
*/
extern TEXT_PROPS TEXT__DefaultProps;

#endif   /* if GUI_WINSUPPORT */
#endif   /* TEXT_PRIVATE_H */
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
