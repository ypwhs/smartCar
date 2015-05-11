#include "gpio.h"
#include "common.h"
#include "uart.h"
#include "cpuidy.h"


 
int main(void)
{
    DelayInit();
    GPIO_QuickInit(HW_GPIOE, 6, kGPIO_Mode_OPP);
    UART_QuickInit(UART0_RX_PD06_TX_PD07, 115200);

    printf("Hardfault Interrupt test!\r\n");
    
    /* close PORTA gate clock */
    SIM->SCGC5 &= ~SIM_SCGC5_PORTA_MASK;
    
    
    printf("operate PORTA without open it's clock gate, that will trigger hardfault!\r\n");
    
    /* R/W PORTA, this will trigger hardfault */
    PORTA->PCR[0] = 0;
    
    while(1)
    {
        /* ÉÁË¸Ð¡µÆ */
        GPIO_ToggleBit(HW_GPIOE, 6);
        DelayMs(500);
    }
}


struct exception_stack_frame
{
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r12;
    uint32_t lr;
    uint32_t pc;
    uint32_t psr;
};

void rt_hw_hard_fault_exception(struct exception_stack_frame *exception_stack)
{
    printf("psr: 0x%08x\r\n", exception_stack->psr);
    printf(" pc: 0x%08x\r\n", exception_stack->pc);
    printf(" lr: 0x%08x\r\n", exception_stack->lr);
    printf("r12: 0x%08x\r\n", exception_stack->r12);
    printf("r03: 0x%08x\r\n", exception_stack->r3);
    printf("r02: 0x%08x\r\n", exception_stack->r2);
    printf("r01: 0x%08x\r\n", exception_stack->r1);
    printf("r00: 0x%08x\r\n", exception_stack->r0);
}

void HardFault_Handler(void)
{
    printf("\r\n HardFault_Handler interrupt!\r\n");
    rt_hw_hard_fault_exception((struct exception_stack_frame *)__get_PSP());
    while(1);
}

void BusFault_Handler(void)
{
    printf("\r\n BusFault_Handler interrupt!\r\n");
    rt_hw_hard_fault_exception((struct exception_stack_frame *)__get_PSP());
    while(1);
}

