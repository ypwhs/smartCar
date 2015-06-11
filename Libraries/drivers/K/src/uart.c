/**
  ******************************************************************************
  * @file    uart.c
  * @author  YANDLD
  * @version V2.5
  * @date    2014.3.25
  * @brief   www.beyondcore.net   http://upcmcu.taobao.com 
  ******************************************************************************
  */
#include "uart.h"
#include "gpio.h"
#include "common.h"


#if __ICCARM__
#include <yfuns.h>
#endif



#if (!defined(UART_BASES))
#ifdef  UART2
    #define UART_BASES {UART0, UART1, UART2}
#elif   UART3
    #define UART_BASES {UART0, UART1, UART2, UART3}
#elif   UART4
    #define UART_BASES {UART0, UART1, UART2, UART3, UART4}
#elif   UART5 
    #define UART_BASES {UART0, UART1, UART2, UART3, UART4, UART5}
#else
    #define UART_BASES {UART0, UART1}
#endif
#endif

/* gloabl const table defination */
static UART_Type * const UART_InstanceTable[] = UART_BASES;
/* callback function slot */
static UART_CallBackTxType UART_CallBackTxTable[ARRAY_SIZE(UART_InstanceTable)] = {NULL};
static UART_CallBackRxType UART_CallBackRxTable[ARRAY_SIZE(UART_InstanceTable)] = {NULL};
/* special use for printf */
static uint8_t UART_DebugInstance;
/* instance clock gate table */

static const struct reg_ops SIM_UARTClockGateTable[] =
{
    
    {(void*)&(SIM->SCGC4), SIM_SCGC4_UART0_MASK},
    {(void*)&(SIM->SCGC4), SIM_SCGC4_UART1_MASK},
    {(void*)&(SIM->SCGC4), SIM_SCGC4_UART2_MASK},
#ifdef UART3
    {(void*)&(SIM->SCGC4), SIM_SCGC4_UART3_MASK},
#endif
#ifdef UART4
    {(void*)&(SIM->SCGC1), SIM_SCGC1_UART4_MASK}, 
#endif
#ifdef UART5
    {(void*)&(SIM->SCGC1), SIM_SCGC1_UART5_MASK},
#endif
};
/* interrupt handler table */
static const IRQn_Type UART_IRQnTable[] = 
{
    UART0_RX_TX_IRQn,
    UART1_RX_TX_IRQn,
    UART2_RX_TX_IRQn,
#ifdef UART3
    UART3_RX_TX_IRQn,
#endif
#ifdef UART4
    UART4_RX_TX_IRQn,
#endif
#ifdef UART5
    UART5_RX_TX_IRQn,
#endif
};

static const uint32_t UART_TIFOSizeTable[] = {1, 4, 8, 16, 32, 64, 128};

#ifdef __cplusplus
 extern "C" {
#endif
     
     
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
__weak int fputc(int ch,FILE *f)
{
	UART_WriteByte(UART_DebugInstance, ch);
	return ch;
}

__weak int fgetc(FILE *f)
{
    uint16_t ch;
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
        UART_WriteByte(UART_DebugInstance, *buffer++);
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
        while(UART_ReadByte(UART_DebugInstance, &ch));
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
}


//! @defgroup CHKinetis
//! @{

//! @defgroup UART
//! @brief UART API functions
//! @{

/**
 * @brief  初始化UART模块 
 * @note   用户需自己进行引脚的复用配置
 * @code
 *      //使用UART0模块 使用115200波特率进行通信
 *    UART_InitTypeDef UART_InitStruct1;      //申请一个结构变量
 *    UART_InitStruct1.instance = HW_UART0;   //选择UART0模块
 *    UART_InitStruct1.baudrate = 115200;     //设置通信速度为115200
 *    UART_InitStruct1.parityMode = kUART_ParityDisabled; //校验位禁止
 *    UART_InitStruct1.bitPerChar = kUART_8BitsPerChar;   //每帧8bit
 *    UART_Init(&UART_InitStruct1);
 * @endcode
 * @param  UART_InitTypeDef: 串口工作配置存储结构体
 *         instance      :芯片串口端口
 *         @arg HW_UART0 :芯片的UART0端口
 *         @arg HW_UART1 :芯片的UART1端口
 *         @arg HW_UART2 :芯片的UART2端口
 *         @arg HW_UART3 :芯片的UART3端口
 *         @arg HW_UART4 :芯片的UART4端口
 *         @arg HW_UART5 :芯片的UART5端口
 * @param  baudrate  :串口通讯速率设置
 * @retval None
 */
void UART_Init(UART_InitTypeDef* UART_InitStruct)
{
    uint16_t sbr;
    uint8_t brfa; 
    static bool is_fitst_init = true;
    
	/* param check */
    assert_param(IS_UART_ALL_INSTANCE(UART_InitStruct->instance));
    
    /* enable clock gate */
    *((uint32_t*) SIM_UARTClockGateTable[UART_InitStruct->instance].addr) |= SIM_UARTClockGateTable[UART_InitStruct->instance].mask;
    
    /* disable Tx Rx first */
    UART_InstanceTable[UART_InitStruct->instance]->C2 &= ~((UART_C2_TE_MASK)|(UART_C2_RE_MASK));
    
    /* baud rate generation */
    sbr = (uint16_t)((UART_InitStruct->srcClock)/((UART_InitStruct->baudrate)*16));
    brfa = ((32*UART_InitStruct->srcClock)/((UART_InitStruct->baudrate)*16)) - 32*sbr;
    
    /* config baudrate */
    UART_InstanceTable[UART_InitStruct->instance]->BDH &= ~UART_BDH_SBR_MASK;
    UART_InstanceTable[UART_InitStruct->instance]->BDL &= ~UART_BDL_SBR_MASK;
    UART_InstanceTable[UART_InitStruct->instance]->C4 &= ~UART_C4_BRFA_MASK;
    
    UART_InstanceTable[UART_InitStruct->instance]->BDH |= UART_BDH_SBR(sbr>>8); 
    UART_InstanceTable[UART_InitStruct->instance]->BDL = UART_BDL_SBR(sbr); 
    UART_InstanceTable[UART_InitStruct->instance]->C4 |= UART_C4_BRFA(brfa);
    
    /* parity */
    switch(UART_InitStruct->parityMode)
    {
        case kUART_ParityDisabled: /* standard N 8 N 1*/
            UART_InstanceTable[UART_InitStruct->instance]->C1 &= ~UART_C1_PE_MASK;
            UART_InstanceTable[UART_InitStruct->instance]->C1 &= ~UART_C1_M_MASK;
            break;
        case kUART_ParityEven:/* 8 bit data + 1bit parity */
            UART_InstanceTable[UART_InitStruct->instance]->C1 |= UART_C1_PE_MASK;
            UART_InstanceTable[UART_InitStruct->instance]->C1 &= ~UART_C1_PT_MASK;
            break;
        case kUART_ParityOdd:
            UART_InstanceTable[UART_InitStruct->instance]->C1 |= UART_C1_PE_MASK;
            UART_InstanceTable[UART_InitStruct->instance]->C1 |= UART_C1_PT_MASK;
            break;
        default:
            break;
    }
    
    /* bit per char */
    /* note: Freescale's bit size config in register are including parity bit! */
    switch(UART_InitStruct->bitPerChar)
    {
        case kUART_8BitsPerChar:
            if(UART_InstanceTable[UART_InitStruct->instance]->C1 & UART_C1_PE_MASK)
            {
                /* parity is enabled it's actually 9bit*/
                UART_InstanceTable[UART_InitStruct->instance]->C1 |= UART_C1_M_MASK;
                UART_InstanceTable[UART_InitStruct->instance]->C4 &= ~UART_C4_M10_MASK;    
            }
            else
            {
                UART_InstanceTable[UART_InitStruct->instance]->C1 &= ~UART_C1_M_MASK;
                UART_InstanceTable[UART_InitStruct->instance]->C4 &= ~UART_C4_M10_MASK;    
            }
            break;
        case kUART_9BitsPerChar:
            if(UART_InstanceTable[UART_InitStruct->instance]->C1 & UART_C1_PE_MASK)
            {
                /* parity is enabled it's actually 10 bit*/
                UART_InstanceTable[UART_InitStruct->instance]->C1 |= UART_C1_M_MASK;
                UART_InstanceTable[UART_InitStruct->instance]->C4 |= UART_C4_M10_MASK;  
            } 
            else
            {
                UART_InstanceTable[UART_InitStruct->instance]->C1 |= UART_C1_M_MASK;
                UART_InstanceTable[UART_InitStruct->instance]->C4 &= ~UART_C4_M10_MASK;      
            }
            break;
        default:
            break;
    }
    UART_InstanceTable[UART_InitStruct->instance]->S2 &= ~UART_S2_MSBF_MASK; /* LSB */
    
    /* enable Tx Rx */
    UART_InstanceTable[UART_InitStruct->instance]->C2 |= ((UART_C2_TE_MASK)|(UART_C2_RE_MASK));
    
    /* link debug instance */
    /* if it's first initalized ,link getc and putc to it */
    if(is_fitst_init)
    {
        UART_DebugInstance = UART_InitStruct->instance;
    }
    is_fitst_init = false;
}

void UART_DeInit(uint32_t instance)
{
    /* waitting sending complete */
    while(!(UART_InstanceTable[instance]->S1 & UART_S1_TDRE_MASK));
	
    /* disable Tx Rx */
    UART_InstanceTable[instance]->C2 &= ~((UART_C2_TE_MASK)|(UART_C2_RE_MASK));
    
    /* disable clock gate */
    *((uint32_t*) SIM_UARTClockGateTable[instance].addr) |= SIM_UARTClockGateTable[instance].mask;
}

#pragma weak UART_SelectDebugInstance
void UART_SelectDebugInstance(uint32_t instance)
{
	/* param check */
    assert_param(IS_UART_ALL_INSTANCE(instance));
    if(*((uint32_t*) SIM_UARTClockGateTable[instance].addr) & SIM_UARTClockGateTable[instance].mask)
    {
        UART_DebugInstance = instance;
    }
}

void UART_EnableTxFIFO(uint32_t instance, bool status)
{
    /* waitting for all data has been shifted out */
    //while(!(UART_InstanceTable[instance]->S1 & UART_S1_TDRE_MASK));
    
    (status)?
    (UART_InstanceTable[instance]->PFIFO |= UART_PFIFO_TXFE_MASK):
    (UART_InstanceTable[instance]->PFIFO &= ~UART_PFIFO_TXFE_MASK);
    
}

void UART_EnableRxFIFO(uint32_t instance, bool status)
{
    (status)?
    (UART_InstanceTable[instance]->PFIFO |= UART_PFIFO_RXFE_MASK):
    (UART_InstanceTable[instance]->PFIFO &= ~UART_PFIFO_RXFE_MASK);
}

uint32_t UART_GetTxFIFOSize(uint32_t instance)
{
    return UART_TIFOSizeTable[(UART_InstanceTable[instance]->PFIFO  \
    & UART_PFIFO_TXFIFOSIZE_MASK) >> UART_PFIFO_TXFIFOSIZE_SHIFT];
}

uint32_t UART_GetRxFIFOSize(uint32_t instance)
{
    return UART_TIFOSizeTable[(UART_InstanceTable[instance]->PFIFO  \
    & UART_PFIFO_RXFIFOSIZE_MASK) >> UART_PFIFO_RXFIFOSIZE_SHIFT];
}

void UART_SetTxFIFOWatermark(uint32_t instance, uint32_t size)
{
    /* disable transmiter and renable it */
    UART_InstanceTable[instance]->C2 &= ~UART_C2_TE_MASK;
    UART_InstanceTable[instance]->TWFIFO = size;
    UART_InstanceTable[instance]->C2 |= UART_C2_TE_MASK;
}

void UART_SetRxFIFOWatermark(uint32_t instance, uint32_t size)
{
    UART_InstanceTable[instance]->C2 &= ~UART_C2_RE_MASK;
    UART_InstanceTable[instance]->RWFIFO = size;
    UART_InstanceTable[instance]->C2 |= UART_C2_RE_MASK;
}

/**
 * @brief  串口发送一个字节
 * @note   阻塞式发送 只有发送完后才会返回
 * @code
 *      //使用UART0模块 发送数据0x5A
 *    UART_WriteByte(HW_UART0, 0x5A);
 * @endcode
 * @param  instance      :芯片串口端口
 *         @arg HW_UART0 :芯片的UART0端口
 *         @arg HW_UART1 :芯片的UART1端口
 *         @arg HW_UART2 :芯片的UART2端口
 *         @arg HW_UART3 :芯片的UART3端口
 *         @arg HW_UART4 :芯片的UART4端口
 *         @arg HW_UART5 :芯片的UART5端口
 * @param  ch: 需要发送的一字节数据
 * @retval None
 */
void UART_WriteByte(uint32_t instance, uint16_t ch)
{
	/* param check */
    assert_param(IS_UART_ALL_INSTANCE(instance));

    if(UART_InstanceTable[instance]->PFIFO & UART_PFIFO_TXFE_MASK)
    {
        /* buffer is used */
        while(UART_InstanceTable[instance]->TCFIFO >= UART_GetTxFIFOSize(instance));
    }
    else
    {
        /* no buffer is used */
        while(!(UART_InstanceTable[instance]->S1 & UART_S1_TDRE_MASK));
    }
    
    UART_InstanceTable[instance]->D = (uint8_t)(ch & 0xFF);
    
    /* config ninth bit */
    uint8_t ninth_bit = (ch >> 8) & 0x01U;
    (ninth_bit)?(UART_InstanceTable[instance]->C3 |= UART_C3_T8_MASK):(UART_InstanceTable[instance]->C3 &= ~UART_C3_T8_MASK);
}


/**
 * @brief  UART接受一个字节
 * @note   非阻塞式接收 立即返回
 * @code
 *      //接收UART0模块的数据
 *      uint8_t data; //申请变量，存储接收的数据
 *      UART_ReadByte(HW_UART0, &data);
 * @endcode
 * @param  instance      :芯片串口端口
 *         @arg HW_UART0 :芯片的UART0端口
 *         @arg HW_UART1 :芯片的UART1端口
 *         @arg HW_UART2 :芯片的UART2端口
 *         @arg HW_UART3 :芯片的UART3端口
 *         @arg HW_UART4 :芯片的UART4端口
 *         @arg HW_UART5 :芯片的UART5端口
 * @param  ch: 接收到的数据指针
 * @retval 0:成功接收到数据  非0:没有接收到数据
 */
uint8_t UART_ReadByte(uint32_t instance, uint16_t *ch)
{
	/* param check */
    assert_param(IS_UART_ALL_INSTANCE(instance));
    uint8_t temp = 0;
    if((UART_InstanceTable[instance]->S1 & UART_S1_RDRF_MASK) != 0)
    {
        /* get ninth bit */
        temp = (UART_InstanceTable[instance]->C3 & UART_C3_R8_MASK) >> UART_C3_R8_SHIFT;
        *ch = temp << 8;
        *ch |= (uint8_t)(UART_InstanceTable[instance]->D);	
        return 0; 		  
    }
    return 1;
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
    switch(config)
    {
        case kUART_IT_Tx:
            (status)?
            (UART_InstanceTable[instance]->C2 |= UART_C2_TIE_MASK):
            (UART_InstanceTable[instance]->C2 &= ~UART_C2_TIE_MASK);
            NVIC_EnableIRQ(UART_IRQnTable[instance]);
            break; 
        case kUART_IT_Rx:
            (status)?
            (UART_InstanceTable[instance]->C2 |= UART_C2_RIE_MASK):
            (UART_InstanceTable[instance]->C2 &= ~UART_C2_RIE_MASK);
            NVIC_EnableIRQ(UART_IRQnTable[instance]);
            break;
        case kUART_DMA_Tx:
            (status)?
            (UART_InstanceTable[instance]->C2 |= UART_C2_TIE_MASK):
            (UART_InstanceTable[instance]->C2 &= ~UART_C2_TIE_MASK);
            (status)?
            (UART_InstanceTable[instance]->C5 |= UART_C5_TDMAS_MASK):
            (UART_InstanceTable[instance]->C5 &= ~UART_C5_TDMAS_MASK);
            break;
        case kUART_DMA_Rx:
            (status)?
            (UART_InstanceTable[instance]->C2 |= UART_C2_RIE_MASK):
            (UART_InstanceTable[instance]->C2 &= ~UART_C2_RIE_MASK);
            (status)?
            (UART_InstanceTable[instance]->C5 |= UART_C5_RDMAS_MASK):
            (UART_InstanceTable[instance]->C5 &= ~UART_C5_RDMAS_MASK);
            break;
        default:
            break;
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
	/* param check */
    assert_param(IS_UART_ALL_INSTANCE(instance));
    
    /* enable clock gate */
    *((uint32_t*) SIM_UARTClockGateTable[instance].addr) |= SIM_UARTClockGateTable[instance].mask;
    if(AppCBFun != NULL)
    {
        UART_CallBackTxTable[instance] = AppCBFun;
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
	/* param check */
    assert_param(IS_UART_ALL_INSTANCE(instance));
    
    /* enable clock gate */
    *((uint32_t*) SIM_UARTClockGateTable[instance].addr) |= SIM_UARTClockGateTable[instance].mask;
    if(AppCBFun != NULL)
    {
        UART_CallBackRxTable[instance] = AppCBFun;
    }
}

 /**
 * @brief  串口快速化配置函数
 * @code
 *      // 初始化 UART4 属性: 115200-N-8-N-1, Tx:PC15 Rx:PC14
 *      UART_QuickInit(UART4_RX_PC14_TX_PC15, 115200);
 * @endcode
 * @param  MAP  : 串口引脚配置缩略图
 *         例如 UART1_RX_PE01_TX_PE00 ：使用串口1的PTE1/PTE0引脚
 * @param  baudrate: 波特率 9600 115200...
 * @retval UART模块号
 */
uint8_t UART_QuickInit(uint32_t MAP, uint32_t baudrate)
{
    uint8_t i;
    uint32_t clock;
    UART_InitTypeDef UART_InitStruct1;
    QuickInit_Type * pq = (QuickInit_Type*)&(MAP);
    UART_InitStruct1.baudrate = baudrate;
    UART_InitStruct1.instance = pq->ip_instance;
    UART_InitStruct1.parityMode = kUART_ParityDisabled;
    UART_InitStruct1.bitPerChar = kUART_8BitsPerChar;
    
    /* src clock */
    CLOCK_GetClockFrequency(kBusClock, &clock);
    if((pq->ip_instance == HW_UART0) || (pq->ip_instance == HW_UART1))
    {
        CLOCK_GetClockFrequency(kCoreClock, &clock); /* UART0 UART1 are use core clock */
    }
    UART_InitStruct1.srcClock = clock;
    
    /* init pinmux */
    for(i = 0; i < pq->io_offset; i++)
    {
        PORT_PinMuxConfig(pq->io_instance, pq->io_base + i, (PORT_PinMux_Type) pq->mux); 
    }
    
    /* init UART */
    UART_Init(&UART_InitStruct1);
    
    /* default: disable hardware buffer */
    UART_EnableTxFIFO(pq->ip_instance, false);
    UART_EnableRxFIFO(pq->ip_instance, false);
    
    return pq->ip_instance;
}

//! @}

//! @}

/**
 * @brief  中断处理函数入口
 * @param  UART0_RX_TX_IRQHandler :芯片的UART0端口中断函数入口
 *         UART1_RX_TX_IRQHandler :芯片的UART1端口中断函数入口
 *         UART2_RX_TX_IRQHandler :芯片的UART2端口中断函数入口
 *         UART3_RX_TX_IRQHandler :芯片的UART3端口中断函数入口
 *         UART4_RX_TX_IRQHandler :芯片的UART4端口中断函数入口
 *         UART5_RX_TX_IRQHandler :芯片的UART5端口中断函数入口
 * @note 函数内部用于中断事件处理
 */
void UART0_RX_TX_IRQHandler(void)
{
    uint16_t ch;
    /* Tx */
    if((UART_InstanceTable[HW_UART0]->S1 & UART_S1_TDRE_MASK) && (UART_InstanceTable[HW_UART0]->C2 & UART_C2_TIE_MASK))
    {
        if(UART_CallBackTxTable[HW_UART0])
        {
            UART_CallBackTxTable[HW_UART0](&ch);
        }
        UART_InstanceTable[HW_UART0]->D = (uint8_t)ch;
    }
    /* Rx */
    if((UART_InstanceTable[HW_UART0]->S1 & UART_S1_RDRF_MASK) && (UART_InstanceTable[HW_UART0]->C2 & UART_C2_RIE_MASK))
    {
        ch = (uint8_t)UART_InstanceTable[HW_UART0]->D;
        if(UART_CallBackRxTable[HW_UART0])
        {
            UART_CallBackRxTable[HW_UART0](ch);
        }    
    }
}

void UART1_RX_TX_IRQHandler(void)
{
    uint16_t ch;
    /* Tx */
    if((UART_InstanceTable[HW_UART1]->S1 & UART_S1_TDRE_MASK) && (UART_InstanceTable[HW_UART1]->C2 & UART_C2_TIE_MASK))
    {
        if(UART_CallBackTxTable[HW_UART1])
        {
            UART_CallBackTxTable[HW_UART1](&ch);
        }
        UART_InstanceTable[HW_UART1]->D = (uint8_t)ch;
    }
    /* Rx */
    if((UART_InstanceTable[HW_UART1]->S1 & UART_S1_RDRF_MASK) && (UART_InstanceTable[HW_UART1]->C2 & UART_C2_RIE_MASK))
    {
        ch = (uint8_t)UART_InstanceTable[HW_UART1]->D;
        if(UART_CallBackRxTable[HW_UART1])
        {
            UART_CallBackRxTable[HW_UART1](ch);
        }    
    }
}

#ifdef UART2
void UART2_RX_TX_IRQHandler(void)
{
    uint16_t ch;
    /* Tx */
    if((UART_InstanceTable[HW_UART2]->S1 & UART_S1_TDRE_MASK) && (UART_InstanceTable[HW_UART2]->C2 & UART_C2_TIE_MASK))
    {
        if(UART_CallBackTxTable[HW_UART2])
        {
            UART_CallBackTxTable[HW_UART2](&ch);
        }
        UART_InstanceTable[HW_UART2]->D = (uint8_t)ch;
    }
    /* Rx */
    if((UART_InstanceTable[HW_UART2]->S1 & UART_S1_RDRF_MASK) && (UART_InstanceTable[HW_UART2]->C2 & UART_C2_RIE_MASK))
    {
        ch = (uint8_t)UART_InstanceTable[HW_UART2]->D;
        if(UART_CallBackRxTable[HW_UART2])
        {
            UART_CallBackRxTable[HW_UART2](ch);
        }    
    }
}
#endif

#ifdef UART3
void UART3_RX_TX_IRQHandler(void)
{
    uint16_t ch;
    /* Tx */
    if((UART_InstanceTable[HW_UART3]->S1 & UART_S1_TDRE_MASK) && (UART_InstanceTable[HW_UART3]->C2 & UART_C2_TIE_MASK))
    {
        if(UART_CallBackTxTable[HW_UART3])
        {
            UART_CallBackTxTable[HW_UART3](&ch);
        }
        UART_InstanceTable[HW_UART3]->D = (uint8_t)ch;
    }
    /* Rx */
    if((UART_InstanceTable[HW_UART3]->S1 & UART_S1_RDRF_MASK) && (UART_InstanceTable[HW_UART3]->C2 & UART_C2_RIE_MASK))
    {
        ch = (uint8_t)UART_InstanceTable[HW_UART3]->D;
        if(UART_CallBackRxTable[HW_UART3])
        {
            UART_CallBackRxTable[HW_UART3](ch);
        }    
    }
}
#endif

#ifdef UART4
void UART4_RX_TX_IRQHandler(void)
{
    uint16_t ch;
    /* Tx */
    if((UART_InstanceTable[HW_UART4]->S1 & UART_S1_TDRE_MASK) && (UART_InstanceTable[HW_UART4]->C2 & UART_C2_TIE_MASK))
    {
        if(UART_CallBackTxTable[HW_UART4])
        {
            UART_CallBackTxTable[HW_UART4](&ch);
        }
        UART_InstanceTable[HW_UART4]->D = (uint8_t)ch;
    }
    /* Rx */
    if((UART_InstanceTable[HW_UART4]->S1 & UART_S1_RDRF_MASK) && (UART_InstanceTable[HW_UART4]->C2 & UART_C2_RIE_MASK))
    {
        ch = (uint8_t)UART_InstanceTable[HW_UART4]->D;
        if(UART_CallBackRxTable[HW_UART4])
        {
            UART_CallBackRxTable[HW_UART4](ch);
        }    
    }
}
#endif

#ifdef UART5
void UART5_RX_TX_IRQHandler(void)
{
    uint16_t ch;
    /* Tx */
    if((UART_InstanceTable[HW_UART5]->S1 & UART_S1_TDRE_MASK) && (UART_InstanceTable[HW_UART5]->C2 & UART_C2_TIE_MASK))
    {
        if(UART_CallBackTxTable[HW_UART5])
        {
            UART_CallBackTxTable[HW_UART5](&ch);
        }
        UART_InstanceTable[HW_UART5]->D = (uint8_t)ch;
    }
    /* Rx */
    if((UART_InstanceTable[HW_UART5]->S1 & UART_S1_RDRF_MASK) && (UART_InstanceTable[HW_UART5]->C2 & UART_C2_RIE_MASK))
    {
        ch = (uint8_t)UART_InstanceTable[HW_UART5]->D;
        if(UART_CallBackRxTable[HW_UART5])
        {
            UART_CallBackRxTable[HW_UART5](ch);
        }    
    }
}
#endif

/*
static const QuickInit_Type UART_QuickInitTable[] =
{
    { 1, 4, 3, 0, 2, 0}, //UART1_RX_PE01_TX_PE00
    { 0, 5, 4,18, 2, 0}, //UART0_RX_PF17_TX_PF18 4
    { 3, 4, 3, 4, 2, 0}, //UART3_RX_PE05_TX_PE04 3
    { 5, 5, 4,19, 2, 0}, //UART5_RX_PF19_TX_PF20 4
    { 5, 4, 3, 8, 2, 0}, //UART5_RX_PE09_TX_PE08 3
    { 2, 4, 3,16, 2, 0}, //UART2_RX_PE17_TX_PE16 3
    { 4, 4, 3,24, 2, 0}, //UART4_RX_PE25_TX_PE24 3
    { 0, 0, 2, 1, 2, 0}, //UART0_RX_PA01_TX_PA02 2
    { 0, 0, 3,14, 2, 0}, //UART0_RX_PA15_TX_PA14 3
    { 3, 1, 3,10, 2, 0}, //UART3_RX_PB10_TX_PB11 3
    { 0, 1, 3,16, 2, 0}, //UART0_RX_PB16_TX_PB17 3
    { 1, 2, 3, 3, 2, 0}, //UART1_RX_PC03_TX_PC04 3
    { 4, 2, 3,14, 2, 0}, //UART4_RX_PC14_TX_PC15 3
    { 3, 2, 3,16, 2, 0}, //UART3_RX_PC16_TX_PC17 3
    { 2, 3, 3, 2, 2, 0}, //UART2_RX_PD02_TX_PD03 3
    { 0, 3, 3, 6, 2, 0}, //UART0_RX_PD06_TX_PD07 3
    { 2, 5, 4,13, 2, 0}, //UART2_RX_PF13_TX_PF14 4
    { 5, 3, 3, 8, 2, 0}, //UART5_RX_PD08_TX_PD09 3
    { 5, 4, 3, 8, 2, 0}, //UART5_RX_PE08_TX_PE09 3
};
*/

#ifdef UART_USE_DMA
#include "dma.h"
static uint32_t DMA2UARTChlTable[5];

static const uint32_t _DMA_UARTTrigTable[] =
{
    UART0_TRAN_DMAREQ,
    UART1_TRAN_DMAREQ,
    UART2_TRAN_DMAREQ,
    UART3_TRAN_DMAREQ,
    UART4_TRAN_DMAREQ,
    UART5_TRAN_DMAREQ
};

static const void* _UART_DMA_sAddrTable[] = 
{
    (void*)&UART0->D,
    (void*)&UART1->D,
#ifdef  UART2   
    (void*)&UART2->D,
#endif
#ifdef  UART3   
    (void*)&UART3->D,
#endif 
#ifdef  UART4
    (void*)&UART4->D,
#endif
#ifdef  UART5
    (void*)&UART5->D,    
#endif
};

void UART_SetDMATxMode(uint32_t instance, bool status)
{
    /* init DMA */
    uint8_t dma_chl;
    
    if(status)
    {
        dma_chl = DMA_ChlAlloc();
        DMA_InitTypeDef DMA_InitStruct;
        DMA_InitStruct.chl = dma_chl;
        DMA_InitStruct.chlTriggerSource = _DMA_UARTTrigTable[instance];
        DMA_InitStruct.triggerSourceMode = kDMA_TriggerSource_Normal;
        DMA_InitStruct.minorLoopByteCnt = 1;
        DMA_InitStruct.majorLoopCnt = 0;
            
        DMA_InitStruct.sAddr = NULL;
        DMA_InitStruct.sLastAddrAdj = 0; 
        DMA_InitStruct.sAddrOffset = 1;
        DMA_InitStruct.sDataWidth = kDMA_DataWidthBit_8;
        DMA_InitStruct.sMod = kDMA_ModuloDisable;
        
        DMA_InitStruct.dAddr = (uint32_t)_UART_DMA_sAddrTable[instance]; 
        DMA_InitStruct.dLastAddrAdj = 0;
        DMA_InitStruct.dAddrOffset = 0;
        DMA_InitStruct.dDataWidth = kDMA_DataWidthBit_8;
        DMA_InitStruct.dMod = kDMA_ModuloDisable;

        DMA_Init(&DMA_InitStruct);
        DMA2UARTChlTable[instance] = dma_chl; 
    }
    else
    {
        DMA_ChlFree(DMA2UARTChlTable[instance]);
    }
    
    /* */
    UART_ITDMAConfig(instance, kUART_DMA_Tx, status);
}


void UART_DMASendByte(uint32_t instance, uint8_t* buf, uint32_t size)
{
    DMA_SetSourceAddress(DMA2UARTChlTable[instance], (uint32_t)buf);
    DMA_SetMajorLoopCounter(DMA2UARTChlTable[instance], size);
    
    /* start transfer */
    DMA_EnableRequest(DMA2UARTChlTable[instance]);
}

uint32_t UART_DMAGetRemainByte(uint32_t instance)
{
    return DMA_GetMajorLoopCount(DMA2UARTChlTable[instance]);
}


#endif
