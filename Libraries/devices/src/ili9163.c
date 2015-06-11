/**
  ******************************************************************************
  * @file    ili9163.c
  * @author  YANDLD
  * @version V2.5
  * @date    2013.12.25
  * @brief   www.beyondcore.net   http://upcmcu.taobao.com 
  * @note    此文件为NRF24L01无线模块的驱动，支持台产和挪威产芯片
  ******************************************************************************
  */



#include "ili9163.h"
#include "spi.h"

#define ILI9163_DEBUG		0
#if ( ILI9163_DEBUG == 1 )
#define ILI9163_TRACE	printf
#else
#define ILI9163_TRACE(...)
#endif


#define LCD_X_MAX   (128)
#define LCD_Y_MAX   (160)
#define RST_PORT     HW_GPIOE
#define RS_PORT      HW_GPIOE
#define RST_PIN      (0)
#define RS_PIN       (5)
#define LCD_RST_LOW()       do {GPIO_WriteBit(RST_PORT, RST_PIN, 0);} while(0)
#define LCD_RST_HIGH()      do {GPIO_WriteBit(RST_PORT, RST_PIN, 1);} while(0)
#define LCD_RS_LOW()        do {GPIO_WriteBit(RS_PORT, RS_PIN, 0);} while(0)
#define LCD_RS_HIGH()       do {GPIO_WriteBit(RS_PORT, RS_PIN, 1);} while(0)





static void LCD_COM(uint8_t cmd)
{
    LCD_RS_LOW();
   // SPI_
   // SPI_ReadWriteByte(uint32_t instance,uint32_t ctar, uint16_t data, uint16_t CSn, SPI_PCS_Type csState);
   // spi_write(&device, &cmd, 1, true);
}

static inline void LCD_DATA(uint8_t data)
{
    LCD_RS_HIGH();
 //   spi_write(&device, &data, 1, true);
}


void ili9163_set_window(int x, int y, int xlen, int ylen)
{
    xlen--;
    ylen--;
    LCD_COM(0x2A);
    LCD_DATA(x>>8);
    LCD_DATA(x);
    LCD_DATA((x+xlen)>>8);
    LCD_DATA((x+xlen));
  
    LCD_COM(0x2B);
    LCD_DATA(y>>8);
    LCD_DATA(y);
    LCD_DATA((y+ylen)>>8);
    LCD_DATA((y+ylen));
}

void ili9163_clear(uint16_t c)
{
	uint32_t i=0;      
	ili9163_set_window(0,0,LCD_X_MAX,LCD_Y_MAX);	 
	LCD_COM(0x2C);     //开始写入GRAM	 	  
	for(i = 0 ;i < LCD_X_MAX*LCD_Y_MAX; i++)
	{
        LCD_DATA(c>>8);
        LCD_DATA(c); 
	}
}  

void ili9163_write_pixel(int x, int y, int c)
{
    ili9163_set_window(x,y,1,1);//设置光标位置 
	LCD_COM(0x2C);
    LCD_DATA(c>>8);
    LCD_DATA(c);
}


int ili9163_init(spi_bus_t bus, uint32_t cs)
{
    uint32_t ret;
    device.csn = cs;
    device.config.baudrate = 50*1000*1000;
    device.config.data_width = 8;
    device.config.mode = SPI_MODE_0 | SPI_MASTER | SPI_MSB;
    ret = spi_bus_attach_device(bus, &device);
    if(ret)
    {
        return ret;
    }
    else
    {
        ret = spi_config(&device);
    }
    GPIO_QuickInit(RST_PORT, RST_PIN, kGPIO_Mode_OPP);
    GPIO_QuickInit(RS_PORT, RS_PIN, kGPIO_Mode_OPP);
	LCD_RST_LOW();    
    DelayMs(1);
	LCD_RST_HIGH();    
	DelayMs(1);
	LCD_COM(0x11);       		  	//关闭睡眠，振荡器工作
	DelayMs(1);
	
	LCD_COM(0x3a);       		  	//每次传送16位数据(VIPF3-0=0101)，每个像素16位(IFPF2-0=101)
	LCD_DATA(0x55);						
 
	LCD_COM(0x26);       		  	
	LCD_DATA(0x04);

	LCD_COM(0xf2);              		//Driver Output Control(1)
	LCD_DATA(0x01);
	
	LCD_COM(0xe0);              		//Driver Output Control(1)
	LCD_DATA(0x3f);
	LCD_DATA(0x25);
	LCD_DATA(0x1c);
	LCD_DATA(0x1e);
	LCD_DATA(0x20);
	LCD_DATA(0x12);
	LCD_DATA(0x2a);
	LCD_DATA(0x90);
	LCD_DATA(0x24);
	LCD_DATA(0x11);
	LCD_DATA(0x00);
	LCD_DATA(0x00);
	LCD_DATA(0x00);
	LCD_DATA(0x00);
	LCD_DATA(0x00);
	 
	LCD_COM(0xe1);              //Driver Output Control(1)
	LCD_DATA(0x20);
	LCD_DATA(0x20);
	LCD_DATA(0x20);
	LCD_DATA(0x20);
	LCD_DATA(0x05);
	LCD_DATA(0x00);
	LCD_DATA(0x15);
	LCD_DATA(0xa7);
	LCD_DATA(0x3d);
	LCD_DATA(0x18);
	LCD_DATA(0x25);
	LCD_DATA(0x2a);
	LCD_DATA(0x2b);
	LCD_DATA(0x2b);  
	LCD_DATA(0x3a);  
	
	LCD_COM(0xb1);              	//设置屏幕刷新频率
	LCD_DATA(0x08);				   	//DIVA=8
	LCD_DATA(0x08);				   	//VPA =8，约90Hz
						 
	LCD_COM(0xb4);              	//LCD Driveing control
	LCD_DATA(0x07);				  	//NLA=1,NLB=1,NLC=1
 
 
	LCD_COM(0xc0);              //LCD Driveing control
	LCD_DATA(0x0a);
	LCD_DATA(0x02);
		
	LCD_COM(0xc1);              //LCD Driveing control
	LCD_DATA(0x02);

	LCD_COM(0xc5);              //LCD Driveing control
	LCD_DATA(0x4f);
	LCD_DATA(0x5a);

	LCD_COM(0xc7);              //LCD Driveing control
	LCD_DATA(0x40);
	
	LCD_COM(0x2a);              	//配置MCU可操作的LCD内部RAM横坐标起始、结束参数
	LCD_DATA(0x00);				   	//横坐标起始地址0x0000
	LCD_DATA(0x00);					
	LCD_DATA(0x00);				   	//横坐标结束地址0x007f(127)
	LCD_DATA(0x7f);
 
	LCD_COM(0x2b);              	//配置MCU可操作的LCD内部RAM纵坐标起始结束参数
	LCD_DATA(0x00);				   	//纵坐标起始地址0x0000
	LCD_DATA(0x00);
	LCD_DATA(0x00);				  	//纵坐标结束地址0x009f(159)
	LCD_DATA(0x9f);

	LCD_COM(0x36);              	//配置MPU和DDRAM对应关系
	LCD_DATA(0xc0);					//MX=1,MY=1

	LCD_COM(0xb7);              	//LCD Driveing control
	LCD_DATA(0x00);				   	//CRL=0
	 
	LCD_COM(0x29);   		  	//开启屏幕显示
	LCD_COM(0x2c);   			//设置为LCD接收数据/命令模式
	ili9163_clear(0x0000);
    return 0;
}

/* CHGUI函数接口 CHGUI只需要使用打点函数就可以实现基本的GUI操作 */
void GUI_DrawPixel(int x, int y)
{
    ili9163_set_window(x,y,1,1);//设置光标位置 
	LCD_COM(0x2C);
    LCD_DATA(0x5555 >> 8);
    LCD_DATA(0x5555 & 0xFF);
}

