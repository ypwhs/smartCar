#include "gpio.h"
#include "common.h"
#include "uart.h"
#include "i2c.h"
#include "at24cxx.h"


int main(void)
{
    uint32_t ret;
    DelayInit();
    GPIO_QuickInit(HW_GPIOE, 6, kGPIO_Mode_OPP);
    UART_QuickInit(UART0_RX_PD06_TX_PD07, 115200);
    
    printf("at24c02 test\r\n");
    
    I2C_QuickInit(I2C0_SCL_PB02_SDA_PB03, 100*1000);
    

    ret = at24cxx_init(0);
    if(ret)
    {
        printf("at24cxx not found\r\n");
        while(1);
    }

    if(at24cxx_self_test())
    {
        printf("at24cxx self test failed\r\n");
        return 1;  
    }
    else
    {
        printf("at24cxx test ok!\r\n");
    }
    
    while(1)
    {
        GPIO_ToggleBit(HW_GPIOE, 6);
        DelayMs(500);
    }
}


