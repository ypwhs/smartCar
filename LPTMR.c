int main(void)
{

    DelayInit();
    /* 打印串口及小灯 */
    GPIO_QuickInit(HW_GPIOD, 10, kGPIO_Mode_OPP);

    initUART();

    /* 快速初始化 LPTMR模块用作脉冲计数功能 */
    LPTMR_PC_QuickInit(LPTMR_ALT2_PC05); /* 脉冲计数 */

    while(1)
    {
        uint32_t value;
        value = LPTMR_PC_ReadCounter(); //获得LPTMR模块的计数值
        printf("LPTMR:%d\r\n", value);
        PDout(10)=!PDout(10);
        DelayMs(100);
    }
}