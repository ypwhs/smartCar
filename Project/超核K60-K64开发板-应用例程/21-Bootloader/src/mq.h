#ifndef MESSAGE_MANAGE_H_INCLUDED
#define MESSAGE_MANAGE_H_INCLUDED

#include <stdint.h>

typedef struct
{
    uint32_t cmd;
    uint32_t size;
    uint8_t* p;
} msg_t;

extern void mq_init(void);
extern uint8_t mq_msg_exist(void);
extern msg_t *mq_pop(void);
extern uint8_t mq_push(msg_t msg);




#endif // MESSAGE_MANAGE_H_INCLUDED
