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
File        : LCD_ConfDefaults.h
Purpose     : Valid LCD configuration and defaults
----------------------------------------------------------------------
*/

#ifndef LCD_CONFIG_DEFAULTS_H
#define LCD_CONFIG_DEFAULTS_H

#include "LCDConf.h"            /* Configuration header file */
#include "GUI_ConfDefaults.h"   /* Needed for GUI_NUM_LAYERS */
/*********************************************************************
*
*      Map the defines for this controller
*
*      #define LCD_<xxx> LCD_<xxx>_0
*
**********************************************************************
*/

#ifdef LCD_INIT_CONTROLLER_0
  #define LCD_INIT_CONTROLLER LCD_INIT_CONTROLLER_0
#endif

#ifdef  LCD_CONTROLLER_0
  #define LCD_CONTROLLER     LCD_CONTROLLER_0
#endif

#ifdef LCD_XSIZE_0
  #define LCD_XSIZE          LCD_XSIZE_0
#endif

#ifdef  LCD_YSIZE_0
  #define LCD_YSIZE          LCD_YSIZE_0
#endif

#ifdef  LCD_BITSPERPIXEL_0
  #define LCD_BITSPERPIXEL   LCD_BITSPERPIXEL_0
#endif

#ifdef  LCD_VYSIZE_0
  #define LCD_VYSIZE          LCD_VYSIZE_0
#endif

#ifdef LCD_MIRROR_X_0
  #define LCD_MIRROR_X  LCD_MIRROR_X_0
#endif

#ifdef LCD_MIRROR_Y_0
  #define LCD_MIRROR_Y  LCD_MIRROR_Y_0
#endif

#ifdef LCD_SWAP_XY_0
  #define LCD_SWAP_XY   LCD_SWAP_XY_0
#endif

#ifdef LCD_CACHE_CONTROL_0
  #define LCD_CACHE_CONTROL LCD_CACHE_CONTROL_0
#endif

#ifdef LCD_FIRSTSEG0_0
  #define LCD_FIRSTSEG0   LCD_FIRSTSEG0_0
#endif

#ifdef LCD_LASTSEG0_0
  #define LCD_LASTSEG0    LCD_LASTSEG0_0
#endif

#ifdef LCD_FIRSTCOM0_0
  #define LCD_FIRSTCOM0   LCD_FIRSTCOM0_0
#endif

#ifdef LCD_LASTCOM0_0
  #define LCD_LASTCOM0    LCD_LASTCOM0_0
#endif

#ifdef LCD_BITSPERPIXEL_L0_0
  #define LCD_BITSPERPIXEL_L0 LCD_BITSPERPIXEL_0
#endif

#ifdef LCD_DELTA_MODE_0
  #define LCD_DELTA_MODE LCD_DELTA_MODE_0
#endif

#ifdef LCD_XORG0_0
  #define LCD_XORG0       LCD_XORG0_0
#endif

#ifdef LCD_YORG0_0
  #define LCD_YORG0       LCD_YORG0_0
#endif

#ifdef LCD_FIXEDPALETTE_0
  #define LCD_FIXEDPALETTE LCD_FIXEDPALETTE_0
#endif

#ifdef LCD_CACHE_0
  #ifdef  LCD_CACHE
    #undef  LCD_CACHE
  #endif
  #define LCD_CACHE LCD_CACHE_0
#endif

#ifdef LCD_CHECKBUSY_0
  #ifdef LCD_CHECKBUSY
    #undef  LCD_CHECKBUSY
  #endif
  #define LCD_CHECKBUSY LCD_CHECKBUSY_0
#endif

#ifdef LCD_SWAP_BYTE_ORDER_0
  #define LCD_SWAP_BYTE_ORDER LCD_SWAP_BYTE_ORDER_0
#endif

#ifdef LCD_SWAP_RB_0
  #define LCD_SWAP_RB LCD_SWAP_RB_0
#endif

#ifdef LCD_PHYSCOLORS_0
  #define LCD_PHYSCOLORS LCD_PHYSCOLORS_0
#endif

#ifndef LCD_ALLOW_NON_OPTIMIZED_MODE
  #define LCD_ALLOW_NON_OPTIMIZED_MODE 0
#endif

/*********************************************************************
*
*      Some error checking
*
**********************************************************************
*/
  #ifndef LCD_CONTROLLER
    #error LCD_CONTROLLER not defined !
  #endif


/*******************************************************************
*
*               Defaults for config switches
*
********************************************************************
*/



#ifndef LCD_NUM_CONTROLLERS
  #define LCD_NUM_CONTROLLERS (1)
#endif

#ifndef LCD_NUM_COLORS
  #define LCD_NUM_COLORS (1L<<LCD_BITSPERPIXEL)
#endif



/*********************************************************************
*
*      Map the defines for controller 0 to general defines
*
**********************************************************************
*/

/* Allow use of multiple physical lines for one logical line ...
   Used for delta-type LCDs, where value should be 2 */

#ifndef LCD_YMAG
  #define LCD_YMAG 1
#endif
#ifndef LCD_XMAG
  #define LCD_XMAG 1
#endif

#ifndef LCD_VXSIZE
  #define LCD_VXSIZE      (LCD_XSIZE*LCD_XMAG) /* virtual x-size */
#endif
#ifndef LCD_VYSIZE
  #define LCD_VYSIZE      (LCD_YSIZE*LCD_YMAG) /* virtual y-size */
#endif

#ifndef LCD_SWAP_BYTE_ORDER
  #define LCD_SWAP_BYTE_ORDER (0)    /* only for 16 bit bus, default is not swapped */
#endif

#ifndef  LCD_MAX_LOG_COLORS
  #define  LCD_MAX_LOG_COLORS (256)
#endif

/* Optionally swap red and blue components */
#ifndef LCD_SWAP_RB
  #define LCD_SWAP_RB 0
#endif

/*********************************************************************
*
*      Defaults for Display 0 (first display)
*
**********************************************************************
*/

#ifndef LCD_MIRROR_X
  #define LCD_MIRROR_X   (0)    /* If active:  X -> -X */
#endif
#ifndef LCD_MIRROR_Y
  #define LCD_MIRROR_Y   (0)    /* If active:  Y -> -Y */
#endif
#ifndef LCD_SWAP_XY
  #define LCD_SWAP_XY    (0)    /* If active:  X <-> Y */
#endif
#ifndef LCD_BITSPERPIXEL_L0
  #define LCD_BITSPERPIXEL_L0 LCD_BITSPERPIXEL
#endif
#ifndef LCD_DELTA_MODE
  #define LCD_DELTA_MODE 0 /**/
#endif

/* Position data for controller 0 */
#ifndef LCD_XORG0
  #define LCD_XORG0     (0)     /* leftmost position covered by this controller */
#endif
#ifndef LCD_YORG0
  #define LCD_YORG0     (0)     /* topmost position covered by this controller */
#endif
#ifndef LCD_XSIZE0
  #define LCD_XSIZE0    (LCD_XSIZE)   /* horizontal size covered by this controller */
#endif
#ifndef LCD_YSIZE0
 #define LCD_YSIZE0    (LCD_YSIZE)   /* vertical size covered by this controller */
#endif
#ifndef LCD_FIRSTSEG0
  #define LCD_FIRSTSEG0 (0)     /* first segment used by this controller */
#endif
#ifndef LCD_LASTSEG0
  #define LCD_LASTSEG0    (LCD_XSIZE-1) /* Contr.0: last segment line used */
#endif
#ifndef LCD_FIRSTCOM0
  #define LCD_FIRSTCOM0 (0)     /* first com used by this controller */
#endif
#ifndef LCD_LASTCOM0
  #define LCD_LASTCOM0    (LCD_YSIZE-1) /* Contr.0: last com line used */
#endif

#if (!LCD_SWAP_XY)
  #define LCD_XSIZE_PHYS  LCD_XSIZE
  #define LCD_YSIZE_PHYS  LCD_YSIZE
  #define LCD_VXSIZE_PHYS LCD_VXSIZE
  #define LCD_VYSIZE_PHYS LCD_VYSIZE
#else
  #define LCD_XSIZE_PHYS  LCD_YSIZE
  #define LCD_YSIZE_PHYS  LCD_XSIZE
  #define LCD_VXSIZE_PHYS LCD_VYSIZE
  #define LCD_VYSIZE_PHYS LCD_VXSIZE
#endif


/**********************************************************
*
*       Color settings
*/
#ifndef LCD_REVERSE
  #define LCD_REVERSE 0
#endif

#ifndef LCD_REVERSE_LUT
  #define LCD_REVERSE_LUT 0
#endif

#ifndef LCD_PHYSCOLORS_IN_RAM
  #define LCD_PHYSCOLORS_IN_RAM 0
#endif

#if !defined (LCD_PHYSCOLORS)
  #if   (LCD_BITSPERPIXEL == 1) | (LCD_BITSPERPIXEL == 2) | (LCD_BITSPERPIXEL == 4)
    #ifndef LCD_FIXEDPALETTE
      #define LCD_FIXEDPALETTE LCD_BITSPERPIXEL
    #endif
  #endif
#endif /* defined (LCD_PHYSCOLORS) */

#if !defined (LCD_FIXEDPALETTE)
  #if !defined (LCD_PHYSCOLORS)
    #if LCD_BITSPERPIXEL == 8
      #define LCD_FIXEDPALETTE 8666
    #elif LCD_BITSPERPIXEL == 12
      #define LCD_FIXEDPALETTE 44412
    #elif LCD_BITSPERPIXEL == 15
      #define LCD_FIXEDPALETTE 555
    #elif LCD_BITSPERPIXEL == 16
      #define LCD_FIXEDPALETTE 565
    #elif LCD_BITSPERPIXEL == 24
      #define LCD_FIXEDPALETTE 888
    #else
      #define LCD_FIXEDPALETTE 0
    #endif
  #else
    #define LCD_FIXEDPALETTE 0
  #endif
#endif

#if (LCD_BITSPERPIXEL > 8) && defined (LCD_PHYSCOLORS)
  #error List of physical colors not permitted if more than 8 bpp are used !
#endif

#if (LCD_FIXEDPALETTE==0)
  #if !defined(LCD_PHYSCOLORS)
    #error You have to define a fixed palette (LCD_FIXEDPALETTE) or
    #error the list of physical colors (LCD_PHYSCOLORS) in LCDConf.h !
  #endif
#endif


/* In order to avoid warnings for undefined parameters */
#ifndef LCD_USE_PARA
  #define LCD_USE_PARA(para) para=para;
#endif

/*********************************************************************
*
*      Defaults for Display 1 (second display)
*
**********************************************************************
*/
#if GUI_NUM_LAYERS > 1

  #ifndef LCD_MIRROR_X_1
    #define LCD_MIRROR_X_1   (0)    /* If active:  X -> -X */
  #endif
  #ifndef LCD_MIRROR_Y_1
    #define LCD_MIRROR_Y_1   (0)    /* If active:  Y -> -Y */
  #endif
  #ifndef LCD_SWAP_XY_1
    #define LCD_SWAP_XY_1    (0)    /* If active:  X <-> Y */
  #endif
  #ifndef LCD_BITSPERPIXEL_L0_1
    #define LCD_BITSPERPIXEL_L0_1 LCD_BITSPERPIXEL_1
  #endif
  #ifndef LCD_DELTA_MODE_1
    #define LCD_DELTA_MODE_1 0 /**/
  #endif

  #ifndef LCD_YMAG_1
    #define LCD_YMAG_1 1
  #endif

  #ifndef LCD_XMAG_1
    #define LCD_XMAG_1 1
  #endif

  #ifndef LCD_VXSIZE_1
    #define LCD_VXSIZE_1      (LCD_XSIZE_1 * LCD_XMAG_1) /* virtual x-size */
  #endif

  #ifndef LCD_VYSIZE_1
    #define LCD_VYSIZE_1      (LCD_YSIZE_1 * LCD_YMAG_1) /* virtual y-size */
  #endif

  #ifndef LCD_NUM_COLORS_1
    #define LCD_NUM_COLORS_1 (1L<<LCD_BITSPERPIXEL_1)
  #endif

  /* Optionally swap red and blue components */
  #ifndef LCD_SWAP_RB_1
    #define LCD_SWAP_RB_1 0
  #endif

/**********************************************************
*
*       Color settings
*/
  #ifndef LCD_REVERSE_1
    #define LCD_REVERSE_1 0
  #endif

  #ifndef LCD_REVERSE_LUT_1
    #define LCD_REVERSE_LUT_1 0
  #endif

  #if !defined (LCD_PHYSCOLORS_1)
    #if   (LCD_BITSPERPIXEL_1 == 1) | (LCD_BITSPERPIXEL_1 == 2) | (LCD_BITSPERPIXEL_1 == 4)
      #ifndef LCD_FIXEDPALETTE_1
        #define LCD_FIXEDPALETTE_1 LCD_BITSPERPIXEL_1
      #endif
    #endif
  #endif /* defined (LCD_PHYSCOLORS) */

  #if !defined (LCD_FIXEDPALETTE_1)
    #if !defined (LCD_PHYSCOLORS_1)
      #if LCD_BITSPERPIXEL_1 == 8
        #define LCD_FIXEDPALETTE_1 8666
      #elif LCD_BITSPERPIXEL_1 == 15
        #define LCD_FIXEDPALETTE_1 555
      #elif LCD_BITSPERPIXEL_1 == 16
        #define LCD_FIXEDPALETTE_1 565
      #else
        #define LCD_FIXEDPALETTE_1 0
      #endif
    #else
      #define LCD_FIXEDPALETTE_1 0
    #endif
  #endif

  #if (LCD_BITSPERPIXEL_1 > 8) && defined (LCD_PHYSCOLORS_1)
    #error List of physical colors not permitted if more than 8 bpp are used !
  #endif

  #if (LCD_FIXEDPALETTE_1 == 0)
    #if !defined(LCD_PHYSCOLORS_1)
      #error You have to define a fixed palette (LCD_FIXEDPALETTE_1) or
      #error the list of physical colors (LCD_PHYSCOLORS_1) in LCDConf.h !
    #endif
  #endif
#endif   /* GUI_NUM_LAYERS > 1 */

/*********************************************************************
*
*      Defaults for Display 2 (third display)
*
**********************************************************************
*/
#if GUI_NUM_LAYERS > 2

  #ifndef LCD_MIRROR_X_2
    #define LCD_MIRROR_X_2   (0)    /* If active:  X -> -X */
  #endif
  #ifndef LCD_MIRROR_Y_2
    #define LCD_MIRROR_Y_2   (0)    /* If active:  Y -> -Y */
  #endif
  #ifndef LCD_SWAP_XY_2
    #define LCD_SWAP_XY_2    (0)    /* If active:  X <-> Y */
  #endif
  #ifndef LCD_BITSPERPIXEL_L0_2
    #define LCD_BITSPERPIXEL_L0_2 LCD_BITSPERPIXEL_2
  #endif
  #ifndef LCD_DELTA_MODE_2
    #define LCD_DELTA_MODE_2 0 /**/
  #endif

  #ifndef LCD_YMAG_2
    #define LCD_YMAG_2 1
  #endif

  #ifndef LCD_XMAG_2
    #define LCD_XMAG_2 1
  #endif

  #ifndef LCD_VXSIZE_2
    #define LCD_VXSIZE_2      (LCD_XSIZE_2 * LCD_XMAG_2) /* virtual x-size */
  #endif

  #ifndef LCD_VYSIZE_2
    #define LCD_VYSIZE_2      (LCD_YSIZE_2 * LCD_YMAG_2) /* virtual y-size */
  #endif

  #ifndef LCD_NUM_COLORS_2
    #define LCD_NUM_COLORS_2 (1L<<LCD_BITSPERPIXEL_2)
  #endif

  /* Optionally swap red and blue components */
  #ifndef LCD_SWAP_RB_2
    #define LCD_SWAP_RB_2 0
  #endif

/**********************************************************
*
*       Color settings
*/
  #ifndef LCD_REVERSE_2
    #define LCD_REVERSE_2 0
  #endif

  #ifndef LCD_REVERSE_LUT_2
    #define LCD_REVERSE_LUT_2 0
  #endif

  #if !defined (LCD_PHYSCOLORS_2)
    #if   (LCD_BITSPERPIXEL_2 == 1) | (LCD_BITSPERPIXEL_2 == 2) | (LCD_BITSPERPIXEL_2 == 4)
      #ifndef LCD_FIXEDPALETTE_2
        #define LCD_FIXEDPALETTE_2 LCD_BITSPERPIXEL_2
      #endif
    #endif
  #endif /* defined (LCD_PHYSCOLORS) */

  #if !defined (LCD_FIXEDPALETTE_2)
    #if !defined (LCD_PHYSCOLORS_2)
      #if LCD_BITSPERPIXEL_2 == 8
        #define LCD_FIXEDPALETTE_2 8666
      #elif LCD_BITSPERPIXEL_2 == 15
        #define LCD_FIXEDPALETTE_2 555
      #elif LCD_BITSPERPIXEL_2 == 16
        #define LCD_FIXEDPALETTE_2 565
      #else
        #define LCD_FIXEDPALETTE_2 0
      #endif
    #else
      #define LCD_FIXEDPALETTE_2 0
    #endif
  #endif

  #if (LCD_BITSPERPIXEL_2 > 8) && defined (LCD_PHYSCOLORS_2)
    #error List of physical colors not permitted if more than 8 bpp are used !
  #endif

  #if (LCD_FIXEDPALETTE_2 == 0)
    #if !defined(LCD_PHYSCOLORS_2)
      #error You have to define a fixed palette (LCD_FIXEDPALETTE_2) or
      #error the list of physical colors (LCD_PHYSCOLORS_2) in LCDConf.h !
    #endif
  #endif
#endif   /* GUI_NUM_LAYERS > 2 */

/*********************************************************************
*
*      Defaults for Display 3 (fourth display)
*
**********************************************************************
*/
#if GUI_NUM_LAYERS > 3

  #ifndef LCD_MIRROR_X_3
    #define LCD_MIRROR_X_3   (0)    /* If active:  X -> -X */
  #endif
  #ifndef LCD_MIRROR_Y_3
    #define LCD_MIRROR_Y_3   (0)    /* If active:  Y -> -Y */
  #endif
  #ifndef LCD_SWAP_XY_3
    #define LCD_SWAP_XY_3    (0)    /* If active:  X <-> Y */
  #endif
  #ifndef LCD_BITSPERPIXEL_L0_3
    #define LCD_BITSPERPIXEL_L0_3 LCD_BITSPERPIXEL_3
  #endif
  #ifndef LCD_DELTA_MODE_3
    #define LCD_DELTA_MODE_3 0 /**/
  #endif

  #ifndef LCD_YMAG_3
    #define LCD_YMAG_3 1
  #endif

  #ifndef LCD_XMAG_3
    #define LCD_XMAG_3 1
  #endif

  #ifndef LCD_VXSIZE_3
    #define LCD_VXSIZE_3      (LCD_XSIZE_3 * LCD_XMAG_3) /* virtual x-size */
  #endif

  #ifndef LCD_VYSIZE_3
    #define LCD_VYSIZE_3      (LCD_YSIZE_3 * LCD_YMAG_3) /* virtual y-size */
  #endif

  #ifndef LCD_NUM_COLORS_3
    #define LCD_NUM_COLORS_3 (1L<<LCD_BITSPERPIXEL_3)
  #endif

  /* Optionally swap red and blue components */
  #ifndef LCD_SWAP_RB_3
    #define LCD_SWAP_RB_3 0
  #endif

/**********************************************************
*
*       Color settings
*/
  #ifndef LCD_REVERSE_3
    #define LCD_REVERSE_3 0
  #endif

  #ifndef LCD_REVERSE_LUT_3
    #define LCD_REVERSE_LUT_3 0
  #endif

  #if !defined (LCD_PHYSCOLORS_3)
    #if   (LCD_BITSPERPIXEL_3 == 1) | (LCD_BITSPERPIXEL_3 == 2) | (LCD_BITSPERPIXEL_3 == 4)
      #ifndef LCD_FIXEDPALETTE_3
        #define LCD_FIXEDPALETTE_3 LCD_BITSPERPIXEL_3
      #endif
    #endif
  #endif /* defined (LCD_PHYSCOLORS) */

  #if !defined (LCD_FIXEDPALETTE_3)
    #if !defined (LCD_PHYSCOLORS_3)
      #if LCD_BITSPERPIXEL_3 == 8
        #define LCD_FIXEDPALETTE_3 8666
      #elif LCD_BITSPERPIXEL_3 == 15
        #define LCD_FIXEDPALETTE_3 555
      #elif LCD_BITSPERPIXEL_3 == 16
        #define LCD_FIXEDPALETTE_3 565
      #else
        #define LCD_FIXEDPALETTE_3 0
      #endif
    #else
      #define LCD_FIXEDPALETTE_3 0
    #endif
  #endif

  #if (LCD_BITSPERPIXEL_3 > 8) && defined (LCD_PHYSCOLORS_3)
    #error List of physical colors not permitted if more than 8 bpp are used !
  #endif

  #if (LCD_FIXEDPALETTE_3 == 0)
    #if !defined(LCD_PHYSCOLORS_3)
      #error You have to define a fixed palette (LCD_FIXEDPALETTE_3) or
      #error the list of physical colors (LCD_PHYSCOLORS_3) in LCDConf.h !
    #endif
  #endif
#endif   /* GUI_NUM_LAYERS > 3 */

/*********************************************************************
*
*      Defaults for Display 4 (fiveth display)
*
**********************************************************************
*/
#if GUI_NUM_LAYERS > 4

  #ifndef LCD_MIRROR_X_4
    #define LCD_MIRROR_X_4   (0)    /* If active:  X -> -X */
  #endif
  #ifndef LCD_MIRROR_Y_4
    #define LCD_MIRROR_Y_4   (0)    /* If active:  Y -> -Y */
  #endif
  #ifndef LCD_SWAP_XY_4
    #define LCD_SWAP_XY_4    (0)    /* If active:  X <-> Y */
  #endif
  #ifndef LCD_BITSPERPIXEL_L0_4
    #define LCD_BITSPERPIXEL_L0_4 LCD_BITSPERPIXEL_4
  #endif
  #ifndef LCD_DELTA_MODE_4
    #define LCD_DELTA_MODE_4 0 /**/
  #endif

  #ifndef LCD_YMAG_4
    #define LCD_YMAG_4 1
  #endif

  #ifndef LCD_XMAG_4
    #define LCD_XMAG_4 1
  #endif

  #ifndef LCD_VXSIZE_4
    #define LCD_VXSIZE_4      (LCD_XSIZE_4 * LCD_XMAG_4) /* virtual x-size */
  #endif

  #ifndef LCD_VYSIZE_4
    #define LCD_VYSIZE_4      (LCD_YSIZE_4 * LCD_YMAG_4) /* virtual y-size */
  #endif

  #ifndef LCD_NUM_COLORS_4
    #define LCD_NUM_COLORS_4 (1L<<LCD_BITSPERPIXEL_4)
  #endif

  /* Optionally swap red and blue components */
  #ifndef LCD_SWAP_RB_4
    #define LCD_SWAP_RB_4 0
  #endif

/**********************************************************
*
*       Color settings
*/
  #ifndef LCD_REVERSE_4
    #define LCD_REVERSE_4 0
  #endif

  #ifndef LCD_REVERSE_LUT_4
    #define LCD_REVERSE_LUT_4 0
  #endif

  #if !defined (LCD_PHYSCOLORS_4)
    #if   (LCD_BITSPERPIXEL_4 == 1) | (LCD_BITSPERPIXEL_4 == 2) | (LCD_BITSPERPIXEL_4 == 4)
      #ifndef LCD_FIXEDPALETTE_4
        #define LCD_FIXEDPALETTE_4 LCD_BITSPERPIXEL_4
      #endif
    #endif
  #endif /* defined (LCD_PHYSCOLORS) */

  #if !defined (LCD_FIXEDPALETTE_4)
    #if !defined (LCD_PHYSCOLORS_4)
      #if LCD_BITSPERPIXEL_4 == 8
        #define LCD_FIXEDPALETTE_4 8666
      #elif LCD_BITSPERPIXEL_4 == 15
        #define LCD_FIXEDPALETTE_4 555
      #elif LCD_BITSPERPIXEL_4 == 16
        #define LCD_FIXEDPALETTE_4 565
      #else
        #define LCD_FIXEDPALETTE_4 0
      #endif
    #else
      #define LCD_FIXEDPALETTE_4 0
    #endif
  #endif

  #if (LCD_BITSPERPIXEL_4 > 8) && defined (LCD_PHYSCOLORS_4)
    #error List of physical colors not permitted if more than 8 bpp are used !
  #endif

  #if (LCD_FIXEDPALETTE_4 == 0)
    #if !defined(LCD_PHYSCOLORS_4)
      #error You have to define a fixed palette (LCD_FIXEDPALETTE_4) or
      #error the list of physical colors (LCD_PHYSCOLORS_4) in LCDConf.h !
    #endif
  #endif
#endif   /* GUI_NUM_LAYERS > 4 */



/*********************************************************************
*
*      Global defaults for all displays
*
**********************************************************************
*/


#if (LCD_BITSPERPIXEL <= 8) && (GUI_NUM_LAYERS < 2)
  #define LCD_PIXELINDEX U8
#else
  #if (LCD_BITSPERPIXEL < 24)
    #define LCD_PIXELINDEX U16
  #else
    #define LCD_PIXELINDEX U32
  #endif
#endif





#endif /* LCD_INTERN_H */



	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
