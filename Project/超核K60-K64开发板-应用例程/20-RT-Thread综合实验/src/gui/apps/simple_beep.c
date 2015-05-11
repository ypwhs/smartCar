#include "drv_beep.h"
#include <stdint.h>
#include <rtthread.h>

char WangMingMei[]=
{
   "6_76_6_3_5_6_|63--|2_32_2_,6_121_2_,6--" 
   "1_23_3_,63_|2--3_5_6.7_6_7_6__7__3_5--" 
   "5_'1__6_'1--'7__'1__'7__'6__6--56_13_2_,6__1__,7_,6_,6_,5.,5_" 
   "1__,6_6__1.,63.2_3_2--3,753__2__,7_3_2__3__,5_1_,6.,6-"  
};

char MoLi[]={"33_5_6_`1_`1_6_|55_6_5-|33_5_6_`1_`1_6_|55_6_5-|"
                       "5553_5_|665-|32_3_53_2_|11_2_1|" };

char YuZuoDeYun[]={"366356_5_6-|6356_53|---,622,612_1_2-|2,612_1_,6---|366356_5_6"
                        };
                       
static int Tone[]=
{
    220, 246, 277, 293, 329, 370, 415,
    440, 493, 554, 587, 659, 739, 830,
    880, 987, 1108, 1174, 1318, 1479, 1661
};
    
static void BeepMusic(uint8_t tone_base, uint8_t time_base, char* music, uint32_t len)
{
    rt_device_t dev_beep;
    dev_beep = rt_device_find("beep");
    if(!dev_beep)
    {
        return ;
    }
    
    static int tone_level;
    tone_level = tone_base;
    int time;
    while(len--)
    {
        time = time_base;
        switch(*(music))
        {
            case ',':
                tone_level-=7;
                break;
            case '`':
                tone_level+=7;
                break;
        }
        if(*music >= '0' && *music <='7')
        {
            switch(*(music+1))
            {
                case '|':
                case ' ':
                    break;
                case '-':
                    time+=time_base;
                    if(*(music+2) == '-') time+=time_base;
                    break;
                case '.':
                    time*=1.5;
                    break;
                case '_':
                    time/=2;
                    if(*(music+2) == '_') time/=2;
                    break;
            }
            //rt_kprintf("%d %d %d \r\n", time , tone_level+(*music-'0'), (*music-'0'));
            rt_device_control(dev_beep, RT_DEVICE_CTRL_SET_BEEP_FRQ, &Tone[tone_level+(*music-'0')]);
            rt_device_control(dev_beep, RT_DEVICE_CTRL_BEEP_START, RT_NULL);
            rt_thread_delay(time);
            tone_level = tone_base;
            //rt_device_control(dev_beep, RT_DEVICE_CTRL_BEEP_STOP, RT_NULL);
        }
        music++;
    }
    rt_device_control(dev_beep, RT_DEVICE_CTRL_BEEP_STOP, RT_NULL);
}

#include <finsh.h>

void beep_music(int argc, char** argv)
{
    rt_device_t dev_beep;
    int tone, time;
    tone = strtoul(argv[1],0,0);
    if(tone >14) tone = 14;
    if((time < 1) || (time > 100)) time = 50;
    time = strtoul(argv[2],0,0);
    BeepMusic(tone, time, WangMingMei, sizeof(WangMingMei));
}

MSH_CMD_EXPORT(beep_music, beep_music 9 50)
