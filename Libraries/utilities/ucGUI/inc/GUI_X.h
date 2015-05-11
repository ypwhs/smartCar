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
File        : GUI_X.h
Purpose     : Declarations for GUI_X module
---------------------------END-OF-HEADER------------------------------

Attention : Do not modify this file ! If you do, you will not
            be able do update to a later GUI version !

*/

#ifndef  GUI_X_H
#define  GUI_X_H

#include "GUI.h"

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

/************************************************************
*
*                    GUI_X_
*
*         externals, to be defined by application
*
*************************************************************

The externals defined below should be defined by the
application. They are per default contained in the module
GUI_X.c.
Note that a lot if not all of these are not required in most target
systems.
For this module, samples are available for configurations
with or without operating system.
*/

/**** Init ****/
void GUI_X_Init(void);

/**** ExecIdle - called if nothing else is left to do ****/
void GUI_X_ExecIdle(void);

/**** Timing routines - required for blinking ****/
int  GUI_X_GetTime(void);
void GUI_X_Delay(int Period);

/**** Multitask routines - required only if multitasking is used (#define GUI_OS 1) ****/
void GUI_X_Unlock(void);
void GUI_X_Lock(void);
U32  GUI_X_GetTaskId(void);
void GUI_X_InitOS(void);

/****      Event driving (optional with multitasking)  ****/
void GUI_X_WaitEvent(void);
void GUI_X_SignalEvent(void);
/**** Recording (logs/warnings and errors) - required only for higher levels ****/
void GUI_X_Log(const char *s);
void GUI_X_Warn(const char *s);
void GUI_X_ErrorOut(const char *s); 

#if defined(__cplusplus)
  }
#endif
#endif   /* ifdef GUI_X_H */

/*************************** End of file ****************************/
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
