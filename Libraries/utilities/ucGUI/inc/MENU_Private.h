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
File        : MENU_Private.h
Purpose     : Internal header file
---------------------------END-OF-HEADER------------------------------
*/

#ifndef MENU_PRIVATE_H
#define MENU_PRIVATE_H

#if GUI_WINSUPPORT

#include "WIDGET.h"
#include "GUI_ARRAY.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
/*********************************************************************
*
*       Status flags
*/
#define MENU_SF_HORIZONTAL              MENU_CF_HORIZONTAL
#define MENU_SF_VERTICAL                MENU_CF_VERTICAL
#define MENU_SF_OPEN_ON_POINTEROVER     MENU_CF_OPEN_ON_POINTEROVER
#define MENU_SF_CLOSE_ON_SECOND_CLICK   MENU_CF_CLOSE_ON_SECOND_CLICK
#define MENU_SF_HIDE_DISABLED_SEL       MENU_CF_HIDE_DISABLED_SEL

#define MENU_SF_ACTIVE                  (1<<6)  /* Internal flag only */
#define MENU_SF_POPUP                   (1<<7)  /* Internal flag only */

/*********************************************************************
*
*       Types
*
**********************************************************************
*/
/*********************************************************************
*
*       menu item
*/
typedef struct {
  MENU_Handle hSubmenu;
  U16         Id;
  U16         Flags;
  int         TextWidth;
  char        acText[1];
} MENU_ITEM;

/*********************************************************************
*
*       menu properties
*/
typedef struct {
  GUI_COLOR                   aTextColor[5];
  GUI_COLOR                   aBkColor[5];
  U8                          aBorder[4];
  const GUI_FONT GUI_UNI_PTR* pFont;
} MENU_PROPS;

/*********************************************************************
*
*       menu object
*/
typedef struct {
  WIDGET      Widget;
  MENU_PROPS  Props;
  GUI_ARRAY   ItemArray;
  WM_HWIN     hOwner;
  U16         Flags;
  char        IsSubmenuActive;
  int         Width;
  int         Height;
  int         Sel;
  #if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
    int DebugId;
  #endif  
} MENU_Obj;

/*********************************************************************
*
*       Macros for internal use
*
**********************************************************************
*/
#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
  MENU_Obj * MENU_h2p(MENU_Handle h);
  #define MENU_H2P(h) MENU_h2p(h)
  #define MENU_INIT_ID(p) p->DebugId = MENU_ID
#else
  #define MENU_H2P(h) (MENU_Obj *)GUI_ALLOC_h2p(h)
  #define MENU_INIT_ID(p)
#endif

/*********************************************************************
*
*       Public data (internal defaults)
*
**********************************************************************
*/

extern MENU_PROPS           MENU__DefaultProps;
extern const WIDGET_EFFECT* MENU__pDefaultEffect;

/*********************************************************************
*
*       Public functions (internal)
*
**********************************************************************
*/

void      MENU__RecalcTextWidthOfItems(MENU_Obj* pObj);
void      MENU__ResizeMenu            (MENU_Handle hObj, MENU_Obj* pObj);
unsigned  MENU__GetNumItems           (MENU_Obj* pObj);
char      MENU__SetItem               (MENU_Handle hObj, MENU_Obj* pObj, unsigned Index, const MENU_ITEM_DATA* pItemData);
void      MENU__SetItemFlags          (MENU_Obj* pObj, unsigned Index, U16 Mask, U16 Flags);
void      MENU__InvalidateItem        (MENU_Handle hObj, const MENU_Obj* pObj, unsigned Index);
int       MENU__FindItem              (MENU_Handle hObj, U16 ItemId, MENU_Handle* phMenu);
int       MENU__SendMenuMessage       (MENU_Handle hObj, WM_HWIN hDestWin, U16 MsgType, U16 ItemId);

#endif /* GUI_WINSUPPORT */
#endif /* MENU_PRIVATE_H */

/*************************** End of file ****************************/
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
