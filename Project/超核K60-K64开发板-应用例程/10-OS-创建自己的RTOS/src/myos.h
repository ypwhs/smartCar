#ifndef __MYOS_H__
#define __MYOS_H__

#include <stdint.h>

#define OS_TASK_MAX_NUM 32

typedef enum OS_TASK_STA
{
  TASK_READY,
  TASK_DELAY,
} OS_TASK_STA;

typedef struct OS_TCB
{
    uint32_t        *StkAddr;
    uint32_t        TimeDly;
    OS_TASK_STA     State;
    const char *    Name;
}OS_TCB,*OS_TCBP;


typedef void (*OS_TASK)(void);
typedef unsigned int OS_STK;


//!< API functions

void OSTaskCreate(const char *name, OS_TCB *tcb, OS_TASK task, OS_STK *stk, uint32_t id);
void OSTimeDly(uint32_t ticks);
void OSTick(void);
void OSInit(void);
void OSStart(void);
OS_TCB* OSGetCurTCB(void);
#endif
