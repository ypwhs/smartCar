#include "gpio.h"
#include "common.h"
#include "uart.h"

 

int main(void)
{
    DelayInit();
    
    /* 初始化一个引脚 设置为推挽输出 */
    GPIO_QuickInit(HW_GPIOD, 7, kGPIO_Mode_OPP);
    GPIO_QuickInit(HW_GPIOE, 6, kGPIO_Mode_OPP);
    
    while(1)
    {
        GPIO_ToggleBit(HW_GPIOD, 7);
        GPIO_ToggleBit(HW_GPIOE, 6);
        DelayMs(50);
    }
}


