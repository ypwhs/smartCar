// dirver lib
#include "chlib_k.h"
#include <absacc.h> 
#include "mb85rc64.h"

// board pin define
#include "boarddef.h"

extern const int gADC_InstanceTable[];
extern const int gADC_ChnTable[];
extern uint16_t gADCValue[16];


//PIT0 ÖÐ¶Ï
void PIT0_ISR(void)
{
    uint16_t val;
    int i;
    
    for(i=0;i<16;i++)
    {
        //ADC sampling and store
        ADC_StartConversion(gADC_InstanceTable[i], gADC_ChnTable[i], kADC_MuxA);
        while(ADC_IsConversionCompleted(gADC_InstanceTable[i], kADC_MuxA) == 1);
        gADCValue[i] = ADC_ReadValue(gADC_InstanceTable[i], kADC_MuxA);
    }
}


