#include "chlib_k.h"
#include <absacc.h> 
#include "mb85rc64.h"


extern uint16_t gADCValue[16];

//UART 0 жу╤к
void UART0_RX_ISR(uint16_t data)
{
    switch(data)
    {
        case '0':
            printf("AAAAAAAAAAAAADC[%d]:%d\r\n", 0, gADCValue[0]);
            break;
        case '1':
            printf("ADC[%d]:%d\r\n", 1, gADCValue[1]);
            break;
        case '2':
            printf("ADC[%d]:%d\r\n", 2, gADCValue[2]);
            break;
        case '3':
            printf("ADC[%d]:%d\r\n", 3, gADCValue[3]);
            break;
        case '4':
            printf("ADC[%d]:%d\r\n", 4, gADCValue[4]);
            break;
        case '5':
            printf("ADC[%d]:%d\r\n", 5, gADCValue[5]);
            break;
        case '6':
            printf("ADC[%d]:%d\r\n", 6, gADCValue[6]);
            break;
        case '7':
            printf("ADC[%d]:%d\r\n", 7, gADCValue[7]);
            break;
        default:
            printf("please add more chl by your self\r\n");
            break;
        
    }
}

