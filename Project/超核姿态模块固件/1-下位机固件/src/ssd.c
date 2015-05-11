#include "flashOS.H"
#include "common.h"
#include "ssd.h"
#include <string.h>

#define SECTER_SIZE      0x000400
#define FILE_ADDR_BEIGN  (0x00000000 + 1024*31)

int SSDInit(void)
{
    uint32_t clock;
    uint32_t flash_clock = CLOCK_GetClockFrequency(kFlashClock, &clock);
    /* fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify) */     
    return Init(0x00000000, clock, 2);
}


int SSD_Read(void *p, int size)
{
    memcpy(p, (uint8_t*)FILE_ADDR_BEIGN, size);
    return 0;
}

int SSD_Write(void*p, int size)
{
    int r;
    EraseSector(FILE_ADDR_BEIGN);
    r = ProgramPage(FILE_ADDR_BEIGN, size, p);
    return r;
}



