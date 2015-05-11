// dirver lib
#include "chlib_k.h"
#include <absacc.h> 
#include "mb85rc64.h"

// board pin define
#include "boarddef.h"

extern uint16_t gADCValue[16];

//PIT 1 ÖÐ¶Ï
void PIT1_ISR(void)
{
    // print data
    int i, index;
    for(i=0;i<16;i++)
    {

    }
    
}