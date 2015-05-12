#include "gpio.h"
#include "common.h"
#include "uart.h"
#include "i2c.h"


void WriteCommand(unsigned char ins)
{
    I2C_Start();
    I2C_SendByte(0x00);I2C_WaitAck();
    I2C_SendByte(ins);I2C_WaitAck();
    I2C_Stop();
//    I2C_WriteSingleRegister(HW_I2C0, 0x78>>1, 0x00, ins);
}
void WriteData(unsigned char dat)
{
    I2C_Start();
    I2C_SendByte(0x40);I2C_WaitAck();
    I2C_SendByte(dat);I2C_WaitAck();
    I2C_Stop();
//    I2C_WriteSingleRegister(HW_I2C0, 0x78>>1, 0x40, dat);
}
void IIC_SetPos(unsigned char x, unsigned char y)
{
  WriteCommand(0xb0 + y);
  WriteCommand(((x & 0xf0) >> 4) | 0x10); //|0x10
  WriteCommand((x & 0x0f) | 0x01); //|0x01
}
//全屏显示 -- Fill_Screen(0x00,0x00)可用作清屏
void Fill_Screen(unsigned char dat1, unsigned char dat2)
{
  unsigned char x, y;

  WriteCommand(0x02);    /*set lower column address*/
  WriteCommand(0x10);    /*set higher column address*/
  WriteCommand(0xB0);    /*set page address*/
  for (y = 0; y < 8; y++)
  {
    WriteCommand(0xB0 + y);  /*set page address*/
    WriteCommand(0x02);    /*set lower column address*/
    WriteCommand(0x10);    /*set higher column address*/

    for (x = 0; x < 64; x++)
    {
      WriteData(dat1);
      WriteData(dat2);
    }
  }
}
void initOLED() {
    I2C_Scan(I2C0_SCL_PB00_SDA_PB01);
    uint32_t instance = I2C_QuickInit(I2C0_SCL_PB00_SDA_PB01, 100*1000);
    int r = I2C_Probe(instance, 0x78>>1);
    if(r)
        printf("OLED connect success\n");
    else
        printf("I2C init failed\n");

  WriteCommand(0xAE);    /*display off*/

  WriteCommand(0x02);    /*set lower column address*/
  WriteCommand(0x10);    /*set higher column address*/

  WriteCommand(0x40);    /*set display start line*/

  WriteCommand(0xB0);    /*set page address*/

  WriteCommand(0x81);    /*contract control*/
  WriteCommand(0x80);    /*128*/

  WriteCommand(0xA1);    /*set segment remap*/

  WriteCommand(0xA6);    /*normal / reverse*/

  WriteCommand(0xA8);    /*multiplex ratio*/
  WriteCommand(0x3F);    /*duty = 1/32*/

  WriteCommand(0xad);    /*set charge pump enable*/
  WriteCommand(0x8b);     /*    0x8a    外供VCC   */

  WriteCommand(0x30);    /*0X30---0X33  set VPP   9V 电压可以调整亮度!!!!*/

  WriteCommand(0xC8);    /*Com scan direction*/

  WriteCommand(0xD3);    /*set display offset*/
  WriteCommand(0x00);   /*   0x20  */

  WriteCommand(0xD5);    /*set osc division*/
  WriteCommand(0x80);

  WriteCommand(0xD9);    /*set pre-charge period*/
  WriteCommand(0x1f);    /*0x22*/

  WriteCommand(0xDA);    /*set COM pins*/
  WriteCommand(0x12);//0x02 -- duanhang xianshi,0x12 -- lianxuhang xianshi!!!!!!!!!

  WriteCommand(0xdb);    /*set vcomh*/
  WriteCommand(0x40);


  WriteCommand(0xAF);    /*display ON*/
}


int main(void)
{
    DelayInit();
    /* 打印串口及小灯 */
    GPIO_QuickInit(HW_GPIOC, 3, kGPIO_Mode_OPP);
    UART_QuickInit(UART0_RX_PB16_TX_PB17, 115200);

    printf("OLED test\r\n");

    initOLED();
    Fill_Screen(0x00, 0x00);

    while(1)
    {
        GPIO_ToggleBit(HW_GPIOC, 3);
      IIC_SetPos(0, 0);
      Fill_Screen(0xAA, 0x55);
      DelayMs(500);
      Fill_Screen(0x00, 0x00);
      DelayMs(500);
    }
}
