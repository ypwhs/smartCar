/**
  ******************************************************************************
  * @file    mpu6050.h
  * @author  YANDLD
  * @version V2.5
  * @date    2013.12.25
  * @brief   www.beyondcore.net   http://upcmcu.taobao.com 
  ******************************************************************************
  */
#ifndef __MPU6050_H__
#define __MPU6050_H__

#include <stdint.h>
#include <stdbool.h>

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x)	(sizeof(x) / sizeof((x)[0]))
#endif

enum accel_scale
{
    AFS_2G,
    AFS_4G,
    AFS_8G,
    AFS_16G
};

enum gyro_scale
{
    GFS_250DPS,
    GFS_500DPS,
    GFS_1000DPS,
    GFS_2000DPS
};

struct mpu_config
{
    enum accel_scale        afs;
    enum gyro_scale         gfs;
    bool                    aenable_self_test;
    bool                    genable_self_test;
    bool                    gbypass_blpf;
};


//!< API function
int mpu6050_init(uint32_t instance);
int mpu6050_read_accel(int16_t* adata);
int mpu6050_read_gyro(int16_t *gdata);
int mpu6050_config(struct mpu_config *config);


#endif
