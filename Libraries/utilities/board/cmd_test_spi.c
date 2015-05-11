#include "shell.h"
#include "spi.h"
#include "gpio.h"
#include "board.h"


static void SPI_ISR(void)
{
    printf("!!\r\n");
}

int DoSPI(int argc, char * const argv[])
{
    int i,instance,data;
    QuickInit_Type pq;
    printf("board:%s fun:%s\r\n",BOARD_NAME, __func__);
    instance = SPI_QuickInit(BOARD_SPI_MAP, kSPI_CPOL1_CPHA1, 1000*1000);
    printf("i2c instance:%d\r\n", instance);
    QuickInitDecode(BOARD_SPI_MAP, &pq);
    for(i = 0; i < pq.io_offset; i++)
    {
        printf("P%c%d ", pq.io_instance+'A', pq.io_base+i);
    }
    printf("\r\n");
    
    SPI_CallbackInstall(instance, SPI_ISR);
    SPI_ITDMAConfig(instance, kSPI_IT_Rx, true);
    
    data = SPI_ReadWriteByte(instance, 0x55);
    printf("0x%x\r\n", data);
    data = SPI_ReadWriteByte(instance, 0xAA);
    printf("0x%x\r\n", data);
    data = SPI_ReadWriteByte(instance, 4);
    printf("0x%x\r\n", data);
    return 0;
}

SHELL_EXPORT_CMD(DoSPI, spi , spi test)

