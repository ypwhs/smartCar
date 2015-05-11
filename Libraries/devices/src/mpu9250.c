/**
  ******************************************************************************
  * @file    mpu9250.c
  * @author  YANDLD
  * @version V2.5
  * @date    2013.12.25
  * @brief   www.beyondcore.net   http://upcmcu.taobao.com 
  ******************************************************************************
  */
  
#include <string.h>
#include "mpu9250.h"
#include "common.h"
#include "i2c.h"

#define MPU9250_DEBUG		1
#if ( MPU9250_DEBUG == 1 )
#include <stdio.h>
#define MPU9250_TRACE	printf
#else
#define MPU9250_TRACE(...)
#endif


#define MPU9250_ID			0x71		
#define	MPU9250_SELF_TEST_X_GYRO		0x00		/***/
#define	MPU9250_SELF_TEST_Y_GYRO		0x01		/***/
#define	MPU9250_SELF_TEST_Z_GYRO		0x02		/***/
#define	MPU9250_SELF_TEST_X_ACCEL		0x0D		/***/
#define	MPU9250_SELF_TEST_Y_ACCEL		0x0E		/***/
#define	MPU9250_SELF_TEST_Z_ACCEL		0x0F		/***/
#define	MPU9250_XG_OFFSET_H			0x13		/***/
#define	MPU9250_XG_OFFSET_L			0x14		/***/
#define	MPU9250_YG_OFFSET_H			0x15		/***/
#define	MPU9250_YG_OFFSET_L			0x16		/***/
#define	MPU9250_ZG_OFFSET_H			0x17		/***/
#define	MPU9250_ZG_OFFSET_L			0x18		/***/
#define	MPU9250_SMPLRT_DIV				0x19		/***/
#define	MPU9250_CONFIG					0x1A		/***/

#define	MPU9250_GYRO_CONFIG                         0x1B
#define MPU9250_GYRO_CONFIG_FS_MASK                 0x18u
#define MPU9250_GYRO_CONFIG_FS_SHIFT                3
#define MPU9250_GYRO_CONFIG_FS(x)                  (((uint32_t)(((uint32_t)(x))<<MPU9250_GYRO_CONFIG_FS_SHIFT))&MPU9250_GYRO_CONFIG_FS_MASK)

#define	MPU9250_ACCEL_CONFIG                        0x1C
#define MPU9250_ACCEL_CONFIG_FS_MASK                0x18u
#define MPU9250_ACCEL_CONFIG_FS_SHIFT               3
#define MPU9250_ACCEL_CONFIG_FS(x)                  (((uint32_t)(((uint32_t)(x))<<MPU9250_ACCEL_CONFIG_FS_SHIFT))&MPU9250_ACCEL_CONFIG_FS_MASK)

#define	MPU9250_ACCEL_CONFIG2 			0x1D		/***/
#define	MPU9250_LP_ACCEL_ODR			0x1E		/***/
#define	MPU9250_WOM_THR				0x1F		/***/
#define	MPU9250_FIFO_EN				0x23		/***/
#define	MPU9250_I2C_MST_CTRL			0x24		/***/
#define	MPU9250_I2C_SLV0_ADDR			0x25		/***/
#define	MPU9250_I2C_SLV0_REG			0x26		/***/
#define	MPU9250_I2C_SLV0_CTRL			0x27		/***/
#define	MPU9250_I2C_SLV1_ADDR			0x28		/***/
#define	MPU9250_I2C_SLV1_REG			0x29		/***/
#define	MPU9250_I2C_SLV1_CTRL			0x2A		/***/
#define	MPU9250_I2C_SLV2_ADDR			0x2B		/***/
#define	MPU9250_I2C_SLV2_REG			0x2C		/***/
#define	MPU9250_I2C_SLV2_CTRL			0x2D		/***/
#define	MPU9250_I2C_SLV3_ADDR			0x2E		/***/
#define	MPU9250_I2C_SLV3_REG			0x2F		/***/
#define	MPU9250_I2C_SLV3_CTRL			0x30		/***/
#define	MPU9250_I2C_SLV4_ADDR			0x31		/***/
#define	MPU9250_I2C_SLV4_REG			0x32		/***/
#define	MPU9250_I2C_SLV4_DO			0x33		/***/
#define	MPU9250_I2C_SLV4_CTRL			0x34		/***/
#define	MPU9250_I2C_SLV4_DI			0x35		/***/
#define	MPU9250_I2C_MST_STATUS			0x36		/***/
#define	MPU9250_INT_PIN_CFG			0x37		/***/
#define	MPU9250_INT_ENABLE				0x38		/***/
#define	MPU9250_INT_STATUS				0x3A		/***/
#define	MPU9250_ACCEL_XOUT_H			0x3B		/***/
#define	MPU9250_ACCEL_XOUT_L			0x3C		/***/
#define	MPU9250_ACCEL_YOUT_H			0x3D		/***/
#define	MPU9250_ACCEL_YOUT_L			0x3E		/***/
#define	MPU9250_ACCEL_ZOUT_H			0x3F		/***/
#define	MPU9250_ACCEL_ZOUT_L			0x40		/***/
#define	MPU9250_TEMP_OUT_H				0x41		/***/
#define	MPU9250_TEMP_OUT_L				0x42		/***/
#define	MPU9250_GYRO_XOUT_H			0x43		/***/
#define	MPU9250_GYRO_XOUT_L			0x44		/***/
#define	MPU9250_GYRO_YOUT_H			0x45		/***/
#define	MPU9250_GYRO_YOUT_L			0x46		/***/
#define	MPU9250_GYRO_ZOUT_H			0x47		/***/
#define	MPU9250_GYRO_ZOUT_L			0x48		/***/
#define	MPU9250_EXT_SENS_DATA_00		0x49		/***/
#define	MPU9250_EXT_SENS_DATA_01		0x4A		/***/
#define	MPU9250_EXT_SENS_DATA_02		0x4B		/***/
#define	MPU9250_EXT_SENS_DATA_03		0x4C		/***/
#define	MPU9250_EXT_SENS_DATA_04		0x4D		/***/
#define	MPU9250_EXT_SENS_DATA_05		0x4E		/***/
#define	MPU9250_EXT_SENS_DATA_06		0x4F		/***/
#define	MPU9250_EXT_SENS_DATA_07		0x50		/***/
#define	MPU9250_EXT_SENS_DATA_08		0x51		/***/
#define	MPU9250_EXT_SENS_DATA_09		0x52		/***/
#define	MPU9250_EXT_SENS_DATA_10		0x53		/***/
#define	MPU9250_EXT_SENS_DATA_11		0x54		/***/
#define	MPU9250_EXT_SENS_DATA_12		0x55		/***/
#define	MPU9250_EXT_SENS_DATA_13		0x56		/***/
#define	MPU9250_EXT_SENS_DATA_14		0x57		/***/
#define	MPU9250_EXT_SENS_DATA_15		0x58		/***/
#define	MPU9250_EXT_SENS_DATA_16		0x59		/***/
#define	MPU9250_EXT_SENS_DATA_17		0x5A		/***/
#define	MPU9250_EXT_SENS_DATA_18		0x5B		/***/
#define	MPU9250_EXT_SENS_DATA_19		0x5C		/***/
#define	MPU9250_EXT_SENS_DATA_20		0x5D		/***/
#define	MPU9250_EXT_SENS_DATA_21		0x5E		/***/
#define	MPU9250_EXT_SENS_DATA_22		0x5F		/***/
#define	MPU9250_EXT_SENS_DATA_23		0x60		/***/
#define	MPU9250_I2C_SLV0_DO			0x63		/***/
#define	MPU9250_I2C_SLV1_DO			0x64		/***/
#define	MPU9250_I2C_SLV2_DO			0x65		/***/
#define	MPU9250_I2C_SLV3_DO			0x66		/***/
#define	MPU9250_I2C_MST_DELAY_CTRL		0x67		/***/
#define	MPU9250_SIGNAL_PATH_RESET		0x68		/***/
#define	MPU9250_MOT_DETECT_CTRL		0x69		/***/
#define	MPU9250_USER_CTRL				0x6A		/***/
#define	MPU9250_PWR_MGMT_1				0x6B		/***/
#define	MPU9250_PWR_MGMT_2				0x6C		/***/
#define	MPU9250_FIFO_COUNTH			0x72		/***/
#define	MPU9250_FIFO_COUNTL			0x73		/***/
#define	MPU9250_FIFO_R_W				0x74		/***/
#define	MPU9250_WHO_AM_I				0x75		/***/
#define	MPU9250_XA_OFFSET_H			0x77		/***/
#define	MPU9250_XA_OFFSET_L			0x78		/***/
#define	MPU9250_YA_OFFSET_H			0x7A		/***/
#define	MPU9250_YA_OFFSET_L			0x7B		/***/
#define	MPU9250_ZA_OFFSET_H				0x7D		/***/
#define	MPU9250_ZA_OFFSET_L			0x7E		/***/

#define AK8963_ADDRESS   (0x0C)
//Magnetometer Registers
#define AK8963_WHO_AM_I  0x00 // should return 0x48
#define AK8963_INFO      0x01
#define AK8963_ST1       0x02  // data ready status bit 0
#define AK8963_XOUT_L    0x03  // data
#define AK8963_XOUT_H    0x04
#define AK8963_YOUT_L    0x05
#define AK8963_YOUT_H    0x06
#define AK8963_ZOUT_L    0x07
#define AK8963_ZOUT_H    0x08
#define AK8963_ST2       0x09  // Data overflow bit 3 and data read error status bit 2
#define AK8963_CNTL      0x0A  // Power down (0000), single-measurement (0001), self-test (1000) and Fuse ROM (1111) modes on bits 3:0
#define AK8963_ASTC      0x0C  // Self test control
#define AK8963_I2CDIS    0x0F  // I2C disable
#define AK8963_ASAX      0x10  // Fuse ROM x-axis sensitivity adjustment value
#define AK8963_ASAY      0x11  // Fuse ROM y-axis sensitivity adjustment value
#define AK8963_ASAZ      0x12  // Fuse ROM z-axis sensitivity adjustment value


struct mpu_device 
{
    uint8_t             addr;
    uint32_t            instance;
    void                *user_data;
    float               mag_adj[3];
    struct mpu_config   user_config;
};

uint8_t Gscale = GFS_250DPS;  // GFS_250DPS, GFS_500DPS, GFS_1000DPS, GFS_2000DPS

static struct mpu_device mpu_dev;

static const uint8_t mpu_addr[] = {0x68, 0x69};

static int write_reg(uint8_t addr, uint8_t val)
{
    return I2C_WriteSingleRegister(mpu_dev.instance, mpu_dev.addr, addr, val);
}

static int ak8963_write_reg(uint8_t addr, uint8_t val)
{
    return I2C_WriteSingleRegister(mpu_dev.instance, AK8963_ADDRESS, addr, val);
}

static uint8_t ak8963_read_reg(uint8_t addr)
{
    uint8_t val;
    I2C_ReadSingleRegister(mpu_dev.instance, AK8963_ADDRESS, addr, &val);
    return val;
}

static uint8_t read_reg(uint8_t addr)
{
    uint8_t val;
    I2C_ReadSingleRegister(mpu_dev.instance, mpu_dev.addr, addr, &val);
    return val;
}

int mpu9250_config(struct mpu_config *config)
{
    uint8_t val;
    memcpy(&mpu_dev.user_config, config, sizeof(struct mpu_config));

    val = read_reg(MPU9250_ACCEL_CONFIG);
    val &= ~MPU9250_ACCEL_CONFIG_FS_MASK;
    val |= MPU9250_ACCEL_CONFIG_FS(config->afs);
    write_reg(MPU9250_ACCEL_CONFIG, val);
    
    val = read_reg(MPU9250_GYRO_CONFIG);
    val &= ~MPU9250_GYRO_CONFIG_FS_MASK;
    val |= MPU9250_GYRO_CONFIG_FS(config->afs);
    write_reg(MPU9250_GYRO_CONFIG, val);
    
    return 0;
}

static int _ak8963_init(void)
{
    uint8_t buf[3], err;
    
    err = ak8963_write_reg(AK8963_CNTL, 0x00);
    DelayMs(10);
    err += ak8963_write_reg(AK8963_CNTL, 0x0F);  /* Enter Fuse ROM access mode */
    DelayMs(10);
    err += I2C_BurstRead(0, AK8963_ADDRESS, AK8963_ASAX, 1, buf, 3);
    mpu_dev.mag_adj[0] = (float)(buf[0] - 128)/256.0f + 1.0f;
    mpu_dev.mag_adj[1] = (float)(buf[1] - 128)/256.0f + 1.0f;
    mpu_dev.mag_adj[2] = (float)(buf[2] - 128)/256.0f + 1.0f;
    MPU9250_TRACE("mag_adj:%f  %f  %f  \r\n", mpu_dev.mag_adj[0], mpu_dev.mag_adj[1], mpu_dev.mag_adj[2]);
    err += ak8963_write_reg(AK8963_CNTL, 0x00);
	DelayMs(10);
    /* 100Hz 14bit mode */
    err += ak8963_write_reg(AK8963_CNTL, 0x06|0x10);
    
    return err;
}

int mpu9250_init(uint32_t instance)
{
    int i;
    uint8_t id;
    
    mpu_dev.instance = instance;
    
    for(i = 0; i < ARRAY_SIZE(mpu_addr); i++)
    {
        if(!I2C_ReadSingleRegister(instance, mpu_addr[i], MPU9250_WHO_AM_I, &id) && (id == MPU9250_ID))
        {
            mpu_dev.addr = mpu_addr[i];
            /** Initialize MPU9250 device*/

            /* init sequence */
            write_reg(MPU9250_PWR_MGMT_1, 0x00); /* wake up device */
            DelayMs(10);
            
            /** get stable time source*/
            /**Set clock source to be PLL with x-axis gyroscope reference, bits 2:0 = 001*/
            /**Auto selects the best available clock source – PLL if ready, else use the Internal oscillator*/
            write_reg(MPU9250_PWR_MGMT_1, 0x01);
            
            /**Configure Gyro and Accelerometer*/
            /**Disable FSYNC and set accelerometer and gyro bandwidth to 44 and 42 Hz, respectively;*/ 
            /**DLPF_CFG = bits 2:0 = 010; this sets the sample rate at 1 kHz for both*/
            /**Maximum delay is 4.9 ms which is just over a 200 Hz maximum rate	*/
            write_reg(MPU9250_PWR_MGMT_1, 0x03);
            
            /**Set sample rate = gyroscope output rate/(1 + SMPLRT_DIV)*/
            /**Use a 200 Hz rate; the same rate set in CONFIG above*/
            write_reg(MPU9250_SMPLRT_DIV, 0x04);
            
            /**Set gyroscope full scale range */
            /**Range selects FS_SEL and AFS_SEL are 0 - 3, so 2-bit values are left-shifted into positions 4:3 */
            uint8_t r;
            r = read_reg(MPU9250_GYRO_CONFIG);/**get status of register*/
            write_reg(MPU9250_GYRO_CONFIG, r &~ 0xE0U);/** Clear self-test bits [7:5]*/
            write_reg(MPU9250_GYRO_CONFIG, r &~ 0x18U);/**Clear AFS bits [4:3]*/	
            write_reg(MPU9250_GYRO_CONFIG, r | (Gscale << 3U));/**Set full scale range for the accelerometer */	
            
            /** Set accelerometer sample rate configuration */
            /** It is possible to get a 4 kHz sample rate from the accelerometer by choosing 1 for */
            /** accel_fchoice_b bit [3]; in this case the bandwidth is 1.13 kHz */
            r = read_reg(MPU9250_ACCEL_CONFIG2);
            write_reg(MPU9250_ACCEL_CONFIG2, r & ~0x0FU); /** Clear accel_fchoice_b (bit 3) and A_DLPFG (bits [2:0])*/  
            write_reg(MPU9250_ACCEL_CONFIG2, r | 0x03U); /** Set accelerometer rate to 1 kHz and bandwidth to 41 Hz*/	
            
            /*
            The accelerometer, gyro, and thermometer are set to 1 kHz sample rates, 
             but all these rates are further reduced by a factor of 5 to 200 Hz because of the SMPLRT_DIV setting
             Configure Interrupts and Bypass Enable
             Set interrupt pin active high, push-pull, and clear on read of INT_STATUS, enable I2C_BYPASS_EN so additional chips 
             can join the I2C bus and all can be controlled by the Kinetis as master
             */
             
            write_reg(MPU9250_INT_PIN_CFG, 0x22U);    
            write_reg(MPU9250_INT_ENABLE, 0x01U);  // Enable data ready (bit 0) interrupt
            
            /* AK8963 */
            id = ak8963_read_reg(AK8963_WHO_AM_I);
            MPU9250_TRACE("AK8963 ID:0x%X\r\n", id);
            if(id == 0x48)
            {
                MPU9250_TRACE("found!addr:0x%X AK8963_WHO_AM_I:0x%X\r\n", mpu_dev.addr, id);
                _ak8963_init();
                return 0;
            }
        }
    }
    return 1;
}



int mpu9250_read_accel_raw(int16_t* x, int16_t* y, int16_t* z)
{
    uint8_t err;
    uint8_t buf[6];
    
    err = I2C_BurstRead(mpu_dev.instance, mpu_dev.addr, MPU9250_ACCEL_XOUT_H, 1, buf, 6);
    
    *x = (int16_t)(((uint16_t)buf[0]<<8) | buf[1]); 	    
    *y = (int16_t)(((uint16_t)buf[2]<<8) | buf[3]); 	    
    *z = (int16_t)(((uint16_t)buf[4]<<8) | buf[5]); 
    return err;    
}

int mpu9250_read_gyro_raw(int16_t* x, int16_t* y, int16_t* z)
{
    uint8_t err;
    uint8_t buf[6];
    
    err = I2C_BurstRead(mpu_dev.instance, mpu_dev.addr, MPU9250_GYRO_XOUT_H, 1, buf, 6);
    
    *x = (int16_t)(((uint16_t)buf[0]<<8) | buf[1]); 	    
    *y = (int16_t)(((uint16_t)buf[2]<<8) | buf[3]); 	    
    *z = (int16_t)(((uint16_t)buf[4]<<8) | buf[5]); 
    return err;    
}

    
int mpu9250_read_mag_raw(int16_t* x, int16_t* y, int16_t* z)
{
    uint8_t err;
    uint8_t buf[7];
    
    /* Read the six raw data and ST2 registers sequentially into data array */
    err = I2C_BurstRead(0,AK8963_ADDRESS, AK8963_XOUT_L, 1, buf, 7);  

    *x = (int16_t)((((int16_t)buf[1] << 8) | buf[0]));
    *y = (int16_t)((((int16_t)buf[3] << 8) | buf[2]));
    *z = (int16_t)((((int16_t)buf[5] << 8) | buf[4]));	
    *x = mpu_dev.mag_adj[0]*(*x);
    *y = mpu_dev.mag_adj[1]*(*y);
    *z = mpu_dev.mag_adj[2]*(*z);
    return err;
}

int mpu9250_read_temp_raw(int16_t *val)
{
    uint8_t err;
    uint8_t buf[2];

    err = I2C_BurstRead(mpu_dev.instance, mpu_dev.addr, MPU9250_TEMP_OUT_H, 1, buf, 2);
    *val = (int16_t)(((int16_t)buf[0]) << 8 | buf[1]);
    
    return err;
}
