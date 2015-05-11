/**
  ******************************************************************************
  * @file    bmp180.c
  * @author  YANDLD
  * @version V2.5
  * @date    2013.12.25
  * @brief   www.beyondcore.net   http://upcmcu.taobao.com 
  ******************************************************************************
  */
  
  
#include <string.h>
#include <math.h>
#include "bmp180.h"
#include "common.h"
#include "i2c.h"



#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x)	(sizeof(x) / sizeof((x)[0]))
#endif
#ifndef SWAP16
#define  SWAP16(s) ((((s) & 0xff) << 8) | (((s) >> 8) & 0xff)) 
#endif

/* BMP180 registers */
#define BMP180_PROM_START_ADDR          0xAA /* E2PROM calibration data start register */
#define BMP180_PROM_DATA_LEN            22   /* E2PROM length */
#define BMP180_CHIP_ID_REG              0xD0 /* Chip ID */
#define BMP180_VERSION_REG              0xD1 /* Version */
#define BMP180_CTRL_MEAS_REG            0xF4 /* Measurements control (OSS[7.6], SCO[5], CTL[4.0] */
#define BMP180_ADC_OUT_MSB_REG          0xF6 /* ADC out MSB  [7:0] */
#define BMP180_ADC_OUT_LSB_REG          0xF7 /* ADC out LSB  [7:0] */
#define BMP180_ADC_OUT_XLSB_REG         0xF8 /* ADC out XLSB [7:3] */
#define BMP180_SOFT_RESET_REG           0xE0 /* Soft reset control */
/* BMP180 Pressure calculation constants */
#define BMP180_PARAM_MG                 3038
#define BMP180_PARAM_MH                -7357
#define BMP180_PARAM_MI                 3791
/* BMP180 Calibration coefficients */
#define BMP180_REG_AC1                  0xAA
#define BMP180_REG_AC2                  0xAC
#define BMP180_REG_AC3                  0xAE
#define BMP180_REG_AC4                  0xB0
#define BMP180_REG_AC5                  0xB2
#define BMP180_REG_AC6                  0xB4
#define BMP180_REG_B1                   0xB6
#define BMP180_REG_B2                   0xB8
#define BMP180_REG_MB                   0xBA
#define BMP180_REG_MC                   0xBC
#define BMP180_REG_MD                   0xBE

__packed struct bmp180_cal
{
    int16_t AC1;
    int16_t AC2;
    int16_t AC3;
    uint16_t AC4;
    uint16_t AC5;
    uint16_t AC6;
    int16_t B1;
    int16_t B2;
    int16_t MB;
    int16_t MC;
    int16_t MD;
    int16_t B5; /* B5 is intermediate variable for pressure compensatation */
};


struct bmp_device 
{
    uint8_t     addr;
    uint32_t    instance;
    void        *user_data;
};

static struct bmp_device bmp_dev;
static const uint8_t bmp_addr[] = {0x77};
static struct bmp180_cal cal;

/*!
 * @brief dump bmp180 calibration data from bmp180 internal eeporm to struct
 *
 * @param pointer of the bmp180 device struct
 */
static int dump_calibration_data(void)
{
    
    I2C_BurstRead(bmp_dev.instance, bmp_dev.addr, BMP180_PROM_START_ADDR, 1, (uint8_t*)&cal, BMP180_PROM_DATA_LEN);

    cal.AC1 = SWAP16(cal.AC1);
    cal.AC2 = SWAP16(cal.AC2);
    cal.AC3 = SWAP16(cal.AC3);
    cal.AC4 = SWAP16(cal.AC4);
    cal.AC5 = SWAP16(cal.AC5);
    cal.AC6 = SWAP16(cal.AC6);
    cal.B1 = SWAP16(cal.B1);
    cal.B2 = SWAP16(cal.B2);
    cal.B5 = SWAP16(cal.B5);
    cal.MB = SWAP16(cal.MB);
    cal.MC = SWAP16(cal.MC);
    cal.MD = SWAP16(cal.MD);
    LIB_TRACE("AC1:%d\r\n", cal.AC1);
    LIB_TRACE("AC2:%d\r\n", cal.AC2);
    LIB_TRACE("AC3:%d\r\n", cal.AC3);
    LIB_TRACE("AC4:%d\r\n", cal.AC4); 
    LIB_TRACE("AC5:%d\r\n", cal.AC5);
    LIB_TRACE("B5:%d\r\n", cal.AC6);
    LIB_TRACE("B1:%d\r\n", cal.B1);
    LIB_TRACE("B2:%d\r\n", cal.B2);
    LIB_TRACE("B5:%d\r\n", cal.B5);
    LIB_TRACE("MB:%d\r\n", cal.MB);
    LIB_TRACE("MC:%d\r\n", cal.MC);
    LIB_TRACE("MD:%d\r\n", cal.MD);
    return 0;
}

static int write_reg(uint8_t addr, uint8_t val)
{
    return I2C_WriteSingleRegister(bmp_dev.instance, bmp_dev.addr, addr, val);
}

static int read_reg(uint8_t addr, uint8_t *val)
{
    return I2C_ReadSingleRegister(bmp_dev.instance, bmp_dev.addr, addr, val);
}

int bmp180_init(uint32_t instance)
{
    int i;
    uint8_t id;
    
    bmp_dev.instance = instance;
    
    for(i = 0; i < ARRAY_SIZE(bmp_addr); i++)
    {
        if(!I2C_ReadSingleRegister(instance, bmp_addr[i], BMP180_CHIP_ID_REG, &id))
        {
            if(id == 0x55)
            {
                bmp_dev.addr = bmp_addr[i];
                dump_calibration_data();
                return 0;     
            }
        }
    }
    return 1;
}


/*
 * @brief start bmp180 conversion
 *
 * @param pointer of the bmp180 device struct
 * @param conversion mode: 
 *        @arg BMP180_T_MEASURE: temperature conversion
 *        @arg BMP180_P0_MEASURE: pressure conversion, oversampling:0
 *        @arg BMP180_P1_MEASURE: pressure conversion, oversampling:1
 *        @arg BMP180_P2_MEASURE: pressure conversion, oversampling:2
 */
int bmp180_start_conversion(uint8_t cmd)
{
    return write_reg(BMP180_CTRL_MEAS_REG, cmd); 
}


/*!
 * @brief read raw temperature data
 *
 * @param pointer of the bmp180 device struct
 * @param data pointer
 */
static int read_raw_temperature(int32_t * data)
{
    uint8_t err;
    uint8_t buf[2];
    
    err = I2C_BurstRead(bmp_dev.instance, bmp_dev.addr, BMP180_ADC_OUT_MSB_REG, 1, buf, 2);
    
    *data = ((((int16_t)(buf[0])) << 8) + (int16_t)(buf[1]));
    return err;
}

/*!
 * @brief read raw perssure data
 *
 * @param pointer of the bmp180 device struct
 * @param data pointer
 * @param oversampling rate data pointer
 */
static int read_raw_pressure(int32_t * data, uint8_t *oss)
{
    uint8_t err;
    uint8_t buf[3];
    uint8_t reg1;
    
    /* read oss */
    read_reg(BMP180_CTRL_MEAS_REG, &reg1);
    *oss = reg1 >> 6;
    
    err = I2C_BurstRead(bmp_dev.instance, bmp_dev.addr, BMP180_ADC_OUT_MSB_REG, 1, buf, 3);

    *data = (buf[0] << 16) + (buf[1] << 8) + buf[2];
    *data >>= (8 - (*oss));
    return err;
}

/*!
 * @brief tell if bmp180 is in conversion
 *
 * @param pointer of the bmp180 device struct
 * @retval 1:busy 0:idle
 */
int is_conversion_busy(void)
{
    uint8_t reg1;
    read_reg(BMP180_CTRL_MEAS_REG, &reg1);
    if(reg1 & (1 << 5)) /* busy */
    {
        return 1;
    }
    return 0;
}

/*!
 * @brief read real temperature data
 *
 * calculate according to bmp180 data sheet
 *
 * @param pointer of the bmp180 device struct
 * @param pointer of temperature, in 10x, eg:156 = 15.6C
 */
int bmp180_read_temperature(int32_t * temperature)
{
    int32_t raw_temperature;
    int32_t x1, x2;

    if(is_conversion_busy())
    {
        return 1;
    }
    if(read_raw_temperature(&raw_temperature))
    {
        return 2;
    }

    LIB_TRACE("raw_t = %d\n\r",raw_temperature);
    LIB_TRACE("ac6 = %d\n\r",cal.AC6);
    LIB_TRACE("ac5 = %d\n\r",cal.AC5);
    LIB_TRACE("ac5/32768 = %f\n\r",((float)cal.AC5)/32768);
    LIB_TRACE("raw_t-ac6 = %d\n\r",(raw_temperature - cal.AC6));

    x1 = (int32_t)((raw_temperature - cal.AC6) * (int32_t)cal.AC5) >> 15;
    x2 = (int32_t)(cal.MC <<11) / (x1 + cal.MD);
    cal.B5 = x1 + x2;
    *temperature = (cal.B5 + (int32_t)8) >> 4;

    return 0;
}

/*!
 * @brief read real pressure data
 *
 * calculate according to bmp180 data sheet
 *
 * @param pointer of the bmp180 device struct
 * @param pointer of pressure, in pa
 */
int bmp180_read_pressure(int32_t * pressure)
{
    int32_t raw_pressure;
    uint8_t oss;
    if(is_conversion_busy())
    {
        return 1;
    }
    /* read raw pressure */
    if(read_raw_pressure(&raw_pressure, &oss))
    {
        return 2;
    }
    /* temperature compensation */
    uint32_t b4, b7;
    int32_t x1, x2, x3, b3, b6;
    int32_t result;
    b6 = cal.B5 - 4000;
        
    x1 = (cal.B2 * (b6 * b6 >> 12)) >> 11;
    x2 = cal.AC2 * b6 >> 11;
    x3 = x1 + x2;

    b3 = ((((int32_t)cal.AC1 * 4 + x3) << oss) + 2) >> 2;
    x1 = cal.AC3 * b6 >> 13;
    x2 = (cal.B1 * (b6 * b6 >> 12)) >> 16;
    x3 = (x1 + x2 + 2) >> 2;
    b4 = (cal.AC4 * (uint32_t)(x3 + 32768)) >> 15;
    b7 = (uint32_t)(raw_pressure - b3) * (50000 >> oss);

    if (b7 < 0x80000000)
    {
        result = (b7 << 1) / b4;
    }
    else
    {
        result = (b7 / b4) << 1;
    }
    x1 = (result >> 8) * (result >> 8);
    x1 = (x1 * 3038) >> 16;
    x2 = (-7357 * result) >> 16;
    result += ((x1 + x2 + 3791) >> 4);
		
    *pressure = result;
    return 0;
}

int bmp180_pressure2altitude(int32_t pressure, int32_t *altitude)
{
	*altitude =(int32_t)(44330.0 * (1.0-pow((double)(pressure) / 101325.0, 1.0/5.255)) );
	return 0;
}

