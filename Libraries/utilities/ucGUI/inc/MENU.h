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
File        : MENU.h
Purpose     : MENU include
--------------------END-OF-HEADER-------------------------------------
*/

#ifndef MENU_H
#define MENU_H

#include "WM.h"
#include "WIDGET.h"
#include "DIALOG_Intern.h"      /* Req. for Create indirect data structure */

#if GUI_WINSUPPORT

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
/*********************************************************************
*
*       Create flags
*/
#define MENU_CF_HORIZONTAL            (0<<0)
#define MENU_CF_VERTICAL              (1<<0)
#define MENU_CF_OPEN_ON_POINTEROVER   (1<<1)  /* Normally a menu opens first when clicked on it       */
#define MENU_CF_CLOSE_ON_SECOND_CLICK (1<<2)  /* Normally a menu closes only when clicked outside it  */
#define MENU_CF_HIDE_DISABLED_SEL     (1<<3)  /* Hides the selection when a disabled item is selected */

/*********************************************************************
*
*       Menu item flags
*/
#define MENU_IF_DISABLED              (1<<0)  /* Indicates that item is disabled    */
#define MENU_IF_SEPARATOR             (1<<1)  /* Indicates that item is a separator */

/*********************************************************************
*
*       Color indices
*/
#define MENU_CI_ENABLED           0
#define MENU_CI_SELECTED          1
#define MENU_CI_DISABLED          2
#define MENU_CI_DISABLED_SEL      3
#define MENU_CI_ACTIVE_SUBMENU    4

/*********************************************************************
*
*       Border indices
*/
#define MENU_BI_LEFT              0
#define MENU_BI_RIGHT             1
#define MENU_BI_TOP               2
#define MENU_BI_BOTTOM            3

/*********************************************************************
*
*       Message types
*/
#define MENU_ON_ITEMSELECT        0   /* Send to owner when selecting a menu item                  */
#define MENU_ON_INITMENU          1   /* Send to owner when for the first time selecting a submenu */
#define MENU_ON_INITSUBMENU       2   /* Send to owner when selecting a submenu                    */
#define MENU_ON_OPEN              3   /* Internal message of menu widget (only send to submenus)   */
#define MENU_ON_CLOSE             4   /* Internal message of menu widget (only send to submenus)   */
#define MENU_IS_MENU              5   /* Internal message of menu widget. Owner must call          */
                                      /* WM_DefaultProc() when not handle the message.             */

/*********************************************************************
*
*       Types
*
**********************************************************************
*/

typedef WM_HMEM MENU_Handle;

/*********************************************************************
*
*       menu message data
*/
typedef struct {
  U16 MsgType;
  U16 ItemId;
} MENU_MSG_DATA;

/*********************************************************************
*
*       menu item data
*/
typedef struct {
  const char* pText;
  U16         Id;
  U16         Flags;
  MENU_Handle hSubmenu;
} MENU_ITEM_DATA;

/*********************************************************************
*
*       Create functions
*
**********************************************************************
*/

MENU_Handle MENU_CreateIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK* cb);
MENU_Handle MENU_CreateEx      (int x0, int y0, int xSize, int ySize, WM_HWIN hParent, int WinFlags, int ExFlags, int Id);

/*********************************************************************
*
*       The callback ...
*
* Do not call it directly ! It is only to be used from within an
* overwritten callback.
*/
void MENU_Callback(WM_MESSAGE * pMsg);

/*********************************************************************
*
*       Standard member functions
*
**********************************************************************
*/

#define MENU_EnableMemdev(hObj)   WM_EnableMemdev(hObj)
#define MENU_DisableMemdev(hObj)  WM_DisableMemdev(hObj)
#define MENU_Delete(hObj)         WM_DeleteWindow(hObj)
#define MENU_Paint(hObj)          WM_Paint(hObj)
#define MENU_Invalidate(hObj)     WM_InvalidateWindow(hObj)

/*********************************************************************
*
*       Individual member functions
*
**********************************************************************
*/

void      MENU_Attach           (MENU_Handle hObj, WM_HWIN hDestWin, int x, int y, int xSize, int ySize, int Flags);
void      MENU_Popup            (MENU_Handle hObj, WM_HWIN hDestWin, int x, int y, int xSize, int ySize, int Flags);
void      MENU_SetOwner         (MENU_Handle hObj, WM_HWIN hOwner);

void      MENU_AddItem          (MENU_Handle hObj, const MENU_ITEM_DATA* pItemData);
void      MENU_DeleteItem       (MENU_Handle hObj, U16 ItemId);
void      MENU_InsertItem       (MENU_Handle hObj, U16 ItemId, const MENU_ITEM_DATA* pItemData);
void      MENU_SetItem          (MENU_Handle hObj, U16 ItemId, const MENU_ITEM_DATA* pItemData);
void      MENU_GetItem          (MENU_Handle hObj, U16 ItemId, MENU_ITEM_DATA* pItemData);
void      MENU_GetItemText      (MENU_Handle hObj, U16 ItemId, char* pBuffer, unsigned BufferSize);
unsigned  MENU_GetNumItems      (MENU_Handle hObj);

void      MENU_DisableItem      (MENU_Handle hObj, U16 ItemId);
void      MENU_EnableItem       (MENU_Handle hObj, U16 ItemId);

/*********************************************************************
*
*       Member functions, get / set properties
*
**********************************************************************
*/

void      MENU_SetTextColor     (MENU_Handle hObj, unsigned ColorIndex, GUI_COLOR Color);
void      MENU_SetBkColor       (MENU_Handle hObj, unsigned ColorIndex, GUI_COLOR Color);
void      MENU_SetBorderSize    (MENU_Handle hObj, unsigned BorderIndex, U8 BorderSize);
void      MENU_SetFont          (MENU_Handle hObj, const GUI_FONT GUI_UNI_PTR* pFont);

/*********************************************************************
*
*       Member functions, get / set defaults
*
**********************************************************************
*/

GUI_COLOR                   MENU_GetDefaultTextColor  (unsigned ColorIndex);
GUI_COLOR                   MENU_GetDefaultBkColor    (unsigned ColorIndex);
U8                          MENU_GetDefaultBorderSize (unsigned BorderIndex);
const WIDGET_EFFECT*        MENU_GetDefaultEffect     (void);
const GUI_FONT GUI_UNI_PTR* MENU_GetDefaultFont       (void);
void                        MENU_SetDefaultTextColor  (unsigned ColorIndex, GUI_COLOR Color);
void                        MENU_SetDefaultBkColor    (unsigned ColorIndex, GUI_COLOR Color);
void                        MENU_SetDefaultBorderSize (unsigned BorderIndex, U8 BorderSize);
void                        MENU_SetDefaultEffect     (const WIDGET_EFFECT* pEffect);
void                        MENU_SetDefaultFont       (const GUI_FONT GUI_UNI_PTR* pFont);

#if defined(__cplusplus)
  }
#endif

#endif   /* GUI_WINSUPPORT */
#endif   /* MENU_H */

/*************************** End of file ****************************/
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
