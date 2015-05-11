#include <rtthread.h>
#include <rthw.h>

#include "gpio.h"
#include "gui.h"
#include "board.h"



void led_thread_entry(void* parameter)
{
    int i,led_num;
    
    uint32_t led_port_tab[] = BOARD_LED_GPIO_BASES;
    uint32_t led_pin_tab[] = BOARD_LED_PIN_BASES;
    led_num = ARRAY_SIZE(led_port_tab);
    for(i=0; i<led_num; i++)
    {
        GPIO_QuickInit(led_port_tab[i], led_pin_tab[i], kGPIO_Mode_OPP);
    }
    while(1)
    {
        for(i=0;i<led_num;i++)
        {
            GPIO_ToggleBit(led_port_tab[i], led_pin_tab[i]);
        }
        rt_thread_delay(50);
    }
}


