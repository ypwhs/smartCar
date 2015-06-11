/**
  ******************************************************************************
  * @file    ftm.c
  * @author  YANDLD
  * @version V2.5
  * @date    2014.3.25
  * @brief   www.beyondcore.net   http://upcmcu.taobao.com 
  * @note    此文件为芯片FTM模块的底层功能函数
  ******************************************************************************
  */
#include "ftm.h"
#include "common.h"
#include "gpio.h"
#include <math.h>

/* leagacy support for Kineis Z Version(inital version) */
#if (!defined(FTM_BASES))

#ifdef FTM1
    #define FTM_BASES {FTM0, FTM1}
#elif  FTM2
    #define FTM_BASES {FTM0, FTM1, FTM2}
#elif  FTM3
    #define FTM_BASES {FTM0, FTM1, FTM2, FTM3}
#else
    #define FTM_BASES {FTM0}     
#endif

#endif


/* global vars */
static FTM_Type * const FTM_InstanceTable[] = FTM_BASES;
static FTM_CallBackType FTM_CallBackTable[ARRAY_SIZE(FTM_InstanceTable)] = {NULL};


static const uint32_t FTM_ChlMaxTable[] = {8,2,2}; /* reference to chip configuration->flextimer configuration */
static const struct reg_ops SIM_FTMClockGateTable[] =
{
    {(void*)&(SIM->SCGC6), SIM_SCGC6_FTM0_MASK},
    {(void*)&(SIM->SCGC6), SIM_SCGC6_FTM1_MASK},
#ifdef FTM2
    {(void*)&(SIM->SCGC3), SIM_SCGC3_FTM2_MASK},
#endif
#ifdef FTM3
    {(void*)&(SIM->SCGC3), SIM_SCGC3_FTM3_MASK},
#endif  
};
static const IRQn_Type FTM_IRQnTable[] = 
{
    FTM0_IRQn,
    FTM1_IRQn,
#ifdef FTM2
    FTM2_IRQn,
#endif
};


/* FTM dual pin mode select */
typedef enum
{
	kFTM_Combine,
    kFTM_Complementary,
    kFTM_DualEdgeCapture,
    kFTM_DeadTime,
    kFTM_Sync,
    kFTM_FaultControl,
}FTM_DualChlConfig_Type;

/* internal use, FTM mode select */
typedef enum
{
    kFTM_Mode_EdgeAligned,
    kFTM_Mode_CenterAligned,
    kFTM_Mode_Combine,
    kFTM_Mode_Complementary,
    kFTM_Mode_InputCapture,
    kFTM_Mode_QuadratureDecoder,
}FTM_Mode_Type;

/* static functions declareation */
static void FTM_SetMode(uint32_t instance, uint8_t chl, FTM_Mode_Type mode);
void FTM_PWM_InvertPolarity(uint32_t instance, uint8_t chl, uint32_t config);

static void _FTM_InitBasic(uint32_t instance, uint32_t modulo, FTM_ClockDiv_Type ps)
{
    /* enable clock gate */
    *(uint32_t*)SIM_FTMClockGateTable[instance].addr |= SIM_FTMClockGateTable[instance].mask;
    
    /* disable FTM, we must set CLKS(0) before config FTM! */
    FTM_InstanceTable[instance]->SC = 0;
    
    /* enable to access all register including enhancecd register(FTMEN bit control whather can access FTM enhanced function) */
    FTM_InstanceTable[instance]->MODE |= FTM_MODE_WPDIS_MASK;
    
    /* set CNT and CNTIN */
    FTM_InstanceTable[instance]->CNT = 0;
    FTM_InstanceTable[instance]->CNTIN = 0;
    
    /* set modulo */
    FTM_InstanceTable[instance]->MOD = modulo;
    
    /* set LOCK bit to load MOD value */
    FTM_InstanceTable[instance]->PWMLOAD = 0xFFFFFFFF;
    
    /* set FTM clock to system clock */
    FTM_InstanceTable[instance]->SC &= ~FTM_SC_CLKS_MASK;
    FTM_InstanceTable[instance]->SC |= FTM_SC_CLKS(1);
    
    /* set ps, this must be done after set modulo */
    FTM_InstanceTable[instance]->SC &= ~FTM_SC_PS_MASK;
    FTM_InstanceTable[instance]->SC |= FTM_SC_PS(ps); 
}


/**
 * @brief  快速配置初始化FTM模块实现正交解码功能       
 * @param  MAP  : FTM工作在正交解码模式下的编码，详见ftm.h文件
 * @retval None
 */
uint32_t FTM_QD_QuickInit(uint32_t MAP, FTM_QD_PolarityMode_Type polarity, FTM_QD_Mode_Type mode)
{
    uint8_t i;
    QuickInit_Type * pq = (QuickInit_Type*)&(MAP);
    
    /* init moudle */
    _FTM_InitBasic(pq->ip_instance, FTM_MOD_MOD_MASK, kFTM_ClockDiv16);
    
    /* set FTM to QD mode */
    FTM_SetMode(pq->ip_instance, 0, kFTM_Mode_QuadratureDecoder);
    
    /* QD mode config */
    switch(polarity)
    {
        case kFTM_QD_NormalPolarity:
            FTM_InstanceTable[pq->ip_instance]->QDCTRL &= ~FTM_QDCTRL_PHAPOL_MASK;
            FTM_InstanceTable[pq->ip_instance]->QDCTRL &= ~FTM_QDCTRL_PHBPOL_MASK;
            break;
        case kFTM_QD_InvertedPolarity:
            FTM_InstanceTable[pq->ip_instance]->QDCTRL |= FTM_QDCTRL_PHAPOL_MASK;
            FTM_InstanceTable[pq->ip_instance]->QDCTRL |= FTM_QDCTRL_PHBPOL_MASK;
            break;
        default:
            break;
    }
    switch(mode)
    {
        case kQD_PHABEncoding:
            FTM_InstanceTable[pq->ip_instance]->QDCTRL &= ~FTM_QDCTRL_QUADMODE_MASK;
            break;
        case kQD_CountDirectionEncoding:
            FTM_InstanceTable[pq->ip_instance]->QDCTRL |= FTM_QDCTRL_QUADMODE_MASK;
            break;
        default:
            break;
    }
    /* init pinmux and pull up */
    for(i = 0; i < pq->io_offset; i++)
    {
        PORT_PinMuxConfig(pq->io_instance, pq->io_base + i, (PORT_PinMux_Type) pq->mux);
        PORT_PinPullConfig(pq->io_instance, pq->io_base + i, kPullUp);
        PORT_PinOpenDrainConfig(pq->io_instance, pq->io_base + i, ENABLE);
    }
    /* init moudle */
    return pq->ip_instance;
}

/**
 * @brief  获得正交解码的数据     
 * @param  instance  :FTM模块号
 * @param  value     :脉冲数据存储地址
 * @param  direction :脉冲方向存储地址
 * @retval None
 */
void FTM_QD_GetData(uint32_t instance, int* value, uint8_t* direction)
{
    *direction = (FTM_InstanceTable[instance]->QDCTRL>>FTM_QDCTRL_QUADIR_SHIFT & 1);
	*value = (FTM_InstanceTable[instance]->CNT & 0xFFFF);
}

void FTM_QD_ClearCount(uint32_t instance)
{
    /* write any value to CNT will set CNT to CNTIN */
    FTM_InstanceTable[instance]->CNT = 0;
}


/*combine channel control*/
/*dual capture control*/
/**
 * @brief  内部函数，用户无需调用
 */
static uint32_t get_channel_pair_index(uint8_t channel)
{
    if((channel == HW_FTM_CH0) || (channel == HW_FTM_CH1))
    {
        return 0;
    }
    else if((channel == HW_FTM_CH2) || (channel == HW_FTM_CH3)) 
    {
        return 1;
    }
    else if((channel == HW_FTM_CH4) || (channel == HW_FTM_CH5)) 
    {
        return 2;
    }
    else 
    {
        return 3;
    }
}


/*!
 * @brief enable FTM peripheral timer channel pair output combine mode.
 * @param instance The FTM peripheral instance number.
 * @param channel  The FTM peripheral channel number.
 * @param enable  true to enable channle pair to combine, false to disable.
 */
/**
 * @brief  内部函数，用户无需调用
 */
#define FTM_COMBINE_CHAN_CTRL_WIDTH  (8)
static void FTM_DualChlConfig(uint32_t instance, uint8_t chl, FTM_DualChlConfig_Type mode, FunctionalState newState)
{
    uint32_t mask = 0;
    switch(mode)
    {
        case kFTM_Combine:
            mask = FTM_COMBINE_COMBINE0_MASK << (get_channel_pair_index(chl) * FTM_COMBINE_CHAN_CTRL_WIDTH);
            break;
        case kFTM_Complementary:
            mask = FTM_COMBINE_COMP0_MASK << (get_channel_pair_index(chl) * FTM_COMBINE_CHAN_CTRL_WIDTH);
            break;    
        case kFTM_DualEdgeCapture:
            mask = FTM_COMBINE_DECAPEN0_MASK << (get_channel_pair_index(chl) * FTM_COMBINE_CHAN_CTRL_WIDTH);
            break;           
        case kFTM_DeadTime:
            mask = FTM_COMBINE_DTEN0_MASK << (get_channel_pair_index(chl) * FTM_COMBINE_CHAN_CTRL_WIDTH);
            break;    
        case kFTM_Sync:
            mask = FTM_COMBINE_SYNCEN0_MASK << (get_channel_pair_index(chl) * FTM_COMBINE_CHAN_CTRL_WIDTH);
            break;   
        case kFTM_FaultControl:
            mask = FTM_COMBINE_FAULTEN0_MASK << (get_channel_pair_index(chl) * FTM_COMBINE_CHAN_CTRL_WIDTH);
            break;    
        default:
            break;
    } 
    (newState == ENABLE)?(FTM_InstanceTable[instance]->COMBINE |= mask):(FTM_InstanceTable[instance]->COMBINE &= ~mask);
}

//!< 设置FTM 工作模式
/**
 * @brief  内部函数，用户无需调用
 */
static void FTM_SetMode(uint32_t instance, uint8_t chl, FTM_Mode_Type mode)
{
    switch(mode)
    {
        case kFTM_Mode_EdgeAligned:
            FTM_InstanceTable[instance]->MODE &= ~FTM_MODE_FTMEN_MASK;
            FTM_InstanceTable[instance]->QDCTRL &= ~FTM_QDCTRL_QUADEN_MASK;
            FTM_InstanceTable[instance]->SC &= ~FTM_SC_CPWMS_MASK;
            FTM_InstanceTable[instance]->CONTROLS[chl].CnSC |= (FTM_CnSC_MSB_MASK|FTM_CnSC_MSA_MASK);
            FTM_PWM_InvertPolarity(instance, chl, kFTM_PWM_HighTrue);
            FTM_DualChlConfig(instance, chl, kFTM_Combine, DISABLE);
            FTM_DualChlConfig(instance, chl, kFTM_Complementary, DISABLE);
            FTM_DualChlConfig(instance, chl, kFTM_DualEdgeCapture, DISABLE);
            FTM_DualChlConfig(instance, chl, kFTM_DeadTime, DISABLE);
            FTM_DualChlConfig(instance, chl, kFTM_Sync, DISABLE);
            FTM_DualChlConfig(instance, chl, kFTM_FaultControl, DISABLE);
            break;
        case kFTM_Mode_CenterAligned:
            FTM_InstanceTable[instance]->MODE &= ~FTM_MODE_FTMEN_MASK;
            FTM_InstanceTable[instance]->QDCTRL &= ~FTM_QDCTRL_QUADEN_MASK;
            FTM_InstanceTable[instance]->SC |= FTM_SC_CPWMS_MASK;
            FTM_DualChlConfig(instance, chl, kFTM_Combine, DISABLE);
            FTM_DualChlConfig(instance, chl, kFTM_Complementary, DISABLE);
            FTM_DualChlConfig(instance, chl, kFTM_DualEdgeCapture, DISABLE);
            FTM_DualChlConfig(instance, chl, kFTM_DeadTime, DISABLE);
            FTM_DualChlConfig(instance, chl, kFTM_Sync, DISABLE);
            FTM_DualChlConfig(instance, chl, kFTM_FaultControl, DISABLE);
            break;
        case kFTM_Mode_InputCapture:   
            /* all configuration on input capture */
            FTM_InstanceTable[instance]->QDCTRL &= ~FTM_QDCTRL_QUADEN_MASK;
            FTM_InstanceTable[instance]->MODE &= ~FTM_MODE_FTMEN_MASK;
            FTM_InstanceTable[instance]->SC &= ~FTM_SC_CPWMS_MASK; 
            FTM_InstanceTable[instance]->CONTROLS[chl].CnSC &= ~(FTM_CnSC_MSB_MASK|FTM_CnSC_MSA_MASK);
            FTM_DualChlConfig(instance, chl, kFTM_Combine, DISABLE);
            FTM_DualChlConfig(instance, chl, kFTM_Complementary, DISABLE);
            FTM_DualChlConfig(instance, chl, kFTM_DualEdgeCapture, DISABLE);
            FTM_DualChlConfig(instance, chl, kFTM_DeadTime, DISABLE);
            FTM_DualChlConfig(instance, chl, kFTM_Sync, DISABLE);
            FTM_DualChlConfig(instance, chl, kFTM_FaultControl, DISABLE);
            break;
        case kFTM_Mode_Complementary:
            FTM_InstanceTable[instance]->QDCTRL &= ~FTM_QDCTRL_QUADEN_MASK;
            FTM_InstanceTable[instance]->SC &= ~FTM_SC_CPWMS_MASK;
            FTM_InstanceTable[instance]->MODE |= FTM_MODE_WPDIS_MASK;
            FTM_InstanceTable[instance]->MODE |= FTM_MODE_FTMEN_MASK;
            FTM_DualChlConfig(instance, chl, kFTM_Combine, ENABLE);
            FTM_DualChlConfig(instance, chl, kFTM_Complementary, ENABLE);
            FTM_DualChlConfig(instance, chl, kFTM_DualEdgeCapture, DISABLE);
            FTM_DualChlConfig(instance, chl, kFTM_DeadTime, DISABLE);
            FTM_DualChlConfig(instance, chl, kFTM_Sync, ENABLE);
            FTM_DualChlConfig(instance, chl, kFTM_FaultControl, DISABLE);
            FTM_InstanceTable[instance]->CONTROLS[chl].CnSC |= (FTM_CnSC_MSB_MASK|FTM_CnSC_MSA_MASK);
            FTM_PWM_InvertPolarity(instance, chl, kFTM_PWM_HighTrue);
            FTM_InstanceTable[instance]->SYNC = FTM_SYNC_CNTMIN_MASK|FTM_SYNC_CNTMAX_MASK;
			FTM_InstanceTable[instance]->SYNC |= FTM_SYNC_SWSYNC_MASK;
            break;
        case kFTM_Mode_Combine:
            FTM_InstanceTable[instance]->QDCTRL &= ~FTM_QDCTRL_QUADEN_MASK;
            FTM_InstanceTable[instance]->SC &= ~FTM_SC_CPWMS_MASK;
            FTM_InstanceTable[instance]->MODE |= FTM_MODE_WPDIS_MASK;
            FTM_InstanceTable[instance]->MODE |= FTM_MODE_FTMEN_MASK;
            FTM_DualChlConfig(instance, chl, kFTM_Combine, ENABLE);
            FTM_DualChlConfig(instance, chl, kFTM_Complementary, DISABLE);
            FTM_DualChlConfig(instance, chl, kFTM_DualEdgeCapture, DISABLE);
            FTM_DualChlConfig(instance, chl, kFTM_DeadTime, DISABLE);
            FTM_DualChlConfig(instance, chl, kFTM_Sync, ENABLE);
            FTM_DualChlConfig(instance, chl, kFTM_FaultControl, DISABLE);
            FTM_InstanceTable[instance]->CONTROLS[chl].CnSC |= (FTM_CnSC_MSB_MASK|FTM_CnSC_MSA_MASK);
            FTM_PWM_InvertPolarity(instance, chl, kFTM_PWM_HighTrue);
            FTM_InstanceTable[instance]->SYNC = FTM_SYNC_CNTMIN_MASK|FTM_SYNC_CNTMAX_MASK;
			FTM_InstanceTable[instance]->SYNC |= FTM_SYNC_SWSYNC_MASK;
            break;   
        case  kFTM_Mode_QuadratureDecoder:
            FTM_InstanceTable[instance]->QDCTRL |= FTM_QDCTRL_QUADEN_MASK;
            FTM_InstanceTable[instance]->SC &= ~FTM_SC_CPWMS_MASK;
            FTM_InstanceTable[instance]->MODE |= FTM_MODE_WPDIS_MASK;
            FTM_InstanceTable[instance]->MODE |= FTM_MODE_FTMEN_MASK;
            FTM_DualChlConfig(instance, chl, kFTM_Combine, ENABLE);
            FTM_DualChlConfig(instance, chl, kFTM_Complementary, DISABLE);
            FTM_DualChlConfig(instance, chl, kFTM_DualEdgeCapture, DISABLE);
            FTM_DualChlConfig(instance, chl, kFTM_DeadTime, DISABLE);
            FTM_DualChlConfig(instance, chl, kFTM_Sync, DISABLE);
            FTM_DualChlConfig(instance, chl, kFTM_FaultControl, DISABLE);
            break;
        default:
            break;
    }
}

//!< 翻转 FTM极性
/**
 * @brief  内部函数，用户无需调用
 */
void FTM_PWM_InvertPolarity(uint32_t instance, uint8_t chl, uint32_t config)
{
    switch(config)
    {
        case kFTM_PWM_HighTrue:
            FTM_InstanceTable[instance]->CONTROLS[chl].CnSC &= ~FTM_CnSC_ELSA_MASK;
            FTM_InstanceTable[instance]->CONTROLS[chl].CnSC |= FTM_CnSC_ELSB_MASK;
            break;
        case kFTM_PWM_LowTrue:
            FTM_InstanceTable[instance]->CONTROLS[chl].CnSC |= FTM_CnSC_ELSA_MASK;
            FTM_InstanceTable[instance]->CONTROLS[chl].CnSC &= ~FTM_CnSC_ELSB_MASK;
            break;
        default:
            break;
    }
}


/**
 * @brief  快速配置初始化FTM模块实现PWM功能
 * @code
 *      
 * //设置FTM0模块的3通道在PTA6引脚中产生1000HZ的pwm波形
 *      FTM_PWM_QuickInit(FTM0_CH3_PA06, 1000); 
 * @endcode         
 * @param  MAP        : FTM工作在PWM模式下的编码，详见ftm.h文件
 * @param  frequencyInHZ  : FTM工作工作频率设置
 * @retval None
 */
uint8_t FTM_PWM_QuickInit(uint32_t MAP, FTM_PWM_Mode_Type mode, uint32_t req)
{
    uint8_t i;
    uint32_t modulo;
    uint32_t clock;
    int32_t pres;
    uint8_t ps = 0;
    QuickInit_Type * pq = (QuickInit_Type*)&(MAP); 
    /* init pinmux */
    for(i = 0; i < pq->io_offset; i++)
    {
        PORT_PinMuxConfig(pq->io_instance, pq->io_base + i, (PORT_PinMux_Type) pq->mux); 
    }
    /* calc req and ps */
    uint32_t min_val = 0xFFFF;
    /* cal ps */
    CLOCK_GetClockFrequency(kBusClock, &clock);
    pres = (clock/req)/FTM_MOD_MOD_MASK;
    if((clock/req)/pres > FTM_MOD_MOD_MASK)
    {
        pres++;
    }
    for(i = 0; i < 7; i++)
    {
        if((ABS(pres - (1<<i))) < min_val)
        {
            ps = i;
            min_val = ABS(pres - (1<<i));
        }
    }
    if(pres > (1<<ps)) ps++;
    if(ps > 7) ps = 7;
    modulo = ((clock/(1<<ps))/req) - 1;
    LIB_TRACE("input frequency:%dHz\r\n", req);
    LIB_TRACE("input clk:%d\r\n", clock);
    LIB_TRACE("ps:%d\r\n", pres);
    LIB_TRACE("modulo:%d\r\n", modulo);
    _FTM_InitBasic(pq->ip_instance, modulo, (FTM_ClockDiv_Type)ps);
    /* set FTM mode */
    switch(mode)
    {
        case kPWM_EdgeAligned:
            FTM_SetMode(pq->ip_instance, pq->channel, kFTM_Mode_EdgeAligned);
            break;
        case kPWM_Combine:
            FTM_SetMode(pq->ip_instance, pq->channel, kFTM_Mode_Combine);
            break;
        case kPWM_Complementary:
            FTM_SetMode(pq->ip_instance, pq->channel, kFTM_Mode_Complementary);
            break;
        default:
            LIB_TRACE("error in FTM_PWM_Init\r\n");
            break;
    }
    /* set duty to 50% */
    FTM_PWM_ChangeDuty(pq->ip_instance, pq->channel, 5000);
    return pq->ip_instance;
}

/**
 * @brief  更改指定引脚的PWM波形占空比
 * @code
 *      //设置FTM0模块的3通道的PWM波形占空比为50%
 *      FTM_PWM_ChangeDuty(HW_FTM0, 3, 5000); 
 * @endcode         
 * @param  instance       : 模块号
 * @param  chl            : FTM 通道号
 * @param  pwmDuty        : 占空比
 * @retval None
 */
void FTM_PWM_ChangeDuty(uint32_t instance, uint8_t chl, uint32_t pwmDuty)
{
    uint32_t cv = ((FTM_InstanceTable[instance]->MOD) * pwmDuty) / 10000;
    /* combine mode */
    if(FTM_InstanceTable[instance]->COMBINE & (FTM_COMBINE_COMBINE0_MASK|FTM_COMBINE_COMBINE1_MASK|FTM_COMBINE_COMBINE2_MASK|FTM_COMBINE_COMBINE3_MASK))
    {
        if(chl%2)
        {
            FTM_InstanceTable[instance]->CONTROLS[chl-1].CnV = 0;
            FTM_InstanceTable[instance]->CONTROLS[chl].CnV = cv;
        }
        else
        {
            FTM_InstanceTable[instance]->CONTROLS[chl].CnV = 0;
            FTM_InstanceTable[instance]->CONTROLS[chl+1].CnV = cv;  
        }
        FTM_InstanceTable[instance]->PWMLOAD = 0xFFFFFFFF;
    }
    else
    {
        /* single chl */
        FTM_InstanceTable[instance]->CONTROLS[chl].CnV = cv; 
    }
}

/**
 * @brief  初始化FTM 输入捕捉功能
 * @param  MAP          : 快速初始化通道列表
 * @param  ps           : 分频
 * @retval None
 */
void FTM_IC_QuickInit(uint32_t MAP, FTM_ClockDiv_Type ps)
{
    uint32_t i;
    QuickInit_Type * pq = (QuickInit_Type*)&(MAP);
    /* init pinmux */
    for(i = 0; i < pq->io_offset; i++)
    {
        PORT_PinMuxConfig(pq->io_instance, pq->io_base + i, (PORT_PinMux_Type) pq->mux); 
    }
    _FTM_InitBasic(pq->ip_instance, FTM_MOD_MOD_MASK, ps);
    FTM_SetMode(pq->ip_instance, pq->channel, kFTM_Mode_InputCapture);
}

/**
 * @brief  设置输入捕捉触发模式
 * @param  instance     : 模块号
 * @param  chl          : 通道
 * @param  mode         : 触发模式
 * @retval None
 */
void FTM_IC_SetTriggerMode(uint32_t instance, uint32_t chl, FTM_IC_Mode_Type mode)
{
    /* clear ELSB & ELSA */
    FTM_InstanceTable[instance]->CONTROLS[chl].CnSC &= ~(FTM_CnSC_ELSB_MASK|FTM_CnSC_ELSA_MASK);
    switch(mode)
    {
        case kFTM_IC_FallingEdge:
            FTM_InstanceTable[instance]->CONTROLS[chl].CnSC &= ~(FTM_CnSC_ELSB_MASK);
            FTM_InstanceTable[instance]->CONTROLS[chl].CnSC |= (FTM_CnSC_ELSA_MASK);
            break;
        case kFTM_IC_RisingEdge:
            FTM_InstanceTable[instance]->CONTROLS[chl].CnSC |= (FTM_CnSC_ELSB_MASK);
            FTM_InstanceTable[instance]->CONTROLS[chl].CnSC &= ~(FTM_CnSC_ELSA_MASK);
            break;
        case kFTM_IC_RisingFallingEdge:
            FTM_InstanceTable[instance]->CONTROLS[chl].CnSC |= (FTM_CnSC_ELSB_MASK|FTM_CnSC_ELSA_MASK);
            break;
        default:
            break;
    }
}

/**
 * @brief  获得FTM通道计数值
 * @param  instance     : 模块号
 * @param  chl          : 通道
 * @retval 通道Counter值
 */
uint32_t FTM_GetChlCounter(uint32_t instance, uint32_t chl)
{
    return FTM_InstanceTable[instance]->CONTROLS[chl].CnV;
}

/**
 * @brief  设置FTM主计数Counter值
 * @param  instance     : 模块号
 * @param  val          : Value
 * @retval None
 */
void FTM_SetMoudleCounter(uint32_t instance, uint32_t val)
{
    FTM_InstanceTable[instance]->CNT = val;
}

/**
 * @brief  设置FTM回调函数
 * @param  instance     : 模块号
 * @param  AppCBFun     : 回调函数指针
 * @retval None
 */
void FTM_CallbackInstall(uint32_t instance, FTM_CallBackType AppCBFun)
{
    if(AppCBFun != NULL)
    {
        FTM_CallBackTable[instance] = AppCBFun;
    }
}

/**
 * @brief  FTM中断DMA控制
 * @param  instance     : 模块号
 * @param  config       : 控制参数
 * @param  flag         : 时能或者禁止
 * @retval None
 */
void FTM_ITDMAConfig(uint32_t instance, FTM_ITDMAConfig_Type config, bool flag)
{
    switch(config)
    {
        case kFTM_IT_TOF:
            (flag)?(FTM_InstanceTable[instance]->SC |= FTM_SC_TOIE_MASK):(FTM_InstanceTable[instance]->SC &= ~FTM_SC_TOIE_MASK);
            break;
        case kFTM_IT_CH0:
        case kFTM_IT_CH1:
        case kFTM_IT_CH2:
        case kFTM_IT_CH3:
        case kFTM_IT_CH4:
        case kFTM_IT_CH5:
        case kFTM_IT_CH6:
        case kFTM_IT_CH7: 
            (flag)?(FTM_InstanceTable[instance]->CONTROLS[config-1].CnSC |= FTM_CnSC_CHIE_MASK):(FTM_InstanceTable[instance]->CONTROLS[config-1].CnSC &= ~FTM_CnSC_CHIE_MASK);
            break;
        case kFTM_DMA_CH0:
        case kFTM_DMA_CH1:
        case kFTM_DMA_CH2:
        case kFTM_DMA_CH3:
        case kFTM_DMA_CH4:
        case kFTM_DMA_CH5:
        case kFTM_DMA_CH6:
        case kFTM_DMA_CH7: 
            (flag)?(FTM_InstanceTable[instance]->CONTROLS[config-1-8].CnSC |= FTM_CnSC_CHIE_MASK):(FTM_InstanceTable[instance]->CONTROLS[config-1-8].CnSC &= ~FTM_CnSC_CHIE_MASK);
            (flag)?(FTM_InstanceTable[instance]->CONTROLS[config-1-8].CnSC |= FTM_CnSC_DMA_MASK):(FTM_InstanceTable[instance]->CONTROLS[config-1-8].CnSC &= ~FTM_CnSC_DMA_MASK);
            break;
        default:
            LIB_TRACE("FTM_ITDMAConfig error!\r\n");
            break;
    }
    NVIC_EnableIRQ(FTM_IRQnTable[instance]);
}

void FTM0_IRQHandler(void)
{
    uint32_t i;
    if(FTM_CallBackTable[0])
    {
        FTM_CallBackTable[0]();
    }
    /* clear pending register */
    if(FTM_InstanceTable[0]->SC & (FTM_SC_TOF_MASK | FTM_SC_TOIE_MASK))
    {
        FTM_InstanceTable[0]->SC &= ~FTM_SC_TOF_MASK;
    }
    for(i = 0; i < FTM_ChlMaxTable[0]; i++)
    {
        if(FTM_InstanceTable[0]->CONTROLS[i].CnSC & (FTM_CnSC_CHIE_MASK | FTM_CnSC_CHF_MASK))
        {
            FTM_InstanceTable[0]->CONTROLS[i].CnSC &= ~FTM_CnSC_CHF_MASK;
            break;
        }
    }
}

void FTM1_IRQHandler(void)
{
    uint32_t i;
    if(FTM_CallBackTable[1])
    {
        FTM_CallBackTable[1]();
    }
    /* clear pending register */
    if(FTM_InstanceTable[1]->SC & (FTM_SC_TOF_MASK | FTM_SC_TOIE_MASK))
    {
        FTM_InstanceTable[1]->SC |= FTM_SC_TOF_MASK;
    }
    for(i = 0; i < FTM_ChlMaxTable[1]; i++)
    {
        if(FTM_InstanceTable[1]->CONTROLS[i].CnSC & (FTM_CnSC_CHIE_MASK | FTM_CnSC_CHF_MASK))
        {
            FTM_InstanceTable[1]->CONTROLS[i].CnSC &= ~FTM_CnSC_CHF_MASK;
            break;
        }
    }
}

void FTM2_IRQHandler(void)
{
    uint32_t i;
    if(FTM_CallBackTable[2])
    {
        FTM_CallBackTable[2]();
    }
    /* clear pending register */
    if(FTM_InstanceTable[2]->SC & (FTM_SC_TOF_MASK | FTM_SC_TOIE_MASK))
    {
        FTM_InstanceTable[2]->SC &= ~FTM_SC_TOF_MASK;
    }
    for(i = 0; i < FTM_ChlMaxTable[2]; i++)
    {
        if(FTM_InstanceTable[2]->CONTROLS[i].CnSC & (FTM_CnSC_CHIE_MASK | FTM_CnSC_CHF_MASK))
        {
            FTM_InstanceTable[2]->CONTROLS[i].CnSC &= ~FTM_CnSC_CHF_MASK;
            break;
        }
    }
}


/*
static const QuickInit_Type FTM_QD_QuickInitTable[] = 
{
    { 1, 0, 6,  8, 2, 0}, //FTM1_QD_PHA_PA08_PHB_PA09 6
    { 1, 0, 7, 12, 2, 0}, //FTM1_QD_PHA_PA12_PHB_PA13 7
    { 1, 1, 6,  0, 2, 0}, //FTM1_QD_PHA_PB00_PHB_PB01  6
    { 2, 0, 6, 10, 2, 0}, //FTM2_QD_PHA_PA10_PHB_PA11  6
    { 2, 1, 6, 18, 2, 0}, //FTM2_QD_PHA_PB18_PHB_PB19 6
};

static const QuickInit_Type FTM_QuickInitTable[] =
{
    { 0, 1, 4, 12, 1, 4}, //FTM0_CH4_PB12 4
    { 0, 1, 4, 13, 1, 5}, //FTM0_CH5_PB13  4
    { 0, 0, 3,  0, 1, 5}, //FTM0_CH5_PA00  3
    { 0, 0, 3,  1, 1, 6}, //FTM0_CH6_PA01  3
    { 0, 0, 3,  2, 1, 7}, //FTM0_CH7_PA02  3
    { 0, 0, 3,  3, 1, 0}, //FTM0_CH0_PA03  3
    { 0, 0, 3,  4, 1, 1}, //FTM0_CH1_PA04  3
    { 0, 0, 3,  5, 1, 2}, //FTM0_CH2_PA05  3
    { 0, 0, 3,  6, 1, 3}, //FTM0_CH3_PA06  3
    { 0, 0, 3,  7, 1, 4}, //FTM0_CH4_PA07  3
    { 0, 2, 4,  1, 1, 0}, //FTM0_CH0_PC01  4
    { 0, 2, 4,  2, 1, 1}, //FTM0_CH1_PC02  4
    { 0, 2, 4,  3, 1, 2}, //FTM0_CH2_PC03  4
    { 0, 2, 4,  4, 1, 3}, //FTM0_CH3_PC04  4
    { 0, 3, 4,  4, 1, 4}, //FTM0_CH4_PD04  4
    { 0, 3, 4,  5, 1, 5}, //FTM0_CH5_PD05  4
    { 0, 3, 4,  6, 1, 6}, //FTM0_CH6_PD06  4
    { 0, 3, 4,  7, 1, 7}, //FTM0_CH7_PD07  4
    { 1, 1, 3, 12, 1, 0}, //FTM1_CH0_PB12  3
    { 1, 1, 3, 13, 1, 1}, //FTM1_CH1_PB13  3
    { 1, 0, 3,  8, 1, 0}, //FTM1_CH0_PA08  3
    { 1, 0, 3,  9, 1, 1}, //FTM1_CH1_PA09  3
    { 1, 0, 3, 12, 1, 0}, //FTM1_CH0_PA12  3
    { 1, 0, 3, 12, 1, 1}, //FTM1_CH1_PA13  3
    { 1, 1, 3,  0, 1, 0}, //FTM1_CH0_PB00  3
    { 1, 1, 3,  1, 1, 1}, //FTM1_CH1_PB01  3
    { 2, 0, 3, 10, 1, 0}, //FTM2_CH0_PA10  3
    { 2, 0, 3, 11, 1, 1}, //FTM2_CH1_PA11  3
    { 2, 1, 3, 18, 1, 0}, //FTM2_CH0_PB18  3
    { 2, 1, 3, 19, 1, 1}, //FTM2_CH1_PB19  3
};
*/

