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
File        : GUIType.h
Purpose     : Include file define the types used for GUI
---------------------------END-OF-HEADER------------------------------

Attention : Do not modify this file ! If you do, you will not
            be able do update to a later GUI version !

*/

#ifndef  GUITYPE_H_INCLUDED
#define  GUITYPE_H_INCLUDED

#include "LCD.h"
#include "GUIConf.h"

/*      *************************************************************
        *                                                           *
        *                Simple types                               *
        *                                                           *
        *************************************************************
*/

typedef const char *  GUI_ConstString;


/*      *************************************************************
        *                                                           *
        *                Structures                                 *
        *                                                           *
        *************************************************************
*/

typedef LCD_COLOR       GUI_COLOR;
typedef LCD_LOGPALETTE  GUI_LOGPALETTE;
typedef LCD_DRAWMODE    GUI_DRAWMODE;
typedef LCD_RECT        GUI_RECT;

typedef struct {
  void      (* pfDraw)(int x0,int y0,int xsize, int ysize, const U8 GUI_UNI_PTR * pPixel, const LCD_LOGPALETTE GUI_UNI_PTR * pLogPal, int xMag, int yMag);
  GUI_COLOR (* pfIndex2Color)(int Index);
} GUI_BITMAP_METHODS;

typedef struct {
  U16P XSize;
  U16P YSize;
  U16P BytesPerLine;
  U16P BitsPerPixel;
  const U8 GUI_UNI_PTR * pData;
  const GUI_LOGPALETTE GUI_UNI_PTR * pPal;
  const GUI_BITMAP_METHODS * pMethods;
} GUI_BITMAP;

/* This structure may not be changed because the data that it
   expects is read in binary form (via any kind of interface,
   at runtime).
   This structure should therefor not be changed.
*/
typedef struct {
  U16 ID;           /* Version 1.00 => 100*/
  U16 Version;
  U16 XSize;
  U16 YSize;
  U16 BytesPerLine;
  U16 BitsPerPixel;
  U16 NumColors;
  U16 HasTrans;
} GUI_BITMAP_STREAM;

typedef struct {
  int x,y;
  unsigned char Pressed;
} GUI_PID_STATE;

typedef struct {
  int xPos;
  int yPos;
  int xSize;
  int ySize;
  int Delay;
} GUI_GIF_IMAGE_INFO;

typedef struct {
  int xSize;
  int ySize;
  int NumImages;
} GUI_GIF_INFO;

/*
      ****************************************
      *                                      *
      *      FONT structures (new in V1.10)  *
      *                                      *
      ****************************************
*/

/* Translation list. Translates a character code into up to 2
   indices of images to display on top of each other;
   'á' -> index('a'), index('´') */
typedef struct {
  I16P c0;
  I16P c1;
} GUI_FONT_TRANSLIST;

typedef struct {
  U16P FirstChar;
  U16P LastChar;
  const GUI_FONT_TRANSLIST GUI_UNI_PTR * pList;
} GUI_FONT_TRANSINFO;

typedef struct {
  U8 XSize;
  U8 XDist;
  U8 BytesPerLine;
  const unsigned char GUI_UNI_PTR * pData;
} GUI_CHARINFO;

typedef struct GUI_FONT_PROP {
  U16P First;                                /* first character               */
  U16P Last;                                 /* last character                */
  const GUI_CHARINFO GUI_UNI_PTR * paCharInfo;            /* address of first character    */
  const struct GUI_FONT_PROP GUI_UNI_PTR * pNext;        /* pointer to next */
} GUI_FONT_PROP;

typedef struct {
  const unsigned char GUI_UNI_PTR * pData;
  const U8 GUI_UNI_PTR * pTransData;
  const GUI_FONT_TRANSINFO GUI_UNI_PTR * pTrans;
  U16P FirstChar;
  U16P LastChar;
  U8 XSize;
  U8 XDist;
  U8 BytesPerLine;
} GUI_FONT_MONO;


typedef struct GUI_FONT_INFO {
  U16P First;                        /* first character               */
  U16P Last;                         /* last character                */
  const GUI_CHARINFO* paCharInfo;    /* address of first character    */
  const struct GUI_FONT_INFO* pNext; /* pointer to next */
} GUI_FONT_INFO;

/*
      ****************************************
      *                                      *
      *      FONT info structure             *
      *                                      *
      ****************************************

This structure is used when retrieving information about a font.
It is designed for future expansion without incompatibilities.
*/
typedef struct {
  U16 Flags;
  U8 Baseline;
  U8 LHeight;     /* height of a small lower case character (a,x) */
  U8 CHeight;     /* height of a small upper case character (A,X) */
} GUI_FONTINFO;

#define GUI_FONTINFO_FLAG_PROP (1<<0)    /* Is proportional */
#define GUI_FONTINFO_FLAG_MONO (1<<1)    /* Is monospaced */
#define GUI_FONTINFO_FLAG_AA   (1<<2)    /* Is an antialiased font */
#define GUI_FONTINFO_FLAG_AA2  (1<<3)    /* Is an antialiased font, 2bpp */
#define GUI_FONTINFO_FLAG_AA4  (1<<4)    /* Is an antialiased font, 4bpp */


/**********************************************************************
*
*         UNICODE Encoding
*
***********************************************************************
*/
typedef U16  tGUI_GetCharCode(const char GUI_UNI_PTR *s);
typedef int  tGUI_GetCharSize(const char GUI_UNI_PTR *s);
typedef int  tGUI_CalcSizeOfChar(U16 Char);
typedef int  tGUI_Encode(char *s, U16 Char);

typedef struct {
  tGUI_GetCharCode*            pfGetCharCode;
  tGUI_GetCharSize*            pfGetCharSize;
  tGUI_CalcSizeOfChar*         pfCalcSizeOfChar;
  tGUI_Encode*                 pfEncode;
} GUI_UC_ENC_APILIST;

/**********************************************************************
*
*                 FONT Encoding
*
***********************************************************************
*/

typedef int  tGUI_GetLineDistX(const char GUI_UNI_PTR *s, int Len);
typedef int  tGUI_GetLineLen(const char GUI_UNI_PTR *s, int MaxLen);
typedef void tGL_DispLine(const char GUI_UNI_PTR *s, int Len);

typedef struct {
  tGUI_GetLineDistX*          pfGetLineDistX;
  tGUI_GetLineLen*            pfGetLineLen;
  tGL_DispLine*               pfDispLine;
} tGUI_ENC_APIList;

extern const tGUI_ENC_APIList GUI_ENC_APIList_SJIS;


/*
      ****************************************
      *                                      *
      *      FONT methods                    *
      *                                      *
      ****************************************

The parameter to the methods called pFont should be of type
GUI_FONT, but unfortunately a lot of compilers can not handle
forward declarations right ...
So it ends up to be a void pointer.
*/

typedef struct GUI_FONT GUI_FONT;

typedef void GUI_DISPCHAR(U16 c);
typedef int  GUI_GETCHARDISTX(U16P c);
typedef void GUI_GETFONTINFO(const GUI_FONT GUI_UNI_PTR * pFont, GUI_FONTINFO * pfi);
typedef char GUI_ISINFONT   (const GUI_FONT GUI_UNI_PTR * pFont, U16 c);

#define DECLARE_FONT(Type)                                     \
void GUI##Type##_DispChar    (U16P c);                         \
int  GUI##Type##_GetCharDistX(U16P c);                         \
void GUI##Type##_GetFontInfo (const GUI_FONT GUI_UNI_PTR * pFont, GUI_FONTINFO * pfi); \
char GUI##Type##_IsInFont    (const GUI_FONT GUI_UNI_PTR * pFont, U16 c)

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

/* MONO: Monospaced fonts */
DECLARE_FONT(MONO);
#define GUI_FONTTYPE_MONO       \
  GUIMONO_DispChar,             \
	GUIMONO_GetCharDistX,         \
	GUIMONO_GetFontInfo,          \
	GUIMONO_IsInFont,             \
  (tGUI_ENC_APIList*)0

/* PROP: Proportional fonts */
DECLARE_FONT(PROP);
#define GUI_FONTTYPE_PROP       \
  GUIPROP_DispChar,             \
	GUIPROP_GetCharDistX,         \
	GUIPROP_GetFontInfo,          \
	GUIPROP_IsInFont,             \
  (tGUI_ENC_APIList*)0

/* PROP: Proportional fonts SJIS */
DECLARE_FONT(PROP);
#define GUI_FONTTYPE_PROP_SJIS  \
  GUIPROP_DispChar,             \
	GUIPROP_GetCharDistX,         \
	GUIPROP_GetFontInfo,          \
	GUIPROP_IsInFont,             \
  &GUI_ENC_APIList_SJIS

/* PROPAA: Proportional, antialiased fonts */
DECLARE_FONT(PROPAA);
#define GUI_FONTTYPE_PROPAA       \
  GUIPROPAA_DispChar,             \
	GUIPROPAA_GetCharDistX,         \
	GUIPROPAA_GetFontInfo,          \
	GUIPROPAA_IsInFont,             \
  (tGUI_ENC_APIList*)0

/* PROPAA: Proportional, antialiased fonts, 2bpp */
DECLARE_FONT(PROP_AA2);
#define GUI_FONTTYPE_PROP_AA2       \
  GUIPROP_AA2_DispChar,             \
	GUIPROP_AA2_GetCharDistX,         \
	GUIPROP_AA2_GetFontInfo,          \
	GUIPROP_AA2_IsInFont,             \
  (tGUI_ENC_APIList*)0

/* PROPAA: Proportional, antialiased fonts, 2bpp, SJIS encoding */
DECLARE_FONT(PROP_AA2);
#define GUI_FONTTYPE_PROP_AA2_SJIS  \
  GUIPROP_AA2_DispChar,             \
	GUIPROP_AA2_GetCharDistX,         \
	GUIPROP_AA2_GetFontInfo,          \
	GUIPROP_AA2_IsInFont,             \
  GUI_ENCODE_SJIS

/* PROPAA: Proportional, antialiased fonts, 4bpp */
DECLARE_FONT(PROP_AA4);
#define GUI_FONTTYPE_PROP_AA4       \
  GUIPROP_AA4_DispChar,             \
	GUIPROP_AA4_GetCharDistX,         \
	GUIPROP_AA4_GetFontInfo,          \
	GUIPROP_AA4_IsInFont,             \
  (tGUI_ENC_APIList*)0

/* PROPAA: Proportional, antialiased fonts, 4bpp, SJIS encoding */
DECLARE_FONT(PROP_AA4);
#define GUI_FONTTYPE_PROP_AA4_SJIS  \
  GUIPROP_AA4_DispChar,             \
	GUIPROP_AA4_GetCharDistX,         \
	GUIPROP_AA4_GetFontInfo,          \
	GUIPROP_AA4_IsInFont,             \
  GUI_ENCODE_SJIS

#if defined(__cplusplus)
  }
#endif

struct GUI_FONT {
  GUI_DISPCHAR*     pfDispChar; 
  GUI_GETCHARDISTX* pfGetCharDistX; 
  GUI_GETFONTINFO*  pfGetFontInfo; 
  GUI_ISINFONT*     pfIsInFont;
  const tGUI_ENC_APIList* pafEncode;
  U8 YSize;
  U8 YDist;
  U8 XMag;
  U8 YMag;
  union {
    const void          GUI_UNI_PTR * pFontData;
    const GUI_FONT_MONO GUI_UNI_PTR * pMono;
    const GUI_FONT_PROP GUI_UNI_PTR * pProp;
  } p;
  U8 Baseline;
  U8 LHeight;     /* height of a small lower case character (a,x) */
  U8 CHeight;     /* height of a small upper case character (A,X) */
};

/*********************************************************************
*
*       Position independent font structures
*/
typedef struct {
  U32 ID;           /* Font file ID */
  U16 YSize;        /* Height of font */
  U16 YDist;        /* Space of font Y */
  U16 Baseline;     /* Index of baseline */
  U16 LHeight;      /* Height of a small lower case character (a) */
  U16 CHeight;      /* Height of a upper case character (A) */
  U16 NumAreas;     /* Number of character areas */
} GUI_SI_FONT;

typedef struct {
  U16 First;        /* Index of first character */
  U16 Last;         /* Index of last character */
} GUI_SIF_CHAR_AREA;

typedef struct {
  U16 XSize;        /* Size of bitmap data in X */
  U16 XDist;        /* Number of pixels for increment cursor in X */
  U16 BytesPerLine; /* Number of bytes per line */
  U16 Dummy;
  U32 OffData;      /* Offset of pixel data */
} GUI_SIF_CHARINFO;

typedef struct tGUI_SIF_APIList_struct {
  GUI_DISPCHAR     * pDispChar;
  GUI_GETCHARDISTX * pGetCharDistX;
  GUI_GETFONTINFO  * pGetFontInfo;
  GUI_ISINFONT     * pIsInFont;
} tGUI_SIF_APIList;

#define GUI_SIF_TYPE          tGUI_SIF_APIList
#define GUI_SIF_TYPE_PROP     &GUI_SIF_APIList_Prop
#define GUI_SIF_TYPE_PROP_AA2 &GUI_SIF_APIList_Prop_AA2
#define GUI_SIF_TYPE_PROP_AA4 &GUI_SIF_APIList_Prop_AA4

/*
      *********************************
      *                               *
      *      Typedefs                 *
      *                               *
      *********************************
*/

#ifndef     GUI_HMEM
  #if GUI_MAXBLOCKS < 32768
    #define     GUI_HMEM        I16P
  #else
    #define     GUI_HMEM        I32
  #endif
#endif
#define     GUI_HMEM_NULL     (0)
typedef     GUI_HMEM      GUI_HWIN;
#endif  /* GUITYPE_H_INCLUDED */

/*************************** End of file ****************************/
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
