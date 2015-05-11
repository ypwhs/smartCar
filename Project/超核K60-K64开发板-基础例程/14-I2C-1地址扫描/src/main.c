#include "gpio.h"
#include "common.h"
#include "uart.h"
#include "i2c.h"

/* 从此例程开始 使用英文注释 */

/* i2c bus scan */
static void I2C_Scan(uint32_t instance)
{
    uint8_t i;
    uint8_t ret;
    for(i = 1; i < 127; i++)
    {
        ret = I2C_BurstWrite(instance , i, 0, 0, NULL, 0);
        if(!ret)
        {
            printf("ADDR:0x%2X(7BIT) | 0x%2X(8BIT) found!\r\n", i, i<<1);
        }
    }
}

int main(void)
{
    uint32_t instance;
    DelayInit();
    GPIO_QuickInit(HW_GPIOE, 6, kGPIO_Mode_OPP);
    UART_QuickInit(UART0_RX_PD06_TX_PD07, 115200);
    
    printf("i2c bus test\r\n");
    /* init i2c */
    instance = I2C_QuickInit(I2C0_SCL_PB02_SDA_PB03, 100*1000);
    I2C_Scan(instance);
    while(1)
    {
        GPIO_ToggleBit(HW_GPIOE, 6);
        DelayMs(500);
    }
}


