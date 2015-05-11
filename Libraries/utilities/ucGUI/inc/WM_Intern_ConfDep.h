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
File        : WM_Intern_ConfDep.h
Purpose     : Windows manager internal include, dependent on LCDConf.h
----------------------------------------------------------------------
*/

#ifndef WM_INTERN_CONFDEP_H            /* Make sure we only include it once */
#define WM_INTERN_CONFDEP_H            /* Make sure we only include it once */

#include "WM_Intern.h"

#if GUI_WINSUPPORT


#ifdef WM_C
  #define GUI_EXTERN
#else
  #define GUI_EXTERN extern
#endif

GUI_EXTERN WM_HWIN   WM__ahDesktopWin[GUI_NUM_LAYERS];   /* No longer depends on LCDConf.h ... Can be moved to an other file */
GUI_EXTERN GUI_COLOR WM__aBkColor[GUI_NUM_LAYERS];       /* No longer depends on LCDConf.h ... Can be moved to an other file */

#undef GUI_EXTERN

#endif   /* GUI_WINSUPPORT */

#endif   /* WM_INTERN_H */

/*************************** End of file ****************************/
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
