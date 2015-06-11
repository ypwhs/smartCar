#include <stdio.h>

#include "common.h"
#include "lpuart.h"
#include "gpio.h"


#if (!defined(LPUART_BASES))
#ifdef      LPUART0
#define     LPUART_BASES    {LPUART0}
#elif       LPUART1
#define     LPUART_BASES    {LPUART0, LPUART1}
#endif
#endif

LPUART_Type * const LPUART_InstanceTable[] = LPUART_BASES;

static const struct reg_ops SIM_LPUARTClockGateTable[] =
{
#ifdef LPUART0
    {(void*)&(SIM->SCGC5), SIM_SCGC5_LPUART0_MASK},
#endif
#ifdef LPUART1
    {(void*)&(SIM->SCGC5), SIM_SCGC5_LPUART1_MASK}, 
#endif
};

/* special use for printf */
static uint8_t UART_DebugInstance;

uint32_t LPUART_QuickInit(uint32_t MAP, uint32_t baudrate)
{
    uint8_t i;
    LPUART_InitTypeDef LPUART_InitStruct1;
    QuickInit_Type * pq = (QuickInit_Type*)&(MAP);
    LPUART_InitStruct1.baudrate = baudrate;
    LPUART_InitStruct1.instance = pq->ip_instance;
    LPUART_InitStruct1.parityMode = kUART_ParityDisabled;
    LPUART_InitStruct1.bitPerChar = kUART_8BitsPerChar;
    LPUART_InitStruct1.srcClock = 48*1000*1000;
    
    /* clock config use IRC48M */
    switch(pq->ip_instance)
    {
        case HW_LPUART0:
            SIM->SOPT2 &= ~SIM_SOPT2_LPUART0SRC_MASK;
            SIM->SOPT2 |= SIM_SOPT2_LPUART0SRC(1);
            break;
        case HW_LPUART1:
            SIM->SOPT2 &= ~SIM_SOPT2_LPUART1SRC_MASK;
            SIM->SOPT2 |= SIM_SOPT2_LPUART1SRC(1);
            break; 
    }
    
    for(i = 0; i < pq->io_offset; i++)
    {
        PORT_PinMuxConfig(pq->io_instance, pq->io_base + i, (PORT_PinMux_Type) pq->mux); 
    }
    
    LPUART_Init(&LPUART_InitStruct1);
    
    return pq->ip_instance;
}

void LPUART_Init(LPUART_InitTypeDef* UART_InitStruct)
{
    uint16_t sbr, sbrTemp, i;
    uint32_t osr, tempDiff, calculatedBaud, baudDiff;
    static bool is_fitst_init = true;
    LPUART_Type *LPUARTx;
    
    LPUARTx = LPUART_InstanceTable[UART_InitStruct->instance];
    
    /* enable clock gate */
    *((uint32_t*) SIM_LPUARTClockGateTable[UART_InitStruct->instance].addr) |= SIM_LPUARTClockGateTable[UART_InitStruct->instance].mask;

    /* disable Tx Rx first */
    LPUARTx->CTRL &= ~(LPUART_CTRL_RE_MASK | LPUART_CTRL_TE_MASK);
    
    osr = 4;
    sbr = (UART_InitStruct->srcClock/(UART_InitStruct->baudrate * osr));
    calculatedBaud = (UART_InitStruct->srcClock / (osr * sbr));
    if (calculatedBaud > UART_InitStruct->baudrate)
    {
        baudDiff = calculatedBaud - UART_InitStruct->baudrate;
    }
    else
    {
        baudDiff = UART_InitStruct->baudrate - calculatedBaud;
    }
    for (i = 5; i <= 32; i++)
    {
        /* calculate the temporary sbr value   */
        sbrTemp = (UART_InitStruct->srcClock/(UART_InitStruct->baudrate * i));
        /* calculate the baud rate based on the temporary osr and sbr values */
        calculatedBaud = (UART_InitStruct->srcClock / (i * sbrTemp));

        if (calculatedBaud > UART_InitStruct->baudrate)
        {
            tempDiff = calculatedBaud - UART_InitStruct->baudrate;
        }
        else
        {
            tempDiff = UART_InitStruct->baudrate - calculatedBaud;
        }

        if (tempDiff <= baudDiff)
        {
            baudDiff = tempDiff;
            osr = i;  /* update and store the best osr value calculated */
            sbr = sbrTemp;  /* update store the best sbr value calculated */
        }
    }

    LPUARTx->BAUD &= ~LPUART_BAUD_SBR_MASK;
    LPUARTx->BAUD &= ~LPUART_BAUD_OSR_MASK;
    LPUARTx->BAUD |= LPUART_BAUD_SBR(sbr) | LPUART_BAUD_OSR(osr-1);
    
    /* parity */
    switch(UART_InitStruct->parityMode)
    {
        case kUART_ParityDisabled:
            LPUARTx->CTRL &= ~LPUART_CTRL_PE_MASK;
            break;
        case kUART_ParityEven:
            LPUARTx->CTRL |= LPUART_CTRL_PE_MASK;
            LPUARTx->CTRL &= ~LPUART_CTRL_PT_MASK;
            break;
        case kUART_ParityOdd:
            LPUARTx->CTRL |= LPUART_CTRL_PE_MASK;
            LPUARTx->CTRL |= LPUART_CTRL_PT_MASK;
            break;
        default:
            break;
    }
   
    /* enable Tx Rx */
    LPUARTx->CTRL |= (LPUART_CTRL_RE_MASK | LPUART_CTRL_TE_MASK);

    /* link debug instance */
    /* if it's first initalized ,link getc and putc to it */
    if(is_fitst_init)
    {
        UART_DebugInstance = UART_InitStruct->instance;
    }
    is_fitst_init = false;
}

void LPUART_WriteByte(uint32_t instance, char ch)
{
    while(!(LPUART_InstanceTable[instance]->STAT & LPUART_STAT_TDRE_MASK));
    LPUART_InstanceTable[instance]->DATA = (ch & 0xFF);
}

uint8_t UART_ReadByte(uint32_t instance, uint8_t *ch)
{    
    if(LPUART_InstanceTable[instance]->STAT & LPUART_STAT_RDRF_MASK)
    {
        *ch = (uint8_t)(LPUART_InstanceTable[instance]->DATA);	
        return 0; 		  
    }
    return 1;
}


#ifdef __CC_ARM // MDK Support
struct __FILE 
{ 
	int handle;
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef¡¯ d in stdio.h. */ 
FILE __stdout;
FILE __stdin;
__weak int fputc(int ch,FILE *f)
{
	LPUART_WriteByte(UART_DebugInstance, ch);
	return ch;
}

__weak int fgetc(FILE *f)
{
    uint8_t ch;
    while(UART_ReadByte(UART_DebugInstance, &ch));
    return (ch & 0xFF);
}

#ifdef __cplusplus
}
#endif


#elif __ICCARM__ /* IAR support */
__weak size_t __write(int handle, const unsigned char * buffer, size_t size)
{
    size_t nChars = 0;
    if (buffer == 0)
    {
        /* This means that we should flush internal buffers.  Since we*/
        /* don't we just return.  (Remember, "handle" == -1 means that all*/
        /* handles should be flushed.)*/
        return 0;
    }
    /* This function only writes to "standard out" and "standard err",*/
    /* for all other file handles it returns failure.*/
    if ((handle != _LLIO_STDOUT) && (handle != _LLIO_STDERR))
    {
        return _LLIO_ERROR;
    }
    /* Send data.*/
    while (size--)
    {
        LPUART_WriteByte(UART_DebugInstance, *buffer++);
        ++nChars;
    }
    return nChars;
}

__weak size_t __read(int handle, unsigned char * buffer, size_t size)
{
    size_t nChars = 0;
    uint16_t ch = 0;
    if (buffer == 0)
    {
        /* This means that we should flush internal buffers.  Since we*/
        /* don't we just return.  (Remember, "handle" == -1 means that all*/
        /* handles should be flushed.)*/
        return 0;
    }
    /* This function only writes to "standard out" and "standard err",*/
    /* for all other file handles it returns failure.*/
    if ((handle != _LLIO_STDIN) && (handle != _LLIO_STDERR))
    {
        return _LLIO_ERROR;
    }
    /* read data.*/
    while (size--)
    {
        while(LPUART_ReadByte(UART_DebugInstance, &ch));
        *buffer++ = (char)ch & 0xFF;
        ++nChars;
    }
    return nChars;
}
#endif

static void UART_putstr(uint32_t instance, const char *str)
{
    while(*str != '\0')
    {
        LPUART_WriteByte(instance, *str++);
    }
}

static void printn(unsigned int n, unsigned int b)
{
    static char *ntab = "0123456789ABCDEF";
    unsigned int a, m;
    if (n / b)
    {
        a = n / b;
        printn(a, b);  
    }
    m = n % b;
    LPUART_WriteByte(UART_DebugInstance, ntab[m]);
}

int UART_printf(const char *fmt, ...)
{
    char c;
    unsigned int *adx = (unsigned int*)(void*)&fmt + 1;
_loop:
    while((c = *fmt++) != '%')
    {
        if (c == '\0') return 0;
        LPUART_WriteByte(UART_DebugInstance, c);
    }
    c = *fmt++;
    if (c == 'd' || c == 'l')
    {
        printn(*adx, 10);
    }
    if (c == 'o' || c == 'x')
    {
        printn(*adx, c=='o'? 8:16 );
    }
    if (c == 's')
    {
        UART_putstr(UART_DebugInstance, (char*)*adx);
    }
    adx++;
    goto _loop;
}

/*
static  QuickInit_Type UART_QuickInitTable[] =
{
    { 0, 4, 4, 20, 2, 0, 0}, //LPUART0_RX_E21_TX_E20  4
    { 0, 1, 3, 16, 2, 0, 0}, //LPUART0_RX_B16_TX_B17  3
    { 0, 0, 2,  1, 2, 0, 0}, //LPUART0_RX_A01_TX_A02  2
    { 1, 0, 3, 18, 2, 0, 0}, //LPUART1_RX_A18_TX_A19  3
    { 1, 2, 3,  3, 2, 0, 0}, //LPUART1_RX_C03_TX_C04  3
    { 0, 3, 3,  6, 2, 0, 0}, //LPUART0_RX_D06_TX_D07  3
};
*/
