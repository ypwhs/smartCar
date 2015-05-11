#include "my_gui.h"
#include "bitmap.h"
#include <stdint.h>

/************************************** 这个UI所必需的全局变量 ********************************************/
//全局变量声明区




BUTTON_Handle  hButton_mainmenu[9];//主界面的按钮句柄
TEXT_Handle hText; //时间显示
GUI_PID_STATE  TouchPoint[2];
uint16_t TouchSum = 0; //累计从触摸到释放滑动的距离，用于判断是滑动还是点触。若是滑动则不执行操作。
uint8_t TouchCmd = 0;  //标志着是点触还是滑动操作。
uint8_t TouchFlag = 0; //触摸标志。
uint8_t Menu_ID  = 0;  //菜单界面的ID
GUI_COLOR  ThemeColor = GUI_BLUE;
GUI_COLOR  DesktopColor = GUI_BLACK;
SingleList  MainMenu_pHead;     /* 主界面的控件链表头指针          */
SingleList  OtherMenu_pHead;    /* 其他界面的控件链表表头指针      */
MenuInfo    OtherMenu_Info;
/*其他界面的菜单控件信息，主要是记录第一个控件和最后一个控件的句柄，用于判断是否滑动至界面最顶端和最低端，
**实现必要的回弹效果
*/
//界面滑动操作禁止。用于在菜单界面上建立对话框的场合，防止操作对话框的同时菜单界面也得到响应。
//使用时要注意对话框不能有关闭按钮，若按下关闭按钮则不会将此值清0，这样的话菜单界面将永远不能响应了。
uint8_t MenuDisnable = 0;   

WM_HWIN hDesktopWindow;//桌面的句柄
/**********************************************************************************************************/
uint8_t CheckFile(char *buf)
{
	while(*buf++){
		if(*buf == '.') break;
	}
	if(*buf == '\0') return _folder;
	else if(*(buf+1) == 'M' && *(buf+2) == 'P' && *(buf+3) == '3') return _mp3;
	else if(*(buf+1) == 'T' && *(buf+2) == 'X' && *(buf+3) == 'T') return _txt;
	else if(*(buf+1) == 'B' && *(buf+2) == 'M' && *(buf+3) == 'P') return _bmp;
  return 0;
}
/********************************************************************************************************** 
** Function name:      MainMenu_Init
** Descriptions:       主界面菜单初始化
** input parameters:   无
** output parameters:  
** Returned value:     无
**********************************************************************************************************/
void MainMenu_Init(void)
{
	uint8_t i = 0;
	SingleList   pos   = NULL;
	MainMenu_pHead = SingleListNodeCreate();  /* 创建单向链表表头节点，这个链表将永远不会被销毁    */
	pos = MainMenu_pHead;
	Menu_ID = MainMenu_ID;
	/***********************一些默认设置****************************/
    FRAMEWIN_SetDefaultFont(&GUI_Font16B_1);
	FRAMEWIN_SetDefaultTitleHeight(19);	
	FRAMEWIN_SetDefaultBarColor(1, ThemeColor); 
	SCROLLBAR_SetDefaultWidth(18);
	LISTBOX_SetDefaultFont(&GUI_Font16B_1);
	LISTBOX_SetDefaultBkColor(2, ThemeColor);
	BUTTON_SetDefaultFont(&GUI_Font16B_1);
	GUI_SetBkColor(GUI_BLACK); 	
	GUI_SetColor(GUI_BLACK);
	WM_SetDesktopColor(DesktopColor); 
	hDesktopWindow = WM_GetDesktopWindow();//获取桌面的句柄
	/**************************************************************/
	 /* Create the button*/   
    hButton_mainmenu[0] = BUTTON_Create(5, 25+220, 90, 90, GUI_ID_BUTTON0, WM_CF_SHOW);   
	hButton_mainmenu[1] = BUTTON_Create(100, 25+220, 90, 90, GUI_ID_BUTTON1, WM_CF_SHOW);
	hButton_mainmenu[2] = BUTTON_Create(5, 120+220, 90, 90, GUI_ID_BUTTON2, WM_CF_SHOW);   
	hButton_mainmenu[3] = BUTTON_Create(100, 120+220, 90, 90, GUI_ID_BUTTON3, WM_CF_SHOW);
	hButton_mainmenu[4] = BUTTON_Create(5, 215+220, 185, 90, GUI_ID_BUTTON4, WM_CF_SHOW);
	
    hButton_mainmenu[5] = BUTTON_Create(5, 310+220, 90, 90, GUI_ID_BUTTON5, WM_CF_SHOW); 
	hButton_mainmenu[6] = BUTTON_Create(100, 310+220, 90, 90, GUI_ID_BUTTON6, WM_CF_SHOW); 
	hButton_mainmenu[7] = BUTTON_Create(5, 405+220, 185, 90, GUI_ID_BUTTON7, WM_CF_SHOW); 
	
	for (i = 0;i < 8;i++) 
    {
        BUTTON_SetTextAlign(hButton_mainmenu[i],GUI_TA_BOTTOM | GUI_TA_LEFT);
        BUTTON_SetTextColor(hButton_mainmenu[i],0,GUI_WHITE);
        BUTTON_SetBkColor  (hButton_mainmenu[i],0,ThemeColor);
        BUTTON_SetBkColor  (hButton_mainmenu[i],1,GUI_GRAY);
        pos = SingleListInsert(MainMenu_pHead,pos,hButton_mainmenu[i]);//将所有主界面按钮句柄插入链表
	}
  /* Set the button text */  
	
	BUTTON_SetFont(hButton_mainmenu[0],&GUI_Font16B_1);
	BUTTON_SetFont(hButton_mainmenu[1],&GUI_Font16B_1);
	BUTTON_SetFont(hButton_mainmenu[2],&GUI_Font16B_1);
	BUTTON_SetFont(hButton_mainmenu[3],&GUI_Font16B_1);
	BUTTON_SetFont(hButton_mainmenu[4],&GUI_Font16B_1);
	BUTTON_SetFont(hButton_mainmenu[5],&GUI_Font16B_1);
	BUTTON_SetFont(hButton_mainmenu[6],&GUI_Font16B_1);
	BUTTON_SetFont(hButton_mainmenu[7],&GUI_Font16B_1);
	BUTTON_SetText(hButton_mainmenu[0],"Music");
	BUTTON_SetText(hButton_mainmenu[1],"MemDev");
	BUTTON_SetText(hButton_mainmenu[2],"Text");
	BUTTON_SetText(hButton_mainmenu[3],"Settings");
	BUTTON_SetText(hButton_mainmenu[4],"Picture");
	BUTTON_SetText(hButton_mainmenu[5],"FFT");
	BUTTON_SetText(hButton_mainmenu[6],"UART");
	BUTTON_SetText(hButton_mainmenu[7],"Oscieiler");
	
	BUTTON_SetBitmapEx (hButton_mainmenu[0],0, &bmmusic4pp, 25, 15);
	BUTTON_SetBitmapEx (hButton_mainmenu[1],0, &bmmemo4pp, 25, 15);
	BUTTON_SetBitmapEx (hButton_mainmenu[3],0, &bmset4pp, 25, 15);
	BUTTON_SetBitmapEx (hButton_mainmenu[4],0, &bmphotobitmap, 110, 15);
	BUTTON_SetBitmapEx (hButton_mainmenu[5],0, &bmFFT1pp, 20, 15);
	BUTTON_SetBitmapEx (hButton_mainmenu[6],0, &bmgame4pp, 25, 15);
	BUTTON_SetBitmapEx (hButton_mainmenu[7],0, &bmoscilloscope1pp, 85, 15);
    while(WM_GetWindowOrgY(hButton_mainmenu[0]) > 25)
    {
        if(WM_GetWindowOrgY(hButton_mainmenu[0]) > 55 )
        {
            WM_Move(MainMenu_pHead,0,-30);
        }
        else 
        {
            WM_Move(MainMenu_pHead,0,-7);
		}
    }
   // hText = TEXT_Create(200, 2,39, 16, GUI_ID_TEXT0, WM_CF_SHOW, "",TEXT_CF_RIGHT);
	//TEXT_SetFont(hText,&GUI_Font13_ASCII);
	//TEXT_SetTextColor(hText,GUI_WHITE);
	
	/* 所有的窗口自动的使用存储设备 */
//   WM_SetCreateFlags(WM_CF_MEMDEV);
}
/********************************************************************************************************** 
** Function name:      Go_MainMenu
** Descriptions:       进入主界面菜单
** input parameters:   无
** output parameters:  
** Returned value:     无
**********************************************************************************************************/
void Go_MainMenu(void)
{
	WM_Move(MainMenu_pHead,-445,0);
	while(WM_GetWindowOrgX(hButton_mainmenu[0]) < 5) {
		if(WM_GetWindowOrgX(hButton_mainmenu[0]) < -35 ) {
			WM_Move(MainMenu_pHead,30,0);
		} else {
			WM_Move(MainMenu_pHead,5 - WM_GetWindowOrgX(hButton_mainmenu[0]),0);
		}
  }
	Menu_ID = MainMenu_ID;
}
/********************************************************************************************************** 
** Function name:      Out_MainMenu
** Descriptions:       退出主界面菜单
** input parameters:   无
** output parameters:  
** Returned value:     无
**********************************************************************************************************/
void Out_MainMenu(void)
{ 
	while(WM_GetWindowOrgX(hButton_mainmenu[0]) < 240) {
		WM_Move(MainMenu_pHead,30,0);
  }
}
/********************************************************************************************************** 
** Function name:      MyCreatDialog
** Descriptions:       实现对话框创建后从左向右滑动进入的效果
** input parameters:   int16_t x,int16_t y,：最终达到的坐标（对话框左上角的坐标）
                       WM_HWIN WM_Handle：对话框的句柄
** output parameters:  
** Returned value:     无
**********************************************************************************************************/
void MyCreatDialog(int16_t x,int16_t y,WM_HWIN WM_Handle)
{
	while(WM_GetWindowOrgX(WM_Handle) < x) {
		WM_MoveWindow (WM_Handle,30, 0);
		GUI_Exec();//重绘
	}
	WM_MoveWindow (WM_Handle,x - WM_GetWindowOrgX(WM_Handle), 0);	
	GUI_Exec();//重绘
}
/********************************************************************************************************** 
** Function name:      MyDeleteDialog
** Descriptions:       实现对话框删除后从左向右滑动推出的效果
** input parameters:   WM_HWIN WM_Handle：对话框的句柄
** output parameters:  
** Returned value:     无
**********************************************************************************************************/
// void MyDeleteDialog(WM_HWIN WM_Handle)
// {
// 	while(WM_GetWindowOrgX(WM_Handle) < 240) {
// 		WM_MoveWindow (WM_Handle,30, 0);
// 		GUI_Exec();//重绘
// 	}
// 	WM_DeleteWindow(WM_Handle);
// }
/********************************************************************************************************** 
** Function name:      Out_OtherMenu
** Descriptions:       退出其他的非主界面菜单（删除所要退出界面里的所有控件）
** input parameters:   无
** output parameters:  
** Returned value:     无
**********************************************************************************************************/
void Out_OtherMenu(void)
{
	uint8_t i = 0;
	SingleList   pos = OtherMenu_pHead;
	for(;i < 6;i++)
	WM_Move(OtherMenu_pHead,40,0);//滑动退出
	
	while(pos) {
	  WM_DeleteWindow(pos -> handle);  //删除链表中的控件
		pos = pos->next;
	}
	SingleListFree(OtherMenu_pHead);     //释放链表
}
/********************************************************************************************************** 
** Function name:      ViscousWM
** Descriptions:       实现界面切换的回弹效果，粘滞效果
** input parameters:   无
** output parameters:  
** Returned value:     无
**********************************************************************************************************/
void ViscousWM(SingleList pHead)
{
  WM_Move(pHead,-20,0);
	WM_Move(pHead,5,0);
	WM_Move(pHead,5,0);
	WM_Move(pHead,5,0);
	WM_Move(pHead,5,0);
}
/********************************************************************************************************** 
** Function name:      WM_Move
** Descriptions:       滑动链表内的所有控件
** input parameters:   SingleList pHead：链表的头指针
                       int dx,int dy ：要移动的位移量
** output parameters:  
** Returned value:     无
**********************************************************************************************************/
void WM_Move(SingleList pHead ,int dx,int dy)
{
    SingleList   pos = pHead;
    if(dy < 0)
    {
        TouchSum += -dy;
    }
    else
    {
        TouchSum +=  dy;
    }
    while(pos)
    {
        WM_MoveWindow (pos -> handle, dx, dy);
        pos = pos->next;
    }
}
/********************************************************************************************************** 
** Function name:      WM_GoTop
** Descriptions:       回到界面顶部
** input parameters:   无
** output parameters:  
** Returned value:     无
**********************************************************************************************************/
// void WM_GoTop(void)
// {
// 	while(WM_GetWindowOrgY(OtherMenu_Info.FirstHandle) < 20) {
// 	while(WM_GetWindowOrgY(OtherMenu_Info.FirstHandle) < 0)
// 		WM_Move(OtherMenu_pHead,0,20);
// 		while(20 - WM_GetWindowOrgY(OtherMenu_Info.FirstHandle) > 5)
// 				WM_Move(OtherMenu_pHead,0,5);
// 		WM_Move(OtherMenu_pHead,0,20 - WM_GetWindowOrgY(OtherMenu_Info.FirstHandle));
// 	}
// }
/********************************************************************************************************** 
** Function name:      WM_MoveCtrl
** Descriptions:       界面滑动控制
** input parameters:   无
** output parameters:  
** Returned value:     无
**********************************************************************************************************/
void WM_MoveCtrl(void)
{
	SingleList MenuHandle;
	MenuInfo   Menu_Info;
	int i = 0;
	//如果触摸屏没有被按下，则直接返回。
	if (TouchFlag == TouchUnPressed) return;
	switch (Menu_ID) {
		
		case MainMenu_ID:
			MenuHandle = MainMenu_pHead;
            Menu_Info.FirstHandle = hButton_mainmenu[0];
            Menu_Info.EndHandle = hButton_mainmenu[7];
			break;
		case SetMenu_ID:
		case TextMenu_ID:
		case FileBrowseMenu_ID:
		case MusicPlayeMenu_ID:
		case ComPortMenu_ID:
            MenuHandle = OtherMenu_pHead;
            Menu_Info.FirstHandle = OtherMenu_Info.FirstHandle;
            Menu_Info.EndHandle = OtherMenu_Info.EndHandle;
		  break;
		case BanMenu:          //不使用MYUI时，将Menu_ID = BanMenu。
			TouchCmd = 0;
			if(MenuDisnable == 0) {
				TouchCmd = 1;
			} else {	
			//	GUI_Exec();   //重绘
				GUI_GetKey(); //读出GUI相应的键值，不能响应键值
			}
			return;
		default:
			return;
	}
	if (TouchFlag == TouchPress) {
	  TouchFlag = TouchPressed;
		TouchSum = 0;
	  while (TouchPoint[0].Pressed == 0){
		  GUI_TOUCH_GetState(&TouchPoint[0]);
	  }
  }
  GUI_TOUCH_GetState(&TouchPoint[1]);
	if (TouchFlag == TouchPressed && TouchPoint[1].Pressed && MenuDisnable == 0) {  //允许滑动操作
		if(TouchPoint[1].y == TouchPoint[0].y) {
		} else {
			WM_Move(MenuHandle,0,TouchPoint[1].y - TouchPoint[0].y);
			TouchPoint[0].y = TouchPoint[1].y ;
			TouchPoint[0].x = TouchPoint[1].x ;
		}
	} 
	if (TouchFlag == TouchUnPress) {
		TouchFlag = TouchUnPressed;	
		if (TouchSum <= 5 && MenuDisnable == 0){    //当界面滑动距离小于5并且允许滑动操作
		  TouchCmd = 1;
		} else {
			TouchCmd = 0;
			//GUI_Exec();   //重绘
			GUI_GetKey(); //读出GUI相应的键值，因为实际上这是滑动操作或者是被禁止的操作，不能响应键值
		}
		/* 实现滑动到最顶部向上的回弹效果 */
		while(WM_GetWindowOrgY(Menu_Info.FirstHandle) > 20) {
			while(WM_GetWindowOrgY(Menu_Info.FirstHandle) > 35)
				WM_Move(MenuHandle,0,-15);
			while(WM_GetWindowOrgY(Menu_Info.FirstHandle) - 20 > 5)
				WM_Move(MenuHandle,0,-5);
			WM_Move(MenuHandle,0,20 - WM_GetWindowOrgY(Menu_Info.FirstHandle));
		}
	  if(WM_GetWindowSizeY(Menu_Info.EndHandle) + WM_GetWindowOrgY(Menu_Info.EndHandle) - WM_GetWindowOrgY(Menu_Info.FirstHandle) < 290){
		} else {
		  i = 320 - WM_GetWindowSizeY(Menu_Info.EndHandle) - 5;
	    /* 实现滑动到最底部向上的回弹效果 */	
		  while(WM_GetWindowOrgY(Menu_Info.EndHandle) < i) {
			  while(i - WM_GetWindowOrgY(Menu_Info.EndHandle) > 15)
				  WM_Move(MenuHandle,0,15);
			  while(i - WM_GetWindowOrgY(Menu_Info.EndHandle) > 5)
				  WM_Move(MenuHandle,0,5);			
		  	WM_Move(MenuHandle,0, i - WM_GetWindowOrgY(Menu_Info.EndHandle));
		  }
    }
		ClearStruct((unsigned char *)&TouchPoint[0],sizeof(TouchPoint));
	}
}
/********************************************************************************************************** 
** Function name:      SetThemeColor
** Descriptions:       改变界面主题颜色
** input parameters:   GUI_COLOR color：颜色值
** output parameters:  无
** Returned value:     无
**********************************************************************************************************/
void SetThemeColor(GUI_COLOR color)
{
	uint8_t i = 8 ;
	while(i){
	  BUTTON_SetBkColor(hButton_mainmenu[i-1],0,color);
	  i--;
	}
	LISTBOX_SetDefaultBkColor(2, color);
	FRAMEWIN_SetDefaultBarColor(1, color);
	ThemeColor = color;
}
/********************************************************************************************************** 
** Function name:      SingleListNodeCreate
** Descriptions:       创建单向链表表头节点
** input parameters:   无
** output parameters:  
** Returned value:     链表头指针
**********************************************************************************************************/
SingleList SingleListNodeCreate ()
{
	SingleList pHead = (SingleList)malloc(sizeof(SingleListNode));
	pHead->next=NULL;
	return pHead;
}
/********************************************************************************************************* 
** Function name:      SingleListInsert
** Descriptions:       单向链表插入运算
** input parameters:   SingleList pList: 要插入的链表
                       SingleList pos: 要插入的节点
                       WM_HMEM x: 要插入的数值
** output parameters:  
** Returned value:     插入节点的指针
**********************************************************************************************************/
SingleList SingleListInsert (SingleList pList,SingleList pos, WM_HMEM x)
{
	SingleList ptr = (SingleList)malloc(sizeof(SingleListNode));
	ptr->handle    = x;
	ptr->next      = pos->next;
	pos->next      = ptr;
	return ptr;
}
/********************************************************************************************************** 
** Function name:      SingleListFree
** Descriptions:       释放链表占用的空间
** input parameters:   CharIn pHead: 链表头指针
** output parameters:  
** Returned value:     无
**********************************************************************************************************/
void SingleListFree (SingleList pHead)
{
	SingleList p = pHead;
	while(p) {
		pHead = pHead->next;
		free(p);
		p = pHead;
	}
}
/********************************************************************************************************** 
** Function name:      ClearStruct
** Descriptions:       将结构体清0
** input parameters:   unsigned char *pta：结构体的首指针
                       int size：结构体的大小
** output parameters:  
** Returned value:     无
**********************************************************************************************************/
void ClearStruct(unsigned char *pta, int size )  
{  
  while(size){  
    *pta++ = 0;  
    size--;  
  }  
}  
