#include "mq.h"

#define C_MAX_SIZE          10


typedef struct
{
    msg_t msgs[C_MAX_SIZE];
    uint8_t front;
    uint8_t rear;
} SqQueue_t;

static SqQueue_t msgQueue;


void mq_init(void)
{
    msgQueue.front = msgQueue.rear = 0;
}

uint8_t mq_msg_exist(void)
{
    return msgQueue.front != msgQueue.rear;
}

msg_t *mq_pop(void)
{
    msg_t *pMsg = (void*)0;

    if(msgQueue.front != msgQueue.rear)
    {

        pMsg = &msgQueue.msgs[msgQueue.front];

        msgQueue.front = (msgQueue.front + 1) % C_MAX_SIZE;
    }

    return pMsg;
}

uint8_t mq_push(msg_t pMsg)
{
    if(msgQueue.front == (msgQueue.rear+1)%C_MAX_SIZE)
    {
        return 0;
    }
    else
    {
        msgQueue.msgs[msgQueue.rear] = pMsg;
        msgQueue.rear = (msgQueue.rear + 1) % C_MAX_SIZE;

        return 1;
    }
}


