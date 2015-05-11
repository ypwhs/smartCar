#ifndef _FILE_OP_H_
#define _FILE_OP_H_

#include <stdint.h>

int SSDInit(void);
int SSD_Read(void *p, int size);
int SSD_Write(void*p, int size);

#endif

