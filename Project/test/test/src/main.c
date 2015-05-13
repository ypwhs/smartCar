#include "gpio.h"
#include "common.h"
#include "uart.h"
#include "spi.h"

void Write_SPI_Command(unsigned char data){
    PEout(4) = 0;
    SPI_ReadWriteByte(HW_SPI1, HW_CTAR0, data, HW_SPI_CS0, kSPI_PCS_ReturnInactive);
}
void Write_SPI_Data(unsigned char data){
    PEout(4) = 1;
    SPI_ReadWriteByte(HW_SPI1, HW_CTAR0, data, HW_SPI_CS0, kSPI_PCS_ReturnInactive);
}
void fill_picture(unsigned char fill_Data)
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		Write_SPI_Command(0xb0+m);		//page0-page1
		Write_SPI_Command(0x00);		//low column start address
		Write_SPI_Command(0x10);		//high column start address
		for(n=0;n<132;n++)
			{
				Write_SPI_Data(fill_Data);
			}
	}
}
void initOLED(){
    SPI_QuickInit(SPI1_SCK_PE02_SOUT_PE01_SIN_PE03, kSPI_CPOL0_CPHA0, 8*1000*1000);
    //PORT_PinMuxConfig(HW_GPIOC, 3, kPinAlt2);
	Write_SPI_Command(0xAE);   //display off
	Write_SPI_Command(0x20);	//Set Memory Addressing Mode
	Write_SPI_Command(0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	Write_SPI_Command(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
	Write_SPI_Command(0xc8);	//Set COM Output Scan Direction
	Write_SPI_Command(0x00);//---set low column address
	Write_SPI_Command(0x10);//---set high column address
	Write_SPI_Command(0x40);//--set start line address
	Write_SPI_Command(0x81);//--set contrast control register
	Write_SPI_Command(0x7f);
	Write_SPI_Command(0xa1);//--set segment re-map 0 to 127
	Write_SPI_Command(0xa6);//--set normal display
	Write_SPI_Command(0xa8);//--set multiplex ratio(1 to 64)
	Write_SPI_Command(0x3F);//
	Write_SPI_Command(0xa4);//0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	Write_SPI_Command(0xd3);//-set display offset
	Write_SPI_Command(0x00);//-not offset
	Write_SPI_Command(0xd5);//--set display clock divide ratio/oscillator frequency
	Write_SPI_Command(0xf0);//--set divide ratio
	Write_SPI_Command(0xd9);//--set pre-charge period
	Write_SPI_Command(0x22); //
	Write_SPI_Command(0xda);//--set com pins hardware configuration
	Write_SPI_Command(0x12);
	Write_SPI_Command(0xdb);//--set vcomh
	Write_SPI_Command(0x20);//0x20,0.77xVcc
	Write_SPI_Command(0x8d);//--set DC-DC enable
	Write_SPI_Command(0x14);//
	Write_SPI_Command(0xaf);//--turn on oled panel
}

int main(void)
{
    DelayInit();
    /* 打印串口及小灯 */
    GPIO_QuickInit(HW_GPIOC, 3, kGPIO_Mode_OPP);
    GPIO_QuickInit(HW_GPIOE, 4, kGPIO_Mode_OPP);
    GPIO_QuickInit(HW_GPIOE, 5, kGPIO_Mode_OPP);
    PEout(5) = 0;
    DelayMs(1);
    PEout(5) = 1;
    UART_QuickInit(UART0_RX_PB16_TX_PB17, 115200);

    printf("OLED test\r\n");

    initOLED();
    DelayMs(1);
    while(1)
    {
        GPIO_ToggleBit(HW_GPIOC, 3);
      fill_picture(0xFF);
      DelayMs(500);
      fill_picture(0x00);
      DelayMs(500);
    }
}
