/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2014  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.26 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The software has been licensed to  ARM LIMITED whose registered office
is situated at  110 Fulbourn Road,  Cambridge CB1 9NJ,  England solely
for  the  purposes  of  creating  libraries  for  ARM7, ARM9, Cortex-M
series,  and   Cortex-R4   processor-based  devices,  sublicensed  and
distributed as part of the  MDK-ARM  Professional  under the terms and
conditions  of  the   End  User  License  supplied  with  the  MDK-ARM
Professional. 
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : GUIDEMO_Graph.c
Purpose     : Demonstration of the GRAPH widget.
----------------------------------------------------------------------
*/

#include "GUIDEMO.h"

#if (SHOW_GUIDEMO_GRAPH && GUI_WINSUPPORT && GUI_SUPPORT_MEMDEV)

/*********************************************************************
*
*       Configuration defines
*
**********************************************************************
*/
#define MAX_GRAPH_SIZE_X  440
#define MAX_GRAPH_SIZE_Y  ((MAX_GRAPH_SIZE_X * 11) / 20)

#define DIST_TO_BORDER    10
#define DIST_TO_WIN       5

#define BORDER_TOP        0
#define BORDER_BOTTOM     11
#define BORDER_LEFT       19
#define BORDER_RIGHT      0

#define COLOR_BK          GUI_DARKGRAY
#define COLOR_BORDER      BK_COLOR_1
#define COLOR_FRAME       GUI_BLACK
#define COLOR_GRID        GUI_GRAY

#define SCALE_H_HEIGHT    4

#define TICK_DIST_H       25
#define TICK_DIST_V       20

#define TIME_RUN          5000
#define TIME_STEP         15

#define MAX_NUM_DATA_OBJ  3

#define GRAPH_DIV         9   // (2^9 = 512) If this value is changed _aWaves[] need to be changed too!
#define GRID_DIST_X       25
#define GRID_DIST_Y       10

/*********************************************************************
*
*       Typedef / Data
*
**********************************************************************
*/
typedef struct {
  char  * pName;
  int     ScaleVOff;
  int     DataVOff;
  int     GridVOff;
  void (* pfAddData)(GRAPH_DATA_Handle hData, int DataID);
  int     NumWaves;
} GRAPH_WAVE;

static int _HeartBeat[] = {
    2,   4,   6,   8,  10,   6,   2,   0,   0,   0,
   -8,  16,  40,  64,  88,  58,  28,  -2, -32, -30,
  -20, -10,   0,   2,   2,   4,   4,   6,   6,   8,
    8,  10,  12,  14,  16,  18,  20,  16,  12,   8,
    4,   2,   2,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0
};

static GUI_COLOR _aColorData[MAX_NUM_DATA_OBJ] = {
  0x50C0FF,
  0xFFC050,
  0x50FFC0,
};

static GRAPH_SCALE_Handle _hScaleH;
static GRAPH_SCALE_Handle _hScaleV;
static int                _DataAdjust;

/*********************************************************************
*
*       Static functions
*
**********************************************************************
*/
/*********************************************************************
*
*       _AddData_Sine
*/
static void _AddData_Sine(GRAPH_DATA_Handle hData, int DataID) {
  static int x1000[MAX_NUM_DATA_OBJ];
  I32        SinHQ;
  int        Multi;
  int        Step;

  switch (DataID) {
  case 0:
    Multi = 70;
    Step  = 3;
    break;
  case 1:
    Multi = 50;
    Step  = 1;
    break;
  case 2:
    Multi = 30;
    Step  = 7;
    break;
  default:
    return;
  }
  SinHQ          = GUI__SinHQ(x1000[DataID]);
  x1000[DataID] += 1000 * Step;
  GRAPH_DATA_YT_AddValue(hData, SHIFT_RIGHT_16(SinHQ * Multi) + _DataAdjust);
}

/*********************************************************************
*
*       _AddData_Heartbeat
*/
static void _AddData_Heartbeat(GRAPH_DATA_Handle hData, int DataID) {
  static int Index;

  GUI_USE_PARA(DataID);
  GRAPH_DATA_YT_AddValue(hData, (_HeartBeat[Index]) + _DataAdjust);
  if (++Index == GUI_COUNTOF(_HeartBeat)) {
    Index = 0;
  }
}

/*********************************************************************
*
*       DATA _aWave - Keep below _AddData-functions
*/
GRAPH_WAVE _aWave[] = {
  { "Heartbeat",        // Name
    157,                // Vertical scale offset in relation to GRAPH_DIV
    152,                // Vertical data  offset in relation to GRAPH_DIV
    21,                 // Vertical grid  offset in relation to GRAPH_DIV
    _AddData_Heartbeat, // Pointer to specific AddData function
    1                   // Number of waves
  },
  { "Sine Waves",
    265,
    253,
    23,
    _AddData_Sine,
    3
  }
};

/*********************************************************************
*
*       _ShowGraph
*/
static void _ShowGraph(GRAPH_Handle hGraph, GRAPH_DATA_Handle hData[], int DataCount, void (* pfAddData)(GRAPH_DATA_Handle hData, int DataID)) {
  int Data_xSize;
  int NextState;
  int TimeStart;
  int TimeDiff;
  int TimeStep;
  int Count;
  int xSize;
  int i;

  xSize      = LCD_GetXSize();
  Data_xSize = xSize - (DIST_TO_BORDER << 1) - (BORDER_LEFT + BORDER_RIGHT);
  Count      = 0;
  //
  // Attach data objects
  //
  for (i = 0; i < DataCount; i++) {
    GRAPH_AttachData(hGraph, hData[i]);
  }
  //
  // Add values before GRAPH is displayed
  //
  while (Count < Data_xSize) {
    for (i = 0; i < DataCount; i++) {
      pfAddData(hData[i], i);
    }
    Count++;
  }
  //
  // Add values depending on time
  //

  TimeStart = GUIDEMO_GetTime();
  do {
    TimeDiff = GUIDEMO_GetTime() - TimeStart;
    for (i = 0; i < DataCount; i++) {
      pfAddData(hData[i], i);
    }
    NextState = GUIDEMO_CheckCancel();
    TimeStep  = GUIDEMO_GetTime() - TimeStart;
    if ((TimeStep - TimeDiff) < TIME_STEP) {
      GUI_Delay(TIME_STEP - (TimeStep - TimeDiff));
    }
  } while ((TimeDiff < TIME_RUN) && (NextState == 0));
  for (i = 0; i < DataCount; i++) {
    GRAPH_DetachData(hGraph, hData[i]);
  }
  GUIDEMO_NotifyStartNext();
}

/*********************************************************************
*
*       _cbBk
*/
static void _cbBk(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
  case WM_PAINT:
    GUIDEMO_DrawBk();
    break;
  default:
    WM_DefaultProc(pMsg);
  }
}

/*********************************************************************
*
*       _GraphDemo
*/
static void _GraphDemo(void) {
  const WIDGET_EFFECT * pEffectOld;
  GRAPH_DATA_Handle     hData[MAX_NUM_DATA_OBJ];
  GRAPH_Handle          hGraph;
  unsigned              i;
  int                   Graph_xSize;
  int                   Graph_ySize;
  int                   Graph_xPos;
  int                   Graph_yPos;
  int                   Data_ySize;
  int                   xSize;
  int                   ySize;

  xSize      = LCD_GetXSize();
  ySize      = LCD_GetYSize();
  pEffectOld = WIDGET_SetDefaultEffect(&WIDGET_Effect_Simple);
  //
  // Set Callback function for background window
  //
  WM_SetCallback(WM_HBKWIN, _cbBk);
  //
  // Determine size of GRAPH
  //
  Graph_xSize = xSize - (DIST_TO_BORDER << 1);            // xsize = Screen size subtracting twice the distance to the border of the screen
  Graph_ySize = ySize - INFO_SIZE_Y - (DIST_TO_WIN << 1); // ysize = Screen size subtracting the window sizes and twice the distance to the windows
  if (Graph_ySize > MAX_GRAPH_SIZE_Y) {
    Graph_ySize = MAX_GRAPH_SIZE_Y;
    if (Graph_xSize > (Graph_ySize * 20) / 11) {
      Graph_xSize = (Graph_ySize * 20) / 11;
    }
  }
  //
  // Create and configure GRAPH_WIDGET
  //
  Graph_xPos = (xSize - Graph_xSize) / 2;
  Graph_yPos = (ySize - Graph_ySize) / 2;
  if (Graph_yPos < INFO_SIZE_Y + DIST_TO_WIN) {
    Graph_yPos = INFO_SIZE_Y + DIST_TO_WIN;
  }
  hGraph = GRAPH_CreateEx(Graph_xPos, Graph_yPos, Graph_xSize, Graph_ySize, WM_HBKWIN, WM_CF_SHOW | WM_CF_CONST_OUTLINE, 0, 0);
  GRAPH_SetBorder(hGraph, BORDER_LEFT, BORDER_TOP, BORDER_RIGHT, BORDER_BOTTOM);
  WM_SetHasTrans (hGraph);
  GRAPH_SetColor (hGraph, COLOR_BK,     GRAPH_CI_BK);
  GRAPH_SetColor (hGraph, COLOR_BORDER, GRAPH_CI_BORDER);
  GRAPH_SetColor (hGraph, COLOR_FRAME,  GRAPH_CI_FRAME);
  GRAPH_SetColor (hGraph, COLOR_GRID,   GRAPH_CI_GRID);
  //
  // Adjust grid
  //
  GRAPH_SetGridVis  (hGraph, 1);
  GRAPH_SetGridDistX(hGraph, GRID_DIST_X);
  GRAPH_SetGridDistY(hGraph, GRID_DIST_Y);
  WM_BringToBottom  (hGraph);
  //
  // Create and configure GRAPH_DATA_YT object
  //
  for (i = 0; i < MAX_NUM_DATA_OBJ; i++) {
    hData[i] = GRAPH_DATA_YT_Create(_aColorData[i], xSize - (DIST_TO_BORDER << 1) - BORDER_LEFT, 0, 0);
  }
  Data_ySize = Graph_ySize - BORDER_BOTTOM;
  //
  // Create and configure GRAPH_SCALE objects
  //
  _hScaleH = GRAPH_SCALE_Create(BORDER_BOTTOM >> 1, GUI_TA_VCENTER, GRAPH_SCALE_CF_HORIZONTAL, TICK_DIST_H);
  _hScaleV = GRAPH_SCALE_Create(BORDER_LEFT   >> 1, GUI_TA_HCENTER, GRAPH_SCALE_CF_VERTICAL,   TICK_DIST_V);
  GRAPH_AttachScale(hGraph, _hScaleH);
  GRAPH_AttachScale(hGraph, _hScaleV);
  GRAPH_SCALE_SetPos(_hScaleH, Graph_ySize - SCALE_H_HEIGHT);
  GRAPH_SCALE_SetOff(_hScaleH, -5);
  //
  // Show some graphs
  //
  for (i = 0; i < GUI_COUNTOF(_aWave); i++) {
    GUIDEMO_SetInfoText(_aWave[i].pName);
    _DataAdjust = GUIDEMO_ShiftRight(Data_ySize * _aWave[i].DataVOff, GRAPH_DIV);
    GRAPH_SetGridOffY (hGraph, GUIDEMO_ShiftRight(Data_ySize * _aWave[i].GridVOff, GRAPH_DIV));
    GRAPH_SCALE_SetOff(_hScaleV, GUIDEMO_ShiftRight((Data_ySize - BORDER_BOTTOM) * _aWave[i].ScaleVOff, GRAPH_DIV));
    _ShowGraph(hGraph, hData, _aWave[i].NumWaves, _aWave[i].pfAddData);
  }
  //
  // Clean up
  //
  GRAPH_DetachScale(hGraph, _hScaleH);
  GRAPH_DetachScale(hGraph, _hScaleV);
  GRAPH_SCALE_Delete(_hScaleH);
  GRAPH_SCALE_Delete(_hScaleV);
  for (i = 0; i < MAX_NUM_DATA_OBJ; i++) {
    GRAPH_DATA_YT_Delete(hData[i]);
  }
  WM_DeleteWindow(hGraph);
  WIDGET_SetDefaultEffect(pEffectOld);
}

/*********************************************************************
*
*       Public functions
*
**********************************************************************
*/
void GUIDEMO_Graph(void) {
  GUIDEMO_ConfigureDemo("Drawing a graph", "Optimized drawing routine\nfor drawing graph data", GUIDEMO_SHOW_CURSOR | GUIDEMO_SHOW_INFO | GUIDEMO_SHOW_CONTROL);
  _GraphDemo();
  GUIDEMO_NotifyStartNext();
}

#else

void GUIDEMO_Graph_C(void);
void GUIDEMO_Graph_C(void) {}

#endif  // SHOW_GUIDEMO_GRAPH && GUI_WINSUPPORT && GUI_SUPPORT_MEMDEV

/*************************** End of file ****************************/
