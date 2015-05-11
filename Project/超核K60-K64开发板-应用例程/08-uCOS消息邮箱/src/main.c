#include "chlib_k.h"
//uCOS
#include "includes.h"


/*
* 入门知识:
* 1- 开启uCOS 3个任务 一个发送信号量和邮箱  另外一个接收信号量并显示 另外一个接收消息邮箱并显示相关信息 
*/

#define TASK_STK_SIZE              (128)  //所有任务统一使用128字节堆栈地址
//定义任务优先级
#define APP_START_TASK_PRIO        (4)  //开始任务
#define APP_MBOX_TASK_PRIO         (8)  //邮箱接收显示任务
#define APP_SEM_TASK_PRIO          (9)  //信号量接收显示任务
#define APP_POST_TASK_PRIO         (10) //邮箱 信号量投递任务
//声明任务堆栈
OS_STK  APP_START_STK[TASK_STK_SIZE];
OS_STK  APP_LED_STK[TASK_STK_SIZE];
OS_STK  APP_MBOX_STK[TASK_STK_SIZE];
OS_STK  APP_SEM_STK[TASK_STK_SIZE];
OS_STK  APP_POST_STK[TASK_STK_SIZE];

OS_EVENT * msg_test;		//按键邮箱事件块指针
OS_EVENT * sem_test;		//蜂鸣器信号量指针		  	   



//邮箱投递，信号量投递任务
void AppPostTask(void *pdata)
{
	static uint8_t key='A';
	uint16_t task_counter=0;
	while(1)
	{
		key++;
		task_counter++;
		if(key > 'Z') key='A';            //改变邮箱投递的数据
		OSMboxPost(msg_test, &key);  //发送消息
		OSSemPost(sem_test);              //发送信号量

        printf("App Post Message&Sem:%d times\r\n", task_counter);
		OSTimeDlyHMSM(0, 0, 0, 300);
	}
}

//邮箱接收函数任务
void AppMBOXTask(void *pdata)
{
	uint8_t* key;
	uint8_t err;
	uint16_t TaskCtr=0;
    pdata = pdata; //防止编译器出错 无实际意义
	while(1)
	{
        key = OSMboxPend(msg_test,0,&err);  //等待消息邮箱
        TaskCtr++;
        printf("Received MBox:%d \r\n", *key);
	}
}
//信号量测试，显示函数
void AppSEMTask(void *pdata)
{
	uint8_t err;
	uint16_t TaskCtr=0;
    pdata=pdata; //防止编译器出错 无实际意义
	while(1)
	{
		OSSemPend(sem_test,0,&err); //等待信号量
		TaskCtr++;
		printf("Received Sem:%d  \r\n", TaskCtr);
	}
}


static void AppStartTask(void *pdata)
{
	pdata = pdata; 		  
	msg_test=OSMboxCreate((void*)0);	//创建消息邮箱
	sem_test=OSSemCreate(0);		//创建信号量	
    
    OSTaskCreate(AppMBOXTask,(void *)0,
                &APP_MBOX_STK[TASK_STK_SIZE-1],
                APP_MBOX_TASK_PRIO); //建立邮箱接收显示任务
    OSTaskCreate(AppSEMTask,(void *)0,
                &APP_SEM_STK[TASK_STK_SIZE-1],
                APP_SEM_TASK_PRIO); //建立信号量接收显示任务
    OSTaskCreate(AppPostTask,(void *)0,
                &APP_POST_STK[TASK_STK_SIZE-1],
                APP_POST_TASK_PRIO); //建立邮箱，信号量投递任务
    printf("uCOSII MBox&Sem DemoTest\r\n");
 	OSTaskSuspend(APP_START_TASK_PRIO);	//挂起起始任务.
}

int main(void)
{
    DelayInit();
    SYSTICK_Init(1000*1000/OS_TICKS_PER_SEC);
    SYSTICK_ITConfig(true);
    
    UART_QuickInit(UART0_RX_PD06_TX_PD07, 115200);
    
    printf("uCOSII test\r\n");

    OSInit();
	OSTaskCreate(AppStartTask,(void *)0,
							&APP_START_STK[TASK_STK_SIZE-1],
							APP_START_TASK_PRIO);
    
    SYSTICK_Cmd(true);
    
    OSStart();
    while(1);
}


