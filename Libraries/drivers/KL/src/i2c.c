/**
  ******************************************************************************
  * @file    i2c.c
  * @author  YANDLD
  * @version V2.5
  * @date    2014.3.26
  * @brief   www.beyondcore.net   http://upcmcu.taobao.com 
  * @note    ??????IIC?????????
  ******************************************************************************
  */
#include "i2c.h"
#include "gpio.h"

#define I2C_GPIO_SIM  1

#if I2C_GPIO_SIM

#define SDA_DDR_OUT()       do {GPIO_PinConfig(i2c.instace, i2c.sda_pin, kOutput);}while(0)
#define SDA_DDR_IN()        do {GPIO_PinConfig(i2c.instace, i2c.sda_pin, kInput);}while(0)
#define SDA_H()             do {GPIO_WriteBit(i2c.instace, i2c.sda_pin, 1);}while(0)
#define SDA_L()             do {GPIO_WriteBit(i2c.instace, i2c.sda_pin, 0);}while(0)
#define SCL_H()             do {GPIO_WriteBit(i2c.instace, i2c.scl_pin, 1);}while(0)
#define SCL_L()             do {GPIO_WriteBit(i2c.instace, i2c.scl_pin, 0);}while(0)
#define I2C_DELAY()         DelayUs(1)

typedef struct
{
    uint32_t instace;
    uint32_t sda_pin;
    uint32_t scl_pin;
}i2c_gpio;

static i2c_gpio i2c;

uint8_t I2C_QuickInit(uint32_t MAP, uint32_t baudrate)
{
    uint8_t i;
    QuickInit_Type * pq = (QuickInit_Type*)&(MAP);
    
    /* open drain and pull up */
    for(i = 0; i < pq->io_offset; i++)
    {
        GPIO_QuickInit(pq->io_instance, pq->io_base + i, kGPIO_Mode_OPP);
        GPIO_WriteBit(pq->io_instance, pq->io_base + i, 1);
        PORT_PinPullConfig(pq->io_instance, pq->io_base + i, kPullUp);
    }

    /* i2c_gpio struct setup */
    i2c.instace = pq->io_instance;
    
    switch(MAP)
    {
        case I2C1_SCL_PE01_SDA_PE00:
            i2c.scl_pin = 1;i2c.sda_pin = 0;
            break;
        case I2C0_SCL_PE19_SDA_PE18:
            i2c.scl_pin = 19;i2c.sda_pin = 18;
            break;
        case I2C0_SCL_PF22_SDA_PF23:
            i2c.scl_pin = 22;i2c.sda_pin = 23;
            break;
        case I2C0_SCL_PB00_SDA_PB01:
            i2c.scl_pin = 0;i2c.sda_pin = 1;
            break;
        case I2C0_SCL_PB02_SDA_PB03:
            i2c.scl_pin = 2;i2c.sda_pin = 3;
            break;
        case I2C1_SCL_PC10_SDA_PC11:
            i2c.scl_pin = 10;i2c.sda_pin = 11;
            break;
        case I2C0_SCL_PD08_SDA_PD09:
            i2c.scl_pin = 8;i2c.sda_pin = 9;
            break;
        case I2C0_SCL_PE24_SDA_PE25:
            i2c.scl_pin = 24;i2c.sda_pin = 25;
            break;
        default:
            break;
    }
    return pq->ip_instance;
}

void I2C_Init(I2C_InitTypeDef* I2C_InitStruct)
{
    
}

static inline uint8_t SDA_IN(void)
{
    return GPIO_ReadBit(i2c.instace, i2c.sda_pin);
}

static bool I2C_Start(void)
{
    SDA_DDR_OUT();
    SDA_H();
    SCL_H();
    I2C_DELAY();
    SDA_L();
    I2C_DELAY();
    SCL_L();
    return true;
}

static void I2C_Stop(void)
{
    SCL_L();
    SDA_L();
    I2C_DELAY();
    SCL_H();
    SDA_H();
    I2C_DELAY();
}

static void I2C_Ack(void)
{
    SCL_L();
    SDA_L();
    I2C_DELAY();
    SCL_H();
    I2C_DELAY();
    SCL_L();
    I2C_DELAY();
}

static void I2C_NAck(void)
{
    SCL_L();
    I2C_DELAY();
    SDA_H();
    I2C_DELAY();
    SCL_H();
    I2C_DELAY();
    SCL_L();
    I2C_DELAY();
}

static bool I2C_WaitAck(void)
{
    uint8_t ack;
    SDA_DDR_IN();
    SCL_L();
    
    I2C_DELAY();
    SCL_H();
    I2C_DELAY();
    ack = SDA_IN();
    SCL_L();
    SDA_DDR_OUT();
    
    return ack;
}

static void I2C_SendByte(uint8_t data)
{
    volatile uint8_t i;
    
    i = 8;
    while(i--)
    {
        if(data & 0x80) SDA_H();
        else SDA_L();
        data <<= 1;
        I2C_DELAY();
        SCL_H();
        I2C_DELAY();
        SCL_L();
    }

}

static uint8_t I2C_GetByte(void)
{
    uint8_t i,byte;
    
    i = 8;
    byte = 0;

    SDA_DDR_IN();
    while(i--)
    {
        SCL_L();
        I2C_DELAY();
        SCL_H();
        I2C_DELAY();
        byte = (byte<<1)|(SDA_IN() & 1);
    }
    SCL_L();
    SDA_DDR_OUT();
    return byte;
}


uint8_t I2C_BurstWrite(uint32_t instance ,uint8_t chipAddr, uint32_t addr, uint32_t addrLen, uint8_t *buf, uint32_t len)
{
    uint8_t *p;
    uint8_t err;
    
    p = (uint8_t*)&addr;
    err = 0;
    chipAddr <<= 1;
    
    I2C_Start();
    I2C_SendByte(chipAddr);
    err += I2C_WaitAck();

    while(addrLen--)
    {
        I2C_SendByte(*p++);
        err += I2C_WaitAck();
    }
    
    while(len--)
    {
        I2C_SendByte(*buf++);
        err += I2C_WaitAck();  
    }

    I2C_Stop();
    return err;
}

uint8_t I2C_WriteSingleRegister(uint32_t instance, uint8_t chipAddr, uint8_t addr, uint8_t data)
{
    return I2C_BurstWrite(instance, chipAddr, addr, 1, &data, 1);
}

int32_t I2C_BurstRead(uint32_t instance ,uint8_t chipAddr, uint32_t addr, uint32_t addrLen, uint8_t *buf, uint32_t len)
{
    uint8_t *p;
    uint8_t err;
    
    p = (uint8_t*)&addr;
    err = 0;
    chipAddr <<= 1;
    
    I2C_Start();
    I2C_SendByte(chipAddr);
    err += I2C_WaitAck();
    
    while(addrLen--)
    {
        I2C_SendByte(*p++);
        err += I2C_WaitAck();
    }
    
    I2C_Start();
    I2C_SendByte(chipAddr+1);
    err += I2C_WaitAck();
    
    while(len--)
    {
        *buf++ = I2C_GetByte();
        if(len)
        {
            I2C_Ack();
        }
    }
    
    I2C_NAck();
    I2C_Stop();
    
    return err;
}

uint8_t I2C_Probe(uint32_t instance, uint8_t chipAddr)
{
    uint8_t err;
    
    err = 0;
    chipAddr <<= 1;
    
    I2C_Start();
    I2C_SendByte(chipAddr);
    err = I2C_WaitAck();
    I2C_Stop();
    return err;
}

uint8_t I2C_ReadSingleRegister(uint32_t instance, uint8_t chipAddr, uint8_t addr, uint8_t* pData)
{
    return I2C_BurstRead(instance, chipAddr, addr, 1, pData, 1);
}


int SCCB_ReadSingleRegister(uint32_t instance, uint8_t chipAddr, uint8_t addr, uint8_t* pData)
{
    uint8_t err;
    uint8_t retry;
    
    retry = 10;
    chipAddr <<= 1;
    
    while(retry--)
    {
        err = 0;
        I2C_Start();
        I2C_SendByte(chipAddr);
        err += I2C_WaitAck();
        
        I2C_SendByte(addr);
        err += I2C_WaitAck();
        
        I2C_Stop();
        I2C_Start();
        I2C_SendByte(chipAddr+1);
        err += I2C_WaitAck();
        
        *pData = I2C_GetByte();
       // err += I2C_WaitAck();
        
        I2C_NAck();
        I2C_Stop();
        if(!err)
        {
            break;
        }
    }

    return err;
}


int SCCB_WriteSingleRegister(uint32_t instance, uint8_t chipAddr, uint8_t addr, uint8_t data)
{
    uint8_t err;
    uint8_t retry;
    
    retry = 10;
    
    while(retry--)
    {
        err = I2C_WriteSingleRegister(instance, chipAddr, addr, data);
        if(!err)
        {
            break;
        }
    }
    return err;
}













#else

/* leagacy support for Kineis Z Version(Inital Version) */
#if (!defined(I2C_BASES))
#ifdef I2C1
    #define I2C_BASES {I2C0, I2C1}
#else
    #define I2C_BASES {I2C0}
#endif
    
#endif

/* gloabl const table defination */
static I2C_Type * const I2C_InstanceTable[] = I2C_BASES;
static I2C_CallBackType I2C_CallBackTable[ARRAY_SIZE(I2C_InstanceTable)] = {NULL};

static const uint32_t SIM_I2CClockGateTable[] =
{
#ifdef SIM_SCGC4_IIC0_MASK
    SIM_SCGC4_IIC0_MASK,
#else
    SIM_SCGC4_I2C0_MASK,
#endif
    
#ifdef I2C1
#ifdef SIM_SCGC4_IIC1_MASK
    SIM_SCGC4_IIC1_MASK,
#else
    SIM_SCGC4_I2C1_MASK,
#endif
#endif
};
static const IRQn_Type I2C_IRQnTable[] = 
{
    I2C0_IRQn,
#ifdef I2C1
    I2C1_IRQn,
#endif
};

//!< clock deiver struct (internal)
typedef struct 
{
    uint8_t icr;            //!< F register ICR value.
    uint16_t sclDivider;    //!< SCL clock divider.
}_I2C_Divider_Type;

//!< @brief I2C divider values.
static const _I2C_Divider_Type I2C_DiverTable[] =
{
    /* ICR  Divider */
    { 0x00, 20 },
    { 0x01, 22 },
    { 0x02, 24 },
    { 0x03, 26 },
    { 0x04, 28 },
    { 0x05, 30 },
    { 0x09, 32 },
    { 0x06, 34 },
    { 0x0a, 36 },
    { 0x07, 40 },
    { 0x0c, 44 },
    { 0x0d, 48 },
    { 0x0e, 56 },
    { 0x12, 64 },
    { 0x0f, 68 },
    { 0x13, 72 },
    { 0x14, 80 },
    { 0x15, 88 },
    { 0x19, 96 },
    { 0x16, 104 },
    { 0x1a, 112 },
    { 0x17, 128 },
    { 0x1c, 144 },
    { 0x1d, 160 },
    { 0x1e, 192 },
    { 0x22, 224 },
    { 0x1f, 240 },
    { 0x23, 256 },
    { 0x24, 288 },
    { 0x25, 320 },
    { 0x26, 384 },
    { 0x2a, 448 },
    { 0x27, 480 },
    { 0x2b, 512 },
    { 0x2c, 576 },
    { 0x2d, 640 },
    { 0x2e, 768 },
    { 0x32, 896 },
    { 0x2f, 960 },
    { 0x33, 1024 },
    { 0x34, 1152 },
    { 0x35, 1280 },
    { 0x36, 1536 },
    { 0x3a, 1792 },
    { 0x37, 1920 },
    { 0x3b, 2048 },
    { 0x3c, 2304 },
    { 0x3d, 2560 },
    { 0x3e, 3072 },
    { 0x3f, 3840 }
};

//!< set i2c baudrate
/**
 * @brief  IIC?????? ???? ??????
 * @param  instance: IIC??? HW_I2C0~2
 * @param  sourceClockInHz :IIC???????
 * @param  baudrate        :IIC??????
 * @retval None
 */
static void I2C_SetBaudrate(uint32_t instance, uint32_t sourceClockInHz, uint32_t baudrate)
{
    /* check if the requested frequency is greater than the max supported baud. */
    if (baudrate > (sourceClockInHz / (1U * 20U)))
    {
        return;
    }
    uint32_t mult;
    uint32_t hz = baudrate;
    uint32_t bestError = 0xffffffffu;
    uint32_t bestMult = 0u;
    uint32_t bestIcr = 0u;
    /* Search for the settings with the lowest error.
     mult is the MULT field of the I2C_F register, and ranges from 0-2. It selects the
     multiplier factor for the divider. */
    for (mult = 0u; (mult <= 2u) && (bestError != 0); ++mult)
    {
        uint32_t multiplier = 1u << mult;
        /* scan table to find best match. */
        uint32_t i;
        for (i = 0u; i < ARRAY_SIZE(I2C_DiverTable); ++i)
        {
            uint32_t computedRate = sourceClockInHz / (multiplier * I2C_DiverTable[i].sclDivider);
            uint32_t absError = hz > computedRate ? hz - computedRate : computedRate - hz;
            if (absError < bestError)
            {
                bestMult = mult;
                bestIcr = I2C_DiverTable[i].icr;
                bestError = absError;
                /* If the error is 0, then we can stop searching because we won't find a
                 better match.*/
                if (absError == 0)
                {
                    break;
                }
            }
        }
    }
    I2C_InstanceTable[instance]->F = (I2C_F_ICR(bestIcr)|I2C_F_MULT(bestMult));
}


//! @defgroup CHKinetis
//! @{


//! @defgroup I2C
//! @brief I2C API functions
//! @{


 /**
 * @brief  ?????IIC??
 * @code
 *      // ???I2C??: ??I2C1???SCL:PC10 SDA:PC11,????:47000Hz
 *      I2C_QuickInit(I2C1_SCL_PC10_SDA_PC11, 47000);
 * @endcode
 * @param  MAP: I2C????????,??i2c.h??
 *         @arg I2C1_SCL_PE01_SDA_PE00
 *         @arg         ...
 *         @arg I2C1_SCL_PC10_SDA_PC11
 * @param  baudrate :???? ???: 48000Hz 76000Hz 96000Hz 376000Hz
 * @retval i2c???
 */
uint8_t I2C_QuickInit(uint32_t MAP, uint32_t baudrate)
{
    uint8_t i;
    I2C_InitTypeDef I2C_InitStruct1;
    QuickInit_Type * pq = (QuickInit_Type*)&(MAP);
    I2C_InitStruct1.baudrate = baudrate;
    I2C_InitStruct1.instance = pq->ip_instance;
    
    /* init pinmux and  open drain and pull up */
    for(i = 0; i < pq->io_offset; i++)
    {
        PORT_PinMuxConfig(pq->io_instance, pq->io_base + i, (PORT_PinMux_Type)pq->mux);
        PORT_PinPullConfig(pq->io_instance, pq->io_base + i, kPullUp); 
        PORT_PinOpenDrainConfig(pq->io_instance, pq->io_base + i, ENABLE);
    }
    
    /* init moudle */
    I2C_Init(&I2C_InitStruct1);
    return pq->ip_instance;
}

/**
 * @brief  ???I2C??
 * @note ??????????
 * @code
 *     //??i2c?1?? ?????48000hz
 *     I2C_InitTypeDef I2C_InitStruct1; //???????
 *     I2C_InitStruct1.baudrate = HW_I2C1; //??i2c?1??
 *     I2C_InitStruct1.instance = 48000;   //??????48000
 *     I2C_Init(&I2C_InitStruct1);
 * @endcode
 * @param  I2C_InitStruct :i2c????????
 * @retval None
 */
void I2C_Init(I2C_InitTypeDef* I2C_InitStruct)
{
	/* param check */
    assert_param(IS_I2C_ALL_INSTANCE(I2C_InitStruct->instance));
    uint32_t freq;
    SIM->SCGC4 |= SIM_I2CClockGateTable[I2C_InitStruct->instance];
    
    /* disable first */
    I2C_InstanceTable[I2C_InitStruct->instance]->C1 &= ~I2C_C1_IICEN_MASK;
    
    /* set baudrate */
    CLOCK_GetClockFrequency(kBusClock, &freq);
    I2C_SetBaudrate(I2C_InitStruct->instance, freq, I2C_InitStruct->baudrate);
    
    /* enable i2c */
    I2C_InstanceTable[I2C_InitStruct->instance]->C1 |= I2C_C1_IICEN_MASK;
}

/**
 * @brief  ??i2c????????
 * @code
 *     //??i2c?1????????
 *     I2C_GenerateSTART(HW_I2C1);
 * @endcode
 * @param  instance :I2C??? 
 *         @arg HW_I2C0  :I2C0??
 *         @arg HW_I2C1  :I2C1??
 *         @arg HW_I2C2  :I2C2??
 * @retval None
 */
void I2C_GenerateSTART(uint32_t instance)
{
	/* param check */
    assert_param(IS_I2C_ALL_INSTANCE(instance));
    I2C_InstanceTable[instance]->C1 |= I2C_C1_TX_MASK;
    I2C_InstanceTable[instance]->C1 |= I2C_C1_MST_MASK;
}

/**
 * @brief  ?????????
 * @code
 *     //??i2c?1????????
 *     I2C_GenerateRESTART(HW_I2C1);
 * @endcode
 * @param  instance :I2C??? 
 *         @arg HW_I2C0  :I2C0??
 *         @arg HW_I2C1  :I2C1??
 *         @arg HW_I2C2  :I2C2??
 * @retval None
 */
void I2C_GenerateRESTART(uint32_t instance)
{
	/* param check */
    assert_param(IS_I2C_ALL_INSTANCE(instance));
    I2C_InstanceTable[instance]->C1 |= I2C_C1_RSTA_MASK;
}

/**
 * @brief  ??????
 * @code
 *     //??i2c?1????????
 *     I2C_GenerateSTOP(HW_I2C1);
 * @endcode
 * @param  instance :I2C??? 
 *         @arg HW_I2C0  :I2C0??
 *         @arg HW_I2C1  :I2C1??
 *         @arg HW_I2C2  :I2C2??
 * @retval None
 */
void I2C_GenerateSTOP(uint32_t instance)
{
	/* param check */
    assert_param(IS_I2C_ALL_INSTANCE(instance));
	I2C_InstanceTable[instance]->C1 &= ~I2C_C1_MST_MASK;
    I2C_InstanceTable[instance]->C1 &= ~I2C_C1_TX_MASK;
}

/**
 * @brief  I2C???????
 * @code
 *     //??i2c?1??????0x5A
 *     I2C_SendData(HW_I2C1, 0x5A);
 * @endcode
 * @param  instance :I2C??? 
 *         @arg HW_I2C0  :I2C0??
 *         @arg HW_I2C1  :I2C1??
 *         @arg HW_I2C2  :I2C2??
 * @param  data     :???????
 * @retval None
 */
void I2C_SendData(uint32_t instance, uint8_t data)
{
	/* param check */
    assert_param(IS_I2C_ALL_INSTANCE(instance));
	I2C_InstanceTable[instance]->D = data;
}

/**
 * @brief  I2C???????
 * @code
 *     //??i2c?1??????
 *      uint8_t data; //????????
 *     data = I2C_ReadData(HW_I2C1);
 * @endcode
 * @param  instance :I2C??? 
 *         @arg HW_I2C0  :I2C0??
 *         @arg HW_I2C1  :I2C1??
 *         @arg HW_I2C2  :I2C2??
 * @retval  data    :?????
 */
uint8_t I2C_ReadData(uint32_t instance)
{
	/* param check */
    assert_param(IS_I2C_ALL_INSTANCE(instance));
	return (I2C_InstanceTable[instance]->D);
}
/**
 * @brief  i2c?????????????
 * @code
 *     //??i2c?1??????0x55???????
 *    I2C_Send7bitAddress(HW_I2C1, 0x55, kI2C_Read);
 * @endcode
 * @param  instance :I2C??? 
 *         @arg HW_I2C0  :I2C0??
 *         @arg HW_I2C1  :I2C1??
 *         @arg HW_I2C2  :I2C2??
 * @param  address: ??????:0~127
 * @param  direction:  ????????
 *         @arg kI2C_Read  : ????
 *         @arg kI2C_Write : ????
 * @retval None
 */
void I2C_Send7bitAddress(uint32_t instance, uint8_t address, I2C_Direction_Type direction)
{
	/* param check */
    assert_param(IS_I2C_ALL_INSTANCE(instance));
    address <<= 1;
	(kI2C_Write == direction)?((address &= 0xFE)):(address |= 0x01);
	I2C_InstanceTable[instance]->D = address;
}

/**
 * @brief  ?????? 
 * @note   ????????SendData????? ????????
 * @param  instance :I2C??? 
 *         @arg HW_I2C0  :I2C0??
 *         @arg HW_I2C1  :I2C1??
 *         @arg HW_I2C2  :I2C2??
 * @retval 0:????? 1:???? 2:??
 */
uint8_t I2C_WaitAck(uint32_t instance)
{
    uint32_t timeout = 0;
	/* param check */
    assert_param(IS_I2C_ALL_INSTANCE(instance));
    /* wait for transfer complete */
    timeout = 0;
    while (((I2C_InstanceTable[instance]->S & I2C_S_TCF_MASK) == 0) && (timeout < 5000))
    {
        timeout++;
        __NOP();
    }
    /* both TCF and IICIF indicate one byte trasnfer complete */
    timeout = 0;
    while (((I2C_InstanceTable[instance]->S & I2C_S_IICIF_MASK) == 0) && (timeout < 5000))
    {
        timeout++;
        __NOP();
    }
    /* IICIF is a W1C Reg, so clear it! */
    I2C_InstanceTable[instance]->S |= I2C_S_IICIF_MASK;
    if(timeout > 4990)
    {
        return 2;
    }
    /* see if we receive the ACK signal */
    if (I2C_InstanceTable[instance]->S & I2C_S_RXAK_MASK)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
/**
 * @brief  ??i2c????????????
 * @code
 *     //??i2c?1??????
 *    I2C_SetMasterMode(HW_I2C1, kI2C_Read);
 * @endcode
 * @param  instance :I2C??? 
 *         @arg HW_I2C0  :I2C0??
 *         @arg HW_I2C1  :I2C1??
 *         @arg HW_I2C2  :I2C2??
 * @param  direction:  ????????
 *         @arg kI2C_Read  : ????
 *         @arg kI2C_Write : ????
 * @retval None
 */
void I2C_SetMasterMode(uint32_t instance, I2C_Direction_Type direction)
{
	/* param check */
    assert_param(IS_I2C_ALL_INSTANCE(instance));
	(direction == kI2C_Write)?(I2C_InstanceTable[instance]->C1 |= I2C_C1_TX_MASK):(I2C_InstanceTable[instance]->C1 &= ~I2C_C1_TX_MASK);
}
/**
 * @brief  ?? NACK ??
 * @param  instance :I2C??? 
 *         @arg HW_I2C0  :I2C0??
 *         @arg HW_I2C1  :I2C1??
 *         @arg HW_I2C2  :I2C2??
 * @retval None
 */
void I2C_GenerateNAck(uint32_t instance)
{
	/* param check */
    assert_param(IS_I2C_ALL_INSTANCE(instance));
	I2C_InstanceTable[instance]->C1 |= I2C_C1_TXAK_MASK;
}
/**
 * @brief  ?? ACK ??
 * @param  instance :I2C??? 
 *         @arg HW_I2C0  :I2C0??
 *         @arg HW_I2C1  :I2C1??
 *         @arg HW_I2C2  :I2C2??
 * @retval None
 */
void I2C_GenerateAck(uint32_t instance)
{
	/* param check */
    assert_param(IS_I2C_ALL_INSTANCE(instance));
	I2C_InstanceTable[instance]->C1 &= ~I2C_C1_TXAK_MASK;
}
/**
 * @brief  I2C?????DMA????
 * @param  instance :I2C??? 
 *         @arg HW_I2C0  :I2C0??
 *         @arg HW_I2C1  :I2C1??
 *         @arg HW_I2C2  :I2C2??
 * @param  config  :???DMA??
 *         @arg kI2C_IT_Disable  :???? 
 *         @arg kI2C_DMA_Disable :??DMA
 *         @arg kI2C_IT_BTC,     :????????
 *         @arg kI2C_DMA_BTC     :??????DMA??
 * @retval None
 */
void I2C_ITDMAConfig(uint32_t instance, I2C_ITDMAConfig_Type config)
{
	/* param check */
    assert_param(IS_I2C_ALL_INSTANCE(instance));
    switch(config)
    {
        case kI2C_IT_Disable:
            NVIC_DisableIRQ(I2C_IRQnTable[instance]);
            I2C_InstanceTable[instance]->C1 &= ~I2C_C1_IICIE_MASK;
            break;
        case kI2C_DMA_Disable: 
            I2C_InstanceTable[instance]->C1 &= ~I2C_C1_DMAEN_MASK;
            break;
        case kI2C_IT_BTC:
            NVIC_EnableIRQ(I2C_IRQnTable[instance]);
            I2C_InstanceTable[instance]->C1 |= I2C_C1_IICIE_MASK;
            break;
        case kI2C_DMA_BTC:
            I2C_InstanceTable[instance]->C1 |= I2C_C1_DMAEN_MASK;
            break;
        default:
            break;
    }
}

/**
 * @brief  ????????
 * @param  instance :I2C??? 
 *         @arg HW_I2C0  :I2C0??
 *         @arg HW_I2C1  :I2C1??
 *         @arg HW_I2C2  :I2C2??
 * @param AppCBFun: ????????
 * @retval None
 * @note ?????????????????
 */
void I2C_CallbackInstall(uint32_t instance, I2C_CallBackType AppCBFun)
{
    if(AppCBFun != NULL)
    {
        I2C_CallBackTable[instance] = AppCBFun;
    }
}

/**
 * @brief  ??I2C??
 * @param  instance :I2C??? 
 *         @arg HW_I2C0  :I2C0??
 *         @arg HW_I2C1  :I2C1??
 *         @arg HW_I2C2  :I2C2??
 * @retval 0 :I2C?  1 :I2C??
 */
uint8_t I2C_IsBusy(uint32_t instance)
{
	if(I2C_InstanceTable[instance]->S & I2C_S_BUSY_MASK)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

/**
 * @brief  ??i2c?????????????
 * @code
 *     //??i2c?1?????????0x55?????????0x01???10??,???data????
 *    I2C_BurstRead(HW_I2C1, 0x55, 0x01, 1, &data, 10);
 * @endcode
 * @param  instance :I2C??? 
 *         @arg HW_I2C0  :I2C0??
 *         @arg HW_I2C1  :I2C1??
 *         @arg HW_I2C2  :I2C2??
 * @param  deviceAddress :??????0~127
 * @param  subAddress    :???????
 * @param  subAddressLen :?????
 * @param  buf         :????????
 * @param  len       :???????
 * @retval ??????? 0 ;?? ?? :??
 */
int32_t I2C_BurstRead(uint32_t instance, uint8_t deviceAddress, uint32_t subAddress, uint32_t subAddressLen, uint8_t* buf, uint32_t len)
{
    uint32_t time_out = 0;
    uint8_t i;
    uint8_t * p = (uint8_t*)&subAddress;
    /* generate START signal */
    I2C_GenerateSTART(instance);
    /* Send 7bit Data with WRITE operation */
    I2C_Send7bitAddress(instance, deviceAddress, kI2C_Write);
    if(I2C_WaitAck(instance))
    {
        I2C_GenerateSTOP(instance);
        time_out = 0;
        while(!I2C_IsBusy(instance) && (time_out < 10000))
        {
            time_out++;
        }
        return 1;
    }
    /* if len = 0 then return */
    if(!len)
    {
        I2C_GenerateSTOP(instance);
        while(!I2C_IsBusy(instance));
        return 0;
    }
    /* send sub address */
    for(i = 0; i < subAddressLen; i++)
    {
        /* send reg address */
        I2C_SendData(instance, *p++);
        if(I2C_WaitAck(instance))
        {
            I2C_GenerateSTOP(instance);
            time_out = 0;
            while(!I2C_IsBusy(instance)  && (time_out < 10000))
            {
                time_out++;
            }
            return 2;
        }
    }
    /* generate RESTART signal */
    I2C_GenerateRESTART(instance);
    /* resend 7bit Address, This time we use READ Command */
    I2C_Send7bitAddress(instance, deviceAddress, kI2C_Read);
    if(I2C_WaitAck(instance))
    {
        I2C_GenerateSTOP(instance);
        time_out = 0;
        while(!I2C_IsBusy(instance));
        return 3;
    }
    /* set master in slave mode */
    I2C_SetMasterMode(instance,kI2C_Read);
    /* dummy read */
    I2C_ReadData(instance); //dummy read
	I2C_GenerateAck(instance);
	I2C_WaitAck(instance);
    /* actually read */
	for(i = 0; i < len - 1; i++)
	{
		*(buf++) = I2C_ReadData(instance);
		I2C_GenerateAck(instance);
        I2C_WaitAck(instance);
	}
    /* read last */
	*buf = I2C_ReadData(instance);
    /* stop and finish */
    I2C_GenerateNAck(instance);
    I2C_WaitAck(instance);
    I2C_GenerateSTOP(instance);
    while(!I2C_IsBusy(instance));
    return 0;
}

/**
 * @brief  ??i2c?????????????
 * @code
 *     //??i2c?1???data??10?????????0x55?????????0x01????
 *    I2C_BurstWrite(HW_I2C1, 0x55, 0x01, 1, &data, 10);
 * @endcode
 * @param  instance :I2C??? 
 *         @arg HW_I2C0  :I2C0??
 *         @arg HW_I2C1  :I2C1??
 *         @arg HW_I2C2  :I2C2??
 * @param  deviceAddress :??????0~127
 * @param  subAddress    :???????
 * @param  subAddressLen :?????
 * @param  buf         :????????
 * @param  len       :???????
 * @retval ??????? 0 ;?? ?? :??
 */
uint8_t I2C_BurstWrite(uint32_t instance ,uint8_t deviceAddress, uint32_t subAddress, uint32_t subAddressLen, uint8_t *buf, uint32_t len)
{
    uint32_t time_out = 0;
    uint8_t * p = (uint8_t*)&subAddress;
    /* generate START signal */
    I2C_GenerateSTART(instance);
    /* send 7bit Data with WRITE operation */
    I2C_Send7bitAddress(instance, deviceAddress, kI2C_Write);
    if(I2C_WaitAck(instance))
    {
        I2C_GenerateSTOP(instance);
        time_out = 0;
        while(!I2C_IsBusy(instance) && (time_out < 10000))
        {
            time_out++;
        }
        return 1;
    }
    /* send all address */
    while(subAddressLen--)
    {
        I2C_SendData(instance, *(p++));
        if(I2C_WaitAck(instance))
        {
            I2C_GenerateSTOP(instance);
            time_out = 0;
            while(!I2C_IsBusy(instance) && (time_out < 10000))
            {
                time_out++;
            }
            return 2;
        }
    }
    /* send all data */
    while(len--)
    {
        I2C_SendData(instance, *(buf++));
        if(I2C_WaitAck(instance))
        {
            I2C_GenerateSTOP(instance);
            time_out = 0;
            while(!I2C_IsBusy(instance) && (time_out < 10000))
            {
                time_out++;
            }
            return 3;
        }
    }
    /* generate stop and wait for line idle */
    I2C_GenerateSTOP(instance);
    time_out = 0;
    while(!I2C_IsBusy(instance));
    return 0;
}

/**
 * @brief  ????????(????????????8??? ?MMA845x??????)
 * @code
 *     //??i2c?1???data??1?????????0x55???????0x01???
 *    I2C_WriteSingleRegister(HW_I2C1, 0x55, 0x01, data);
 * @endcode
 * @param  instance :I2C??? 
 *         @arg HW_I2C0  :I2C0??
 *         @arg HW_I2C1  :I2C1??
 *         @arg HW_I2C2  :I2C2??
 * @param  deviceAddress :??????0~127
 * @param  registerAddress: ??????????
 * @param  Data: ???????
 * @retval 0:??  1:?????? 2:??????
 */
uint8_t I2C_WriteSingleRegister(uint32_t instance, uint8_t deviceAddress, uint8_t registerAddress, uint8_t data)
{
    return I2C_BurstWrite(instance ,deviceAddress, registerAddress, 1, &data, 1);
}

/**
 * @brief  ?????????(????????????8??? ?MMA845x??????)
 * @code
 *     //??i2c?1???????0x55???????0x01???,???data?
 *    I2C_WriteSingleRegister(HW_I2C1, 0x55, 0x01, &data);
 * @endcode
 * @param  instance :I2C??? 
 *         @arg HW_I2C0  :I2C0??
 *         @arg HW_I2C1  :I2C1??
 *         @arg HW_I2C2  :I2C2??
 * @param  deviceAddress :??????0~127
 * @param  registerAddress: ??????????
 * @param  pData: ????
 * @retval 0:??  1:?????? 2:??????
 */
uint8_t I2C_ReadSingleRegister(uint32_t instance, uint8_t deviceAddress, uint8_t registerAddress, uint8_t* pData)
{
    return I2C_BurstRead(instance, deviceAddress, registerAddress, 1, pData, 1);
}


#if 0
int SCCB_ReadSingleRegister(uint32_t instance, uint8_t chipAddr, uint8_t subAddr, uint8_t* pData)
{
    uint32_t time_out = 0;
    I2C_GenerateSTART(instance);
    /* Send 7bit Data with WRITE operation */
    I2C_Send7bitAddress(instance, chipAddr, kI2C_Write);
    if(I2C_WaitAck(instance))
    {
        I2C_GenerateSTOP(instance);
        time_out = 0;
        while(!I2C_IsBusy(instance) && (time_out < 10000))
        {
            time_out++;
        }
        return 1;
    }
    /* send reg address */
    I2C_SendData(instance, subAddr);
    if(I2C_WaitAck(instance))
    {
        I2C_GenerateSTOP(instance);
        time_out = 0;
        while(!I2C_IsBusy(instance)  && (time_out < 10000))
        {
            time_out++;
        }
        return 2;
    }
    I2C_GenerateSTOP(instance);
    while(!I2C_IsBusy(instance));
    time_out = 0;
    /* generate START signal */
    I2C_GenerateSTART(instance);
    /* resend 7bit Address, This time we use READ Command */
    I2C_Send7bitAddress(instance, chipAddr, kI2C_Read);
    if(I2C_WaitAck(instance))
    {
        I2C_GenerateSTOP(instance);
        time_out = 0;
        while(!I2C_IsBusy(instance));
        return 3;
    }
    /* set master in slave mode */
    I2C_SetMasterMode(instance,kI2C_Read);
    /* dummy read */
    I2C_ReadData(instance);
	I2C_GenerateNAck(instance);
	I2C_WaitAck(instance);
    I2C_GenerateSTOP(instance);
    while(!I2C_IsBusy(instance));
    *pData = I2C_ReadData(instance);

    return 0;
}

int SCCB_WriteSingleRegister(uint32_t instance, uint8_t chipAddr, uint8_t subAddr, uint8_t data)
{
    return I2C_WriteSingleRegister(instance, chipAddr, subAddr, data);
}
#endif


//! @}

//! @}
/**
 * @brief  ????????
 * @param  I2C0_IRQHandler :???I2C0????????
 *         I2C1_IRQHandler :???I2C1????????
 * @note ????????????
 */
void I2C0_IRQHandler(void)
{
    // clear pending bit
    I2C_InstanceTable[HW_I2C0]->S |= I2C_S_IICIF_MASK;
    if(I2C_CallBackTable[HW_I2C0])
    {
        I2C_CallBackTable[HW_I2C0]();
    }
    
}

#ifdef I2C1
void I2C1_IRQHandler(void)
{
    /* clear pending bit */
    I2C_InstanceTable[HW_I2C1]->S |= I2C_S_IICIF_MASK;
    if(I2C_CallBackTable[HW_I2C1])
    {
        I2C_CallBackTable[HW_I2C1]();
    }
}
#endif



#if 0
static const QuickInit_Type I2C_QuickInitTable[] =
{
    { 1, 4, 6, 0, 2, 0}, //I2C1_SCL_PE01_SDA_PE00 6
    { 0, 4, 4,18, 2, 0}, //I2C0_SCL_PE19_SDA_PE18 4
    { 0, 5, 2,22, 2, 0}, //I2C0_SCL_PF22_SDA_PF23 2
    { 0, 1, 2, 0, 2, 0}, //I2C0_SCL_PB00_SDA_PB01 2
    { 0, 1, 2, 2, 2, 0}, //I2C0_SCL_PB02_SDA_PB03 2
    { 1, 2, 2,10, 2, 0}, //I2C1_SCL_PC10_SDA_PC11 2
    { 0, 3, 2, 8, 2, 0}, //I2C0_SCL_PD08_SDA_PD09 2
    { 0, 4, 5,24, 2, 0}, //I2C0_SCL_PE24_SDA_PE25 5 //only on K64
};
#endif

#endif


/* i2c bus scan test */
void I2C_Scan(uint32_t MAP)
{
    uint8_t i;
    uint8_t ret;
    uint32_t instance;
    instance = I2C_QuickInit(MAP, 100*1000);
    for(i = 1; i < 127; i++)
    {
        ret = I2C_Probe(instance , i);
        if(!ret)
        {
            LIB_TRACE("ADDR:0x%2X(7BIT) | 0x%2X(8BIT) found!\r\n", i, i<<1);
        }
    }
}


