/**
  ******************************************************************************
  * @file    mma845x.c
  * @author  YANDLD
  * @version V2.4
  * @date    2013.5.23
  * @brief   超核原子核核心板 BSP构件 MMA845x加速度传感器驱动
  ******************************************************************************
  */
#include "mma845x.h"
//MMA845x_写寄存器
 void MMA845x_WriteRegister(I2C_Type* I2Cx, unsigned char u8RegisterAddress, unsigned char u8Data)
{
    //发送地址
    I2C_GenerateSTART(MMA845x_I2C_PORT);
    I2C_Send7bitAddress(MMA845x_I2C_PORT,MMA845x_IIC_ADDRESS,I2C_MASTER_WRITE);
    I2C_WaitAck(MMA845x_I2C_PORT);
    //发送需要写入的寄存器地址
    I2C_SendData(MMA845x_I2C_PORT,u8RegisterAddress);
    I2C_WaitAck(MMA845x_I2C_PORT);
    //发送数据
    I2C_SendData(MMA845x_I2C_PORT,u8Data);
    I2C_WaitAck(MMA845x_I2C_PORT);
    //结束
    I2C_GenerateSTOP(MMA845x_I2C_PORT);
    while(I2C_IsLineBusy(MMA845x_I2C_PORT) == TRUE);
}
//MMA85x读寄存器
uint8_t MMA845x_ReadRegister(I2C_Type* I2Cx, uint8_t Addr)
{
    uint8_t result;
    //发送从机地址
    I2C_GenerateSTART(I2Cx);
    I2C_Send7bitAddress(I2Cx,0x38,I2C_MASTER_WRITE);
    I2C_WaitAck(I2Cx);
    //需要读取的寄存器
    I2C_SendData(I2Cx, Addr);
    I2C_WaitAck(I2Cx);
    //重新开始
    I2C_GenerateRESTART(I2Cx);
    //主机读
    I2C_Send7bitAddress(I2Cx,0x38,I2C_MASTER_READ);
    I2C_WaitAck(I2Cx);
    //设置Kinetis为接收模式
    I2C_SetMasterMode(I2Cx,I2C_MASTER_READ);
    I2C_GenerateAck(I2Cx);
    result = I2Cx->D;
    I2C_WaitAck(I2Cx);
    //发送停止信号
    I2C_GenerateSTOP(I2Cx);
    //读取数据
    result =  I2Cx->D;
    return result;
}

//初始化MMA745x
void MMA845x_Init(void)
{	
	I2C_InitTypeDef I2C_InitStruct1;
	I2C_InitStruct1.I2CxMAP = MMA845x_I2C_PIN_MAP;
	I2C_InitStruct1.I2C_ClockSpeed = I2C_CLOCK_SPEED_200KHZ;
	I2C_Init(&I2C_InitStruct1);
	
	MMA845x_WriteRegister(MMA845x_I2C_PORT,CTRL_REG1,ASLP_RATE_20MS+DATA_RATE_1250US);
	MMA845x_WriteRegister(MMA845x_I2C_PORT,XYZ_DATA_CFG_REG, FULL_SCALE_2G); //2G
	MMA845x_WriteRegister(MMA845x_I2C_PORT,HP_FILTER_CUTOFF_REG, PULSE_LPF_EN_MASK ); //低通滤波
	MMA845x_WriteRegister(MMA845x_I2C_PORT,CTRL_REG1, ACTIVE_MASK);          //激活状态
}
 
uint16_t MMA845x_GetData(uint8_t REG_Address)
{
	uint8_t H,L;
	H = MMA845x_ReadRegister(MMA845x_I2C_PORT,REG_Address);
	L = MMA845x_ReadRegister(MMA845x_I2C_PORT,REG_Address+1);
	return ((H<<8)+L);  
}

