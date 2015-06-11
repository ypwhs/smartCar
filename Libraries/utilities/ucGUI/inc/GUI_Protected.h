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
File        : GUI_Protected.h
Purpose     : GUI internal declarations
---------------------------END-OF-HEADER------------------------------
*/

#ifndef  GUI_PROTECTED_H
#define  GUI_PROTECTED_H

#include "GUI.h"
#include "LCD_Protected.h"
#include "GUIDebug.h"
#if GUI_WINSUPPORT
  #include "WM_GUI.h"
#endif

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

/**********************************************************************
*
*                        Defaults for config switches
*
***********************************************************************

  The config switches below do not affect the interface in GUI.h and
  are therefor not required to be in GUI.h.
*/

/* Short address area.
   For  most compilers, this is "near" or "__near"
   We do not use this except for some CPUs which we know to always have some
   near memory, because the GUI_Context ans some other data will be declared
   to be in this short address (near) memory area as it has a major effect
   on performance.
   Please define in GUIConf.h (if you want to use it)
*/
#ifndef GUI_SADDR
  #define GUI_SADDR
#endif

#ifndef GUI_DEFAULT_FONT
  #define GUI_DEFAULT_FONT    &GUI_Font6x8
#endif

#ifndef GUI_DEFAULT_CURSOR
  #define GUI_DEFAULT_CURSOR  &GUI_CursorArrowM
#endif

#ifndef GUI_DEFAULT_BKCOLOR
  #define GUI_DEFAULT_BKCOLOR GUI_BLACK
#endif

#ifndef GUI_DEFAULT_COLOR
  #define GUI_DEFAULT_COLOR   GUI_WHITE
#endif

#ifndef GUI_X_SIGNAL_EVENT
  #define GUI_X_SIGNAL_EVENT()
#endif

#ifndef GUI_X_WAIT_EVENT
  #define GUI_X_WAIT_EVENT() GUI_X_ExecIdle()
#endif


/*      *********************************
        *                               *
        *      Angles                   *
        *                               *
        *********************************
*/

#define GUI_45DEG  512
#define GUI_90DEG  (2*GUI_45DEG)
#define GUI_180DEG (4*GUI_45DEG)
#define GUI_360DEG (8*GUI_45DEG)





/*****************************************************
*
*        Locking checks
*
******************************************************
*/

#if defined (WIN32) && defined (_DEBUG) && GUI_OS
  #define GUI_ASSERT_LOCK()   GUITASK_AssertLock()
  #define GUI_ASSERT_NO_LOCK() GUITASK_AssertNoLock()
  void GUITASK_AssertLock(void);
  void GUITASK_AssertNoLock(void);
#else
  #define GUI_ASSERT_LOCK()
  #define GUI_ASSERT_NO_LOCK()
#endif

/*****************************************************
*
*        Usage internals
*
******************************************************
*/

typedef GUI_HMEM GUI_USAGE_Handle;
typedef struct tsUSAGE_APIList tUSAGE_APIList;
typedef struct GUI_Usage GUI_USAGE;
#define GUI_USAGE_h GUI_USAGE_Handle



typedef GUI_USAGE_h tUSAGE_CreateCompatible(GUI_USAGE* p);
typedef void        tUSAGE_AddPixel        (GUI_USAGE* p, int x, int y);
typedef void        tUSAGE_AddHLine        (GUI_USAGE* p, int x0, int y0, int len);
typedef void        tUSAGE_Clear           (GUI_USAGE* p);
typedef void        tUSAGE_Delete          (GUI_USAGE_h h);
typedef int         tUSAGE_GetNextDirty    (GUI_USAGE* p, int *pxOff, int yOff);
#define GUI_USAGE_H2P(h) ((GUI_USAGE*)GUI_ALLOC_h2p(h))


void GUI_USAGE_DecUseCnt(GUI_USAGE_Handle  hUsage);

GUI_USAGE_Handle GUI_USAGE_BM_Create(int x0, int y0, int xsize, int ysize, int Flags);
void    GUI_USAGE_Select(GUI_USAGE_Handle hUsage);
void    GUI_USAGE_AddRect(GUI_USAGE*  pUsage, int x0, int y0, int xSize, int ySize);
#define GUI_USAGE_AddPixel(p, x,y)            p->pAPI->pfAddPixel(p,x,y)
#define GUI_USAGE_AddHLine(p,x,y,len)         p->pAPI->pfAddHLine(p,x,y,len)
#define GUI_USAGE_Clear(p)                    p->pAPI->pfClear(p)
#define GUI_USAGE_Delete(p)                   p->pAPI->pfDelete(p)
#define GUI_USAGE_GetNextDirty(p,pxOff, yOff) p->pAPI->pfGetNextDirty(p,pxOff, yOff)

struct tsUSAGE_APIList {
  tUSAGE_AddPixel*                pfAddPixel;
  tUSAGE_AddHLine*                pfAddHLine;
  tUSAGE_Clear*                   pfClear;
  tUSAGE_CreateCompatible*        pfCreateCompatible;
  tUSAGE_Delete*                  pfDelete;
  tUSAGE_GetNextDirty*            pfGetNextDirty;
} ;

struct GUI_Usage {
  I16P x0, y0, XSize, YSize;
  const tUSAGE_APIList *pAPI;
  I16 UseCnt;
};

/*****************************************************
*
*        GUI_MEMDEV
*
******************************************************
*/

#if GUI_SUPPORT_MEMDEV
  typedef struct {
    I16P                   x0, y0, XSize, YSize;
    int                    NumColors;
    unsigned               BytesPerLine;
    unsigned               BitsPerPixel;
    unsigned               LayerIndex;
    GUI_HMEM               hUsage;
    tLCDDEV_Color2Index*   pfColor2Index;
    tLCDDEV_Index2Color*   pfIndex2Color;
    tLCDDEV_GetIndexMask*  pfGetIndexMask;
    const tLCDDEV_APIList* pAPIList;
  } GUI_MEMDEV;

  #define      GUI_MEMDEV_H2P(h) ((GUI_MEMDEV*)GUI_ALLOC_h2p(h))

  void         GUI_MEMDEV__CopyFromLCD (GUI_MEMDEV_Handle hMem);
  void         GUI_MEMDEV__GetRect     (GUI_RECT* pRect);
  unsigned     GUI_MEMDEV__Color2Index (LCD_COLOR Color);
  LCD_COLOR    GUI_MEMDEV__Index2Color (int Index);
  unsigned int GUI_MEMDEV__GetIndexMask(void);

  GUI_MEMDEV_Handle GUI_MEMDEV__CreateFixed(int x0, int y0, int xsize, int ysize, int Flags
                                          ,const tLCDDEV_APIList * pMemDevAPI
                                          ,tLCDDEV_Color2Index*        pfColor2Index
                                          ,tLCDDEV_Index2Color*        pfIndex2Color
                                          ,tLCDDEV_GetIndexMask*       pfGetIndexMask);

#endif

/*******************************************************************
*
*                   LCD_HL_ level defines
*
********************************************************************
*/

#if GUI_SUPPORT_AA
  #define LCD_HL_DrawHLine             GUI_Context.pLCD_HL->pfDrawHLine
  #define LCD_HL_DrawPixel             GUI_Context.pLCD_HL->pfDrawPixel
#else
  #define LCD_HL_DrawHLine             LCD_DrawHLine
  #define LCD_HL_DrawPixel             LCD_DrawPixel
#endif


/*********************************************************************
*
*                     Helper functions
*
***********************************************************************
*/

#define GUI_ZEROINIT(Obj) GUI_MEMSET(Obj, 0, sizeof(Obj))
int  GUI_cos(int angle);
int  GUI_sin(int angle);
extern const U32 GUI_Pow10[10];

/* GUIAALib.c --- direct anti-aliased drawing */
int  GUI_AA_Init(int x0, int x1);
int  GUI_AA_Init_HiRes(int x0, int x1);
void GUI_AA_Exit(void);
I16 GUI_AA_HiRes2Pixel(int HiRes);

void GL_FillCircleAA_HiRes(int x0, int y0, int r);

void GUI_AA__DrawCharAA2NoTrans(int x0, int y0, int XSize, int YSize, int BytesPerLine, const U8*pData);
void GUI_AA__DrawCharAA2Trans  (int x0, int y0, int XSize, int YSize, int BytesPerLine, const U8*pData);
void GUI_AA__DrawCharAA4       (int x0, int y0, int XSize, int YSize, int BytesPerLine, const U8*pData);

/* System independent font routines */
int        GUI_SIF__GetCharDistX(U16P c);
void       GUI_SIF__GetFontInfo (const GUI_FONT GUI_UNI_PTR * pFont, GUI_FONTINFO * pfi);
char       GUI_SIF__IsInFont    (const GUI_FONT GUI_UNI_PTR * pFont, U16 c);
const U8 * GUI_SIF__GetpCharInfo(const GUI_FONT GUI_UNI_PTR * pFont, U16P c);

/* Conversion routines */
void GUI_AddHex     (U32 v, U8 Len, char**ps);
void GUI_AddBin     (U32 v, U8 Len, char**ps);
void GUI_AddDecMin  (I32 v, char**ps);
void GUI_AddDec     (I32 v, U8 Len, char**ps);
void GUI_AddDecShift(I32 v, U8 Len, U8 Shift, char**ps);
long GUI_AddSign    (long v, char**ps);
int  GUI_Long2Len   (I32 v);

#if GUI_SUPPORT_UNICODE
  #define GUI_UC__GetCharSize(sText)  GUI_Context.pUC_API->pfGetCharSize(sText)
  #define GUI_UC__GetCharCode(sText)  GUI_Context.pUC_API->pfGetCharCode(sText)
#else
  #define GUI_UC__GetCharSize(sText)  GUI_UC_GetCharSize(sText)
  #define GUI_UC__GetCharCode(sText)  GUI_UC_GetCharCode(sText)
#endif

int   GUI_UC__CalcSizeOfChar   (U16 Char);
U16   GUI_UC__GetCharCodeInc   (const char GUI_UNI_PTR ** ps);
int   GUI_UC__NumChars2NumBytes(const char GUI_UNI_PTR * s, int NumChars);
int   GUI_UC__NumBytes2NumChars(const char GUI_UNI_PTR * s, int NumBytes);

int  GUI__GetLineNumChars  (const char GUI_UNI_PTR *s, int MaxNumChars);
int  GUI__GetNumChars      (const char GUI_UNI_PTR *s);
int  GUI__GetLineDistX     (const char GUI_UNI_PTR *s, int Len);
int  GUI__GetFontSizeY     (void);
int  GUI__HandleEOLine     (const char GUI_UNI_PTR **ps);
void GUI__DispLine         (const char GUI_UNI_PTR *s, int Len, const GUI_RECT* pr);
void GUI__AddSpaceHex      (U32 v, U8 Len, char**ps);
void GUI__CalcTextRect     (const char GUI_UNI_PTR* pText, const GUI_RECT* pTextRectIn, GUI_RECT* pTextRectOut, int TextAlign);

int  GUI__WrapGetNumCharsDisp       (const char GUI_UNI_PTR * pText, int xSize, GUI_WRAPMODE WrapMode);
int  GUI__WrapGetNumCharsToNextLine (const char GUI_UNI_PTR * pText, int xSize, GUI_WRAPMODE WrapMode);
int  GUI__WrapGetNumBytesToNextLine (const char GUI_UNI_PTR * pText, int xSize, GUI_WRAPMODE WrapMode);
void GUI__memset    (U8 * p, U8 Fill, int NumBytes);
void GUI__memset16  (U16* p, U16 Fill, int NumWords);
int  GUI__strlen    (const char GUI_UNI_PTR * s);
int  GUI__strcmp    (const char GUI_UNI_PTR * s0, const char GUI_UNI_PTR * s1);
int  GUI__strcmp_hp (GUI_HMEM hs0, const char GUI_UNI_PTR * s1);

/* Get cursor position */
int  GUI__GetCursorPosX     (const char GUI_UNI_PTR * s, int Index, int MaxNumChars);
int  GUI__GetCursorPosChar  (const char GUI_UNI_PTR * s, int x, int NumCharsToNextLine);
U16  GUI__GetCursorCharacter(const char GUI_UNI_PTR * s, int Index, int MaxNumChars, int * pIsRTL);

/* Arabic support */
U16  GUI__GetPresentationForm     (U16 Char, U16 Next, U16 Prev, int * pIgnoreNext);
void GUI__DispLineArabic          (const char GUI_UNI_PTR * s, int MaxNumChars, const GUI_RECT * pRect);
int  GUI__GetLineDistXArabic      (const char GUI_UNI_PTR * s, int MaxNumChars);
int  GUI__IsArabicCharacter       (U16 c);
int  GUI__GetCursorPosXArabic     (const char GUI_UNI_PTR * s, int Index, int MaxNumChars);
int  GUI__GetCursorPosCharArabic  (const char GUI_UNI_PTR * s, int x, int MaxNumChars);
U16  GUI__GetCursorCharacterArabic(const char GUI_UNI_PTR * s, int Index, int MaxNumChars, int * pIsRTL);

/* Reading data routines */
U16 GUI__Read16(const U8 ** ppData);
U32 GUI__Read32(const U8 ** ppData);

/* Virtual screen support */
void GUI__GetOrg(int * px, int * py);

/*********************************************************************
*
*             2d - GL
*
**********************************************************************
*/

void GL_DispChar         (U16 c);
void GL_DrawArc          (int x0, int y0, int rx, int ry, int a0, int a1);
void GL_DrawBitmap       (const GUI_BITMAP GUI_UNI_PTR * pBM, int x0, int y0);
void GL_DrawCircle       (int x0, int y0, int r);
void GL_DrawEllipse      (int x0, int y0, int rx, int ry);
void GL_DrawHLine        (int y0, int x0, int x1);
void GL_DrawPolygon      (const GUI_POINT* pPoints, int NumPoints, int x0, int y0);
void GL_DrawPoint        (int x, int y);
void GL_DrawLine1        (int x0, int y0, int x1, int y1);
void GL_DrawLine1Ex      (int x0, int y0, int x1, int y1, unsigned * pPixelCnt);
void GL_DrawLineRel      (int dx, int dy);
void GL_DrawLineTo       (int x, int y);
void GL_DrawLineToEx     (int x, int y, unsigned * pPixelCnt);
void GL_DrawLine         (int x0, int y0, int x1, int y1);
void GL_DrawLineEx       (int x0, int y0, int x1, int y1, unsigned * pPixelCnt);
void GL_MoveTo           (int x, int y);
void GL_FillCircle       (int x0, int y0, int r);
void GL_FillCircleAA     (int x0, int y0, int r);
void GL_FillEllipse      (int x0, int y0, int rx, int ry);
void GL_FillPolygon      (const GUI_POINT* pPoints, int NumPoints, int x0, int y0);
void GL_SetDefault       (void);

/************************************************************
*
*                 Callback pointers for dynamic linkage
*
*************************************************************
Dynamic linkage pointers reduces configuration hassles.
*/
typedef int  GUI_tfTimer(void);
typedef char GUI_CURSOR_tfTempHide  (const GUI_RECT* pRect);
typedef void GUI_CURSOR_tfTempUnhide(void);
typedef int  WM_tfHandlePID(void);


/************************************************************
*
*        Cursors 
*
*************************************************************
*/

extern GUI_CONST_STORAGE unsigned char  GUI_Pixels_ArrowS[45];
extern GUI_CONST_STORAGE unsigned char  GUI_Pixels_ArrowM[60];
extern GUI_CONST_STORAGE unsigned char  GUI_Pixels_ArrowL[150];
extern GUI_CONST_STORAGE unsigned char  GUI_Pixels_CrossS[33];
extern GUI_CONST_STORAGE unsigned char  GUI_Pixels_CrossM[126];
extern GUI_CONST_STORAGE unsigned char  GUI_Pixels_CrossL[248];
extern GUI_CONST_STORAGE unsigned char  GUI_PixelsHeaderM[5 * 17];


extern GUI_CONST_STORAGE GUI_LOGPALETTE GUI_CursorPal;
extern GUI_CONST_STORAGE GUI_LOGPALETTE GUI_CursorPalI;

/************************************************************
*
*        Text rotation
*
*************************************************************
*/

extern GUI_RECT  GUI_RectDispString; /* Used by LCD_Rotate...() and GUI_DispStringInRect() */

/*********************************************************************
*
*       Multitasking support
*
**********************************************************************
*/
extern int GUITASK__EntranceCnt;

/*********************************************************************
*
*             Bitmap related functions
*
**********************************************************************
*/

int       GUI_GetBitmapPixelIndex(const GUI_BITMAP GUI_UNI_PTR * pBMP, unsigned x, unsigned y);
GUI_COLOR GUI_GetBitmapPixelColor(const GUI_BITMAP GUI_UNI_PTR * pBMP, unsigned x, unsigned y);

void      GUI__DrawBitmap16bpp(int x0, int y0, int xsize, int ysize, const U8 GUI_UNI_PTR * pPixel, const LCD_LOGPALETTE GUI_UNI_PTR * pLogPal, int xMag, int yMag, tLCDDEV_Index2Color * pfIndex2Color);

extern const GUI_UC_ENC_APILIST GUI_UC_None;

/************************************************************
*
*         EXTERNs for GL_CORE
*
*************************************************************
*/

#ifdef  GL_CORE_C
  #define GUI_EXTERN
#else
  #define GUI_EXTERN extern
#endif

GUI_EXTERN GUI_SADDR GUI_CONTEXT        GUI_Context;        /* Thread wide globals */
GUI_EXTERN GUI_SADDR char               GUI_DecChar;
GUI_EXTERN           GUI_tfTimer*       GUI_pfTimerExec;
GUI_EXTERN           WM_tfHandlePID*    WM_pfHandlePID;
GUI_EXTERN      void (*GUI_pfDispCharStyle)(U16 Char);

#if GUI_COMPILER_SUPPORTS_FP && GUI_SUPPORT_ROTATION
  GUI_EXTERN const     tLCD_APIList *     GUI_pLCD_APIList;   /* Used for rotating text */
#endif

#if GUI_SUPPORT_CURSOR
  GUI_EXTERN GUI_CURSOR_tfTempHide*   GUI_CURSOR_pfTempHide;
  GUI_EXTERN GUI_CURSOR_tfTempUnhide* GUI_CURSOR_pfTempUnhide;
#endif

GUI_EXTERN I16 GUI_OrgX, GUI_OrgY;
GUI_EXTERN U8  GUI_MoveRTL;

#undef GUI_EXTERN


#if defined(__cplusplus)
}
#endif 

#endif   /* Avoid multiple inclusion */

/*************************** End of file ****************************/
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
