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
File        : WM_Intern.h
Purpose     : Windows manager internal include
----------------------------------------------------------------------
*/

#ifndef WM_INTERN_H            /* Make sure we only include it once */
#define WM_INTERN_H            /* Make sure we only include it once */

#include "WM.h"
#include "GUI_Protected.h"       /* For GUI_Context */


#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

#if GUI_WINSUPPORT

/*********************************************************************
*
*       defines
*
**********************************************************************

  The following could be placed in a file of its own as it is not
  used outside of the window manager

*/
/* Basic Windows status flags.
   For module-internally use only !
*/
#define WM_SF_HASTRANS          WM_CF_HASTRANS
#define WM_SF_MEMDEV            WM_CF_MEMDEV
#define WM_SF_MEMDEV_ON_REDRAW  WM_CF_MEMDEV_ON_REDRAW
#define WM_SF_DISABLED          WM_CF_DISABLED  /* Disabled: Does not receive PID (mouse & touch) input */
#define WM_SF_ISVIS             WM_CF_SHOW      /* Is visible flag */

#define WM_SF_STAYONTOP         WM_CF_STAYONTOP
#define WM_SF_LATE_CLIP         WM_CF_LATE_CLIP
#define WM_SF_ANCHOR_RIGHT      WM_CF_ANCHOR_RIGHT
#define WM_SF_ANCHOR_BOTTOM     WM_CF_ANCHOR_BOTTOM
#define WM_SF_ANCHOR_LEFT       WM_CF_ANCHOR_LEFT
#define WM_SF_ANCHOR_TOP        WM_CF_ANCHOR_TOP

#define WM_SF_INVALID           WM_CF_ACTIVATE  /* We reuse this flag, as it is create only and Invalid is status only */

#define WM_SF_CONST_OUTLINE     WM_CF_CONST_OUTLINE       /* Constant outline.*/

#define WM_HANDLE2PTR(hWin) ((WM_Obj*)GUI_ALLOC_h2p(hWin))    /* older form ... to be eliminated */
#define WM_H2P(hWin)        ((WM_Obj*)GUI_ALLOC_h2p(hWin))


#if GUI_DEBUG_LEVEL  >= GUI_DEBUG_LEVEL_LOG_WARNINGS
  #define WM_ASSERT_NOT_IN_PAINT() { if (WM__PaintCallbackCnt) \
                                       GUI_DEBUG_ERROROUT("Function may not be called from within a paint event"); \
                                   }
#else
  #define WM_ASSERT_NOT_IN_PAINT()
#endif

/*********************************************************************
*
*       Data types & structures
*
**********************************************************************
*/
typedef struct {
  WM_HWIN hWin;
  WM_Obj* pWin;
} WM_PAINTINFO;

typedef struct {
  WM_HWIN hOld;
  WM_HWIN hNew;
} WM_NOTIFY_CHILD_HAS_FOCUS_INFO;

typedef struct WM_CRITICAL_HANDLE {
  struct  WM_CRITICAL_HANDLE * pNext;
  volatile WM_HWIN hWin;
} WM_CRITICAL_HANDLE;

/*********************************************************************
*
*       Data (extern)
*
**********************************************************************
*/
extern U16                    WM__CreateFlags;
extern WM_HWIN                WM__hCapture;
extern WM_HWIN                WM__hWinFocus;
extern char                   WM__CaptureReleaseAuto;
extern WM_tfPollPID*          WM_pfPollPID;
extern U8                     WM__PaintCallbackCnt;      /* Public for assertions only */
extern GUI_PID_STATE          WM_PID__StateLast;
extern U8                     WM_IsActive;

#if WM_SUPPORT_TRANSPARENCY
  extern int     WM__TransWindowCnt;
  extern WM_HWIN WM__hATransWindow;
#endif

#if WM_SUPPORT_DIAG
  extern void (*WM__pfShowInvalid)(WM_HWIN hWin);
#endif

extern WM_CRITICAL_HANDLE     WM__CHWinModal;
extern WM_CRITICAL_HANDLE     WM__CHWinLast;

#if GUI_SUPPORT_MOUSE
  extern WM_CRITICAL_HANDLE   WM__CHWinMouseOver;
#endif

#ifdef WM_C
  #define GUI_EXTERN
#else
  #define GUI_EXTERN extern
#endif

GUI_EXTERN U16     WM__NumWindows;
GUI_EXTERN U16     WM__NumInvalidWindows;
GUI_EXTERN WM_HWIN WM__FirstWin;
GUI_EXTERN WM_CRITICAL_HANDLE*  WM__pFirstCriticalHandle;
#undef GUI_EXTERN

/*********************************************************************
*
*       Prototypes
*
**********************************************************************
*/

void    WM__ActivateClipRect        (void);
int     WM__ClipAtParentBorders     (GUI_RECT* pRect, WM_HWIN hWin);
void    WM__Client2Screen           (const WM_Obj* pWin, GUI_RECT *pRect);
void    WM__DetachWindow            (WM_HWIN hChild);
void    WM__ForEachDesc(WM_HWIN hWin, WM_tfForEach * pcb, void * pData);
void    WM__GetClientRectWin        (const WM_Obj* pWin, GUI_RECT* pRect);
WM_HWIN WM__GetFirstSibling         (WM_HWIN hWin);
WM_HWIN WM__GetFocussedChild        (WM_HWIN hWin);
int     WM__GetHasFocus             (WM_HWIN hWin);
WM_HWIN WM__GetLastSibling          (WM_HWIN hWin);
WM_HWIN WM__GetPrevSibling          (WM_HWIN hWin);
int     WM__GetWindowSizeX          (const WM_Obj* pWin);
int     WM__GetWindowSizeY          (const WM_Obj* pWin);
void    WM__InsertWindowIntoList    (WM_HWIN hWin, WM_HWIN hParent);
void    WM__Invalidate1Abs          (WM_HWIN hWin, const GUI_RECT * pRect);
void    WM__InvalidateAreaBelow     (const GUI_RECT* pRect, WM_HWIN StopWin);
void    WM__InvalidateRectEx        (const GUI_RECT* pInvalidRect, WM_HWIN hParent, WM_HWIN hStop);
void    WM__InvalidateTransAreaAbove(const GUI_RECT* pRect, WM_HWIN StopWin);
int     WM__IntersectRect           (GUI_RECT* pDest, const GUI_RECT* pr0, const GUI_RECT* pr1);
int     WM__IsAncestor              (WM_HWIN hChild, WM_HWIN hParent);
int     WM__IsAncestorOrSelf        (WM_HWIN hChild, WM_HWIN hParent);
int     WM__IsChild                 (WM_HWIN hWin, WM_HWIN hParent);
int     WM__IsEnabled               (WM_HWIN hWin);
int     WM__IsInModalArea           (WM_HWIN hWin);
int     WM__IsInWindow              (WM_Obj * pWin, int x, int y);
int     WM__IsWindow                (WM_HWIN hWin);
void    WM__LeaveIVRSearch          (void);
void    WM__MoveTo                  (WM_HWIN hWin, int x, int y);
void    WM__MoveWindow              (WM_HWIN hWin, int dx, int dy);
void    WM__NotifyVisChanged        (WM_HWIN hWin, GUI_RECT * pRect);
int     WM__RectIsNZ                (const GUI_RECT* pr);
void    WM__RemoveWindowFromList    (WM_HWIN hWin);
void    WM__RemoveFromLinList       (WM_HWIN hWin);
void    WM__Screen2Client           (const WM_Obj* pWin, GUI_RECT *pRect);
void    WM__SendMsgNoData           (WM_HWIN hWin, U8 MsgId);
void    WM__SendMessage             (WM_HWIN hWin, WM_MESSAGE* pm);
void    WM__SendMessageIfEnabled    (WM_HWIN hWin, WM_MESSAGE* pm);
void    WM__SendMessageNoPara       (WM_HWIN hWin, int MsgId);
void    WM__SendPIDMessage          (WM_HWIN hWin, WM_MESSAGE* pMsg);
int     WM__SetScrollbarH           (WM_HWIN hWin, int OnOff);
int     WM__SetScrollbarV           (WM_HWIN hWin, int OnOff);
void    WM__UpdateChildPositions    (WM_Obj* pObj, int dx0, int dy0, int dx1, int dy1);
void    WM_PID__GetPrevState        (GUI_PID_STATE* pPrevState);
void    WM_PID__SetPrevState        (GUI_PID_STATE* pPrevState);
void    WM__SendTouchMessage        (WM_HWIN hWin, WM_MESSAGE* pMsg);

U16     WM_GetFlags(WM_HWIN hWin);
void    WM__PaintWinAndOverlays     (WM_PAINTINFO* pInfo);
int     WM__Paint                   (WM_HWIN hWin, WM_Obj* pWin);
void    WM__AddCriticalHandle       (WM_CRITICAL_HANDLE* pCH);
void    WM__RemoveCriticalHandle    (WM_CRITICAL_HANDLE* pCH);

#endif   /* GUI_WINSUPPORT */

#if defined(__cplusplus)
  }
#endif

#endif   /* WM_INTERN_H */

/*************************** End of file ****************************/
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
