#include "gpio.h"
#include "common.h"

 
int main(void)
{
    /* ≥ı ºªØDelay */
    DelayInit();
    DWT_DelayInit();

    GPIO_QuickInit(HW_GPIOE, 6, kGPIO_Mode_OPP);

    GPIO_WriteBit(HW_GPIOE, 6, 0);
    
    while(1)
    {
        PEout(6) = !PEout(6);
        DWT_DelayMs(500);
    }
}


