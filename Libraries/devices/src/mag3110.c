#include "mag3110.h"
#include "math.h"
/*  Max Min Value in each axis  */
int16_t MAG3110_XMax = -1024;
int16_t MAG3110_XMin = 1024;
int16_t MAG3110_YMax = -1024;
int16_t MAG3110_YMin = 1024;
int16_t MAG3110_ZMax = -1024;
int16_t MAG3110_ZMin = 1024;
/*     (Max+Min)/2              */
int16_t MAG3110_XOff = 0;
int16_t MAG3110_YOff = 0;
int16_t MAG3110_ZOff = 0;
/*      Gain                    */
float   MAG3110_XGain = 1;
float   MAG3110_YGain = 0;
float   MAG3110_ZGain = 0;
/***********************************************************************************************
   * @function: MAG3110_WriteRegister
   * @param   : Register  : Register Addr in LCM
                Value     : Value
   * @return  : none
   * @detail  : also known as WriteCommand in some application
************************************************************************************************/
void static MAG3110_WriteRegister(uint8_t Register, uint8_t Value)
{
  I2C_GenerateSTART(MAG3110_I2C_BASE_PTR);
  I2C_Send7bitAddress(MAG3110_I2C_BASE_PTR,MAG3110_SLAVE_I2C_ADDR,I2C_MASTER_WRITE);
  I2C_WaitAck(MAG3110_I2C_BASE_PTR);
  
  I2C_SendData(MAG3110_I2C_BASE_PTR,Register);
  I2C_WaitAck(MAG3110_I2C_BASE_PTR);
  
  I2C_SendData(MAG3110_I2C_BASE_PTR,Value);
  I2C_WaitAck(MAG3110_I2C_BASE_PTR);

  I2C_GenerateSTOP(MAG3110_I2C_BASE_PTR);
  while(I2C_IsLineBusy(MAG3110_I2C_BASE_PTR) == TRUE);
}
/***********************************************************************************************
   * @function: MAG3110_ReadRegister
   * @param   : Register  : Register Addr in LCM
   * @return  : Register Value
   * @detail  : none
************************************************************************************************/
uint8_t  MAG3110_ReadRegister(uint8_t Register)
{
   uint8_t result; 
  I2C_GenerateSTART(MAG3110_I2C_BASE_PTR);
  
  I2C_Send7bitAddress(MAG3110_I2C_BASE_PTR,MAG3110_SLAVE_I2C_ADDR,I2C_MASTER_WRITE);
  I2C_WaitAck(MAG3110_I2C_BASE_PTR);
  
  I2C_SendData(MAG3110_I2C_BASE_PTR, Register);
  I2C_WaitAck(MAG3110_I2C_BASE_PTR);
  
  I2C_GenerateRESTART(MAG3110_I2C_BASE_PTR);
  
  I2C_Send7bitAddress(MAG3110_I2C_BASE_PTR,MAG3110_SLAVE_I2C_ADDR,I2C_MASTER_READ);
  I2C_WaitAck(MAG3110_I2C_BASE_PTR);
  I2C_SetMasterMode(MAG3110_I2C_BASE_PTR,I2C_MASTER_READ);
  
  result = I2C_ReadData(MAG3110_I2C_BASE_PTR);
  I2C_GenerateNAck(MAG3110_I2C_BASE_PTR);
  I2C_WaitAck(MAG3110_I2C_BASE_PTR);
  I2C_GenerateSTOP(MAG3110_I2C_BASE_PTR);
  result =  I2C_ReadData(MAG3110_I2C_BASE_PTR);
  while(I2C_IsLineBusy(MAG3110_I2C_BASE_PTR) == TRUE);
  return result;
}

/***********************************************************************************************
   * @function: MAG3110_GetXYZData
   * @param   : MAG3110_XYZDataTypeDef  : DataStruct for MAG3110
   * @return  : TRUE: Succ  FALSE:Err
   * @detail  : none
************************************************************************************************/
uint8_t MAG3110_GetXYZData(MAG3110_XYZDataTypeDef* MAG3110_XYZDataStruct)
{
  uint8_t ret;
  ret = MAG3110_ReadRegister(MAG_DR_STATUS);
  if(ret & MAG_DR_STATUS_ZYXDR_MASK)
  {
    MAG3110_XYZDataStruct->m_XData = MAG3110_ReadRegister(MAG_OUT_X_LSB) + 256*MAG3110_ReadRegister(MAG_OUT_X_MSB);
    MAG3110_XYZDataStruct->m_YData = MAG3110_ReadRegister(MAG_OUT_Y_LSB) + 256*MAG3110_ReadRegister(MAG_OUT_Y_MSB);
    MAG3110_XYZDataStruct->m_ZData = MAG3110_ReadRegister(MAG_OUT_Z_LSB) + 256*MAG3110_ReadRegister(MAG_OUT_Z_MSB);  
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}
/***********************************************************************************************
   * @function: MAG3110_GetAngle
   * @param   : MAG3110_XYZDataTypeDef  : DataStruct for MAG3110
   * @return  : TRUE: Succ  FALSE:Err
   * @detail  : none
************************************************************************************************/
uint8_t MAG3110_GetAngle(uint16_t* res)
{
  int16_t x,y,z;
  int16_t angle;
  MAG3110_XYZDataTypeDef MAG3110_XYZDataStruct1;
  if(MAG3110_GetXYZData(&MAG3110_XYZDataStruct1) == TRUE)
  {
    x = MAG3110_XYZDataStruct1.m_XData;
    y = MAG3110_XYZDataStruct1.m_YData;
		z = MAG3110_XYZDataStruct1.m_ZData;
		/*  find if MAG3110 is calibrated, if not, use deafault macro  */
		if((MAG3110_XOff == 0) && (MAG3110_YOff == 0) && (MAG3110_ZOff == 0)) //no cal ,use deafalut value
		{
			x-= MAG3110_DEFAULT_XOFF;y-= MAG3110_DEFAULT_YOFF;z-= MAG3110_DEFAULT_ZOFF;
			MAG3110_XGain = MAG3110_DEFAULT_XGAIN;
			MAG3110_YGain = MAG3110_DEFAULT_YGAIN;
			MAG3110_ZGain = MAG3110_DEFAULT_ZGAIN;
			x = ((float)x*MAG3110_XGain);
			y = ((float)y*MAG3110_YGain);
			z = ((float)z*MAG3110_ZGain);
		}
		else //caled ,use real-time cal value 
		{
			x-= MAG3110_XOff; y -= MAG3110_YOff; z -= MAG3110_ZOff;
			x = ((float)x*MAG3110_XGain);
			y = ((float)y*MAG3110_YGain);
			z = ((float)z*MAG3110_ZGain);
		}

		angle = (atan2((float)y,(float)x)*180/3.14) + 180;
    *res = angle;
    return TRUE;
  }
  return FALSE;
}
/***********************************************************************************************
   * @function: MAG3110_StartCalibrateOnce
   * @param   : Calibrate
   * @return  : TRUE: Succ  FALSE:Err
   * @detail  : this function need to call many times to calibrate
************************************************************************************************/
uint8_t MAG3110_StartCalibrateOnce(void)
{
  MAG3110_XYZDataTypeDef MAG3110_XYZDataStruct1;
  uint16_t abs_XRangle = 0;
  uint16_t abs_YRangle = 0;
  uint16_t abs_ZRangle = 0;
  if(MAG3110_GetXYZData(&MAG3110_XYZDataStruct1) == TRUE)
  {
    //X
    if(MAG3110_XYZDataStruct1.m_XData > MAG3110_XMax)
    {
      MAG3110_XMax = MAG3110_XYZDataStruct1.m_XData;
    }
    if(MAG3110_XYZDataStruct1.m_XData < MAG3110_XMin)
    {
      MAG3110_XMin = MAG3110_XYZDataStruct1.m_XData;
    }
    //Y
    if(MAG3110_XYZDataStruct1.m_YData > MAG3110_YMax)
    {
      MAG3110_YMax = MAG3110_XYZDataStruct1.m_YData;
    }
    if(MAG3110_XYZDataStruct1.m_YData < MAG3110_YMin)
    {
      MAG3110_YMin = MAG3110_XYZDataStruct1.m_YData;
    }
		//Z
    if(MAG3110_XYZDataStruct1.m_ZData > MAG3110_ZMax)
    {
      MAG3110_ZMax = MAG3110_XYZDataStruct1.m_ZData;
    }
    if(MAG3110_XYZDataStruct1.m_ZData < MAG3110_ZMin)
    {
      MAG3110_ZMin = MAG3110_XYZDataStruct1.m_ZData;
    }
  MAG3110_XOff = (MAG3110_XMax + MAG3110_XMin)/2;
  MAG3110_YOff = (MAG3110_YMax + MAG3110_YMin)/2;
  MAG3110_ZOff = (MAG3110_ZMax + MAG3110_ZMin)/2;
		
  MAG3110_XGain = 1;
  abs_XRangle = abs(MAG3110_XMax - MAG3110_XMin);
  abs_YRangle = abs(MAG3110_YMax - MAG3110_YMin);
  abs_ZRangle = abs(MAG3110_ZMax - MAG3110_ZMin);
  MAG3110_YGain = MAG3110_XGain*((float)abs_YRangle/(float)abs_XRangle);
  MAG3110_ZGain = MAG3110_XGain*((float)abs_ZRangle/(float)abs_XRangle);
  /* you may display MAG3110_XOff yoff zff ,ygain zgain here for use  */
    return TRUE;
  }
  return FALSE;
}

/* Initalize MAG3110     */
uint8_t MAG3110_Init(void)
{
  uint8_t RegisterValue = 0;
  I2C_InitTypeDef I2C_InitStruct1;
  /* Initalize Kinetis I2C module  */
  I2C_InitStruct1.I2CxMAP = MAG3110_I2C_PORT;
  I2C_InitStruct1.I2C_ClockSpeed = MAG3110_I2C_SPEED;
  I2C_Init(&I2C_InitStruct1);
  
  /* see if master can read WHO_AM_I correct*/
  RegisterValue = MAG3110_ReadRegister(MAG_WHO_AM_I);
  if(RegisterValue != MAG_WHO_AM_I_VALUE) 
  {
    return FALSE;
  }
   /*  set common value     */
  MAG3110_WriteRegister(MAG_CTRL_REG1,0x00);
  MAG3110_WriteRegister(MAG_CTRL_REG1,0x19); //10Hz
  MAG3110_WriteRegister(MAG_CTRL_REG2,0x80);
  
  return TRUE;
}



