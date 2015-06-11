#include "shell.h"
#include "ftm.h"
#include "board.h"
#include "common.h"

int DoFTM(int argc, char * const argv[])
{
    int pwm_freq;
    
    pwm_freq = 3000;
    QuickInit_Type type;
    QuickInitDecode(BOARD_FTM_MAP, &type);
    printf("FTM test freq:%d on P%d%d\r\n", pwm_freq, type.io_instance + 'A', type.io_offset);
    printf("FTM%dCH%d\r\n", type.ip_instance, type.channel);
    FTM_PWM_QuickInit(BOARD_FTM_MAP, kPWM_EdgeAligned, 3000);
    
    //void FTM_PWM_ChangeDuty(uint32_t instance, uint8_t chl, uint32_t pwmDuty);
    //void FTM_PWM_InvertPolarity(uint32_t instance, uint8_t chl, uint32_t config);

    return 0;
}

SHELL_EXPORT_CMD(DoFTM, ftm , ftm test)
    

