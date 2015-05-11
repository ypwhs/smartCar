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
File        : GUI_ARRAY.h
Purpose     : Array handling routines
---------------------------END-OF-HEADER------------------------------
*/

#ifndef GUIARRAY_H
#define GUIARRAY_H

#include "WM_Intern.h"

#if GUI_WINSUPPORT

typedef enum {
  GUI_ARRAY_STATE_NOT_CREATED,
  GUI_ARRAY_STATE_CREATED,
  GUI_ARRAY_STATE_DELETED
} GUI_ARRAY_STATE;

typedef struct {
  U16 NumItems;
  WM_HMEM haHandle;   /* Handle to buffer holding handles */
  #if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
    GUI_ARRAY_STATE InitState;
  #endif
} GUI_ARRAY;



int      GUI_ARRAY_AddItem        (GUI_ARRAY* pThis, const void *pNew, int Len);
void     GUI_ARRAY_Delete         (GUI_ARRAY* pThis);
WM_HMEM  GUI_ARRAY_GethItem       (const GUI_ARRAY* pThis, unsigned int Index);
unsigned GUI_ARRAY_GetNumItems    (const GUI_ARRAY* pThis);
void*    GUI_ARRAY_GetpItem       (const GUI_ARRAY* pThis, unsigned int Index);
/* Optional methods */
int      GUI_ARRAY_SethItem       (GUI_ARRAY* pThis, unsigned int Index, WM_HMEM hItem);
WM_HMEM  GUI_ARRAY_SetItem        (GUI_ARRAY* pThis, unsigned int Index, const void* pData, int Len);
void     GUI_ARRAY_DeleteItem     (GUI_ARRAY* pThis, unsigned int Index);
char     GUI_ARRAY_InsertBlankItem(GUI_ARRAY* pThis, unsigned int Index);
WM_HMEM  GUI_ARRAY_InsertItem     (GUI_ARRAY* pThis, unsigned int Index, int Len);
void*    GUI_ARRAY_ResizeItem     (GUI_ARRAY* pThis, unsigned int Index, int Len);

#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
  void GUI_ARRAY_Create (      GUI_ARRAY * pThis);
  void GUI_ARRAY_Check  (const GUI_ARRAY * pThis);
  #define GUI_ARRAY_CREATE(pThis) GUI_ARRAY_Create(pThis)
  #define GUI_ARRAY_CHECK(pThis)  GUI_ARRAY_Check(pThis)
#else
  #define GUI_ARRAY_CREATE(pThis)
  #define GUI_ARRAY_CHECK(pThis)
#endif


#endif

#endif
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
