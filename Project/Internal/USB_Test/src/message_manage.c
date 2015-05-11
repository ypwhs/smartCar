#include "message_manage.h"

#define C_MAX_SIZE          10
//
typedef struct
{
    MessageType_t m_Msg[C_MAX_SIZE];
    uint8_t front;
    uint8_t rear;
}SqQueue_t;

//
static SqQueue_t msgQueue;
////
void fn_queue_init(void)
{
    msgQueue.front = msgQueue.rear = 0;
}
////
uint8_t fn_msg_exist(void)
{
    return msgQueue.front != msgQueue.rear;
}
////
MessageType_t *fn_msg_pop(void)
{
    MessageType_t *pMsg = (void*)0;
    if(msgQueue.front != msgQueue.rear)
    {
        pMsg = &msgQueue.m_Msg[msgQueue.front];
        msgQueue.front = (msgQueue.front + 1) % C_MAX_SIZE;
    }
    return pMsg;
}
////
uint8_t fn_msg_push(MessageType_t pMsg)
{ 
    if(msgQueue.front == (msgQueue.rear+1)%C_MAX_SIZE)
    {
        return 0;
    }
    else
    {
        msgQueue.m_Msg[msgQueue.rear] = pMsg;
        msgQueue.rear = (msgQueue.rear + 1) % C_MAX_SIZE;
        
        return 1;
    }
}


