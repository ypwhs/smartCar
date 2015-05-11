#ifndef __MAG3110_H
#define __MAG3110_H

#include "sys.h"
#include "i2c.h"
#include "math.h"

/*  external pin define   */
#define MAG3110_INT_PORT  PTA
#define MAG3110_INT_PIN   (4)

/* Interface define  */
#define MAG3110_I2C_BASE_PTR      I2C1
#define MAG3110_I2C_PORT          I2C1_SCL_PC10_SDA_PC11
#define MAG3110_I2C_SPEED         I2C_CLOCK_SPEED_100KHZ

/*  MAG3110 hardware Param Define   */
#define MAG3110_SLAVE_I2C_ADDR      (uint8_t)(0x1C)
/*  MAG3110 Internal register address define */
#define MAG_DR_STATUS 0x00
#define MAG_OUT_X_MSB 0x01
#define MAG_OUT_X_LSB 0x02
#define MAG_OUT_Y_MSB 0x03
#define MAG_OUT_Y_LSB 0x04
#define MAG_OUT_Z_MSB 0x05
#define MAG_OUT_Z_LSB 0x06
#define MAG_WHO_AM_I  0x07
#define MAG_SYSMOD    0x08
#define MAG_OFF_X_MSB 0x09
#define MAG_OFF_X_LSB 0x0A
#define MAG_OFF_Y_MSB 0x0B
#define MAG_OFF_Y_LSB 0x0C
#define MAG_OFF_Z_MSB 0x0D
#define MAG_OFF_Z_LSB 0x0E
#define MAG_DIE_TEMP  0x0F
#define MAG_CTRL_REG1 0x10
#define MAG_CTRL_REG2 0x11
/* CTRL1 Bit Fields */
#define MAG_CTRL1_AC_MASK                  0x01
#define MAG_CTRL1_AC_SHIFT                 0
#define MAG_CTRL1_TM_MASK                  0x02
#define MAG_CTRL1_TM_SHIFT                 1
#define MAG_CTRL1_FR_MASK                  0x04
#define MAG_CTRL1_FR_SHIFT                 2
#define MAG_CTRL1_OS_MASK                  0x18
#define MAG_CTRL1_OS_SHIFT                 3
#define MAG_CTRL1_OS(x)                    (((uint8_t)(((uint8_t)(x))<<MAG_CTRL1_OS_SHIFT))&MAG_CTRL1_OS_MASK)
#define MAG_CTRL1_DR_MASK                  0xE0
#define MAG_CTRL1_DR_SHIFT                 5
#define MAG_CTRL1_DR(x)                    (((uint8_t)(((uint8_t)(x))<<MAG_CTRL1_DR_SHIFT))&MAG_CTRL1_DR_MASK)
/* SYSMOD Bit Fields */
#define MAG_SYSMOD_SYSMOD_MASK             0x03
#define MAG_SYSMOD_SYSMOD_SHIFT            0
#define MAG_SYSMOD_SYSMOD(x)               (((uint8_t)(((uint8_t)(x))<<MAG_SYSMOD_SYSMOD_SHIFT))&MAG_SYSMOD_SYSMOD_MASK)
/* DR_STATUS Bit Fields */
#define MAG_DR_STATUS_XDR_MASK             0x01
#define MAG_DR_STATUS_XDR_SHIFT            0
#define MAG_DR_STATUS_YDR_MASK             0x02
#define MAG_DR_STATUS_YDR_SHIFT            1
#define MAG_DR_STATUS_ZDR_MASK             0x04
#define MAG_DR_STATUS_ZDR_SHIFT            2
#define MAG_DR_STATUS_ZYXDR_MASK           0x08
#define MAG_DR_STATUS_ZYXDR_SHIFT          3
#define MAG_DR_STATUS_XOW_MASK             0x10
#define MAG_DR_STATUS_XOW_SHIFT            4
#define MAG_DR_STATUS_YOW_MASK             0x20
#define MAG_DR_STATUS_YOW_SHIFT            5
#define MAG_DR_STATUS_ZOW_MASK             0x40
#define MAG_DR_STATUS_ZOW_SHIFT            6
#define MAG_DR_STATUS_ZYXOW_MASK           0x80
#define MAG_DR_STATUS_ZYXOW_SHIFT          7
/**
 * @}
 */ /* end of group MAG3110 Masks */

   
/* MAG3110 Internal const value   */
#define MAG_WHO_AM_I_VALUE  (uint8_t)(0xC4)

/* MAG3110 Data TypeDef*/
typedef struct    
{
  int16_t m_XData;
  int16_t m_YData;
  int16_t m_ZData;
}MAG3110_XYZDataTypeDef;
   
/*   default calibration value   */
#define MAG3110_DEFAULT_XOFF      (186)
#define MAG3110_DEFAULT_YOFF      (-331)
#define MAG3110_DEFAULT_ZOFF      (-435)
#define MAG3110_DEFAULT_XGAIN      (1)
#define MAG3110_DEFAULT_YGAIN      (1.09)
#define MAG3110_DEFAULT_ZGAIN      (0.96)

/*   API    functions      */
uint8_t MAG3110_Init(void);
uint8_t  MAG3110_ReadRegister(uint8_t Register);
void  MAG3110_WriteRegister(uint8_t Register, uint8_t Value);
uint8_t MAG3110_GetXYZData(MAG3110_XYZDataTypeDef* MAG3110_XYZDataStruct);
uint8_t MAG3110_GetAngle(uint16_t* res);
uint8_t MAG3110_StartCalibrateOnce(void);

#endif



