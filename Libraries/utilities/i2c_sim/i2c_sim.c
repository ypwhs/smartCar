#include "i2c_sim.h"
#include "gpio.h"


static uint32_t ginstance, gsdaPin, gsclPin;

#define GPIO_I2C_DELAY          DelayUs(2);
#define GPIO_I2C_SDA(bit)       GPIO_WriteBit(ginstance, gsdaPin, bit)
#define GPIO_I2C_SCL(bit)       GPIO_WriteBit(ginstance, gsclPin, bit)
#define GPIO_I2C_READ           GPIO_ReadBit(ginstance, gsdaPin)
#define GPIO_I2C_SDA_DIR(dir)   GPIO_PinConfig(ginstance, gsdaPin, (GPIO_PinConfig_Type)(dir))
#define GPIO_I2C_ACTIVE         do { } while (0)
#define GPIO_I2C_TRISTATE       do { } while (0)


/* static void  i2c_SendReset(void); */
static void  i2c_SendStart(void);
static void  i2c_SendStop(void);
static void  i2c_SendAck(int32_t ack);
static int32_t i2c_WriteByte(uint8_t data);
static uint8_t i2c_ReadByte(int32_t ack);

void I2C_SIM_Init(uint32_t instance, uint8_t sdaPin, uint8_t sclPin)
{
    /* init GPIO */
    GPIO_QuickInit(instance, sdaPin, kGPIO_Mode_OPP);
    GPIO_QuickInit(instance, sclPin, kGPIO_Mode_OPP);
    ginstance = instance;
    gsdaPin = sdaPin;
    gsclPin = sclPin;
}


/*!
 * @brief Send START.
 *
 * Send START. High -> Low on SDA while SCL is High.
 */
static void i2c_SendStart(void)
{  
    GPIO_I2C_DELAY;
    GPIO_I2C_SDA(1);
    GPIO_I2C_ACTIVE;
    GPIO_I2C_DELAY;
    GPIO_I2C_SCL(1);
    GPIO_I2C_DELAY;
    GPIO_I2C_SDA(0);
    GPIO_I2C_DELAY;
}

/*!
 * @brief Send STOP.
 *
 * Send STOP. STOP: Low -> High on SDA while SCL is High.
 */
static void i2c_SendStop(void)
{
    GPIO_I2C_SCL(0);
    GPIO_I2C_DELAY;
    GPIO_I2C_SDA(0);
    GPIO_I2C_ACTIVE;
    GPIO_I2C_DELAY;
    GPIO_I2C_SCL(1);
    GPIO_I2C_DELAY;
    GPIO_I2C_SDA(1);
    GPIO_I2C_DELAY;
    GPIO_I2C_TRISTATE;
}

/*!
 * @brief Send ACK.
 *
 * Send ACK. Ack should be GPIO_I2C_ACK or GPIO_I2C_NOACK.
 */
static void i2c_SendAck(int32_t ack)
{
    GPIO_I2C_SCL(0);
    GPIO_I2C_DELAY;
    GPIO_I2C_ACTIVE;
    GPIO_I2C_SDA(ack);
    GPIO_I2C_DELAY;
    GPIO_I2C_SCL(1);
    GPIO_I2C_DELAY;
    GPIO_I2C_DELAY;
    GPIO_I2C_SCL(0);
    GPIO_I2C_DELAY;
}

/*-----------------------------------------------------------------------
 * Send 8 bits and look for an acknowledgement.
 */
/*!
 * @brief Send 8 bits and look for an acknowledgement.
 *
 * @param[in] data      Data that will be write.
 *
 * @return Ackledge value. Not a nack is an ack.
 */
static int32_t i2c_WriteByte(uint8_t data)
{
    int32_t j;
    int32_t nack;

    GPIO_I2C_ACTIVE;
    for (j = 0; j < 8; j++)
    {
        GPIO_I2C_SCL(0);
        GPIO_I2C_DELAY;
        GPIO_I2C_SDA(data & 0x80);
        GPIO_I2C_DELAY;
        GPIO_I2C_SCL(1);
        GPIO_I2C_DELAY;
        GPIO_I2C_DELAY;

        data <<= 1;
    }

    /* Look for an <ACK>(negative logic) and return it. */
    GPIO_I2C_SCL(0);
    GPIO_I2C_DELAY;
    GPIO_I2C_SDA(1);
    GPIO_I2C_SDA_DIR(kInput);
    GPIO_I2C_TRISTATE;
    GPIO_I2C_DELAY;
    GPIO_I2C_SCL(1);
    GPIO_I2C_DELAY;
    GPIO_I2C_DELAY;
    nack = GPIO_I2C_READ;
    GPIO_I2C_SCL(0);
    GPIO_I2C_DELAY;
    GPIO_I2C_ACTIVE;
    GPIO_I2C_SDA_DIR(kOutput);

    return (nack);    /* not a nack is an ack */
}

/*!
 * @brief Read one byte.
 *
 * if ack == GPIO_I2C_ACK, ACK the byte so can continue reading, else
 * send GPIO_I2C_NOACK to end the read.
 *
 * @param[in] ack    Ack data that to be sent.
 *
 * @return Read data byte.
 */
static uint8_t i2c_ReadByte(int32_t ack)
{
    uint32_t  data;
    int32_t  j;

    /* Read 8 bits, MSB first. */
    GPIO_I2C_TRISTATE;
    GPIO_I2C_SDA_DIR(kInput);
    GPIO_I2C_DELAY;
    data = 0;
    for (j = 0; j < 8; j++)
    {
        GPIO_I2C_SCL(0);
        GPIO_I2C_DELAY;
        GPIO_I2C_DELAY;
        GPIO_I2C_SCL(1);
        GPIO_I2C_DELAY;
        data <<= 1;
        data |= GPIO_I2C_READ;
        GPIO_I2C_DELAY;
    }
    GPIO_I2C_SCL(0);
    GPIO_I2C_SDA_DIR(kOutput);
    i2c_SendAck(ack);

    return (data);
}

/*!
 * @brief Probe to see if a chip is present.
 * Probe to see if a chip is present. Also good for checking for the
 * completion of EEPROM writes since the chip stops responding until
 * the write completes (typically 10mSec).
 *
 * @param[in] addr    GPIO_I2C slave device address.
 *
 * @return Probe successful or failed.
 */
uint8_t i2c_gpio_probe(uint8_t addr)
{
    int32_t rc;

    /*
     * perform 1 byte write transaction with just address byte
     * (fake write)
     */
    i2c_SendStart();
    /*
     * Our address is by default shifted left by 1
     */
    /* rc = i2c_WriteByte((addr << 1) | 0); */
    rc = i2c_WriteByte((addr) | 0);
    i2c_SendStop();

    return (rc ? 1 : 0);
}

/*!
 * @brief Read bytes.
 *
 * This function will read data from i2c slave device.
 *
 * @param[in] chip   Chip number.
 * @param[in] addr   Address in GPIO_I2C slave device.
 * @param[in] alen   Address len.
 * @param[in] buffer Buffer address that will be read to.
 * @param[in] len    Len of data that will be read.
 *
 * @return Read successful or not.
 */
uint8_t i2c_gpio_read(uint8_t chip, uint32_t addr, int32_t alen, uint8_t *buffer, int32_t len)
{
    int32_t shift;

    /*
     * Do the addressing portion of a write cycle to set the
     * chip's address pointer.  If the address length is zero,
     * don't do the normal write cycle to set the address pointer,
     * there is no address pointer in this chip.
     */
    i2c_SendStart();
    if (alen > 0)
    {
        /* 
         * Our address is by default shifted left by 1
         */
        if (i2c_WriteByte(chip | 0))
        {
            /* write cycle */
            i2c_SendStop();
            return (1);
        }
        shift = (alen - 1) * 8;
        while (alen-- > 0)
        {
            if (i2c_WriteByte(addr >> shift))
            {
                return (1);
            }
            shift -= 8;
        }

        /*
         * Some GPIO_I2C chips need a stop/start sequence here,
         * other chips don't work with a full stop and need
         * only a start.  Default behaviour is to send the
         * stop/start sequence.
         */
        /* i2c_SendStop(); */
        i2c_SendStart();
    }

    /*
     * Send the chip address again, this time for a read cycle.
     * Then read the data.  On the last byte, we do a NACK instead
     * of an ACK(len == 0) to terminate the read.
     * Our address is by default shifted left by 1
     */
    /* i2c_WriteByte((chip << 1) | 1); */
    i2c_WriteByte(chip | 1);    /* read cycle */
    while (len-- > 0)
    {
        *buffer++ = i2c_ReadByte(len == 0);
    }
    i2c_SendStop();

    return (0);
}

/*!
 * @brief Write bytes.
 *
 * This function will write data to i2c slave device.
 *
 * @param[in] chip   Chip address.
 * @param[in] addr   Address in GPIO_I2C slave device.
 * @param[in] alen   Address len.
 * @param[in] buffer Buffer address that will be write.
 * @param[in] len    Len of data that will be write.
 *
 * @return Write failed times.
 */
uint8_t i2c_gpio_write(uint8_t chip, uint32_t addr, int32_t alen, uint8_t *buffer, int32_t len)
{
    int32_t shift, failures = 0;

    i2c_SendStart();
    /*
     * Our address is by default shifted left by 1
     */
    if (i2c_WriteByte(chip | 0))
    {
        /* write cycle */
        i2c_SendStop();
        return (1);
    }
    shift = (alen - 1) * 8;
    while (alen-- > 0)
    {
        if (i2c_WriteByte(addr >> shift))
        {
            return (1);
        }
        shift -= 8;
    }

    while (len-- > 0)
    {
        if (i2c_WriteByte(*buffer++))
        {
            failures++;
        }
    }
    i2c_SendStop();

    return (failures);
}

int I2C_SIM_BurstRead(uint8_t deviceAddress, uint32_t subAddress, uint32_t subAddressLen, uint8_t* buf, uint32_t len)
{
    deviceAddress<<=1;
    return !i2c_gpio_read(deviceAddress, subAddress, subAddressLen, buf, len);
}


int I2C_SIM_BurstWrite(uint8_t deviceAddress, uint32_t subAddress, uint32_t subAddressLen, uint8_t *buf, uint32_t len)
{
    deviceAddress<<=1;
    return !i2c_gpio_write(deviceAddress, subAddress, subAddressLen, buf, len);
}

uint8_t I2C_SIM_Probe(uint8_t addr)
{
    addr<<=1;
    return i2c_gpio_probe(addr);
}



