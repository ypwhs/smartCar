/**
  ******************************************************************************
  * @file    hmc5883.c
  * @author  YANDLD
  * @version V2.5
  * @date    2013.12.25
  * @brief   www.beyondcore.net   http://upcmcu.taobao.com 
  ******************************************************************************
  */
#include "hmc5883.h"
#include "i2c.h"
#include <string.h>

#define HMC_CFG1		0x00		//Register ConfigA
#define HMC_CFG2		0x01		//Register ConfigB	
#define HMC_MOD			0x02		//Mode Register
#define HMC_DX_MSB		0x03		//data output x	MSB 
#define HMC_DX_LSB		0x04		//data output x LSB
#define HMC_DY_MSB		0x05		//data output y MSB
#define HMC_DY_LSB		0x06		//data output y LSB
#define HMC_DZ_MSB		0x07		//data output z MSB
#define HMC_DZ_LSB		0x08		//data output z LSB
#define HMC_STATUE		0x09		//Statue Register
#define HMC_IDTF_A		0x0A		//Identification Register A
#define HMC_IDTF_B		0x0B		//Identification Register B
#define HMC_IDTF_C		0x0C		//Identification Register C

struct hmc_device 
{
    uint8_t     addr;
    uint32_t    instance;
    void        *user_data;
};

static struct hmc_device hmc_dev;

static const uint8_t hmc_addr[] = {0x1E};

static int write_reg(uint8_t addr, uint8_t val)
{
    return I2C_WriteSingleRegister(hmc_dev.instance, hmc_dev.addr, addr, val);
}

int hmc5883_init(uint32_t instance)
{
    int i;
    uint8_t id;
    
    hmc_dev.instance = instance;
    
    for(i = 0; i < ARRAY_SIZE(hmc_addr); i++)
    {
        if(!I2C_ReadSingleRegister(instance, hmc_addr[i], HMC_IDTF_A, &id))
        {
            if(id == 'H')
            {
                hmc_dev.addr = hmc_addr[i];
                
                /* init sequence */
                write_reg(HMC_CFG1, 0x78);
                write_reg(HMC_CFG2, 0x80);
                write_reg(HMC_MOD, 0x00);  
                return 0;     
            }
        }
    }
    return 1;
}



int hmc5883_read_data(int16_t *mdata)
{
    uint8_t err;
    uint8_t buf[7];
    
    err = I2C_BurstRead(hmc_dev.instance, hmc_dev.addr, HMC_DX_MSB, 1, buf, 7);
    
    mdata[0] = (int16_t)(((uint16_t)buf[0]<<8)+buf[1]); 	    
    mdata[1] = (int16_t)(((uint16_t)buf[2]<<8)+buf[3]); 	    
    mdata[2] = (int16_t)(((uint16_t)buf[4]<<8)+buf[5]); 
    return err;    
}
