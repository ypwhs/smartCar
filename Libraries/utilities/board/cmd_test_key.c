#include "shell.h"
#include "gpio.h"
#include "board.h"

static void GPIOA_ISR(uint32_t array)
{
    printf("%s\r\n", __func__);
}

static void GPIOB_ISR(uint32_t array)
{
    printf("%s\r\n", __func__);
}
static void GPIOC_ISR(uint32_t array)
{
    printf("%s\r\n", __func__);
}
static void GPIOD_ISR(uint32_t array)
{
    printf("%s\r\n", __func__);
}
static void GPIOE_ISR(uint32_t array)
{
    printf("%s\r\n", __func__);
}

int DoKEY(int argc, char * const argv[])
{
    int i,key_num;
    uint32_t key_port_tab[] = BOARD_KEY_GPIO_BASES;
    uint32_t key_pin_tab[] = BOARD_KEY_PIN_BASES;
    
    key_num = ARRAY_SIZE(key_port_tab);
    printf("board:%s fun:%s\r\n",BOARD_NAME, __func__);
    
    printf("key num:%d please press key\r\n", key_num);
    
    GPIO_CallbackInstall(HW_GPIOA, GPIOA_ISR);
    GPIO_CallbackInstall(HW_GPIOB, GPIOB_ISR);
    GPIO_CallbackInstall(HW_GPIOC, GPIOC_ISR);
    GPIO_CallbackInstall(HW_GPIOD, GPIOD_ISR);
    GPIO_CallbackInstall(HW_GPIOE, GPIOE_ISR);
    
    /* init */
    for(i = 0; i < key_num; i++)
    {
        GPIO_QuickInit(key_port_tab[i], key_pin_tab[i], kGPIO_Mode_IPU);
        GPIO_ITDMAConfig(key_port_tab[i], key_pin_tab[i], kGPIO_IT_RisingEdge, true);
    }
    
    return 0;
}

SHELL_EXPORT_CMD(DoKEY, key , key test)
    

