#include "chlib_k.h"

#include "shell.h"
#include "board.h"

#include "ads7843.h"
#include "spi.h"
//#include "types.h"
struct spi_bus bus; 

extern int Image$$RW_IRAM1$$ZI$$Limit;
extern int Image$$RW_IRAM1$$RW$$Limit;
extern int Image$$ER_IROM1$$RO$$Limit;

int main(void)
{
    uint32_t i;
    DelayInit();
    UART_QuickInit(BOARD_UART_DEBUG_MAP, 115200);
    printf("HelloWorld\r\n");
    shell_init();
    /* 设置Flexbus 速度 速度太快ili9320LCD读点有问题 */
    #ifndef MK10D5
    SIM->CLKDIV1 |= SIM_CLKDIV1_OUTDIV3(4);
    #endif
    
    /* ZI Section end address */
    printf("IRAM1_ZI:0x%08X KB\r\n", ((uint32_t)&Image$$RW_IRAM1$$ZI$$Limit));
    
    /* RW Section end address */
    printf("IRAM1_RW:0x%08X KB\r\n", ((uint32_t)&Image$$RW_IRAM1$$RW$$Limit));
    
    /* RO Section end address */
    printf("IROM1_RO:0x%08X KB\r\n", ((uint32_t)&Image$$ER_IROM1$$RO$$Limit));
    
    while(1)
    {
        shell_main_loop("SHELL>>");
    }
}


