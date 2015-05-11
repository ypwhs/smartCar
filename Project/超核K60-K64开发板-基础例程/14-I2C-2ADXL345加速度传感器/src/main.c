#include "gpio.h"
#include "common.h"
#include "uart.h"
#include "i2c.h"
#include "adxl345.h"

int main(void)
{
    short x,y,z;
    short ax, ay, az;
    uint32_t ret;
    DelayInit();
    GPIO_QuickInit(HW_GPIOE, 6, kGPIO_Mode_OPP);
    UART_QuickInit(UART0_RX_PD06_TX_PD07, 115200);
    
    I2C_QuickInit(I2C0_SCL_PB02_SDA_PB03, 100*1000);
    
    printf("adxl345 test\r\n");
    
    ret = adxl345_init(0);
    if(ret)
    {
        printf("no device found!%d\r\n", ret);
    }

    adxl345_calibration();
    
    while(1)
    {
        if(!adxl345_readXYZ(&x, &y, &z))
        {
            adxl345_convert_angle(x, y, z, &ax, &ay, &az);
            printf("X:%3d Y:%3d Z:%3d AX:%3d AY:%3d AZ:%3d  \r", x, y, z, ax, ay ,az); 
        }
        GPIO_ToggleBit(HW_GPIOE, 6);
        DelayMs(20);
    }
}


