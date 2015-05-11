#ifndef _My_UI_h
#define _My_UI_h
#include <stdio.h>
#include <stdlib.h>
#include "GUI.H"
#include "BUTTON.h"
#include "progbar.h"
#include "framewin.h"
#include "TEXT.h"
#include "WM.h"
#include "LISTBOX.h"
#include "MULTIEDIT.h"
#include "LISTVIEW.h"
#include <stdint.h>

#define _folder   1
#define _mp3      2
#define _txt      3
#define _bmp      4

#define TouchUnPress    0   //首次检测到没有按下
#define TouchUnPressed  1   //没有按下
#define TouchPress      2   //首次检测到按下
#define TouchPressed    3   //按下
/*   菜单界面区分的ID号   */
#define BanMenu             200
#define MainMenu_ID         1 
#define MusicPlayeMenu_ID   3 
#define FileBrowseMenu_ID   5
#define SetMenu_ID          10
#define TextMenu_ID         15
#define ComPortMenu_ID      20

#define  Button_ID0   0x17a
#define  Button_ID1   0x17b
#define  Button_ID2   0x17c
#define  Button_ID3   0x17d
#define  Button_ID4   0x17e
#define  Button_ID5   0x17f
#define  Button_ID6   160
#define  Button_ID7   170
#define  Button_ID8   180
#define  Button_ID9   190
#define  Button_ID10  200
#define  Button_ID11  210
#define  Button_ID12  220
#define  Button_ID13  230
#define  Button_ID14  231
#define  Button_ID15  232
#define  Button_ID16  233
// #define _up   1
// #define _down 2

typedef struct SingleListNode {
	WM_HMEM                  handle;
	struct SingleListNode   *next;

}SingleListNode;                                                      /* 声明SingleListNode为新的类型名*/
typedef SingleListNode    *SingleList;                                /* 定义指针类型                  */

typedef struct {
	WM_HMEM                  FirstHandle;
	WM_HMEM                  EndHandle;
}MenuInfo;                 


/************************************** 这个UI所必需的全局变量 ********************************************/
//变量声明
#define MAX_FILE_CNT 30
extern uint16_t  FileCnt;

extern BUTTON_Handle  hButton_mainmenu[9];//主界面的按钮句柄
extern TEXT_Handle hText; //时间显示
extern GUI_PID_STATE  TouchPoint[2];
extern uint16_t TouchSum ; //累计从触摸到释放滑动的距离，用于判断是滑动还是点触。若是滑动则不执行操作。
extern uint8_t TouchCmd;  //标志着是点触还是滑动操作。
extern uint8_t TouchFlag; //触摸标志。
extern uint8_t Menu_ID;  //菜单界面的ID
extern GUI_COLOR  ThemeColor;
extern GUI_COLOR  DesktopColor;
extern SingleList  MainMenu_pHead;     /* 主界面的控件链表头指针          */
extern SingleList  OtherMenu_pHead;    /* 其他界面的控件链表表头指针      */
extern MenuInfo    OtherMenu_Info;
/*其他界面的菜单控件信息，主要是记录第一个控件和最后一个控件的句柄，用于判断是否滑动至界面最顶端和最低端，
**实现必要的回弹效果
*/
//界面滑动操作禁止。用于在菜单界面上建立对话框的场合，防止操作对话框的同时菜单界面也得到响应。
//使用时要注意对话框不能有关闭按钮，若按下关闭按钮则不会将此值清0，这样的话菜单界面将永远不能响应了。
extern uint8_t MenuDisnable;   
extern const GUI_FONT GUI_FontHZ_MicrosoftFont_13;
extern WM_HWIN hDesktopWindow;//桌面的句柄
/**********************************************************************************************************/

SingleList SingleListNodeCreate(void);
SingleList SingleListInsert(SingleList pList,SingleList pos,WM_HMEM x);
void SingleListFree(SingleList pHead);
void ClearStruct(unsigned char *pta, int size ) ;

uint8_t CheckFile(char *buf);

void MainMenu_Init(void); /* 主界面初始化*/
void Go_MainMenu(void);   /* 回到主界面*/
void Out_MainMenu(void);  /*退出主界面，注意主界面只是不在屏幕上显示而已，但仍然存在，在屏幕显示区域之外*/
void Out_OtherMenu(void);

void MyCreatDialog(int16_t x,int16_t y,WM_HWIN WM_Handle);
void MyDeleteDialog(WM_HWIN WM_Handle);
void ViscousWM(SingleList pHead);

void WM_MoveCtrl(void);   /* 菜单界面的滑动控制*/
void WM_GoTop(void);      /* 回界面顶部*/
void WM_Move(SingleList pHead ,int dx,int dy); /* 实现链表内的所有控件滑动*/
void SetThemeColor(GUI_COLOR color);
uint8_t FATAPI_OpenFile(uint8_t *filename);
//函数声明
static uint8_t CreateNextFilePath(char *buf);
static uint8_t CreateLastFilePath(void);
#endif
