#include <stdint.h>
#include <string.h>
#include "myos.h"



extern void OSCtxSw(void);
/* current running TCB */
OS_TCBP OSTCBCur; 
/* the nect TCB to be running */
OS_TCBP OSTCBHighRdy;
OS_TCB  OSTCB_Idle;
OS_STK  OSTaskIdleStack[64];
/* TCB pointer array */
static OS_TCBP OS_TCB_TABLE[OS_TASK_MAX_NUM];

 /**
 * @brief  get the first Ready task and running it
 * @param  None
 * @retval None
 */
void _OS_Scheduling(void)
{
    uint32_t i;
    for(i = 0; i < OS_TASK_MAX_NUM; i++)
    {
        if(OS_TCB_TABLE[i] != NULL)
        {
            if(OS_TCB_TABLE[i]->State == TASK_READY)
            {
               // printf("ID:%d\r\n", i);
                OSTCBHighRdy = OS_TCB_TABLE[i];
                break;
            }
        }
    }
    OSCtxSw();
}

 /**
 * @brief  when a task finish(which mean it's not a super loop), it come here
 * @param  None
 * @retval None
 */
void _TaskEnd(void)
{
    uint32_t i;
    uint32_t end_task_id;
    printf("_TaskEnd\r\n");
    /* get the TCB and delete it */
    for(i = 0; i < OS_TASK_MAX_NUM; i++)
    {
        if(OS_TCB_TABLE[i] == OSTCBCur)
        {
            end_task_id = i;
            break;
        }
    }
    OS_TCB_TABLE[end_task_id] = NULL;
    _OS_Scheduling();
}

void _TaskIdle(void)
{
    printf("Task Idle Enter\r\n");
    while(1)
    {
        _OS_Scheduling();
    }
}

void OSInit(void)
{
    /* create idle task */
    OSTaskCreate("idle", &OSTCB_Idle, _TaskIdle, &OSTaskIdleStack[63], OS_TASK_MAX_NUM-1);
    OSTCBCur = OS_TCB_TABLE[OS_TASK_MAX_NUM-1];
}

void OSStart(void)
{
    _OS_Scheduling();
}

void OSTimeDly(uint32_t ticks)
{
    OSTCBCur->TimeDly = ticks;
    OSTCBCur->State = TASK_DELAY;
    _OS_Scheduling();
}

 /**
 * @brief  count each task's delay counter if it reach 0, then set State to TASK_READY
 * @note   this function should be called proidely
 * @param  None
 * @retval None
 */
void OSTick(void)
{
    uint32_t i;

    for(i = 0; i < OS_TASK_MAX_NUM; i++)
    {
        if(OS_TCB_TABLE[i] != NULL)
        {
            if(OS_TCB_TABLE[i]->State == TASK_DELAY)
            {
                OS_TCB_TABLE[i]->TimeDly--;
                if(OS_TCB_TABLE[i]->TimeDly == 0)
                {
                    OS_TCB_TABLE[i]->State = TASK_READY; 
                }
            }  
        }
    }
}

void OSTaskCreate(const char * name, OS_TCB *tcb, OS_TASK task, OS_STK *stk, uint32_t id)
{
    OS_STK  *p_stk;
    p_stk      = stk;
    p_stk      = (OS_STK *)((OS_STK)(p_stk) & 0xFFFFFFF8u);
    
    *(--p_stk) = (OS_STK)0x01000000uL;                          //xPSR
    *(--p_stk) = (OS_STK)task;                                  // Entry Point
    *(--p_stk) = (OS_STK)_TaskEnd;                              // R14 (LR)
    *(--p_stk) = (OS_STK)0x12121212uL;                          // R12
    *(--p_stk) = (OS_STK)0x03030303uL;                          // R3
    *(--p_stk) = (OS_STK)0x02020202uL;                          // R2
    *(--p_stk) = (OS_STK)0x01010101uL;                          // R1
    *(--p_stk) = (OS_STK)0x00000000u;                           // R0
    
    *(--p_stk) = (OS_STK)0x11111111uL;                          // R11
    *(--p_stk) = (OS_STK)0x10101010uL;                          // R10
    *(--p_stk) = (OS_STK)0x09090909uL;                          // R9
    *(--p_stk) = (OS_STK)0x08080808uL;                          // R8
    *(--p_stk) = (OS_STK)0x07070707uL;                          // R7
    *(--p_stk) = (OS_STK)0x06060606uL;                          // R6
    *(--p_stk) = (OS_STK)0x05050505uL;                          // R5
    *(--p_stk) = (OS_STK)0x04040404uL;                          // R4
    
    OS_TCB_TABLE[id] = tcb;
    OS_TCB_TABLE[id]->TimeDly = 1;
    OS_TCB_TABLE[id]->State = TASK_READY;
    OS_TCB_TABLE[id]->Name = name;
    tcb->StkAddr = p_stk;
}

OS_TCB* OSGetCurTCB(void)
{
    return OSTCBCur;
}

