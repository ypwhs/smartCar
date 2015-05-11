#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

#include <stdint.h>



typedef __packed struct
{
    int16_t trans_accel[3];
    int16_t trans_gyro[3];
    int16_t trans_mag[3];
    int16_t trans_roll;
    int16_t trans_pitch;
    int16_t trans_yaw;
    int32_t trans_pressure;
}transmit_user_data;


//!< API
uint32_t user_data2buffer(transmit_user_data* data, uint8_t* buf);

#endif


