#include "ili9320.h"
#include "gpio.h"
#include "flexbus.h"


#define ILI9320_DEBUG		1
#if ( ILI9320_DEBUG == 1 )
#define ILI9320_TRACE	printf
#else
#define ILI9320_TRACE(...)
#endif
uint32_t lcd_id;
static void write_reg(uint16_t addr, uint16_t val)
{
    WMLCDCOM(addr);
    WMLCDDATA(val);
}

static uint16_t read_reg(uint16_t addr)
{
    WMLCDCOM(addr);
    return ILI9320_DATA_ADDRESS;
}

uint16_t ili9320_get_id(void)
{
    return read_reg(0x00);
}

static uint16_t LCD_BGR2RGB(uint16_t c)
{
	uint16_t  r,g,b,rgb;   
	b=(c>>0)&0x1f;
	g=(c>>5)&0x3f;
	r=(c>>11)&0x1f;	 
	rgb=(b<<11)+(g<<5)+(r<<0);		 
	return(rgb);
} 


int ili9320_read_pixel(int x, int y)
{
    int value;
    write_reg(0x0020, x);
    write_reg(0x0021, y);
    WMLCDCOM(0x0022);
    value = ILI9320_DATA_ADDRESS;
    WMLCDCOM(0x0022);
    value = ILI9320_DATA_ADDRESS;
    return LCD_BGR2RGB(value);
}

void ili9320_clear(int c)
{
	int i;
    ili9320_set_window(0, 0, LCD_X_MAX, LCD_Y_MAX);
	WMLCDCOM(0x22);
	for(i = 0; i < (LCD_X_MAX * LCD_Y_MAX); i++)
	{
		WMLCDDATA(c);	   
	}
}

void ili9320_write_gram(uint16_t *buf, int len)
{
    WMLCDCOM(0x0022);
    while(len--)
    {
        WMLCDDATA(*buf++);
    }
}
void ili9320_write_pixel(int x, int y, int c)
{
    switch(lcd_id)
    {
        case 0x9320:
            write_reg(0x0020, x);
            write_reg(0x0021, y);
            WMLCDCOM(0x0022);
            WMLCDDATA(c);
            break;
        case 0x8989:
            write_reg(0x004e,x);        
            write_reg(0x004f,y);  
            WMLCDCOM(0x0022);
            WMLCDDATA(c);
            break;
        default:
            break;   
    }
  
}

void ili9320_set_window(int x, int y, int xlen, int ylen)
{

    switch(lcd_id)
    {
        case 0x9320:
            write_reg(0x0020, x);
            write_reg(0x0021, y);
            write_reg(0x0050, x);
            write_reg(0x0052, y);
            write_reg(0x0051, x + xlen - 1);
            write_reg(0x0053, y + ylen - 1);  
            break;        
        case 0x8989:
            write_reg(0x004e,x);       
            write_reg(0x004f,y);  
            write_reg(0x0044, (x|((x + xlen - 1)<<8)));
            write_reg(0x0045, y);
            write_reg(0x0046, y + ylen - 1);   
            break;
        default:
            break;   
    }    
} 

void ili9320_hline(int xs, int xe, int y, int c)
{
    switch(lcd_id)
    {
        case 0x9320:
            write_reg(0x03,(1<<5)|(1<<4)|(0<<3)|(1<<12));
            write_reg(0x0020, xs);
            write_reg(0x0021, y);
            WMLCDCOM(0x0022);
            while(xs < xe)
            {
                WMLCDDATA(c);
                xs++;
            }
          //  write_reg(0x03,(1<<5)|(1<<4)|(0<<3)|(1<<12));
            break;
        case 0x8989:
            write_reg(0x11,(1<<5)|(1<<4)|(0<<3));
            write_reg(0x004e, xs);
            write_reg(0x004f, y);
            WMLCDCOM(0x0022);
            while(xs < xe)
            {
            WMLCDDATA(c);
            xs++;
            }
            write_reg(0x11,(1<<5)|(1<<4)|(0<<3));
            break;
        default:
            break;   
    }
   
}


void ili9320_vline(int ys, int ye, int x, int c)
{
    
    switch(lcd_id)
    {
        case 0x9320:
            write_reg(0x03,(0<<5)|(0<<4)|(1<<3)|(1<<12));
            write_reg(0x0020, x);
            write_reg(0x0021, ys);
            WMLCDCOM(0x0022);
            while(ys < ye)
            {
                WMLCDDATA(c);
                ys++;
            }
           // write_reg(0x03,(1<<5)|(1<<4)|(0<<3)|(1<<12));
            break;
        case 0x8989:
            write_reg(0x11,(0<<5)|(0<<4)|(1<<3));
            write_reg(0x004e, x);
            write_reg(0x004f, ys);
            WMLCDCOM(0x0022);
            while(ys < ye)
            {
            WMLCDDATA(c);
            ys++;
            }
          // write_reg(0x11,(1<<5)|(1<<4)|(0<<3));
            break;
        default:
            break;   
    }
}

void ili9320_init(void)
{
   
    uint32_t gpio_instance;
    /* ??flexbus???? ?????? ?????????? */
    /* Flexbus Init */
    SIM->SCGC5 |= (SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK);
    /*control signals */
    PORTB->PCR[19] = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;          // FB_OE
    PORTD->PCR[1]  = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;          // CS0
    PORTA->PCR[26] = PORT_PCR_MUX(6)|PORT_PCR_DSE_MASK;          // A27
    PORTC->PCR[16] = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;          // FB_BE_23_16
    
    /*
    PORTB->PCR[18] = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           //  FB_AD15
    PORTC->PCR[0]  = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           //  FB_AD14
    PORTC->PCR[1]  = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           //  FB_AD13
    PORTC->PCR[2]  = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           //  FB_AD12
    PORTC->PCR[4]  = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           //  FB_AD11
    PORTC->PCR[5]  = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           //  FB_AD10
    PORTC->PCR[6]  = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           //  FB_AD9
    PORTC->PCR[7]  = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           //  FB_AD8
    PORTC->PCR[8]  = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           //  FB_AD7
    PORTC->PCR[9]  = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           //  FB_AD6
    PORTC->PCR[10] = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           //  FB_AD5
    PORTD->PCR[2]  = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           //  FB_AD4
    PORTD->PCR[3]  = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           //  FB_AD3
    PORTD->PCR[4]  = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           //  FB_AD2
    PORTD->PCR[5]  = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           //  FB_AD1
    PORTD->PCR[6]  = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           //  FB_AD0
    */
    
    PORTB->PCR[17]  = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           //  FB_AD16
    PORTB->PCR[16]  = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           //  FB_AD17
    PORTB->PCR[11]  = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           //  FB_AD18
    PORTB->PCR[10]  = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           //  FB_AD19
    PORTB->PCR[9]  = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           //  FB_AD20
    PORTB->PCR[8]  = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           //  FB_AD21
    PORTB->PCR[7]  = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           //  FB_AD22
    PORTB->PCR[6]  = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           //  FB_AD23
    PORTC->PCR[15]  = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           //  FB_AD24
    PORTC->PCR[14]  = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           //  FB_AD25
    PORTC->PCR[13]  = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           //  FB_AD26
    PORTC->PCR[12]  = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           //  FB_AD27
    PORTB->PCR[23]  = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           //  FB_AD28
    PORTB->PCR[22]  = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           //  FB_AD29
    PORTB->PCR[21]  = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           //  FB_AD30
    PORTB->PCR[20]  = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;           //  FB_AD31 
    
    FLEXBUS_InitTypeDef FLEXBUS_InitStruct;
    FLEXBUS_InitStruct.ADSpaceMask = 0x800;
    FLEXBUS_InitStruct.autoAckMode = kFLEXBUS_AutoAckEnable;
    FLEXBUS_InitStruct.CSn = kFLEXBUS_CS0;
    FLEXBUS_InitStruct.dataAlignMode = kFLEXBUS_DataLeftAligned;
    FLEXBUS_InitStruct.dataWidth = kFLEXBUS_PortSize_16Bit;
    FLEXBUS_InitStruct.baseAddress = ILI9320_BASE;
    FLEXBUS_InitStruct.ByteEnableMode = kFLEXBUS_BE_AssertedWrite;
    FLEXBUS_InitStruct.div = 2;
    FLEXBUS_Init(&FLEXBUS_InitStruct);

    FLEXBUS_PortMuxConfig(kFLEXBUS_CSPMCR_Group3, kFLEXBUS_CSPMCR_GROUP3_BE_23_16);
    
    /* advanced config */
    FLEXBUS_AdvancedConfigTypeDef config;
    config.kFLEXBUS_brustWriteEnable = false;
    config.kFLEXBUS_brustReadEnable = false;
    config.kFLEXBUS_EXTS = true;
    config.kFLEXBUS_ASET = 1;
    config.kFLEXBUS_RDAH = 1;
    config.kFLEXBUS_WRAH = 1;
    config.kFLEXBUS_WS = 6;
    FLEXBUS_AdvancedConfig(FLEXBUS_InitStruct.CSn, &config);
    
    /* Back light */
    gpio_instance = GPIO_QuickInit(HW_GPIOC, 3, kGPIO_Mode_OPP);
    GPIO_WriteBit(gpio_instance, 3, 1); 
    /* reset */
    gpio_instance = GPIO_QuickInit(HW_GPIOC, 19, kGPIO_Mode_OPP);
    GPIO_WriteBit(gpio_instance, 19, 0); 
    DelayMs(5);
    GPIO_WriteBit(gpio_instance, 19, 1);
    DelayMs(5);
    
    lcd_id = ili9320_get_id();
    
    switch(lcd_id)
    {
        case 0x9320:
            write_reg(0xe5,0x8000);  // Set the internal vcore voltage    
            write_reg(0x00,0x0001);  // start OSC    
            write_reg(0x2b,0x0010);  //Set the frame rate as 80 when the internal resistor is used for oscillator circuit    
            write_reg(0x01,0x0100);  //s720  to  s1 ; G1 to G320    
            write_reg(0x02,0x0700);  //set the line inversion    
            //LCD_WR_REG(0x03,0x1018);  //65536 colors     
            write_reg(0x03,0x1030);   
            //??
            #ifdef LCD_USE_HORIZONTAL
            write_reg(0x03,(0<<5)|(0<<4)|(1<<3)|(1<<12));
            #else
            write_reg(0x03,(1<<5)|(1<<4)|(0<<3)|(1<<12));
            #endif

            write_reg(0x04,0x0000);   
            write_reg(0x08,0x0202);  
            write_reg(0x09,0x0000);   
            write_reg(0x0a,0x0000);   
            write_reg(0x0c,0x0000);
            write_reg(0x0d,0x0000);   
            write_reg(0x0f,0x0000);    
            write_reg(0x50,0x0000);
            write_reg(0x51,0x00ef);   
            write_reg(0x52,0x0000);   
            write_reg(0x53,0x013f);   
            write_reg(0x60,0x2700);   
            write_reg(0x61,0x0001);   
            write_reg(0x6a,0x0000);   
            write_reg(0x80,0x0000);   
            write_reg(0x81,0x0000);   
            write_reg(0x82,0x0000);   
            write_reg(0x83,0x0000);   
            write_reg(0x84,0x0000);   
            write_reg(0x85,0x0000);   
            write_reg(0x90,0x0010);   
            write_reg(0x92,0x0000);   
            write_reg(0x93,0x0003);   
            write_reg(0x95,0x0110);   
            write_reg(0x97,0x0000);   
            write_reg(0x98,0x0000);    
         
            write_reg(0x10,0x0000);   
            write_reg(0x11,0x0000);   
            write_reg(0x12,0x0000);   
            write_reg(0x13,0x0000);   
            DelayMs(20);   
            write_reg(0x10,0x17b0);   
            write_reg(0x11,0x0004);   
            ;   
            write_reg(0x12,0x013e);   
            ;   
            write_reg(0x13,0x1f00);   
            write_reg(0x29,0x000f);   
            ;   
            write_reg(0x20,0x0000);   
            write_reg(0x21,0x0000);   
         
            write_reg(0x30,0x0204);   
            write_reg(0x31,0x0001);   
            write_reg(0x32,0x0000);   
            write_reg(0x35,0x0206);   
            write_reg(0x36,0x0600);   
            write_reg(0x37,0x0500);   
            write_reg(0x38,0x0505);   
            write_reg(0x39,0x0407);   
            write_reg(0x3c,0x0500);   
            write_reg(0x3d,0x0503); 
            write_reg(0x07,0x0173);            
            break;
        case 0x8989:
            write_reg(0x0000,0x0001);
            write_reg(0x0003,0xA8A4);
            write_reg(0x000C,0x0000);     
            write_reg(0x000D,0x080C);    
            write_reg(0x000E,0x2B00);     
            write_reg(0x001E,0x00B0);     
            write_reg(0x0001,0x2B3F);  
            write_reg(0x0002,0x0600);  
            write_reg(0x0010,0x0000);  
            write_reg(0x0011,0x6070);
            write_reg(0x0005,0x0000);  
            write_reg(0x0006,0x0000);  
            write_reg(0x0016,0xEF1C);  
            write_reg(0x0017,0x0003);  
            write_reg(0x0007,0x0233);
            write_reg(0x000B,0x0000);  
            write_reg(0x000F,0x0000);
            write_reg(0x0041,0x0000);  
            write_reg(0x0042,0x0000);  
            write_reg(0x0048,0x0000);  
            write_reg(0x0049,0x013F);  
            write_reg(0x004A,0x0000);  
            write_reg(0x004B,0x0000);  
            write_reg(0x0044,0xEF00);  
            write_reg(0x0045,0x0000);  
            write_reg(0x0046,0x013F);  
            write_reg(0x0030,0x0707);  
            write_reg(0x0031,0x0204);  
            write_reg(0x0032,0x0204);  
            write_reg(0x0033,0x0502);  
            write_reg(0x0034,0x0507);  
            write_reg(0x0035,0x0204);  
            write_reg(0x0036,0x0204);  
            write_reg(0x0037,0x0502);  
            write_reg(0x003A,0x0302);  
            write_reg(0x003B,0x0302);  
            write_reg(0x0023,0x0000);  
            write_reg(0x0024,0x0000);  
            write_reg(0x0025,0x8000);  
            write_reg(0x004f,0);
            write_reg(0x004e,0);

            break;
        default:
            break;
    }
    
    //???? 
   // ILI9320_TRACE("ID:0x%X\r\n", ili9320_get_id());
    
    ili9320_clear(BLACK);
}

void GUI_DrawPixel(int color, int x, int y)
{
    switch(lcd_id)
    {
        case 0x9320:
            write_reg(0x0020, x);
            write_reg(0x0021, y);
            WMLCDCOM(0x0022);
            WMLCDDATA(color);
            break;
        case 0x8989:
            write_reg(0x004e,x);        
            write_reg(0x004f,y);  
            WMLCDCOM(0x0022);
            WMLCDDATA(color);
            break;
        default:
            break;   
    }
    
}

void GUI_DriverInit(void)
{
    ili9320_init();
}
