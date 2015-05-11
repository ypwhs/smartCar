#include "mb85rc64.h"
#include "i2c.h"
#include "boarddef.h"

//MB85RC64 driver 

#define MB85RC64_ADDR       (0x50)
#define MB85RC64_LEN        (8192)


static uint32_t gInstance;

void MB85RC64_Init(uint32_t I2CMAP)
{
    gInstance = I2C_QuickInit(BOARD_I2C_MAP, 100*1000);
}


int  MB85RC64_Read(uint16_t pos, uint8_t * buf, uint32_t len)
{
    pos = BSWAP_16(pos);
    return I2C_BurstRead(gInstance, MB85RC64_ADDR, pos, 2, buf, len); 
}

uint8_t MB85RC64_ReadByte(uint16_t pos)
{
    uint8_t data;
    pos = BSWAP_16(pos);
    I2C_BurstRead(gInstance, MB85RC64_ADDR, pos, 2, &data, 1);
    return data;
}

int MB85RC64_WriteByte(uint16_t pos, uint8_t data)
{
    pos = BSWAP_16(pos);
    return I2C_BurstWrite(gInstance, MB85RC64_ADDR, pos, 2, &data, 1); 
}


bool MB85RC64_Test(uint32_t MAP)
{
    bool r = false;
    uint8_t data;
    uint8_t buf[64];
    
    MB85RC64_Init(MAP);
    MB85RC64_Read(MB85RC64_LEN-1, buf, 1);
    
    data = buf[0];
    
    MB85RC64_WriteByte(MB85RC64_LEN-1, 'H');
    MB85RC64_Read(MB85RC64_LEN-1, buf, 1);
    
    if(buf[0] == 'H')
    {
        r = true; 
    }
    MB85RC64_WriteByte(MB85RC64_LEN-1, data);
    return r;
}



