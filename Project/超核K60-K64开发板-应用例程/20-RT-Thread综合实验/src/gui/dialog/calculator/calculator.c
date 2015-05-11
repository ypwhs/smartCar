#include    "gui_appdef.h"



/*********************************************************************
*
*       static data
*
**********************************************************************
*/

#define GUI_ID_BUTTON10   GUI_ID_USER+1
#define GUI_ID_BUTTON11   GUI_ID_USER+2
#define GUI_ID_BUTTON12   GUI_ID_USER+3
#define GUI_ID_BUTTON13   GUI_ID_USER+4
#define GUI_ID_BUTTON14   GUI_ID_USER+5
#define GUI_ID_BUTTON15   GUI_ID_USER+6
#define GUI_ID_BUTTON16   GUI_ID_USER+7

typedef	struct{
	double operate1;					//第一个操作数
	double operate2;					//第二个操作数
	double result;					//操作结果
	// 0 第一个操作数 1 第二个操作数
	uint8_t  operatenum;				  //当前操作数
	#define ADD 		0x01			//加法
	#define SUB 		0x02			//减法
	#define MUL			0x04			//乘法
	#define DIV			0x08			//除法
	#define EQUAL		0x10			//等于
	#define NUMBER	0x20			//数字
	#define OPERATE	0x40			//运算符
	#define DOT			0x80			//小数点
	uint8_t pressedsymbol;			   //当前按下的符号
	uint8_t IllegalExpression;			//是否是合法的表达式
	char keyval;						//当前按下的按钮
	uint8_t close;	
}CALCULATE;

CALCULATE Calculate={
0,
0,
0,
0,
0,
0,
0,
0,
};

//char *DisplayContent=NULL;		//需要显示的内容

char DisplayContent[100];

WM_HWIN CalculatorDlg;


void Operate(double *operate,double *result,char symbol);
static uint8_t IsOperator(char opt);
void OperateLeftToRight(char *buf,double *result);

/*********************************************************************
*
*       Dialog resource
*
* This table conatins the info required to create the dialog.
* It has been created by ucGUIbuilder.
*/

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { FRAMEWIN_CreateIndirect,  "Calculator",        0,                       0,  0,  240,320,FRAMEWIN_CF_ACTIVE,0},
    { BUTTON_CreateIndirect,    "7",                 GUI_ID_BUTTON0,          16, 48, 50, 27, 0,0},
    { BUTTON_CreateIndirect,    "8",                 GUI_ID_BUTTON1,          91, 48, 50, 27, 0,0},
    { BUTTON_CreateIndirect,    "9",                 GUI_ID_BUTTON2,          166,48, 50, 27, 0,0},
    { BUTTON_CreateIndirect,    "4",                 GUI_ID_BUTTON3,          15, 89, 50, 27, 0,0},
    { BUTTON_CreateIndirect,    "5",                 GUI_ID_BUTTON4,          91, 89, 50, 27, 0,0},
    { BUTTON_CreateIndirect,    "6",                 GUI_ID_BUTTON5,          166,89, 50, 27, 0,0},
    { BUTTON_CreateIndirect,    "1",                 GUI_ID_BUTTON6,          15, 130,50, 27, 0,0},
    { BUTTON_CreateIndirect,    "2",                 GUI_ID_BUTTON7,          91, 130,50, 27, 0,0},
    { BUTTON_CreateIndirect,    "3",                 GUI_ID_BUTTON8,          166,130,50, 27, 0,0},
    { BUTTON_CreateIndirect,    "0",                 GUI_ID_BUTTON9,          15, 174,100,27, 0,0},
    { BUTTON_CreateIndirect,    ".",                 GUI_ID_BUTTON10,         166,174,65, 35, 0,0},
    { BUTTON_CreateIndirect,    "/",                 GUI_ID_BUTTON11,         241,48, 65, 35, 0,0},
    { BUTTON_CreateIndirect,    "*",                 GUI_ID_BUTTON12,         241,89, 65, 35, 0,0},
    { BUTTON_CreateIndirect,    "-",                 GUI_ID_BUTTON13,         241,130,65, 35, 0,0},
    { BUTTON_CreateIndirect,    "+",                 GUI_ID_BUTTON14,         241,174,65, 35, 0,0},
    { BUTTON_CreateIndirect,    "c",                 GUI_ID_BUTTON15,         316,47, 60, 77, 0,0},
    { BUTTON_CreateIndirect,    "=",                 GUI_ID_BUTTON16,         316,130,60, 79, 0,0},
    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT0,            16, 3,  230,30, 0,0}
};






/*****************************************************************
**      FunctionName:void InitDialog(WM_MESSAGE * pMsg)
**      Function: to initialize the Dialog items
**                                                      
**      call this function in _cbCallback --> WM_INIT_DIALOG
*****************************************************************/

static void InitDialog(WM_MESSAGE * pMsg)
{
    WM_HWIN hWin = pMsg->hWin;
    //
    //FRAMEWIN
    //
    FRAMEWIN_SetClientColor(hWin,0x808080);
    FRAMEWIN_SetFont(hWin,&GUI_FontComic18B_1);
    FRAMEWIN_SetTextAlign(hWin,GUI_TA_VCENTER|GUI_TA_CENTER);
    FRAMEWIN_AddCloseButton(hWin, FRAMEWIN_BUTTON_RIGHT, 0);
    FRAMEWIN_SetTitleHeight(hWin,18);
    //
    //GUI_ID_BUTTON0
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON0),BUTTON_CI_UNPRESSED,0xe16941);
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON0),&GUI_FontComic24B_1);
    //
    //GUI_ID_BUTTON1
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON1),BUTTON_CI_UNPRESSED,0xe16941);
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON1),&GUI_FontComic24B_1);
    //
    //GUI_ID_BUTTON2
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON2),BUTTON_CI_UNPRESSED,0xe16941);
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON2),&GUI_FontComic24B_1);
    //
    //GUI_ID_BUTTON3
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON3),BUTTON_CI_UNPRESSED,0xe16941);
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON3),&GUI_FontComic24B_1);
    //
    //GUI_ID_BUTTON4
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON4),BUTTON_CI_UNPRESSED,0xe16941);
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON4),&GUI_FontComic24B_1);
    //
    //GUI_ID_BUTTON5
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON5),BUTTON_CI_UNPRESSED,0xe16941);
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON5),&GUI_FontComic24B_1);
    //
    //GUI_ID_BUTTON6
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON6),BUTTON_CI_UNPRESSED,0xe16941);
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON6),&GUI_FontComic24B_1);
    //
    //GUI_ID_BUTTON7
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON7),BUTTON_CI_UNPRESSED,0xe16941);
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON7),&GUI_FontComic24B_1);
    //
    //GUI_ID_BUTTON8
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON8),BUTTON_CI_UNPRESSED,0xe16941);
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON8),&GUI_FontComic24B_1);
    //
    //GUI_ID_BUTTON9
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON9),BUTTON_CI_UNPRESSED,0xe16941);
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON9),&GUI_FontComic24B_ASCII);
    //
    //GUI_ID_BUTTON10
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON10),BUTTON_CI_UNPRESSED,0xe16941);
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON10),&GUI_FontComic24B_1);
    //
    //GUI_ID_BUTTON11
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON11),BUTTON_CI_UNPRESSED,0xe16941);
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON11),&GUI_FontComic24B_1);
    //
    //GUI_ID_BUTTON12
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON12),BUTTON_CI_UNPRESSED,0xe16941);
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON12),&GUI_FontComic24B_1);
    //
    //GUI_ID_BUTTON13
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON13),BUTTON_CI_UNPRESSED,0xe16941);
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON13),&GUI_FontComic24B_1);
    //
    //GUI_ID_BUTTON14
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON14),BUTTON_CI_UNPRESSED,0xe16941);
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON14),&GUI_FontComic24B_1);
    //
    //GUI_ID_BUTTON15
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON15),BUTTON_CI_UNPRESSED,0xe16941);
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON15),&GUI_FontComic24B_1);
    //
    //GUI_ID_BUTTON16
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON16),BUTTON_CI_UNPRESSED,0xe16941);
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON16),&GUI_FontComic24B_1);
    //
    //GUI_ID_EDIT0
    //
		EDIT_SetMaxLen(WM_GetDialogItem(hWin,GUI_ID_EDIT0),100);
    EDIT_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_EDIT0),EDIT_CI_ENABLED,0x90ee90);
    EDIT_SetFont(WM_GetDialogItem(hWin,GUI_ID_EDIT0),&GUI_FontComic18B_1);
    EDIT_SetTextAlign(WM_GetDialogItem(hWin,GUI_ID_EDIT0),GUI_TA_VCENTER|GUI_TA_RIGHT);
    EDIT_SetText(WM_GetDialogItem(hWin,GUI_ID_EDIT0),DisplayContent);

}




/*********************************************************************
*
*       Dialog callback routine
*/
static void _cbCallback(WM_MESSAGE * pMsg) 
{
    int NCode, Id;
		int i=0;
		char str[8]; 
    WM_HWIN hWin = pMsg->hWin;
    switch (pMsg->MsgId) 
    {
				case WM_DELETE:
							Calculate.close=1;
							break;
        case WM_INIT_DIALOG:
            InitDialog(pMsg);
            break;
        case WM_KEY:
            switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key) 
            {
                case GUI_KEY_ESCAPE:
                    GUI_EndDialog(hWin, 1);
                    break;
                case GUI_KEY_ENTER:
                    GUI_EndDialog(hWin, 0);
                    break;
            }
            break;
        case WM_NOTIFY_PARENT:
            Id = WM_GetId(pMsg->hWinSrc); 
            NCode = pMsg->Data.v;        
            switch (Id) 
            {
                case GUI_ID_OK:
                    if(NCode==WM_NOTIFICATION_RELEASED)
                        GUI_EndDialog(hWin, 0);
                    break;
                case GUI_ID_CANCEL:
                    if(NCode==WM_NOTIFICATION_RELEASED)
                        GUI_EndDialog(hWin, 0);
                    break;
								case GUI_ID_BUTTON0:	// 7
										if(NCode==WM_NOTIFICATION_RELEASED){
												Calculate.keyval='7';
											  Calculate.pressedsymbol |= NUMBER;
										}
										break;
								case GUI_ID_BUTTON1:	// 8
										if(NCode==WM_NOTIFICATION_RELEASED){
											 Calculate.keyval='8';
											 Calculate.pressedsymbol |= NUMBER;
										}
										break;
								case GUI_ID_BUTTON2:// 9
										if(NCode==WM_NOTIFICATION_RELEASED){
											Calculate.keyval='9';
											Calculate.pressedsymbol |= NUMBER;
										}
										break;
								case GUI_ID_BUTTON3:	// 4
										if(NCode==WM_NOTIFICATION_RELEASED){
											Calculate.keyval='4';
											Calculate.pressedsymbol |= NUMBER;
										}
										break;
								case GUI_ID_BUTTON4:	//5
										if(NCode==WM_NOTIFICATION_RELEASED){
											Calculate.keyval='5';
											Calculate.pressedsymbol |= NUMBER;
										}
										break;
								case GUI_ID_BUTTON5:// 6
										if(NCode==WM_NOTIFICATION_RELEASED){
											Calculate.keyval='6';
											Calculate.pressedsymbol |= NUMBER;
										}
										break;
								case GUI_ID_BUTTON6://1
										if(NCode==WM_NOTIFICATION_RELEASED){
											Calculate.keyval='1';
											Calculate.pressedsymbol |= NUMBER;
										}
										break;
								case GUI_ID_BUTTON7://2
										if(NCode==WM_NOTIFICATION_RELEASED){
											Calculate.keyval='2';
											Calculate.pressedsymbol |= NUMBER;
										}
										break;
								case GUI_ID_BUTTON8://3
										if(NCode==WM_NOTIFICATION_RELEASED){
											Calculate.keyval='3';
											Calculate.pressedsymbol |= NUMBER;
										}
										break;
								case GUI_ID_BUTTON9://0
										if(NCode==WM_NOTIFICATION_RELEASED){
											Calculate.keyval='0';
											Calculate.pressedsymbol |= NUMBER;
										}
										break;
								case GUI_ID_BUTTON10:// .
										if(NCode==WM_NOTIFICATION_RELEASED){
											  if((Calculate.pressedsymbol&NUMBER)==NUMBER&&(Calculate.pressedsymbol&DOT)!=DOT){
													Calculate.pressedsymbol |= DOT;
													Calculate.keyval='.';
													Calculate.pressedsymbol &= ~NUMBER;
													strcat(DisplayContent,&Calculate.keyval);
													EDIT_SetText(WM_GetDialogItem(CalculatorDlg,GUI_ID_EDIT0),DisplayContent);	
													Calculate.keyval=0;														
												}

										}
										break;
								case GUI_ID_BUTTON11:// /
										if(NCode==WM_NOTIFICATION_RELEASED){
											if((Calculate.pressedsymbol&NUMBER)==NUMBER&&(Calculate.pressedsymbol&OPERATE)!=OPERATE){
												Calculate.keyval='/';
												Calculate.pressedsymbol |= OPERATE;
												Calculate.pressedsymbol &= ~NUMBER;
												Calculate.pressedsymbol &= ~DOT;		//接下来可以输入小数点
												strcat(DisplayContent,&Calculate.keyval);
												EDIT_SetText(WM_GetDialogItem(CalculatorDlg,GUI_ID_EDIT0),DisplayContent);	
												Calculate.keyval=0;		
											}
										}
										break;
								case GUI_ID_BUTTON12:// *
										if(NCode==WM_NOTIFICATION_RELEASED){
											if((Calculate.pressedsymbol&NUMBER)==NUMBER&&(Calculate.pressedsymbol&OPERATE)!=OPERATE){
												Calculate.keyval='*';
												Calculate.pressedsymbol |= OPERATE;
												Calculate.pressedsymbol &= ~NUMBER;
												Calculate.pressedsymbol &= ~DOT;		//接下来可以输入小数点
												strcat(DisplayContent,&Calculate.keyval);
												EDIT_SetText(WM_GetDialogItem(CalculatorDlg,GUI_ID_EDIT0),DisplayContent);	
												Calculate.keyval=0;	
											}
										}
										break;
								case GUI_ID_BUTTON13:// -
										if(NCode==WM_NOTIFICATION_RELEASED){
											if((Calculate.pressedsymbol&NUMBER)==NUMBER&&(Calculate.pressedsymbol&OPERATE)!=OPERATE){
												Calculate.keyval='-';
												Calculate.pressedsymbol |= OPERATE;
												Calculate.pressedsymbol &= ~NUMBER;
												Calculate.pressedsymbol &= ~DOT;		//接下来可以输入小数点
												strcat(DisplayContent,&Calculate.keyval);
												EDIT_SetText(WM_GetDialogItem(CalculatorDlg,GUI_ID_EDIT0),DisplayContent);	
												Calculate.keyval=0;													
											}
										}
										break;
								case GUI_ID_BUTTON14:// +
										if(NCode==WM_NOTIFICATION_RELEASED){
											if((Calculate.pressedsymbol&NUMBER)==NUMBER&&(Calculate.pressedsymbol&OPERATE)!=OPERATE){
												Calculate.keyval='+';
												Calculate.pressedsymbol |= OPERATE;
												Calculate.pressedsymbol &= ~NUMBER;
												Calculate.pressedsymbol &= ~DOT;		//接下来可以输入小数点
												strcat(DisplayContent,&Calculate.keyval);
												EDIT_SetText(WM_GetDialogItem(CalculatorDlg,GUI_ID_EDIT0),DisplayContent);	
												Calculate.keyval=0;													
											}
										}
										break;
								case GUI_ID_BUTTON15:// c
										if(NCode==WM_NOTIFICATION_RELEASED){
												strcpy(DisplayContent,"");
											  Calculate.result=0;
												EDIT_SetText(WM_GetDialogItem(CalculatorDlg,GUI_ID_EDIT0),DisplayContent);
												Calculate.pressedsymbol &= ~NUMBER;		
												Calculate.keyval=0;	
										}
										break;
								case GUI_ID_BUTTON16:// =
										if(NCode==WM_NOTIFICATION_RELEASED){
											 printf("DisplayContent:%s\r\n",DisplayContent);
												if(strlen(DisplayContent)>0){
													OperateLeftToRight(DisplayContent,&Calculate.result);
												//	printf("Calculate.result:%d\r\n",(uint32_t)Calculate.result);
													if((Calculate.result-(uint32_t)Calculate.result)!=0){
														//sprintf((char *)DisplayContent,"%0.11e",(uint32_t)Calculate.result); 不行，不知道为什么
														sprintf((char *)DisplayContent,"%d",(uint32_t)Calculate.result);
														strcat(DisplayContent,".");
														sprintf((char *)str,"%d",(uint32_t)((Calculate.result-(uint32_t)Calculate.result)*10000000));
														strcat(DisplayContent,str);
														for(i=strlen(DisplayContent)-1;i>=0;i--){
																if(DisplayContent[i]!='0'){
																	DisplayContent[i+1]='\0';
																	break;
																}
														}
													}else{
														sprintf(DisplayContent,"%ld",(uint32_t)Calculate.result);
													}
													EDIT_SetText(WM_GetDialogItem(CalculatorDlg,GUI_ID_EDIT0),DisplayContent);
													Calculate.keyval=0;			
									
												}
										}
										break;										
            }
            break;
        default:
            WM_DefaultProc(pMsg);
    }
}

 /*
***************************************************************
*函数名：Operate
*功能：	 计算
*参数：	 无
*返回值：无
****************************************************************
*/
void Operate(double *operate,double *result,char symbol){
	 switch(symbol){
			case '+':
					*result += *operate;
				break;
			case '-':
				  *result -= *operate;
				break;
			case '*':
					(*result) *= (*operate);
				break;
			case '/':
					if((*operate)==0){
						Calculate.IllegalExpression = 1;
					}else{
						(*result) = (double)(*result)/(double)(*operate);

					}
				break;
			default:
				break;
	 }
}


 /*
***************************************************************
*函数名：IsOperator
*功能：	 判断此字符是不是运算符
*参数：	 
*返回值：0:是 1：不是
****************************************************************
*/
static uint8_t IsOperator(char opt){
	if(opt=='+'||opt=='-'||opt=='*'||opt=='/'){
			return 0;
	}
	return 1;
}


 /*
***************************************************************
*函数名：OperateLeftToRight
*功能：	 从左到右计算表达式，不考虑优先级 输入的表达式必须合法
*参数：	 
*返回值：
****************************************************************
*/
void OperateLeftToRight(char *buf,double *result){
	double opt;
	char *temp;
	char val;
	int count=0;
	char sym;
    temp = (char*)rt_malloc(100);
    while(*buf){
		val = *buf;
		if(!IsOperator(*buf)){
				count++;
				if(count==1){
					*result = atof(temp);
					sym = *buf;				//得到操作符
				}else if(count==2){
					count=1;
					opt= atof(temp);
					Operate(&opt,result,sym);
					sym = *buf;	
				}
				strcpy(temp,"");
		}else{
			strcat(temp,&val);
		}
		buf++;
	}
	opt = atof(temp);
	Operate(&opt,result,sym);
	rt_free(temp);
}


WM_HWIN GUI_AppDispCalculator(void)
{

    strcpy(DisplayContent,"");
    WM_SetDesktopColor(GUI_WHITE);      /* Automacally update desktop window */
    WM_SetCreateFlags(WM_CF_MEMDEV); 
    CalculatorDlg = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, 0, 0, 0);
    while(1){
    if((Calculate.pressedsymbol&NUMBER)==NUMBER){
 						strcat(DisplayContent,&Calculate.keyval);
 						EDIT_SetText(WM_GetDialogItem(CalculatorDlg,GUI_ID_EDIT0),DisplayContent);	
						Calculate.keyval=0;		
						Calculate.pressedsymbol &= ~OPERATE;					
				}		
				if(Calculate.close){
						Calculate.close = 0;

						break;
				}
				GUI_Delay(10);
		}
}

