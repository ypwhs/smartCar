//包含超核库所需的头文件
#include "gpio.h"
#include "common.h"
#include "uart.h"
#include "i2c.h"
#include "systick.h"
#include "stdio.h"
#include "pit.h"
#include "nrf24l01.h"
#include "wdog.h"
#include "board.h"
#include "protocol.h"

//包含 姿态模块所需的头文件
#include "mpu6050.h"
#include "hmc5883.h"
#include "bmp180.h"
#include "imu.h"
#include "trans.h"
#include "dma.h"
#include "spi.h"

#include "ssd.h"

int g2401Avalable;
/* 校准数据 */

struct calibration_data
{
    int  flag;
    float meg_x_gain;
    float meg_y_gain;
    float meg_z_gain;
    int   meg_x_off;
    int   meg_y_off;
    int   meg_z_off;
};


static struct calibration_data cal_data;

static imu_float_euler_angle_t angle;
static imu_raw_data_t raw_data;
static int32_t temperature;
static int32_t pressure;
static uint8_t buf[64];


int hmc5883_read_data2(int16_t* x, int16_t* y, int16_t* z)
{
    int r;
    int16_t xraw,yraw,zraw;
    r = hmc5883_read_data(&xraw, &yraw, &zraw);
    if(!r)
    {
        *x = cal_data.meg_x_gain  *(xraw - cal_data.meg_x_off);
        *y = cal_data.meg_y_gain *(yraw - cal_data.meg_y_off);
        *z = cal_data.meg_z_gain *(zraw - cal_data.meg_z_off);
        xraw = *x;
        yraw = *y;
        zraw = *z;
    }
    return r;
}

//实现姿态解算的回调并连接回调
static imu_io_install_t IMU_IOInstallStruct1 = 
{
    .imu_get_accel = mpu6050_read_accel,
    .imu_get_gyro = mpu6050_read_gyro,
    .imu_get_mag = hmc5883_read_data,
};



static void PIT_CH1_ISR(void)
{
    uint32_t len;
    static transmit_user_data send_data;

    send_data.trans_accel[0] = raw_data.ax;
    send_data.trans_accel[1] = raw_data.ay;
    send_data.trans_accel[2] = raw_data.az;
    send_data.trans_gyro[0] = raw_data.gx;
    send_data.trans_gyro[1] = raw_data.gy;
    send_data.trans_gyro[2] = raw_data.gz;
    send_data.trans_mag[0] = raw_data.mx;
    send_data.trans_mag[1] = raw_data.my;
    send_data.trans_mag[2] = raw_data.mz;
    send_data.trans_pitch = (int16_t)(angle.imu_pitch*100);
    send_data.trans_roll = (int16_t)(angle.imu_roll*100);
    send_data.trans_yaw = 1800 + (int16_t)(angle.imu_yaw*10);
    
    /* pressure */
    send_data.trans_pressure = pressure;
    
    /* set buffer */
    len = user_data2buffer(&send_data, buf);
    trans_start_send_data(buf, len);

    GPIO_ToggleBit(HW_GPIOA, 1);
    WDOG_Refresh();
}



int init_2401(void)
{
    int r,i;
    QuickInit_Type pq;
    QuickInitDecode(SPI0_SCK_PC05_SOUT_PC06_SIN_PC07, &pq);
    SPI_QuickInit(BOARD_SPI_MAP, kSPI_CPOL0_CPHA0, 1000*1000);


    
    PORT_PinMuxConfig(BOARD_SPI_CS_PORT, BOARD_SPI_CS_PIN, kPinAlt2);
    GPIO_QuickInit(HW_GPIOB, 0, kGPIO_Mode_OPP);
    
    /* init 2401 */
    r = nrf24l01_init(HW_SPI0, BOARD_SPI_CS);
    r = nrf24l01_probe();
    g2401Avalable = true;
    if(r)
    {
        g2401Avalable = false;
    }
    printf("2401 init:%d\r\n", r);
    return r;
}

int init_sensor(void)
{
    uint32_t ret;

    I2C_QuickInit(I2C0_SCL_PE19_SDA_PE18, 100*1000);
    
    ret = mpu6050_init(0);
    if(ret)
    {
        printf("mpu6050 failed:%d\r\n", ret);
    }
    ret = hmc5883_init(0);
    if(ret)
    {
        printf("hmc5883 failed:%d\r\n", ret);
    }
    ret = bmp180_init(0);
    if(ret)
    {
        printf("bmp180 failed:%d\r\n", ret);
    }
    return ret;
}

# if 0
void MagnetometerCalibration(struct calibration_data * cal)
{
    uint32_t i;
    int r;
    int16_t x,y,z;
    int16_t xmax, xmin, ymax, ymin, zmax, zmin;
    xmax = 0;
    xmin = 0xFFFF;
    ymax = 0;
    ymin = 0xFFFF;
    zmax = 0;
    zmin = 0xFFFF;
    printf("start calibration, read flash...\r\n");
    SSD_Read(cal, sizeof(struct calibration_data));
    if(cal->flag == 0x5A)
    {
        printf("flash data read succ\r\n");
        printf("Gain X:%f Y:%f Z:%f\r\n", cal->meg_x_gain, cal->meg_y_gain, cal->meg_z_gain);
        printf("Off X:%d Y:%d Z:%d\r\n", cal->meg_x_off, cal->meg_y_off, cal->meg_z_off);
        return;
    }
    for(i=0;i<1000;i++)
    {
        r = hmc5883_read_data(&x, &y, &z);
        if(!r)
        {
            if(xmax < x) xmax = x;
            if(xmin > x) xmin = x;
            if(ymax < y) ymax = y;
            if(ymin > y) ymin = y;
            if(zmax < z) zmax = z;
            if(zmin > z) zmin = z; 
        }
        DelayMs(10);
        printf("time:%04d xmax:%04d xmin:%04d ymax:%04d ymin%04d zmax:%04d zmin:%04d\r", i,xmax,xmin,ymax,ymin,zmax,zmin);
    }
    cal->meg_x_off = (xmax + xmin) / 2;
    cal->meg_x_gain=1;
    cal->meg_y_off = (ymax + ymin) / 2;
    cal->meg_y_gain= (float)(xmax - xmin) / (float)(ymax -ymin);
    cal->meg_z_off = (zmax + zmin) / 2;
    cal->meg_z_gain= (float)(xmax - xmin) / (float)(zmax -zmin);
    /* see if we get data correct */
    if((xmax < 300) || (ymax < 300) || (zmax < 300) || (cal->meg_y_gain < 0.8) || (cal->meg_z_gain < 0.8))
    {
        printf("cal failed, setting to default param\r\n");
        /* inject with default data */
        cal->meg_x_off = 0;
        cal->meg_y_off = 0;
        cal->meg_z_off = 0;
        cal->meg_x_gain = 1;
        cal->meg_y_gain = 1;
        cal->meg_z_gain = 1;
    }
    printf("Gain X:%f Y:%f Z:%f\r\n", cal->meg_x_gain, cal->meg_y_gain, cal->meg_z_gain);
    printf("Off X:%d Y:%d Z:%d\r\n", cal->meg_x_off, cal->meg_y_off, cal->meg_z_off);
    cal->flag = 0x5A;
    SSD_Write(cal, sizeof(struct calibration_data));
}
#endif

#define BMP_STATUS_T_START          (0x00)
#define BMP_STATUS_T_COMPLETE       (0x01)
#define BMP_STATUS_P_START          (0x02)
#define BMP_STATUS_P_COMPLETE       (0x03)
#define BMP_STATUS_T_WAIT           (0x04)
#define BMP_STATUS_P_WAIT           (0x05)

int main(void)
{
    int i;
    static int bmpStatus = BMP_STATUS_T_START;
    uint32_t ret;
    uint32_t uart_instance;
    /* basic hardware */
    DelayInit();
    GPIO_QuickInit(HW_GPIOA, 1, kGPIO_Mode_OPP);  
    uart_instance = UART_QuickInit(BOARD_UART_DEBUG_MAP, 115200);
    
    /* force I2C bus to unlock */
    GPIO_QuickInit(HW_GPIOE, 18, kGPIO_Mode_OPP);
    GPIO_QuickInit(HW_GPIOE, 19, kGPIO_Mode_OPP);
    for(i=0;i<18;i++)
    {
        PEout(19) = !PEout(19); //SCL:E19
        DelayMs(1);
    }
    
    /* show power on */
    for(i=0;i<6;i++)
    {
        PAout(1) = !PAout(1);
        DelayMs(30);
    }
    
    /* flash operation */
   // SSDInit();
    
    /* init sensor&2401 */
    init_sensor();
    init_2401();
    /* init transfer */
    trans_init(HW_DMA_CH1, uart_instance);
    nrf24l01_set_tx_mode();
    /* install imu raw date callback */
    imu_io_install(&IMU_IOInstallStruct1);
    
    /* read meg cal data */
//    MagnetometerCalibration(&cal_data);
    
    WDOG_QuickInit(100);
    
    PIT_QuickInit(HW_PIT_CH1, 1000*20);
    PIT_CallbackInstall(HW_PIT_CH1, PIT_CH1_ISR);
    PIT_ITDMAConfig(HW_PIT_CH1, kPIT_IT_TOF, true);
    
    while(1)
    {
        //获取欧拉角 获取原始角度
        DisableInterrupts();
        ret = imu_get_euler_angle(&angle, &raw_data);
        EnableInterrupts();
        if(g2401Avalable)
        {
            nrf24l01_write_packet(buf, 32);
        }
        if(ret)
        {
            DisableInterrupts();
            printf("imu module fail!\r\n");
            while(1);
        }
        /* sample temperature and pressure */
        switch(bmpStatus)
        {
            case BMP_STATUS_T_START:
                bmp180_start_conversion(BMP180_T_MEASURE);
                bmpStatus = BMP_STATUS_T_WAIT;
                break;
            case BMP_STATUS_T_WAIT:
                if(!is_conversion_busy())
                {
                    bmpStatus = BMP_STATUS_T_COMPLETE;
                }
                break;
            case BMP_STATUS_T_COMPLETE:
                bmp180_read_temperature(&temperature);
                bmpStatus = BMP_STATUS_P_START;
                break;
            case BMP_STATUS_P_START:
                bmp180_start_conversion(BMP180_P3_MEASURE);
                bmpStatus = BMP_STATUS_P_WAIT;
                break;
            case BMP_STATUS_P_WAIT:
                if(!is_conversion_busy())
                {
                    bmpStatus = BMP_STATUS_P_COMPLETE;
                }
                break;
            case BMP_STATUS_P_COMPLETE:
                bmp180_read_pressure(&pressure);
                bmpStatus = BMP_STATUS_T_START;
                break;
            default:
                break;
        }
    }
}


