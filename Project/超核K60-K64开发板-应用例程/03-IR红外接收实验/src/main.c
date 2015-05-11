#include "gpio.h"
#include "common.h"
#include "uart.h"
#include "dma.h"
/* CH Kinetis固件库 V2.50 版本 */
/* 修改主频 请修改 CMSIS标准文件 system_MKxxxx.c 中的 CLOCK_SETUP 宏 */
 
/*
     实验名称：IR红外接收实验
     实验平台：渡鸦开发板
     板载芯片：MK60DN512ZVQ10
 实验效果：使用中断方式进行红外解码，通过串口将数据发送出去  
*/
/* 红外引脚定义PTE28 */
#define IR_PORT  HW_GPIOE
#define IR_PIN   (28)
#define IR_DATA  PEin(IR_PIN)

/* 红外解码数据 */
static uint8_t InfraredExec(uint8_t * code)
{
	uint8_t i,j;
	uint32_t time_out = 0;
	while((IR_DATA == 0) && (time_out < 30*10))
	{
		time_out++;
		DelayUs(100);
	}
	if(time_out > 92 || time_out < 87) return 1;
	time_out = 0;
	while((IR_DATA == 1) && (time_out < 30*10))
	{
		time_out++;
		DelayUs(100);
	}
	if((time_out > 47) || (time_out < 43)) return 2;

	//开始解码
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 8; j++)
		{
			while(IR_DATA == 0); //浪费掉低电平时间
			time_out = 0;
			while(IR_DATA == 1)  //读取高电平时间
			{
				DelayUs(100);
				time_out++;
			}
			code[i] = code[i]>>1;
			if(time_out > 9) code[i] |= 0x80;	 //1.68为高电平
			
		}
	}
	return 0;
}

/* GPIO中断回调函数 */
void GPIO_ISR(uint32_t pinArray)
{
    /* 关闭中断 */
    GPIO_ITDMAConfig(IR_PORT, IR_PIN, kGPIO_IT_FallingEdge, false);
    uint8_t code[4];
    if(!InfraredExec(code))
    {
        /* 接收成功 打印ID 蜂鸣器也响一下 */
        printf("infrared:0x%02X 0x%02X 0x%02X 0x%02X\r\n", code[0], code[1], code[2], code[3]);
        GPIO_WriteBit(HW_GPIOA, 6, 1);
        DelayMs(100);
        GPIO_WriteBit(HW_GPIOA, 6, 0);
    }
    GPIO_ITDMAConfig(IR_PORT, IR_PIN, kGPIO_IT_FallingEdge, true);
}

int main(void)
{
    DelayInit();
    GPIO_QuickInit(HW_GPIOE, 6, kGPIO_Mode_OPP);
    UART_QuickInit(UART0_RX_PD06_TX_PD07, 115200);
    /* 初始化蜂鸣器引脚 */
    GPIO_QuickInit(HW_GPIOA, 6, kGPIO_Mode_OPP);
    
    printf("Infrared test please press remote key...\r\n");
    
    /* 配置 IR引脚为中断输入 */
    GPIO_QuickInit(IR_PORT, IR_PIN, kGPIO_Mode_IPU);
    /* 设置中断回调 以及开始中断 */
    GPIO_CallbackInstall(IR_PORT, GPIO_ISR);
    GPIO_ITDMAConfig(IR_PORT, IR_PIN, kGPIO_IT_FallingEdge, true);
    
    while(1)
    {
        GPIO_ToggleBit(HW_GPIOE, 6);
        DelayMs(500);
    }
}


