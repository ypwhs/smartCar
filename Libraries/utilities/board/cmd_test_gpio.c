#include "shell.h"
#include "gpio.h"
#include "board.h"

int DoGPIO(int argc, char * const argv[])
{
    int i,j,led_num;
    uint32_t led_port_tab[] = BOARD_LED_GPIO_BASES;
    uint32_t led_pin_tab[] = BOARD_LED_PIN_BASES;
    
    led_num = ARRAY_SIZE(led_port_tab);
    printf("board:%s fun:%s\r\n",BOARD_NAME, __func__);
    printf("led num:%d\r\n", led_num);
    
    for(i=0; i<led_num; i++)
    {
        GPIO_QuickInit(led_port_tab[i], led_pin_tab[i], kGPIO_Mode_OPP);
    }
    
    for(j=0;j<10;j++)
    {
        for(i=0;i<led_num;i++)
        {
            GPIO_ToggleBit(led_port_tab[i], led_pin_tab[i]);
        }
        DelayMs(500);
    }
    return 0;
}

SHELL_EXPORT_CMD(DoGPIO, gpio , gpio test)

