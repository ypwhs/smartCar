#include "gpio.h"
#include "common.h"
#include "uart.h"
#include "scope.h"
#include "i2c.h"
#include "CHZT02.h"
#include "imu.h"
#include "mpu9250.h"
#include "protocol.h"

void imu_test(void)
{
    uint32_t len;
    imu_io_install_t IOInstallStruct;
    imu_float_euler_angle_t angle;
    imu_raw_data_t raw;
    transmit_user_data send_data;
    static uint8_t buf[64];
    
    IOInstallStruct.imu_get_accel = mpu9250_read_accel_raw;
    IOInstallStruct.imu_get_gyro =  mpu9250_read_gyro_raw;
    IOInstallStruct.imu_get_mag = mpu9250_read_mag_raw;
    
    imu_io_install(&IOInstallStruct); 

    while(1)
    {
        

        imu_get_euler_angle(&angle, &raw);
        
        memset(&send_data, 0, sizeof(transmit_user_data));
        
        send_data.trans_pitch = (int16_t)(angle.imu_pitch*100);
        send_data.trans_roll = (int16_t)(angle.imu_roll*100);
        send_data.trans_yaw = (int16_t)(angle.imu_yaw*10);
       //  send_data.trans_yaw = 0;

        uint8_t *p;
        
        p = buf;
        
        /* set buffer */
        len = user_data2buffer(&send_data, buf);
        
        while(len--)
        {
            UART_WriteByte(HW_UART0, *p++);
        }
        
       // printf("P:%0.2f R:%0.2f Y:%0.2f  \r", angle.imu_pitch, angle.imu_roll, angle.imu_yaw);
        //DelayMs(1);
        GPIO_ToggleBit(HW_GPIOC, 3);
    }
}

void mpu9250_test(void)
{
    uint8_t err;
    int16_t ax,ay,az,gx,gy,gz,mx,my,mz;

    
    err = 0;
    
    err += mpu9250_read_accel_raw(&ax, &ay, &az);
    err += mpu9250_read_gyro_raw(&gx, &gy, &gz);
    err += mpu9250_read_mag_raw(&mx, &my, &mz);
    
    if(err)
    {
        printf("!err:%d\r\n", err);
        while(1);
    }
    printf("ax:%05d ay:%05d az:%05d gx:%05d gy:%05d gz:%05d mx:%d my:%d mz:%d    \r", ax ,ay, az, gx, gy, gz, mx, my, mz);
}

int main(void)
{
	uint32_t clock;
    struct mpu_config config;
    
	DelayInit();    
    GPIO_QuickInit(HW_GPIOC, 3, kGPIO_Mode_OPP);    
    UART_QuickInit(UART0_RX_PA01_TX_PA02, 115200);    
    printf("HelloWorld\r\n");
    CLOCK_GetClockFrequency(kCoreClock, &clock);
    printf("CoreClock:%dHz\r\n", clock);
    CLOCK_GetClockFrequency(kBusClock, &clock);
    printf("kBusClock:%dHz\r\n", clock);

    /* MCU driver init */
    I2C_QuickInit(I2C0_SCL_PB00_SDA_PB01, 100*1000);
    GPIO_QuickInit(HW_GPIOD, 7, kGPIO_Mode_OPP);
	GPIO_WriteBit(HW_GPIOD, 7, 0);
    DelayMs(10);
    
    /* sensor init */
    mpu9250_init(0);
    
    config.afs = AFS_16G;
    config.gfs = GFS_2000DPS;
    config.aenable_self_test = false;
    config.genable_self_test = false;
    mpu9250_config(&config);

    while(1)
    {
       // mpu9250_test();
        imu_test();
		GPIO_ToggleBit(HW_GPIOC, 3);
        DelayMs(10);
    }
}


