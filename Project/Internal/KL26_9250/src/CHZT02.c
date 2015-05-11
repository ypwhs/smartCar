#include "gpio.h"
#include "common.h"
#include "CHZT02.h"
#include "CH9250.h"
#include "stdio.h"
#include "systick.h"
#include "imu.h"
extern float gyroBias[3], accelBias[3]; 	/**from CH9250.c*/
extern float magCalibration[3], magbias[3]; /**from CH9250.c*/
extern uint8_t Ascale;      				/**from CH9250.c*/
extern uint8_t Gscale;  					/**from CH9250.c*/						
extern uint8_t Mscale;  					/**from CH9250.c*/		
extern uint8_t Mmode;  						/**from CH9250.c*/
extern float aRes, gRes, mRes;				/**from CH9250.c*/
extern int16_t accelCount[3];  // Stores the 16-bit signed accelerometer sensor output
extern int16_t gyroCount[3];   // Stores the 16-bit signed gyro sensor output
extern int16_t magCount[3];    // Stores the 16-bit signed magnetometer sensor output
extern float magCalibration[3], magbias[3];  // Factory mag calibration and mag bias
extern float gyroBias[3], accelBias[3]; // Bias corrections for gyro and accelerometer
extern int16_t tempCount;   // Stores the real internal chip temperature in degrees Celsius
extern float temperature;
extern float SelfTest[6];

imu_io_install_t  			ch9250_imu_io_install_t;
imu_float_euler_angle_t		ch9250_imu_float_euler_angle_t;   
imu_raw_data_t				ch9250_imu_raw_data_t;	

int8_t CHZT02_Init(void) {
	/**Switch mosfet startup*/
	GPIO_QuickInit(CHZT02_SensorEN_Group, CHZT02_SensorEN_Pin, CHZT02_SensorEN_OutputMode);
	GPIO_WriteBit(CHZT02_SensorEN_Group, CHZT02_SensorEN_Pin, CHZT02_SensorEnable);
	/**I2C Init*/
	I2C_QuickInit(CHZT02_I2C_MAP,180000);
	/**MPU9250 Init*/
	uint8_t mpu9250_ID = 0;
	funsionDataInit();						/**global value overtake*/
	ch9250GetId(&mpu9250_ID);
	/**optional*/	printf("CHZT02_Init-------sensor MPU9250 id is 0x%x\n\r",mpu9250_ID);
	ch9250Reset();
	ch9250Calibrate(gyroBias, accelBias);
	/**optional*/	printf("x gyro bias = %f\n\r", gyroBias[0]);
	/**optional*/	printf("y gyro bias = %f\n\r", gyroBias[1]);
	/**optional*/	printf("z gyro bias = %f\n\r", gyroBias[2]);
	/**optional*/	printf("x accel bias = %f\n\r",accelBias[0]);
	/**optional*/	printf("y accel bias = %f\n\r",accelBias[1]);
	/**optional*/	printf("z accel bias = %f\n\r",accelBias[2]);
	DelayMs(200);
	ch9250Init();
	/**optional*/	printf("MPU9250 initialized for active data mode....\n\r");
	ch8963Init(magCalibration);
	/**optional*/	printf("AK8963 initialized for active data mode....\n\r"); // Initialize device for active mode read of magnetometer
	/**optional*/	printf("Accelerometer full-scale range = %f  g\n\r", 2.0f*(float)(1<<Ascale));
	/**optional*/	printf("Gyroscope full-scale range = %f  deg/s\n\r", 250.0f*(float)(1<<Gscale));
	/**optional*/	ch9250RegisterValueCheck();
	/**optional*/	if(Mscale == 0) printf("Magnetometer resolution = 14  bits\n\r");
	/**optional*/	if(Mscale == 1) printf("Magnetometer resolution = 16  bits\n\r");
	/**optional*/	if(Mmode == 2)  printf("Magnetometer ODR = 8 Hz\n\r");
	/**optional*/	if(Mmode == 6)  printf("Magnetometer ODR = 100 Hz\n\r");
	DelayMs(200);					
	ch9250GetAres(); // Get accelerometer sensitivity
    ch9250GetGres(); // Get gyro sensitivity
    ch9250GetMres(); // Get magnetometer sensitivity					
 	/**optional*/	printf("Accelerometer sensitivity is %f LSB/g \n\r", 1.0f/aRes);
 	/**optional*/	printf("Gyroscope sensitivity is %f LSB/deg/s \n\r", 1.0f/gRes);
 	/**optional*/	printf("Magnetometer sensitivity is %f LSB/G \n\r", 1.0f/mRes);						
	magbias[0] = +470.;  // User environmental x-axis correction in milliGauss, should be automatically calculated
    magbias[1] = +120.;  // User environmental x-axis correction in milliGauss
    magbias[2] = +125.;  // User environmental x-axis correction in milliGauss	
	
	ch9250_imu_io_install_t.imu_get_accel = ch9250ReadAccelData; 
	ch9250_imu_io_install_t.imu_get_gyro = ch9250ReadGyroData;
	ch9250_imu_io_install_t.imu_get_mag = ch9250ReadMagData;
	imu_io_install(&ch9250_imu_io_install_t);//imu Func Loading
	return 0;
}

int8_t CHZT02_Running (void) {
	uint8_t status = 0;
	I2C_ReadSingleRegister(0,MPU9250_ADDR,MPU9250_INT_STATUS,&status);
	if(status & 0x01) {  // On interrupt, check if data ready interrupt
//		ch9250ReadAccelData(&accelCount[0],&accelCount[1],&accelCount[2]);  // Read the x/y/z adc values   
//		// Now we'll calculate the accleration value into actual g's
//		ch9250_imu_raw_data_t.ax = (float)accelCount[0]*aRes - accelBias[0];  // get actual g value, this depends on scale being set
//		ch9250_imu_raw_data_t.ay = (float)accelCount[1]*aRes - accelBias[1];   
//		ch9250_imu_raw_data_t.az = (float)accelCount[2]*aRes - accelBias[2];  

//		ch9250ReadGyroData(&gyroCount[0],&gyroCount[1],&gyroCount[2]);  // Read the x/y/z adc values
//		// Calculate the gyro value into actual degrees per second
//		ch9250_imu_raw_data_t.gx = (float)gyroCount[0]*gRes - gyroBias[0];  // get actual gyro value, this depends on scale being set
//		ch9250_imu_raw_data_t.gy = (float)gyroCount[1]*gRes - gyroBias[1];  
//		ch9250_imu_raw_data_t.gz = (float)gyroCount[2]*gRes - gyroBias[2];   

//		ch9250ReadMagData(&magCount[0],&magCount[1],&magCount[2]);  // Read the x/y/z adc values   
//		// Calculate the magnetometer values in milliGauss
//		// Include factory calibration per data sheet and user environmental corrections
//		ch9250_imu_raw_data_t.mx = (float)magCount[0]*mRes*magCalibration[0] - magbias[0];  // get actual magnetometer value, this depends on scale being set
//		ch9250_imu_raw_data_t.my = (float)magCount[1]*mRes*magCalibration[1] - magbias[1];  
//		ch9250_imu_raw_data_t.mz = (float)magCount[2]*mRes*magCalibration[2] - magbias[2];		
		imu_get_euler_angle(&ch9250_imu_float_euler_angle_t,&ch9250_imu_raw_data_t);
	}
	
	return 0;
}
