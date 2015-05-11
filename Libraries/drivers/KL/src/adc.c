/**
  ******************************************************************************
  * @file    adc.c
  * @author  YANDLD
  * @version V2.5
  * @date    2014.3.25
  * @brief   www.beyondcore.net   http://upcmcu.taobao.com 
  ******************************************************************************
  */
  
#include "adc.h"
#include "gpio.h"


ADC_Type * const ADC_InstanceTable[] = ADC_BASES;
static ADC_CallBackType ADC_CallBackTable[ARRAY_SIZE(ADC_InstanceTable)] = {NULL};

static const struct reg_ops SIM_ADCClockGateTable[] =
{
    {(void*)&(SIM->SCGC6), SIM_SCGC6_ADC0_MASK},
};

static const IRQn_Type ADC_IRQnTable[] = 
{
    ADC0_IRQn,
};

/**
 * @brief  快速完成AD初始化配置
 * @code
 *    //初始化 ADC0 通道20 引脚DM1 单端 精度 12位
 *    ADC_QuickInit(ADC0_SE20_DM1, kADC_SingleDiff12or13);
 *    //读取AD转换结果
 *    value = ADC_QuickReadValue(ADC0_SE20_DM1);
 * @endcode
 * @param  MAP: 快速初始化宏，详见ADC.H文件
 * @param  resolutionMode: 转换分辨率设置
 *         @arg kADC_SingleDiff8or9   :转换精度为8/9位
 *         @arg kADC_SingleDiff10or11 :转换精度为10/11位
 *         @arg kADC_SingleDiff12or13 :转换精度为12/13位
 *         @arg kADC_SingleDIff16     :转换精度为16位
 * @retval ADC模块号
 */
uint8_t ADC_QuickInit(uint32_t MAP, ADC_ResolutionMode_Type resolutionMode)
{
    uint8_t i;
    QuickInit_Type * pq = (QuickInit_Type*)&(MAP);
    ADC_InitTypeDef AD_InitStruct1;
    AD_InitStruct1.instance = pq->ip_instance;
    AD_InitStruct1.clockDiv = kADC_ClockDiv2;
    AD_InitStruct1.resolutionMode = resolutionMode;
    AD_InitStruct1.triggerMode = kADC_TriggerSoftware;
    AD_InitStruct1.singleOrDiffMode = kADC_Single;
    AD_InitStruct1.continueMode = kADC_ContinueConversionDisable;
    AD_InitStruct1.hardwareAveMode = kADC_HardwareAverageDisable;
    AD_InitStruct1.vref = kADC_VoltageVREF;
    
    /* init pinmux */
    for(i = 0; i < pq->io_offset; i++)
    {
        PORT_PinMuxConfig(pq->io_instance, pq->io_base + i, (PORT_PinMux_Type) pq->mux);
        PORT_PinPullConfig(pq->io_instance, pq->io_base + i, kPullDisabled); 
    }
    /* init moudle */
    ADC_Init(&AD_InitStruct1);
    
    /* init adc chlmux */
    ADC_ChlMuxConfig(pq->ip_instance, pq->reserved);
    
    return pq->ip_instance;
}


/**
 * @brief  判断AD转换是否结束
 * @code
 *      //查看ADC0 模块的A通道的转换是否完成
 *      ADC_IsConversionCompleted(HW_ADC0, kADC_MuxA); 
 * @endcode         
 * @param  instance: ADC 模块号
 *         @arg HW_ADC0  :ADC0模块
 *         @arg HW_ADC1  :ADC1模块
 *         @arg HW_ADC2  :ADC2模块
 * @param  mux: ADC 转换器通道 复用 选择
 *         @arg kADC_MuxA   :A通道模式
 *         @arg kADC_MuxB   :B通道模式
 * @retval 
 *         @arg 0:转换完成
 *         @arg 1:转换未完成
 */
uint8_t ADC_IsConversionCompleted(uint32_t instance, uint32_t mux)
{
    if(ADC_InstanceTable[instance]->SC1[mux] & ADC_SC1_COCO_MASK)
    {
        return 0;
    }
    return 1; 
}

/**
 * @brief  读取ADC转换数据
 * @note   立即返回 非阻塞式 
 * @code
 *    //读取 ADC0模块下的在A模式下数据转换结果
 *    uint32_t data;   //存储转换结果
 *    data = ADC_ReadValue(HW_ADC0, kADC_MuxA);
 * @endcode
 * @param  instance: ADC 模块号
 *         @arg HW_ADC0  :ADC0模块
 *         @arg HW_ADC1  :ADC1模块
 *         @arg HW_ADC2  :ADC2模块
 * @param  mux: ADC 转换器通道 复用 选择
 *         @arg kADC_MuxA   :A通道模式
 *         @arg kADC_MuxB   :B通道模式
 * @retval :读取结果 如果当前还未完成转换 则返回上一次结果
 */
int32_t ADC_ReadValue(uint32_t instance, uint32_t mux)
{
    return ADC_InstanceTable[instance]->R[mux];
}

/**
 * @brief  读取ADC转换结果(简化版) 只需填入ADC快速初始化宏即可
 * @note   阻塞式 直到数据转换完成    
 * @code
 *    //读取AD0模块20通道DM1引脚的转换结果
 *    uint32_t value;   //存储数据转换结果
 *    value = ADC_QuickReadValue(ADC0_SE20_DM1);
 * @endcode
 * @param  MAP: 快速初始化宏，详见ADC.H文件
 * @retval 转换结果
 */
int32_t ADC_QuickReadValue(uint32_t MAP)
{
    QuickInit_Type * pq = (QuickInit_Type*)&(MAP);
    uint32_t instance = pq->ip_instance;
    uint32_t chl = pq->channel;
    ADC_StartConversion(instance, chl, kADC_MuxA);
    /* waiting for ADC complete */
    while((ADC_InstanceTable[instance]->SC1[kADC_MuxA] & ADC_SC1_COCO_MASK) == 0);
    return ADC_ReadValue(instance, kADC_MuxA);
}

/**
 * @brief  ADC开始一次转换 
 * @note   立即返回 非阻塞式 不等待转换结果
 * @code
 *    //启动 ADC0 通道20 在A模式下数据转换
 *    ADC_StartConversion(HW_ADC0, 20, kADC_MuxA);
 * @endcode
 * @param  instance: ADC 模块号
 *         @arg HW_ADC0  :ADC0模块
 *         @arg HW_ADC1  :ADC1模块
 *         @arg HW_ADC2  :ADC2模块
 * @param  chl: ADC 通道号
 * @param  mux: ADC 转换器通道 复用 选择
 *         @arg kADC_MuxA   :A转换器触发
 *         @arg kADC_MuxB   :B转换器触发
 * @retval None
 */
void ADC_StartConversion(uint32_t instance, uint32_t chl, uint32_t mux)
{
    ADC_InstanceTable[instance]->SC1[mux] &= ~(ADC_SC1_ADCH_MASK);
    ADC_InstanceTable[instance]->SC1[mux] |= ADC_SC1_ADCH(chl);
}

void ADC_ChlMuxConfig(uint32_t instance, uint32_t mux)
{
    if(kADC_ChlMuxA ==  mux)
    {
        ADC_InstanceTable[instance]->CFG2 &= ~ADC_CFG2_MUXSEL_MASK;
    }
    if(kADC_ChlMuxB ==  mux)
    {
        ADC_InstanceTable[instance]->CFG2 |= ADC_CFG2_MUXSEL_MASK; 
    }
}

/**
 * @brief  AD采集校准函数，内部函数
 * @code
 *      //查看ADC0 模块的A通道的转换是否完成
 *      ADC_IsConversionCompleted(HW_ADC0, kADC_MuxA); 
 * @endcode         
 * @param  instance: ADC 模块号
 *         @arg HW_ADC0  :ADC0模块
 *         @arg HW_ADC1  :ADC1模块
 *         @arg HW_ADC2  :ADC2模块
 * @param  mux: ADC 转换器通道 复用 选择
 *         @arg kADC_MuxA   :A通道模式
 *         @arg kADC_MuxB   :B通道模式
 * @retval 
 *         @arg 0:转换完成
 *         @arg 1:转换未完成
 */
static int32_t ADC_Calibration(uint32_t instance)
{
    volatile uint32_t i;
    uint32_t PG, MG;
    // set max avarage to get the best cal results
    ADC_InstanceTable[instance]->SC3 |= ADC_SC3_AVGS_MASK;
    ADC_InstanceTable[instance]->SC3 |= ADC_SC3_AVGS(3);
	ADC_InstanceTable[instance]->CFG1 |=  ADC_CFG1_ADICLK(3); 
    
    ADC_InstanceTable[instance]->SC3 |= ADC_SC3_CALF_MASK; /* Clear the calibration's flag */
    ADC_InstanceTable[instance]->SC3 |= ADC_SC3_CAL_MASK;  /* Enable the calibration */
    ADC_ITDMAConfig(instance, kADC_MuxA, kADC_IT_EOF, false);
    for(i=0;i<100000;i++);
    //while(ADC_IsConversionCompleted(instance, 0)) {};      /* Wait conversion is competed */
    if(ADC_InstanceTable[instance]->SC3 & ADC_SC3_CALF_MASK)
    {
        /* cal failed */
        return 1;
    }
    /* Calculate plus-side calibration */
    PG = 0U;
    PG += ADC_InstanceTable[instance]->CLP0;
    PG += ADC_InstanceTable[instance]->CLP1;
    PG += ADC_InstanceTable[instance]->CLP2;
    PG += ADC_InstanceTable[instance]->CLP3;
    PG += ADC_InstanceTable[instance]->CLP4;
    PG += ADC_InstanceTable[instance]->CLPS;
    PG = 0x8000U | (PG>>1U);
    
    /* Calculate minus-side calibration */
    MG = 0U;
    MG += ADC_InstanceTable[instance]->CLM0;
    MG += ADC_InstanceTable[instance]->CLM1;
    MG += ADC_InstanceTable[instance]->CLM2;
    MG += ADC_InstanceTable[instance]->CLM3;
    MG += ADC_InstanceTable[instance]->CLM4;
    MG += ADC_InstanceTable[instance]->CLMS;
    MG = 0x8000U | (MG>>1U);
    ADC_InstanceTable[instance]->SC3 &= ~ADC_SC3_CAL_MASK;  /* Disable the calibration */
    ADC_InstanceTable[instance]->PG = PG;
    ADC_InstanceTable[instance]->MG = MG;
    return 0;
}

/**
 * @brief  ADC模块工作初始化配置
 * @code
 *   //使用adc0模块的1通道 单端模式 8位精度 软件触发
 *   ADC_InitTypeDef ADC_InitStruct1;  //申请一个结构体
 *   ADC_InitStruct1.chl = 1;  //1通道
 *   ADC_InitStruct1.clockDiv = kADC_ClockDiv8; //ADC转换时钟为输入时钟(默认BusClock) 的8分频，和转换速度相关
 *   ADC_InitStruct1.instance = HW_ADC0;        //选择ADC0模块
 *   ADC_InitStruct1.resolutionMode = kADC_SingleDiff8or9; //单端模式下8位精度 查分模式下9位精度
 *   ADC_InitStruct1.SingleOrDifferential = kADC_Single;   //选择单端模式
 *   ADC_InitStruct1.triggerMode = kADC_TriggleSoftware;   //设置为软件触发
 *   ADC_InitStruct1.vref = kADC_VoltageVREF;              //使用外部VERFH VREFL 作为模拟电压参考
 *   //初始化ADC模块
 *   ADC_Init(&ADC_InitStruct1);
 * @endcode
 * @param  ADC_InitStruct: ADC初始化结构体，内容详见注释
 * @retval None
 */
void ADC_Init(ADC_InitTypeDef* ADC_InitStruct)
{
    /* enable clock gate */
    *(uint32_t*)SIM_ADCClockGateTable[ADC_InitStruct->instance].addr |= SIM_ADCClockGateTable[ADC_InitStruct->instance].mask;
    
    /* do calibration */
    ADC_Calibration(ADC_InitStruct->instance);
    
	/* set clock configuration */
	ADC_InstanceTable[ADC_InitStruct->instance]->CFG1 &= ~ADC_CFG1_ADIV_MASK;
	ADC_InstanceTable[ADC_InitStruct->instance]->CFG1 |=  ADC_CFG1_ADIV(ADC_InitStruct->clockDiv); 
    
    /* voltage reference */
    ADC_InstanceTable[ADC_InitStruct->instance]->SC2 &= ~ADC_SC2_REFSEL_MASK;
    ADC_InstanceTable[ADC_InitStruct->instance]->SC2 |= ADC_SC2_REFSEL(ADC_InitStruct->vref);
    
    /* resolutionMode */
	ADC_InstanceTable[ADC_InitStruct->instance]->CFG1 &= ~(ADC_CFG1_MODE_MASK); 
	ADC_InstanceTable[ADC_InitStruct->instance]->CFG1 |= ADC_CFG1_MODE(ADC_InitStruct->resolutionMode);
    
    /* trigger mode */
    (kADC_TriggerHardware == ADC_InitStruct->triggerMode)?(ADC_InstanceTable[ADC_InitStruct->instance]->SC2 |=  ADC_SC2_ADTRG_MASK):(ADC_InstanceTable[ADC_InitStruct->instance]->SC2 &=  ADC_SC2_ADTRG_MASK);
    
    /* if continues conversion */
    (kADC_ContinueConversionEnable == ADC_InitStruct->continueMode)?(ADC_InstanceTable[ADC_InitStruct->instance]->SC3 |= ADC_SC3_ADCO_MASK):(ADC_InstanceTable[ADC_InitStruct->instance]->SC3 &= ~ADC_SC3_ADCO_MASK);
    
    /* if hardware average enabled */
    switch(ADC_InitStruct->hardwareAveMode)
    {
        case kADC_HardwareAverageDisable:
            ADC_InstanceTable[ADC_InitStruct->instance]->SC3 &= ~ADC_SC3_AVGS_MASK;
            break;
        case kADC_HardwareAverage_4:
            ADC_InstanceTable[ADC_InitStruct->instance]->SC3 &= ~ADC_SC3_AVGS_MASK;
            ADC_InstanceTable[ADC_InitStruct->instance]->SC3 |= ADC_SC3_AVGS(0);
            break;
        case kADC_HardwareAverage_8:
            ADC_InstanceTable[ADC_InitStruct->instance]->SC3 &= ~ADC_SC3_AVGS_MASK;
            ADC_InstanceTable[ADC_InitStruct->instance]->SC3 |= ADC_SC3_AVGS(1);
            break;
        case kADC_HardwareAverage_16:
            ADC_InstanceTable[ADC_InitStruct->instance]->SC3 &= ~ADC_SC3_AVGS_MASK;
            ADC_InstanceTable[ADC_InitStruct->instance]->SC3 |= ADC_SC3_AVGS(2);
            break;
        case kADC_HardwareAverage_32:
            ADC_InstanceTable[ADC_InitStruct->instance]->SC3 &= ~ADC_SC3_AVGS_MASK;
            ADC_InstanceTable[ADC_InitStruct->instance]->SC3 |= ADC_SC3_AVGS(3);
            break;
        default:
            break;
    }
}

/**
 * @brief  ADC中断及DMA功能开关函数
 * @code
 *    //配置AD0模块 转换完成中断
 *    ADC_ITDMAConfig(HW_ADC0, kADC_MuxA, kADC_IT_EOF);
 * @endcode         
 * @param  instance: ADC 模块号
 *         @arg HW_ADC0  :ADC0模块
 *         @arg HW_ADC1  :ADC1模块
 *         @arg HW_ADC2  :ADC2模块
 * @param  mux: ADC 转换器通道 复用 选择
 *         @arg kADC_MuxA   :A通道模式
 *         @arg kADC_MuxB   :B通道模式
 * @param  config:   ADC中断及DMA配置
 *         @arg kADC_IT_Disable    :关闭中断
 *         @arg kADC_DMA_Disable   :关闭DMA功能 
 *         @arg kADC_IT_EOF        :转换完成中断
 *         @arg kADC_DMA_EOF       :DMA完成中断
 * @retval None
 */
void ADC_ITDMAConfig(uint32_t instance, uint32_t mux, ADC_ITDMAConfig_Type config, bool status)
{
    switch(config)
    {
        case kADC_IT_EOF:
            (status)?
            (ADC_InstanceTable[instance]->SC1[mux] |= ADC_SC1_AIEN_MASK):
            (ADC_InstanceTable[instance]->SC1[mux] &= ~ADC_SC1_AIEN_MASK);
            NVIC_EnableIRQ(ADC_IRQnTable[instance]);
            break; 
        case kADC_DMA_EOF:
            (status)?
            (ADC_InstanceTable[instance]->SC2 |= ADC_SC2_DMAEN_MASK):
            (ADC_InstanceTable[instance]->SC2 &= ~ADC_SC2_DMAEN_MASK);
            break;
        default:
            break;
    }
}

/**
 * @brief  注册中断回调函数
 * @param  instance: ADC 模块号
 *         @arg HW_ADC0  :ADC0模块
 *         @arg HW_ADC1  :ADC1模块
 *         @arg HW_ADC2  :ADC2模块
 * @param AppCBFun: 回调函数指针入口
 * @retval None
 * @note 对于此函数的具体应用请查阅应用实例
 */
void ADC_CallbackInstall(uint32_t instance, ADC_CallBackType AppCBFun)
{
    if(AppCBFun != NULL)
    {
        ADC_CallBackTable[instance] = AppCBFun;
    }
}

//! @}

/**
 * @brief  中断处理函数入口
 * @note 函数内部用于中断事件处理
 */
void ADC0_IRQHandler(void)
{
    if(ADC_CallBackTable[HW_ADC0] != NULL)
    {
        ADC_CallBackTable[HW_ADC0]();
    }
}


/*
    uint32_t ip_instance:3;
    uint32_t io_instance:3;
    uint32_t mux:3;
    uint32_t io_base:5;
    uint32_t io_offset:5;
    uint32_t channel:5;
    uint32_t reserved:8;
reserved: [bit.7]: 0-SE 1-DIFF
reserved: [bit.0]: 0-A  1-B
const QuickInit_Type ADC_QuickInitTable[] =
{
    { 0, 4, 0,16, 1, 1, 0}, //ADC0_SE1A_PE16
    { 0, 4, 0,17, 1, 5, 0}, //ADC0_SE5A_PE17
    { 0, 4, 0,18, 1, 2, 0}, //ADC0_SE2A_PE18
    { 0, 4, 0,19, 1, 6, 0}, //ADC0_SE6A_PE19
    { 0, 4, 0,20, 1, 0, 0}, //ADC0_SE0A_PE20
    { 0, 4, 0,21, 1, 4, 0}, //ADC0_SE4A_PE21
    { 0, 4, 0,22, 1, 3, 0}, //ADC0_SE3A_PE22
    { 0, 4, 0,23, 1, 7, 0}, //ADC0_SE7A_PE23
    { 0, 4, 0,29, 1, 4, 1}, //ADC0_SE4B_PE29
    { 0, 4, 0,30, 1,23, 0}, //ADC0_SE23A_PE30
    { 0, 1, 0, 0, 1, 8, 0}, //ADC0_SE8A_PB00
    { 0, 1, 0, 1, 1, 9, 0}, //ADC0_SE9A_PB01
    { 0, 1, 0, 2, 1,12, 0}, //ADC0_SE12A_PB02
    { 0, 1, 0, 3, 1,13, 0}, //ADC0_SE13A_PB03
    { 0, 2, 0, 0, 1,14, 0}, //ADC0_SE14A_PC00
    { 0, 2, 0, 1, 1,15, 0}, //ADC0_SE15A_PC01
    { 0, 2, 0, 2, 1,11, 0}, //ADC0_SE11A_PC02
    { 0, 3, 0, 1, 1, 5, 1}, //ADC0_SE5B_PD01
    { 0, 3, 0, 5, 1, 6, 1}, //ADC0_SE6B_PD05
    { 0, 3, 0, 6, 1, 7, 1}, //ADC0_SE7B_PD06
};
*/

