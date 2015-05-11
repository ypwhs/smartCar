#include "imu_rev.h"
#include <string.h>

static imu_rev_init_t g_hander;
static char rev_buf[64];

enum input_status
{
    STATUS_IDLE,
    STATUS_SOF,
    STATUS_LEN,
    STATUS_DATA,
};

void imu_rev_init(imu_rev_init_t installer)
{
    g_hander = installer;
}

int imu_rev_get_data(imu_data_t data)
{
    data->accl[0] = (rev_buf[0]<<8) + rev_buf[1];
    data->accl[1] = (rev_buf[2]<<8) + rev_buf[3];
    data->accl[2] = (rev_buf[4]<<8) + rev_buf[5];

    data->gyro[0] = (rev_buf[6]<<8) + rev_buf[7];
    data->gyro[1] = (rev_buf[8]<<8) + rev_buf[9];
    data->gyro[2] = (rev_buf[10]<<8) + rev_buf[11];
    
    data->mag[0] = (rev_buf[12]<<8) + rev_buf[13];
    data->mag[1] = (rev_buf[14]<<8) + rev_buf[15];
    data->mag[2] = (rev_buf[16]<<8) + rev_buf[17];
    
    data->roll = (rev_buf[18]<<8) + rev_buf[19];
    data->pitch = (rev_buf[20]<<8) + rev_buf[21];
    data->yaw = (rev_buf[22]<<8) + rev_buf[23];
    
    data->presure = (rev_buf[27]<<24) + (rev_buf[26]<<16) + (rev_buf[25]<<8) + (rev_buf[24]<<0);
    
    return 0;
}

void imu_rev_process(char ch, enum imu_rev_mode mode)
{
    static int len = 0;
    static int i;

    static enum input_status status = STATUS_IDLE;
    if(mode == IMU_REV_Polling)
    {
        /* get a char if using polling mode */
        if(g_hander->getc())
        {
            ch = g_hander->getc();
        }
    }
    
    /* running status machine */
    switch(status)
    {
        case STATUS_IDLE:
            if((uint8_t)ch == 0x88)
            {
                status = STATUS_SOF;
            }
            break;
        case STATUS_SOF:
            if((uint8_t)ch == 0xAF)
            {
                status = STATUS_LEN;
            }
            break;
        case STATUS_LEN:
            len = ch;
            status = STATUS_DATA;
            i = 0;
            break;
        case STATUS_DATA:
            
            if(i == len)
            {
                status = STATUS_IDLE;
                g_hander->handler();
                break;
            }
            rev_buf[i++] = ch;
            break;
        default:
            break;
    }
}


