#include "gpio.h"
#include "common.h"
#include "uart.h"
#include "ftm.h"
#include "lptmr.h"
#include "pit.h"
/* CH Kinetis固件库 V2.50 版本 */
/* 修改主频 请修改 CMSIS标准文件 system_MKxxxx.c 中的 CLOCK_SETUP 宏 */


/* LPTMR 作为脉冲计数时 只能测量一路脉冲 */

/*
     实验名称：LPTMR脉冲计数实验
     实验平台：渡鸦开发板
     板载芯片：MK60DN512ZVQ10
 实验效果：使用PTC1引脚产生1KHz的方波，使用LTPMR的PTC5引脚对脉冲进行计数
      使用PIT中断中读取LPTM模块的计数个数
      小灯周期性闪烁，闪烁时间间隔500ms  
   注：需要使用跳线将PTC1引脚和PTC5引脚连接起来      
*/
//PIT中断中读取脉冲计数的值
static void PIT_ISR(void)
{
    uint32_t value;
    value = LPTMR_PC_ReadCounter(); //获得LPTMR模块的计数值
    LPTMR_ClearCounter();  //计数器归零
    printf("LPTMR:%dHz\r\n", value);
}

int main(void)
{
    DelayInit();
    
    GPIO_QuickInit(HW_GPIOE, 6, kGPIO_Mode_OPP); /* LED */
    UART_QuickInit(UART0_RX_PD06_TX_PD07, 115200); /* 为了代码简单 这里直接调用快速初始化了 */
    
    printf("lptrm pulse counter test connect PC05&PC01\r\n");
    
    /* 在PC01 上产生一定频率的方波 便于脉冲计数测量 */
    FTM_PWM_QuickInit(FTM0_CH0_PC01, kPWM_EdgeAligned, 1000);
    /* 设置FTM 的占空比 */
    FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH0, 5000); // 50%占空比 0-10000 对应 0-100%
    
    /* 快速初始化 LPTMR模块用作脉冲计数功能 */
    LPTMR_PC_QuickInit(LPTMR_ALT2_PC05); /* 脉冲计数 */
    
    /* 开启一个PIT定时器 产生1S中断 在中断中读取LPTMR的计数值 */
    PIT_QuickInit(HW_PIT_CH0, 1000 * 1000);
    PIT_CallbackInstall(HW_PIT_CH0, PIT_ISR);
    PIT_ITDMAConfig(HW_PIT_CH0, kPIT_IT_TOF, true);
    
    while(1)
    {
        GPIO_ToggleBit(HW_GPIOE, 6);
        DelayMs(500);
    }
}


