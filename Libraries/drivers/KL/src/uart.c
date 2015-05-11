/**
  ******************************************************************************
  * @file    uart.c
  * @author  YANDLD
  * @version V2.5
  * @date    2014.3.26
  * @brief   www.beyondcore.net   http://upcmcu.taobao.com 
  ******************************************************************************
  */
  
#include "uart.h"
#include "gpio.h"
#include "common.h"

#ifdef UART_USE_STDIO
#include <stdio.h>
#endif

#if __ICCARM__
#include <yfuns.h>
#endif


#define UARTN_BASES {(void*)UART0, (void*)UART1, (void*)UART2}
/* gloabl const table defination */
const void* UART_InstanceTable[] = UARTN_BASES;
static UART_CallBackTxType UART_CallBackTxTable[ARRAY_SIZE(UART_InstanceTable)] = {NULL};
static UART_CallBackRxType UART_CallBackRxTable[ARRAY_SIZE(UART_InstanceTable)] = {NULL};
/* special use for printf */
static uint8_t UART_DebugInstance;

static const struct reg_ops SIM_UARTClockGateTable[] =
{
    {(void*)&(SIM->SCGC4), SIM_SCGC4_UART0_MASK},
    {(void*)&(SIM->SCGC4), SIM_SCGC4_UART1_MASK},
    {(void*)&(SIM->SCGC4), SIM_SCGC4_UART2_MASK},
};

static const IRQn_Type UART_IRQnTable[] = 
{
    UART0_IRQn,
    UART1_IRQn,
    UART2_IRQn,
};

#ifdef UART_USE_STDIO
#ifdef __CC_ARM // MDK Support
struct __FILE 
{ 
	int handle;
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef’ d in stdio.h. */ 
FILE __stdout;
FILE __stdin;
int fputc(int ch,FILE *f)
{
	UART_WriteByte(UART_DebugInstance, ch);
	return ch;
}

int fgetc(FILE *f)
{
    uint16_t ch;
    while(UART_ReadByte(UART_DebugInstance, &ch));
    return (ch & 0xFF);
}
#elif __ICCARM__ /* IAR support */
size_t __write(int handle, const unsigned char * buffer, size_t size)
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
        UART_WriteByte(UART_DebugInstance, *buffer++);
        ++nChars;
    }
    return nChars;
}

size_t __read(int handle, unsigned char * buffer, size_t size)
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
        while(UART_ReadByte(UART_DebugInstance, &ch));
        *buffer++ = (char)ch & 0xFF;
        ++nChars;
    }
    return nChars;
}


#endif /* comiler support */

#else /* DO NOT USE STDIO */
static void UART_putstr(uint32_t instance, const char *str)
{
    while(*str != '\0')
    {
        UART_WriteByte(instance, *str++);
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
    UART_WriteByte(UART_DebugInstance, ntab[m]);
}

int UART_printf(const char *fmt, ...)
{
    char c;
    unsigned int *adx = (unsigned int*)(void*)&fmt + 1;
_loop:
    while((c = *fmt++) != '%')
    {
        if (c == '\0') return 0;
        UART_WriteByte(UART_DebugInstance, c);
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
    return 0;
}
#endif /*end of UART_USE_STDIO */

uint8_t UART_QuickInit(uint32_t MAP, uint32_t baudrate)
{
    uint8_t i;
    uint32_t clock;
    UART_InitTypeDef UART_InitStruct1;
    QuickInit_Type * pq = (QuickInit_Type*)&(MAP);
    UART_InitStruct1.baudrate = baudrate;
    UART_InitStruct1.instance = pq->ip_instance;
    
    /* clock source */
    if(pq->ip_instance == HW_UART0)
    {
        CLOCK_GetClockFrequency(kMCGOutClock, &clock);
        
        /* use PLL/2 or FLL */
        SIM->SOPT2 &= ~SIM_SOPT2_UART0SRC_MASK;
        SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1);  
	
        if(MCG->C6 & MCG_C6_PLLS_MASK) /* PLL */
        {
            SIM->SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;
            clock /= 2;
        }
        else /* FLL */
        {
            SIM->SOPT2 &= ~SIM_SOPT2_PLLFLLSEL_MASK;
        }
    }
    else
    {
        CLOCK_GetClockFrequency(kBusClock, &clock);
    }
    UART_InitStruct1.srcClock = clock;
    
    /* init pinmux */
    for(i = 0; i < pq->io_offset; i++)
    {
        PORT_PinMuxConfig(pq->io_instance, pq->io_base + i, (PORT_PinMux_Type) pq->mux); 
    }
    
    /* init UART */
    UART_Init(&UART_InitStruct1);
    
    return pq->ip_instance;
}

/**
 * @brief  配置UART模块的中断或DMA属性
 * @code
 *      //配置UART0模块开启接收中断功能
 *      UART_ITDMAConfig(HW_UART0, kUART_IT_Rx, true);
 * @endcode
 * @param  instance      :芯片串口端口
 *         @arg HW_UART0 :芯片的UART0端口
 *         @arg HW_UART1 :芯片的UART1端口
 *         @arg HW_UART2 :芯片的UART2端口
 *         @arg HW_UART3 :芯片的UART3端口
 *         @arg HW_UART4 :芯片的UART4端口
 *         @arg HW_UART5 :芯片的UART5端口
 * @param  status      :开关
 * @param  config: 工作模式选择
 *         @arg kUART_IT_Tx:
 *         @arg kUART_DMA_Tx:
 *         @arg kUART_IT_Rx:
 *         @arg kUART_DMA_Rx:
 * @retval None
 */
void UART_ITDMAConfig(uint32_t instance, UART_ITDMAConfig_Type config, bool status)
{
    /* enable clock gate */
    *((uint32_t*) SIM_UARTClockGateTable[instance].addr) |= SIM_UARTClockGateTable[instance].mask;
    UART_Type * UARTx = (UART_Type*)UART_InstanceTable[instance];
    
    switch(config)
    {
        case kUART_IT_Tx:
            (status)?
            (UARTx->C2 |= UART_C2_TIE_MASK):
            (UARTx->C2 &= ~UART_C2_TIE_MASK);
            NVIC_EnableIRQ(UART_IRQnTable[instance]);
            break; 
        case kUART_IT_Rx:
            (status)?
            (UARTx->C2 |= UART_C2_RIE_MASK):
            (UARTx->C2 &= ~UART_C2_RIE_MASK);
            NVIC_EnableIRQ(UART_IRQnTable[instance]);
            break;
        case kUART_DMA_Tx:
            (status)?
            (UARTx->C2 |= UART_C2_TIE_MASK):
            (UARTx->C2 &= ~UART_C2_TIE_MASK);
            if(instance == HW_UART0)
            {
                (status)?
                (UART0->C5 |= UART0_C5_TDMAE_MASK):
                (UART0->C5 &= ~UART0_C5_TDMAE_MASK);
            }
            else
            {
                (status)?
                (UARTx->C4 |= UART_C4_TDMAS_MASK):
                (UARTx->C4 &= ~UART_C4_TDMAS_MASK);
            }
            break;
        case kUART_DMA_Rx:
            (status)?
            (UARTx->C2 |= UART_C2_RIE_MASK):
            (UARTx->C2 &= ~UART_C2_RIE_MASK);
            if(instance == HW_UART0)
            {
                (status)?
                (UART0->C5 |= UART0_C5_RDMAE_MASK):
                (UART0->C5 &= ~UART0_C5_RDMAE_MASK);
            }
            else
            {
                (status)?
                (UARTx->C4 |= UART_C4_RDMAS_MASK):
                (UARTx->C4 &= ~UART_C4_RDMAS_MASK); 
            }
            break;
        default:
            break;
    }
}

/**
 * @brief  注册接收中断回调函数
 * @param  instance      :芯片串口端口
 *         @arg HW_UART0 :芯片的UART0端口
 *         @arg HW_UART1 :芯片的UART1端口
 *         @arg HW_UART2 :芯片的UART2端口
 *         @arg HW_UART3 :芯片的UART3端口
 *         @arg HW_UART4 :芯片的UART4端口
 *         @arg HW_UART5 :芯片的UART5端口
 * @param AppCBFun: 回调函数指针入口
 * @retval None
 * @note 对于此函数的具体应用请查阅应用实例
 */
void UART_CallbackRxInstall(uint32_t instance, UART_CallBackRxType AppCBFun)
{
    /* enable clock gate */
    *((uint32_t*) SIM_UARTClockGateTable[instance].addr) |= SIM_UARTClockGateTable[instance].mask;
    if(AppCBFun != NULL)
    {
        UART_CallBackRxTable[instance] = AppCBFun;
    }
}

/**
 * @brief  注册发送中断回调函数
 * @param  instance      :芯片串口端口
 *         @arg HW_UART0 :芯片的UART0端口
 *         @arg HW_UART1 :芯片的UART1端口
 *         @arg HW_UART2 :芯片的UART2端口
 *         @arg HW_UART3 :芯片的UART3端口
 *         @arg HW_UART4 :芯片的UART4端口
 *         @arg HW_UART5 :芯片的UART5端口
 * @param AppCBFun: 回调函数指针入口
 * @retval None
 * @note 对于此函数的具体应用请查阅应用实例
 */
void UART_CallbackTxInstall(uint32_t instance, UART_CallBackTxType AppCBFun)
{
    /* enable clock gate */
    *((uint32_t*) SIM_UARTClockGateTable[instance].addr) |= SIM_UARTClockGateTable[instance].mask;
    if(AppCBFun != NULL)
    {
        UART_CallBackTxTable[instance] = AppCBFun;
    }
}

void UART_Init(UART_InitTypeDef* UART_InitStruct)
{
    uint16_t sbr;
    static bool is_fitst_init = true;
    /* enable clock gate */
    *((uint32_t*) SIM_UARTClockGateTable[UART_InitStruct->instance].addr) |= SIM_UARTClockGateTable[UART_InitStruct->instance].mask;
    
    UART_Type * UARTx = (UART_Type*)UART_InstanceTable[UART_InitStruct->instance];
    
    /* disable Tx Rx first */
    UARTx->C2 &= ~((UART_C2_TE_MASK)|(UART_C2_RE_MASK));
	
    /* disable Tx Rx */
    UARTx->C2 &= ~((UART_C2_TE_MASK)|(UART_C2_RE_MASK));
    
    /* config baudrate */
    sbr = UART_InitStruct->srcClock/((UART_InitStruct->baudrate)*16);
    UARTx->BDH &= ~(UART_BDH_SBR_MASK);
    UARTx->BDH |= (sbr>>8) & UART_BDH_SBR_MASK;
    UARTx->BDL = (sbr & UART_BDL_SBR_MASK);
    
    /* enable Tx Rx */
    UARTx->C2 |= ((UART_C2_TE_MASK)|(UART_C2_RE_MASK));
    
    /* link debug instance */
    /* if it's first initalized ,link getc and putc to it */
    if(is_fitst_init)
    {
        UART_DebugInstance = UART_InitStruct->instance;
    }
    is_fitst_init = false;
}

void UART_WriteByte(uint32_t instance, uint16_t ch)
{
    UART_Type * UARTx = (UART_Type*)UART_InstanceTable[instance];
    while(!(UARTx->S1 & UART_S1_TDRE_MASK));
    UARTx->D = (uint8_t)ch;
}

uint8_t UART_ReadByte(uint32_t instance, uint16_t *ch)
{
    UART_Type * UARTx = (UART_Type*)UART_InstanceTable[instance];
    
    /* clear OverRun */
    //UARTx->S1 |= UART_S1_OR_MASK;
    
    if(UARTx->S1 & UART_S1_RDRF_MASK)
    {
        *ch = (uint8_t)(UARTx->D);	
        return 0; 		  
    }
    return 1;
}

void UART0_IRQHandler(void)
{
    uint16_t ch;
    UART_Type * UARTx = (UART_Type*)UART_InstanceTable[HW_UART0];
    /* Tx */
    if((UARTx->S1 & UART_S1_TDRE_MASK) && (UARTx->C2 & UART_C2_TIE_MASK))
    {
        if(UART_CallBackTxTable[HW_UART0])
        {
            UART_CallBackTxTable[HW_UART0](&ch);
        }
        UARTx->D = (uint8_t)ch;
    }
    /* Rx */
    if((UARTx->S1 & UART_S1_RDRF_MASK) && (UARTx->C2 & UART_C2_RIE_MASK))
    {
        ch = (uint8_t)UARTx->D;
        if(UART_CallBackRxTable[HW_UART0])
        {
            UART_CallBackRxTable[HW_UART0](ch);
        }    
    }
}

void UART1_IRQHandler(void)
{
    uint16_t ch;
    UART_Type * UARTx = (UART_Type*)UART_InstanceTable[HW_UART1];
    /* Tx */
    if((UARTx->S1 & UART_S1_TDRE_MASK) && (UARTx->C2 & UART_C2_TIE_MASK))
    {
        if(UART_CallBackTxTable[HW_UART1])
        {
            UART_CallBackTxTable[HW_UART1](&ch);
        }
        UARTx->D = (uint8_t)ch;
    }
    /* Rx */
    if((UARTx->S1 & UART_S1_RDRF_MASK) && (UARTx->C2 & UART_C2_RIE_MASK))
    {
        ch = (uint8_t)UARTx->D;
        if(UART_CallBackRxTable[HW_UART1])
        {
            UART_CallBackRxTable[HW_UART1](ch);
        }    
    }
}

void UART2_IRQHandler(void)
{
    uint16_t ch;
    UART_Type * UARTx = (UART_Type*)UART_InstanceTable[HW_UART2];
    /* Tx */
    if((UARTx->S1 & UART_S1_TDRE_MASK) && (UARTx->C2 & UART_C2_TIE_MASK))
    {
        if(UART_CallBackTxTable[HW_UART2])
        {
            UART_CallBackTxTable[HW_UART2](&ch);
        }
        UARTx->D = (uint8_t)ch;
    }
    /* Rx */
    if((UARTx->S1 & UART_S1_RDRF_MASK) && (UARTx->C2 & UART_C2_RIE_MASK))
    {
        ch = (uint8_t)UARTx->D;
        if(UART_CallBackRxTable[HW_UART2])
        {
            UART_CallBackRxTable[HW_UART2](ch);
        }    
    } 
}



/*
static const QuickInit_Type UART_QuickInitTable[] =
{
    { 1, 4, 3, 0, 2, 0}, //UART1_RX_PE01_TX_PE00
    { 2, 4, 3,16, 2, 0}, //UART2_RX_PE17_TX_PE16
    { 0, 4, 4,20, 2, 0}, //UART0_RX_PE21_TX_PE20
    { 2, 4, 4,22, 2, 0}, //UART2_RX_PE23_TX_PE22
    { 0, 0, 2, 1, 2, 0}, //UART0_RX_PA01_TX_PA02
    { 0, 0, 3,14, 2, 0}, //UART0_RX_PA15_TX_PA14
    { 1, 0, 3,18, 2, 0}, //UART1_RX_PA18_TX_PA19
    { 0, 1, 3,16, 2, 0}, //UART0_RX_PB16_TX_PB17
    { 2, 3, 3, 2, 2, 0}, //UART2_RX_PD02_TX_PD03
    { 2, 3, 3, 4, 2, 0}, //UART2_RX_PD04_TX_PD05
    { 0, 3, 3, 6, 2, 0}, //UART0_RX_PD06_TX_PD07
};
    for(i=0;i<ARRAY_SIZE(UART_QuickInitTable);i++)
    {
        printf("(0x%08XU)\r\n", QuickInitEncode(&UART_QuickInitTable[i]));
    }
*/
