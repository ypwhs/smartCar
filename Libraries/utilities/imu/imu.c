/**
  ******************************************************************************
  * @file    imu.c
  * @author  YANDLD
  * @version V2.5
  * @date    2015.3.26
  * @brief   www.beyondcore.net   http://upcmcu.taobao.com 
  * @note    
  ******************************************************************************
  */
#include "imu.h"



/*******************************************************************************
 * Defination
 ******************************************************************************/
#define PI             3.1415926f
#define Kp             3.0f     /* proportional gain governs rate of convergence to accelerometer/magnetometer */
#define Ki             0.002f     /* integral gain governs rate of convergence of gyroscope biases */


#define Gyro_Gr        (0.00052653)
   
float halfT = 0.002f;

/* sliding filter */
#if 0
static uint32_t imu_sliding_filter(imu_raw_data_t raw_data, imu_raw_data_t * filter_data)
{
    int32_t sum_accel_x = 0;
    int32_t sum_accel_y = 0;
    int32_t sum_accel_z = 0;
    int32_t sum_gyro_x = 0;
    int32_t sum_gyro_y = 0;
    int32_t sum_gyro_z = 0;
    
    /* fifo */
    static int16_t fifo_accel_x[SLIDING_FILTER_DEEP];
    static int16_t fifo_accel_y[SLIDING_FILTER_DEEP];
    static int16_t fifo_accel_z[SLIDING_FILTER_DEEP];
    static int16_t fifo_gyro_x[SLIDING_FILTER_DEEP];
    static int16_t fifo_gyro_y[SLIDING_FILTER_DEEP];
    static int16_t fifo_gyro_z[SLIDING_FILTER_DEEP];
    
    for(int i = SLIDING_FILTER_DEEP-1; i > 0; i--)
    {
        fifo_accel_x[i] =  fifo_accel_x[i-1];
        fifo_accel_y[i] =  fifo_accel_y[i-1];
        fifo_accel_z[i] =  fifo_accel_z[i-1];		
        fifo_gyro_x[i] =  fifo_gyro_x[i-1];
        fifo_gyro_y[i] =  fifo_gyro_y[i-1];
        fifo_gyro_z[i] =  fifo_gyro_z[i-1];
    }
    
    /* fifo input */
    fifo_accel_x[0] = raw_data.ax;
    fifo_accel_y[0] = raw_data.ay;
    fifo_accel_z[0] = raw_data.az;
    fifo_gyro_x[0] = raw_data.gx;
    fifo_gyro_y[0] = raw_data.gy;
    fifo_gyro_z[0] = raw_data.gz;
    
    /** fifo calculate */
    for(int i=0; i < SLIDING_FILTER_DEEP; i++)
    {
        sum_accel_x += fifo_accel_x[i];
        sum_accel_y += fifo_accel_y[i];
        sum_accel_z += fifo_accel_z[i];
        sum_gyro_x += fifo_gyro_x[i];
        sum_gyro_y += fifo_gyro_y[i];
        sum_gyro_z += fifo_gyro_z[i];	
    }
    filter_data->ax = sum_accel_x/SLIDING_FILTER_DEEP;
    filter_data->ay = sum_accel_y/SLIDING_FILTER_DEEP; 
    filter_data->az = sum_accel_z/SLIDING_FILTER_DEEP; 
    filter_data->gx = sum_gyro_x/SLIDING_FILTER_DEEP;
    filter_data->gy = sum_gyro_y/SLIDING_FILTER_DEEP; 
    filter_data->gz = sum_gyro_z/SLIDING_FILTER_DEEP; 
    filter_data->mx = raw_data.mx;
    filter_data->my = raw_data.my;
    filter_data->mz = raw_data.mz; 
    return 0;
}
#endif



static inline float invSqrt(float x) 
{
	float halfx = 0.5f * x;
	float y = x;
	long i = *(long*)&y;
	i = 0x5f3759df - (i>>1);
	y = *(float*)&i;
	y = y * (1.5f - (halfx * y * y));
	return y;
}


//!< the mx my mz order are related to PCB layout!!
#ifdef URANUS2
static inline void updateAHRS(float gx,float gy,float gz,float ax,float ay,float az,float my,float mx,float mz, attitude_t * angle)
#elif URANUS
static inline void updateAHRS(float gx,float gy,float gz,float ax,float ay,float az,float mx,float mz,float my, attitude_t * angle)
#endif
{
    #ifdef URANUS2
    mz = -mz;
    #endif
    float norm = 0;
    float hx = 0, hy = 0, hz = 0, bx = 0, bz = 0;			
    float vx = 0, vy = 0, vz = 0, wx = 0, wy = 0, wz = 0;		
    float ex = 0, ey = 0, ez = 0;
    static float q0 = 1; 
    static float q1 = 0; 
    static float q2 = 0;
    static float q3 = 0;
    static float exInt = 0, eyInt = 0, ezInt = 0;

    float q0q0 = q0*q0;
    float q0q1 = q0*q1;
    float q0q2 = q0*q2;
    float q0q3 = q0*q3;
    float q1q1 = q1*q1;
    float q1q2 = q1*q2;
    float q1q3 = q1*q3;
    float q2q2 = q2*q2;
    float q2q3 = q2*q3;
    float q3q3 = q3*q3;

    if(ax*ay*az==0)
    {
        return;    
    }
		
    
    norm = invSqrt(ax*ax + ay*ay + az*az);
    ax = ax * norm;
    ay = ay * norm;
    az = az * norm;

    norm = invSqrt(mx*mx + my*my + mz*mz);
    mx = mx * norm;
    my = my * norm;
    mz = mz * norm;
	
    /* compute reference direction of flux */
    hx = 2*mx*(0.5f - q2q2 - q3q3) + 2*my*(q1q2 - q0q3) + 2*mz*(q1q3 + q0q2);
    hy = 2*mx*(q1q2 + q0q3) + 2*my*(0.5f - q1q1 - q3q3) + 2*mz*(q2q3 - q0q1);
    hz = 2*mx*(q1q3 - q0q2) + 2*my*(q2q3 + q0q1) + 2*mz*(0.5f - q1q1 - q2q2);         

    bx = 1/invSqrt((hx*hx) + (hy*hy));
    bz = hz; 
	
    /* estimated direction of gravity and flux (v and w) */
    vx = 2*(q1q3 - q0q2);
    vy = 2*(q0q1 + q2q3);
    vz = q0q0 - q1q1 - q2q2 + q3q3 ;

    wx = 2*bx*(0.5 - q2q2 - q3q3) + 2*bz*(q1q3 - q0q2);
    wy = 2*bx*(q1q2 - q0q3) + 2*bz*(q0q1 + q2q3);
    wz = 2*bx*(q0q2 + q1q3) + 2*bz*(0.5 - q1q1 - q2q2);
    
    /* error is sum of cross product between reference direction of fields and direction measured by sensors */
    ex = (ay*vz - az*vy) + (my*wz - mz*wy);
    ey = (az*vx - ax*vz) + (mz*wx - mx*wz);
    ez = (ax*vy - ay*vx) + (mx*wy - my*wx);

    exInt = exInt + ex * Ki;
    eyInt = eyInt + ey * Ki;
    ezInt = ezInt + ez * Ki;

    /* adjusted gyroscope measurements */
    gx = gx + Kp*ex + exInt;
    gy = gy + Kp*ey + eyInt;
    gz = gz + Kp*ez + ezInt;

    /* integrate quaternion rate and normalise */
    q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
    q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
    q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
    q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;

    /* normalise quaternion */
    norm = invSqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
    q0 = q0 * norm;
    q1 = q1 * norm;
    q2 = q2 * norm;
    q3 = q3 * norm;
    
    /* output data */
    angle->Y = atan2(2 * q1 * q2 + 2 * q0 * q3, q0*q0+q1*q1-q2*q2-q3*q3)* 57.3; 
    angle->P = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3;																			// pitcho???
    angle->R = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3;
}

//!< this functino must be called about every 2ms to get accurate eular angles

uint32_t imu_get_euler_angle(float *adata, float *gdata, float *mdata, attitude_t *angle)
{

    updateAHRS( (float)gdata[0] * Gyro_Gr,
                (float)gdata[1] * Gyro_Gr,
                (float)gdata[2] * Gyro_Gr,
                (float)adata[0],
                (float)adata[1],
                (float)adata[2],
                (float)mdata[0],
                (float)mdata[1],
                (float)mdata[2],
                angle);
    return 0;
}




