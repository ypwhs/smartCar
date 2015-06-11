/**
  ******************************************************************************
  * @file    calibration.c
  * @author  YANDLD
  * @version V2.5
  * @date    2015.3.26
  * @brief   www.beyondcore.net   http://upcmcu.taobao.com 
  * @note    dymalic caliberation impletmentation
  ******************************************************************************
  */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "calibration.h"

#define M_MAX           (1200)
#define M_MIN           (-1200)
#define G_MAX           (900)
#define G_MIN           (-900)
#define CAL_MAGIC       (0x5ACB)

static struct dcal_t dcal;
static struct dcal_t inital_dcal;

static int is_gval_ok(int16_t data)
{
    if((data > G_MAX) || (data < G_MIN))
    {
        return 1;
    }
    return 0; 
}

static int is_mval_ok(int16_t data)
{
    if((data > M_MAX) || (data < M_MIN))
    {
        return 1;
    }
    return 0;
}
   
void dcal_print(struct dcal_t * dc)
{
    printf("calibrartion data:\r\n");

    printf("gyro offset:%d %d %d \r\n", dc->go[0], dc->go[1], dc->go[2]);
    printf("acce offset:%d %d %d \r\n", dc->ao[0], dc->ao[1], dc->ao[2]);
    printf("magn offset:%d %d %d \r\n", dc->mo[0], dc->mo[1], dc->mo[2]);
    printf("mag gain:%f %f %f \r\n",    dc->mg[0], dc->mg[1], dc->mg[2]);
    printf("mag max:%d %d %d \r\n",     dc->m_max[0], dc->m_max[1], dc->m_max[2]);
    printf("mag min:%d %d %d \r\n",     dc->m_min[0], dc->m_min[1], dc->m_min[2]);
}

static void reset_dcal_data(struct dcal_t *dc)
{
    int i;
    for(i=0;i<3;i++)
    {
        dc->m_min[i] = 0;
        dc->m_max[i] = 0;
        dc->mg[i] = 1.000;
        dc->mo[i] = 0;
    } 
}

void dcal_init(struct dcal_t *dc)
{
    int i;

    if((dc) && (dc->magic == CAL_MAGIC))
    {
        memcpy(&dcal, dc, sizeof(struct dcal_t));
        memcpy(&inital_dcal, dc, sizeof(struct dcal_t));
        printf("load dcal value!\r\n");
    }
    else
    {
        reset_dcal_data(dc);
        reset_dcal_data(&dcal);
        reset_dcal_data(&inital_dcal);
        printf("no initial dcal value\r\n");  
    }
}

void dcal_ginput(int16_t *gdata)
{
    int i;
    
    for(i=0;i<3;i++)
    {
        if(is_mval_ok(gdata[i]))
        {
         //   printf("data of out rangle!\r\n");
            return;
        }
    }
    
//    for(i=0;i<3;i++)
//    {
//        if(gdata[i] > 50)
//        {
//            dcal.go[i]++;
//        }
//        if(gdata[i] < -50)
//        {
//            dcal.go[i]--;
//        }
//    }
}

/* this function must be called every 100 ms */
void dcal_minput(int16_t *mdata)
{
    int i;
    static float last_gain;
    dcal.need_update = false;
    for(i=0;i<3;i++)
    {
        if(is_mval_ok(mdata[i]))
        {
            memcpy(&dcal, &inital_dcal, sizeof(struct dcal_t));
            return;
        }
        
        if(dcal.m_max[i] < mdata[i])
        {
            dcal.m_max[i] = mdata[i];
        }
            
        if(dcal.m_min[i] > mdata[i])
        {
            dcal.m_min[i] = mdata[i];
        }
        
        dcal.mo[i] = (dcal.m_max[i] + dcal.m_min[i])/2;
    }
    
    dcal.mg[0] = 1.000;
    dcal.mg[1] = (float)(dcal.m_max[1] - dcal.m_min[1])/(float)(dcal.m_max[0] - dcal.m_min[0]);
    dcal.mg[2]  = (float)(dcal.m_max[2] - dcal.m_min[2])/(float)(dcal.m_max[0] - dcal.m_min[0]);
        
    /* constant val */
    if(dcal.mg[1] != last_gain)
    {
        if((dcal.mg[1] < 1.1) && (dcal.mg[1] > 0.9) && (dcal.mg[2] < 1.1) && (dcal.mg[2] > 0.9) && (dcal.mg[1] != last_gain))
        {
            dcal.need_update = true;
        }
        last_gain = dcal.mg[1];
    }
}

void dcal_output(struct dcal_t *dc)
{
    dcal.magic = CAL_MAGIC;
    memcpy(dc, &dcal, sizeof(struct dcal_t));
}

