#include "ili9325.h"
#include "gpio.h"
#include "flexbus.h"




#define ILI9325_DEBUG		0
#if ( ILI9325_DEBUG == 1 )
#define ILI9325_TRACE	printf
#else
#define ILI9325_TRACE(...)
#endif



static void write_reg(uint16_t addr, uint16_t val)
{
    WMLCDCOM(addr);
    WMLCDDATA(val);
}

static uint16_t read_reg(uint16_t addr)
{
    WMLCDCOM(addr);
    return ILI9325_DATA_ADDRESS;
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

void ili9325_write_pixel(int x, int y, int c)
{
    write_reg(0x0020, x);
    write_reg(0x0021, y);
    WMLCDCOM(0x0022);
    WMLCDDATA(c);
}


int ili9325_read_pixel(int x, int y)
{
    int value;
    write_reg(0x0020, x);
    write_reg(0x0021, y);
    WMLCDCOM(0x0022);
    value = ILI9325_DATA_ADDRESS;
    WMLCDCOM(0x0022);
    value = ILI9325_DATA_ADDRESS;
    return value;
}


uint16_t ili9325_get_id(void)
{
    return read_reg(0x00);
}


void ili9325_set_window(int x, int y, int xlen, int ylen)
{
    write_reg(0x0020, x);
    write_reg(0x0021, y);
    write_reg(0x0050, x);
    write_reg(0x0052, y);
    write_reg(0x0051, x + xlen - 1);
    write_reg(0x0053, y + ylen - 1);                      
} 



void ili9325_clear(int c)
{
	int i;
    ili9325_set_window(0, 0, LCD_X_MAX, LCD_Y_MAX);
	WMLCDCOM(0x22);
	for(i = 0; i < (LCD_X_MAX * LCD_Y_MAX); i++)
	{
		WMLCDDATA(c);	   
	}
}


void ili9325_init(void)
{
    uint16_t id;
    uint32_t gpio_instance;
    /* 减低flexbus总线速度 总线速度太高 不能正确执行读点操作 */
    /* Flexbus Init */
    SIM->SCGC5 |= (SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK);
    /*control signals */
    PORTB->PCR[19] = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;          // FB_OE
    PORTD->PCR[0]  = PORT_PCR_MUX(5)|PORT_PCR_DSE_MASK;          // CS1
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
    FLEXBUS_InitStruct.CSn = kFLEXBUS_CS1;
    FLEXBUS_InitStruct.dataAlignMode = kFLEXBUS_DataLeftAligned;
    FLEXBUS_InitStruct.dataWidth = kFLEXBUS_PortSize_16Bit;
    FLEXBUS_InitStruct.baseAddress = ILI9325_BASE;
    FLEXBUS_InitStruct.ByteEnableMode = kFLEXBUS_BE_AssertedWrite;
    FLEXBUS_InitStruct.div = 4;
    FLEXBUS_Init(&FLEXBUS_InitStruct);
    /* 配置Flexbus 引脚复用 */
    FLEXBUS_PortMuxConfig(kFLEXBUS_CSPMCR_Group3, kFLEXBUS_CSPMCR_GROUP3_BE_23_16);
    /* Back light */
    gpio_instance = GPIO_QuickInit(HW_GPIOC, 1, kGPIO_Mode_OPP);
    GPIO_WriteBit(gpio_instance, 1, 1); 
    /* reset */
    gpio_instance = GPIO_QuickInit(HW_GPIOC, 18, kGPIO_Mode_OPP);
    GPIO_WriteBit(gpio_instance, 18, 0); 
    DelayMs(5);
    GPIO_WriteBit(gpio_instance, 18, 1);
    DelayMs(5); 
    
//************* Start Initial Sequence **********// 
    write_reg(0x0001, 0x0100);     // set SS and SM bit 
    write_reg(0x0002, 0x0700);     // set 1 line inversion 
    write_reg(0x0003, 0x1030);     // set GRAM write direction and BGR=1.  
    write_reg(0x0004, 0x0000);     // Resize register 
    write_reg(0x0008, 0x0202);        // set the back porch and front porch 
    write_reg(0x0009, 0x0000);     // set non-display area refresh cycle ISC[3:0] 
    write_reg(0x000A, 0x0000);     // FMARK function 
    write_reg(0x000C, 0x0000);        // RGB interface setting
    write_reg(0x000D, 0x0000);     // Frame marker Position 
    write_reg(0x000F, 0x0000);         // RGB interface polarity 
    //*************Power On sequence ****************// 
    write_reg(0x0010, 0x0000);      // SAP, BT[3:0], AP, DSTB, SLP, STB 
    write_reg(0x0011, 0x0007);      // DC1[2:0], DC0[2:0], VC[2:0] 
    write_reg(0x0012, 0x0000);      // VREG1OUT voltage 
    write_reg(0x0013, 0x0000);      // VDV[4:0] for VCOM amplitude 
    write_reg(0x0007, 0x0001);  
    DelayMs(20);                              // Dis-charge capacitor power voltage 
    write_reg(0x0010, 0x1690);      // SAP, BT[3:0], AP, DSTB, SLP, STB 
    write_reg(0x0011, 0x0227);      // DC1[2:0], DC0[2:0], VC[2:0] 
    DelayMs(5);                       // Delay 50ms   
    write_reg(0x0012, 0x009D);      // Internal reference voltage= Vci; 
    DelayMs(5);                       // Delay 50ms 
    write_reg(0x0013, 0x1900);      // Set VDV[4:0] for VCOM amplitude 
    write_reg(0x0029, 0x0025);      // Set VCM[5:0] for VCOMH 
    write_reg(0x002B, 0x000D);      // Set Frame Rate  
    DelayMs(5);                               // Delay 50ms 
    write_reg(0x0020, 0x0000);      // GRAM horizontal Address 
    write_reg(0x0021, 0x0000);      // GRAM Vertical Address
    // ----------- Adjust the Gamma    Curve ----------// 
    write_reg(0x0030, 0x0007); 
    write_reg(0x0031, 0x0303); 
    write_reg(0x0032, 0x0003); 
    write_reg(0x0035, 0x0206); 
    write_reg(0x0036, 0x0008); 
    write_reg(0x0037, 0x0406); 
    write_reg(0x0038, 0x0304); 
    write_reg(0x0039, 0x0007); 
    write_reg(0x003C, 0x0602); 
    write_reg(0x003D, 0x0008); 
    //------------------ Set GRAM area ---------------// 
    write_reg(0x0050, 0x0000);      // Horizontal GRAM Start Address 
    write_reg(0x0051, 0x00EF);      // Horizontal GRAM End Address 
    write_reg(0x0052, 0x0000);      // Vertical GRAM Start Address 
    write_reg(0x0053, 0x013F);      // Vertical GRAM Start Address 
    write_reg(0x0060, 0xA700);      // Gate Scan Line 
    write_reg(0x0061, 0x0001);      // NDL,VLE, REV
    write_reg(0x006A, 0x0000);      // set scrolling line 
    //-------------- Partial Display Control ---------// 
    write_reg(0x0080, 0x0000); 
    write_reg(0x0081, 0x0000); 
    write_reg(0x0082, 0x0000); 
    write_reg(0x0083, 0x0000); 
    write_reg(0x0084, 0x0000); 
    write_reg(0x0085, 0x0000); 
    //-------------- Panel Control -------------------// 
    write_reg(0x0090, 0x0010); 
    write_reg(0x0092, 0x0600); 
       
    write_reg(0x0007, 0x0133);      // 262K color and display ON 
 
    
    //开启显示 
    ILI9325_TRACE("ID:0x%X\r\n", ili9325_get_id());
    ili9325_clear(RED);
}

