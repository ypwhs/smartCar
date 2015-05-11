#ifndef _TRANS_H_
#define _TRANS_H_

#include <stdint.h>



int trans_init(uint8_t dmaChl, uint32_t uartInstance);
int trans_start_send_data(uint8_t* buf, uint32_t size);
int trans_is_transmission_complete(void);

#endif
