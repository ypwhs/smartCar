/**
  ******************************************************************************
  * @file    mpu6050.c
  * @author  YANDLD
  * @version V2.5
  * @date    2013.12.25
  * @brief   www.beyondcore.net   http://upcmcu.taobao.com 
  ******************************************************************************
  */
#include "mpu6050.h"
#include "i2c.h"


#define MPU6050_DEBUG		0
#if ( MPU6050_DEBUG == 1 )
#define MPU6050_TRACE	printf
#else
#define MPU6050_TRACE(...)
#endif

/**********The register define of the MPU-6050***********************************/
/*W&R : Louis Bradshaw  In GDUPT*/
/*DataSheet Translate : Louis Bradshaw*/
/*Time: 2013-3-28 02:22*/
/*Carefully Active : 
This Register_Map are based on the MUP_6050.
It's difficient from the MPU-6000 */
/********Register Name**********Register Addr**************/
/*******|				|******|    		 |*************/
#define	 AUX_VDDIO					0x01		//1		辅助I2C电源选择寄存器			
#define	 SELF_TEST_X				0x0D		//13 	X轴自检寄存器
#define	 SELF_TEST_Y				0x0E		//14 	Y轴自检寄存器		
#define	 SELF_TEST_Z				0x0F		//15 	Z轴自检寄存器
#define	 SELF_TEST_A				0x10		//16	加速度计自检
#define	 SMPLRT_DIV				 	0x19		//19	样本频率分频寄存器
#define	 CONFIG					 	0x1A		//26	配置寄存器		
#define  GYRO_CONFIG				0x1B		//27	陀螺仪配置寄存器	
#define  ACCEL_CONFIG 				0x1C		//28	加速寂渲眉拇嫫髌				
#define  FF_THR	 					0x1D		//29	自由落体阈值寄存器				
#define  FF_DUR						0x1E		//30	自由落体持续时间寄存器			
#define  MOT_THR					0x1F		//31	运动探测阈值寄存器			
#define  MOT_DUR					0x20		//32	运动探测持续时间寄存器				
#define  ZRMOT_THR					0x21		//33	零运动阈值检测寄存器				
#define  ZRMOT_DUR					0x22		//34	零运动持续时间寄存器		
#define  FIFO_EN					0x23		//35	FIFO使能寄存器				
#define  I2C_MST_CTRL				0x24		//36	I2C主机控制寄存器				
#define  I2C_SLV0_ADDR				0x25		//37	I2C从机0地址寄存器				
#define  I2C_SLV0_REG				0x26		//38	I2C从机0寄存器			
#define  I2C_SLV0_CTRL				0x27		//39	I2C从机0控制寄存器		
#define  I2C_SLV1_ADDR				0x28		//40	I2C从机1地址寄存器			
#define  I2C_SLV1_REG				0x29		//41	I2C从机1寄存器			
#define  I2C_SLV1_CTRL				0x2A		//42	I2C从机1控制寄存器			
#define  I2C_SLV2_ADDR				0x2B		//43	I2C从机2地址寄存器				
#define  I2C_SLV2_REG				0x2C		//44	I2C从机2寄存器				
#define  I2C_SLV2_CTRL				0x2D		//45	I2C从机2控制寄存器			
#define  I2C_SLV3_ADDR				0x2E		//46	I2C从机3地址寄存器			
#define  I2C_SLV3_REG 				0x2F		//47	I2C从机3寄存器			
#define  I2C_SLV3_CTRL				0x30		//48	I2C从机3控制寄存器		
#define  I2C_SLV4_ADDR	 			0x31		//49	I2C从机4地址寄存器				
#define  I2C_SLV4_REG				0x32		//50	I2C从机4寄存器			
#define  I2C_SLV4_DO				0x33		//51	I2C从机4直接输出寄存器（Direct Output）			
#define  I2C_SLV4_CTRL				0x34		//52	I2C从机4控制寄存器			
#define  I2C_SLV2_DI				0x35		//53	I2C从机4直接输入寄存器（Direct Iutput）				
#define  I2C_MST_STATUS				0x36		//54	I2C主机状态寄存器				
#define  INT_PIN_CFG				0x37		//55	中断引脚/旁路使能配置寄存器			
#define  INT_ENABLE					0x38		//56	中断使能寄存器				
#define  INT_STATUS					0x3A		//58	中断状态寄存器				
#define  ACCEL_XOUT_H				0x3B		//59	加速计测量值寄存器		X轴高八位		
#define  ACCEL_XOUT_L				0x3C		//60	加速计测量值寄存器		X轴低八位
#define  ACCEL_YOUT_H				0x3D		//61	加速计测量值寄存器		Y轴高八位		
#define  ACCEL_YOUT_L				0x3E		//62	加速计测量值寄存器		Y轴低八位		
#define  ACCEL_ZOUT_H				0x3F		//63	加速计测量值寄存器		Z轴高八位		
#define  ACCEL_ZOUT_L				0x40		//64	加速计测量值寄存器		Z轴低八位		
#define  TEMP_OUT_H					0x41		//65	温度测量值寄存器		高八位			
#define  TEMP_OUT_L					0x42		//66	温度测量值寄存器		低八位			
#define  GYRO_XOUT_H				0x43		//67	陀螺仪测量值寄存器		X轴高八位		
#define  GYRO_XOUT_L				0x44		//68	陀螺仪测量值寄存器		X轴低八位		
#define  GYRO_YOUT_H				0x45		//69	陀螺仪测量值寄存器		Y轴高八位		
#define  GYRO_YOUT_L				0x46		//70	陀螺仪测量值寄存器		Y轴低八位		
#define  GYRO_ZOUT_H				0x47		//71	陀螺仪测量值寄存器		Z轴高八位		
#define  GYRO_ZOUT_L				0x48		//72	陀螺仪测量值寄存器		Z轴低八位		
#define  EXT_SENS_DATA_00			0x49		//73	外接传感器数据寄存器0	（磁力计用）			
#define  EXT_SENS_DATA_01			0x4A		//74	外接传感器数据寄存器1	（磁力计用）			
#define  EXT_SENS_DATA_02			0x4B		//75	外接传感器数据寄存器2	（磁力计用）			
#define  EXT_SENS_DATA_03			0x4C		//76	外接传感器数据寄存器3	（磁力计用）			
#define  EXT_SENS_DATA_04			0x4D		//77	外接传感器数据寄存器4	（磁力计用）			
#define  EXT_SENS_DATA_05			0x4E		//78	外接传感器数据寄存器5	（磁力计用）			
#define  EXT_SENS_DATA_06			0x4F		//79	外接传感器数据寄存器6	（磁力计用）			
#define  EXT_SENS_DATA_07			0x50		//80	外接传感器数据寄存器7	（磁力计用）			
#define  EXT_SENS_DATA_08			0x51		//81	外接传感器数据寄存器8	（磁力计用）			
#define  EXT_SENS_DATA_09			0x52		//82	外接传感器数据寄存器9	（磁力计用）			
#define  EXT_SENS_DATA_10			0x53		//83	外接传感器数据寄存器10	（磁力计用）			
#define  EXT_SENS_DATA_11			0x54		//84	外接传感器数据寄存器11	（磁力计用）			
#define  EXT_SENS_DATA_12			0x55		//85	外接传感器数据寄存器12	（磁力计用）			
#define  EXT_SENS_DATA_13			0x56		//86	外接传感器数据寄存器13	（磁力计用）			
#define  EXT_SENS_DATA_14			0x57		//87	外接传感器数据寄存器14	（磁力计用）			
#define  EXT_SENS_DATA_15			0x58		//88	外接传感器数据寄存器15	（磁力计用）			
#define  EXT_SENS_DATA_16			0x59		//89	外接传感器数据寄存器16	（磁力计用）			
#define  EXT_SENS_DATA_17			0x5A		//90	外接传感器数据寄存器17	（磁力计用）			
#define  EXT_SENS_DATA_18			0x5B		//91	外接传感器数据寄存器18	（磁力计用）			
#define  EXT_SENS_DATA_19			0x5C		//92	外接传感器数据寄存器19	（磁力计用）			
#define  EXT_SENS_DATA_20			0x5D		//93	外接传感器数据寄存器20	（磁力计用）			
#define  EXT_SENS_DATA_21			0x5E		//94	外接传感器数据寄存器21	（磁力计用）			
#define  EXT_SENS_DATA_22			0x5F		//95	外接传感器数据寄存器22	（磁力计用）			
#define  EXT_SENS_DATA_23			0x60		//96	外接传感器数据寄存器23	（磁力计用）			
#define  MOT_DETECT_STATUS			0x61		//97	运动探测状态寄存器				
#define  I2C_SLV0_D0				0x63		//99	I2C0模式数据输出寄存器				
#define  I2C_SLV1_D0				0x64		//100	I2C1模式数据输出寄存器			
#define  I2C_SLV2_D0				0x65		//101	I2C2模式数据输出寄存器			
#define  I2C_SLV3_D0				0x66		//102	I2C3模式数据输出寄存器			
#define  I2C_MST_DELAY_CTRL			0x67		//103	I2C主机模式延时控制寄存器				
#define  SINGLE_PATH_RESET			0x68		//104	数据信号路径复位寄存器（清空模拟和数字信号路径）			
#define  MOT_DETECT_CTRL			0x69		//105	运动探测控制寄存器			
#define  USER_CTRL					0x6A		//106	用户控制寄存器			
#define  PWR_MGMT_1					0x6B		//107	电源管理寄存器1			
#define  PWR_MGMT_2					0x6C		//108	电源管理寄存器2		
#define  FIFO_COUNTH				0x72		//		FIFO计数器寄存器高八位		
#define  FIFO_COUNTL				0x73		//		FIFO计数器寄存器低八位		
#define  FIFO_R_W					0x74		//		FIFO读写寄存器		
#define  WHO_AM_I					0x75		//		身份验证寄存器		
/***********************************************************************************************************/

/* PWR_MGMT_1	 Bit Fields */
#define MPU_PWR_MGMT_1_DEVICE_RESET_MASK	0x80u				
#define MPU_PWR_MGMT_1_DEVICE_RESET_SHIFT	7				
#define MPU_PWR_MGMT_1_SLEEP_MASK			0x40u			
#define MPU_PWR_MGMT_1_SLEEP_RESET_SHIFT	6				
#define MPU_PWR_MGMT_1_CYCLE_MASK			0x20u			
#define MPU_PWR_MGMT_1_CYCLE_RESET_SHIFT	5				
#define MPU_PWR_MGMT_1_TEMP_DIS_MASK		0x8u			
#define MPU_PWR_MGMT_1_TEMP_DIS_SHIFT		3			
#define MPU_PWR_MGMT_1_CLKSEL_MASK			0x3u		
#define MPU_PWR_MGMT_1_CLKSEL_SHIFT			0
#define MPU_PWR_MGMT_1_CLKSEL_DATA(x)		(((uint8_t)(((uint8_t)(x))<<MPU_PWR_MGMT_1_CLKSEL_SHIFT))&MPU_PWR_MGMT_1_CLKSEL_MASK)
/* CONFIG	 Bit Fields */
#define MPU_CONFIG_EXT_SYNC_SET_MASK		0x38u
#define MPU_CONFIG_EXT_SYNC_SET_SHIFT		3
#define MPU_CONFIG_EXT_SYNC_SET_DATA(x)		(((uint8_t)(((uint8_t)(x))<<MPU_CONFIG_EXT_SYNC_SET_SHIFT))&MPU_CONFIG_EXT_SYNC_SET_MASK)
#define MPU_CONFIG_DLPF_CFG_MASK			0x3u
#define MPU_CONFIG_DLPF_CFG_SHIFT			0
#define MPU_CONFIG_DLPF_CFG_DATA(x)			(((uint8_t)(((uint8_t)(x))<<MPU_CONFIG_DLPF_CFG_SHIFT))&MPU_CONFIG_DLPF_CFG_MASK)
/* SMPLRT_DIV	 Bit Fields */
#define MPU_SMPLRT_DIV_DATA_MASK			0xFFu
#define MPU_SMPLRT_DIV_DATA_SHIFT			0
#define MPU_SMPLRT_DIV_DATA(x)		 		(((uint8_t)(((uint8_t)(x))<<MPU_SMPLRT_DIV_DATA_SHIFT))&MPU_SMPLRT_DIV_DATA_MASK)
/* GYRO_CONFIG	 Bit Fields */
#define MPU_GYRO_CONFIG_XG_ST_MASK			0x80u
#define MPU_GYRO_CONFIG_XG_ST_SHIFT			7
#define MPU_GYRO_CONFIG_YG_ST_MASK			0x40u
#define MPU_GYRO_CONFIG_YG_ST_SHIFT			6
#define MPU_GYRO_CONFIG_ZG_ST_MASK			0x20u
#define MPU_GYRO_CONFIG_ZG_ST_SHIFT			5
#define MPU_GYRO_CONFIG_FS_SEL_MASK			0x18u
#define MPU_GYRO_CONFIG_FS_SEL_SHIFT		3
#define MPU_GYRO_CONFIG_FS_SEL_DATA(x)		(((uint8_t)(((uint8_t)(x))<<MPU_GYRO_CONFIG_FS_SEL_SHIFT))&MPU_GYRO_CONFIG_FS_SEL_MASK)
/* ACCEL_CONFIG	 Bit Fields */
#define MPU_ACCEL_CONFIG_XA_ST_MASK			0x80u
#define MPU_ACCEL_CONFIG_XA_ST_SHIFT			7
#define MPU_ACCEL_CONFIG_YA_ST_MASK			0x40u
#define MPU_ACCEL_CONFIG_YA_ST_SHIFT			6
#define MPU_ACCEL_CONFIG_ZA_ST_MASK			0x20u
#define MPU_ACCEL_CONFIG_ZA_ST_SHIFT			5
#define MPU_ACCEL_CONFIG_AFS_SEL_MASK		0x18u
#define MPU_ACCEL_CONFIG_AFS_SEL_SHIFT			3
#define MPU_ACCEL_CONFIG_AFS_SEL_DATA(x)	(((uint8_t)(((uint8_t)(x))<<MPU_ACCEL_CONFIG_AFS_SEL_SHIFT))&MPU_ACCEL_CONFIG_AFS_SEL_MASK)	
/***********Device base address*************/

struct mpu_device 
{
    uint8_t     addr;
    uint32_t    instance;
    void        *user_data;
};

static struct mpu_device mpu_dev;

static const uint8_t mpu_addr[] = {0x68, 0x69};

static int write_reg(uint8_t addr, uint8_t val)
{
    return I2C_WriteSingleRegister(mpu_dev.instance, mpu_dev.addr, addr, val);
}

int mpu6050_init(uint32_t instance)
{
    int i;
    uint8_t id;
    
    mpu_dev.instance = instance;
    
    for(i = 0; i < ARRAY_SIZE(mpu_addr); i++)
    {
        if(!I2C_ReadSingleRegister(instance, mpu_addr[i], WHO_AM_I, &id))
        {
            if(id == 0x68)
            {
                mpu_dev.addr = mpu_addr[i];
                
                /* init sequence */
                write_reg(PWR_MGMT_1, 0x00);
                write_reg(SMPLRT_DIV, 0x0A);
                write_reg(CONFIG, 0x00);
                write_reg(AUX_VDDIO,0x80);
                write_reg(GYRO_CONFIG, 0x18);
                write_reg(ACCEL_CONFIG, 0x00);
                write_reg(I2C_MST_CTRL, 0x00);
                write_reg(INT_PIN_CFG, 0x02);
                
                /* init sequence */
                MPU6050_TRACE("mpu6050 found!addr:0x%X\r\n", device.chip_addr);
                return 0;     
            }
        }
    }
    return 1;
}




int mpu6050_read_accel(int16_t* x, int16_t* y, int16_t* z)
{
    uint8_t err;
    uint8_t buf[6];
    
    err = I2C_BurstRead(mpu_dev.instance, mpu_dev.addr, ACCEL_XOUT_H, 1, buf, 6);
    
    *x=(int16_t)(((uint16_t)buf[0]<<8)+buf[1]); 	    
    *y=(int16_t)(((uint16_t)buf[2]<<8)+buf[3]); 	    
    *z=(int16_t)(((uint16_t)buf[4]<<8)+buf[5]); 
    return err;    
}

//!< read gyro data
int mpu6050_read_gyro(int16_t* x, int16_t* y, int16_t* z)
{
    uint8_t err;
    uint8_t buf[6];
    
    err = I2C_BurstRead(mpu_dev.instance, mpu_dev.addr, GYRO_XOUT_H, 1, buf, 6);
    
    *x=(int16_t)(((uint16_t)buf[0]<<8)+buf[1]); 	    
    *y=(int16_t)(((uint16_t)buf[2]<<8)+buf[3]); 	    
    *z=(int16_t)(((uint16_t)buf[4]<<8)+buf[5]); 
    return err;    
}

