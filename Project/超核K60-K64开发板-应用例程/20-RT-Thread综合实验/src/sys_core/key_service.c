#include <rtthread.h>
#include <rthw.h>

#include "gpio.h"
#include "gui.h"
#include "board.h"
#include "gui_appdef.h"

void key_thread_entry(void* parameter)
{
    int i,key_num;
    uint32_t key_port_tab[] = {HW_GPIOE, HW_GPIOE};
    uint32_t key_pin_tab[] = {26, 27};
    key_num = ARRAY_SIZE(key_port_tab);
    for(i=0; i<key_num; i++)
    {
        GPIO_QuickInit(key_port_tab[i], key_pin_tab[i], kGPIO_Mode_IPU);
    }
    while(1)
    {
        for(i=0; i<key_num; i++)
        {
            if(GPIO_ReadBit(key_port_tab[i], key_pin_tab[i]) == 0)
            {
                switch(i)
                {
                    case 0:
                        GUI_SendKeyMsg(GUI_KEY_ENTER, 1);
                        break;
                    case 1:
                        GUI_SendKeyMsg(GUI_KEY_ESCAPE, 1);
                        break;
                    default:
                        break;
                }
                
            }
        }
        rt_thread_delay(10);
    }
}


