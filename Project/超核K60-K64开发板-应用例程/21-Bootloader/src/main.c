#include "common.h"
#include "uart.h"
#include "dma.h"
#include "mq.h"
#include "FlashOS.H"


/*  

bootloader demo:

bootlaoder demo use UART to download bin image file(addr must be at 0x5000) via DNW(a file download software running on PC)

instructions:

1. Download firmware(this project) to board via Debugger(Jlink)
2. Open DNW, set COM settings(baud:115200) and get connectted.(Serial Port->Connection)
3. Reset board, infomation will be printed on DNW, then, press 'D' to indicate that a new download is needed(within 2S).
4. Use DNW to open bin file(we offer a test.bin in this folder) and download will begin automically.
5. Create your own bootloader bin file: open 0-¹¤³ÌÄ£°å and switch to bootloader project setting, complile and generate your boolader bin file in ./OBJ/bootloader/xxx.bin

*/

#define SECTER_SIZE             0x000800
#define PROGRAM_ADDR_BASE       0x00005000
#define PROGRAM_SIZE            (512- 20)*1024
#define TIME_OUT                2000


#define BOOTLAODER_LOG    printf

static uint8_t Buf1[SECTER_SIZE];
static uint8_t Buf2[SECTER_SIZE];

static void UART_DMARevInit(uint8_t dmaChl)
{
    DMA_InitTypeDef DMA_InitStruct1 = {0};
    DMA_InitStruct1.chl = dmaChl;
    DMA_InitStruct1.chlTriggerSource = UART0_REV_DMAREQ;
    DMA_InitStruct1.triggerSourceMode = kDMA_TriggerSource_Normal;
    DMA_InitStruct1.minorLoopByteCnt = 1;
    DMA_InitStruct1.majorLoopCnt = SECTER_SIZE;
    
    DMA_InitStruct1.sAddr = (uint32_t)&UART0->D;
    DMA_InitStruct1.sLastAddrAdj = 0; 
    DMA_InitStruct1.sAddrOffset = 0;
    DMA_InitStruct1.sDataWidth = kDMA_DataWidthBit_8;
    DMA_InitStruct1.sMod = kDMA_ModuloDisable;
    
    DMA_InitStruct1.dAddr = NULL; 
    DMA_InitStruct1.dLastAddrAdj = -SECTER_SIZE;
    DMA_InitStruct1.dAddrOffset = 1;
    DMA_InitStruct1.dDataWidth = kDMA_DataWidthBit_8;
    DMA_InitStruct1.dMod = kDMA_ModuloDisable;
    DMA_Init(&DMA_InitStruct1); 

   // DMA_EnableAutoDisableRequest(dmaChl, false);
}

static void RunningApplication(uint32_t addr)
{
    BOOTLAODER_LOG("running @ 0x%08X...\r\n", addr);
    DelayMs(1);
    
    /* close UART RX IT */
    UART_ITDMAConfig(HW_UART0, kUART_IT_Rx, false);
    
    /* reset vector table */
    SCB->VTOR = addr;
    
    /* set MSP */
    __set_MSP(*(uint32_t*)(addr));
    
    /* jump to application */
    void (*pfun)(void); 
    uint32_t jump_addr;
    jump_addr = *(uint32_t*)(addr + 4);  /* reset hander */
    pfun = (void(*)(void))jump_addr;
    pfun();
}



static int MKP512FlashInit(void)
{
    uint32_t clock;
    uint32_t flash_clock = CLOCK_GetClockFrequency(kFlashClock, &clock);
    /* fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify) */         
    return Init(PROGRAM_ADDR_BASE, clock, 2);
}

static void* SwitchBuffer(void* p1, void *p2)
{
    static bool flag;
    static void *p;
    ((flag = !flag) == true)?(p = p1):(p = p2);
    return p;
}

static void DMA_ISR(void)
{
    msg_t msg;
    void *p;
    p = SwitchBuffer(Buf1, Buf2);
    DMA_SetDestAddress(HW_DMA_CH0, (uint32_t)p);
    if(p == Buf1)
    {
        msg.p = Buf2;
    }
    else
    {
        msg.p = Buf1;
    }
    mq_push(msg);
   
}
    
int main(void)
{
    volatile uint32_t transfer_size;
    volatile uint32_t file_counter;
    volatile uint32_t file_size;
    volatile uint32_t i;
    volatile uint32_t program_addr = PROGRAM_ADDR_BASE;
    DelayInit();
    UART_QuickInit(UART0_RX_PD06_TX_PD07, 115200);
    BOOTLAODER_LOG("press [D] to download...\r\n");
    
    /* initilize ssd(stand flash driver) */
    MKP512FlashInit();
    
    /* initalize message queue */
    mq_init();

    /* waitting for download key */
    while(1)
    {
        uint16_t ch;
        if((UART_ReadByte(HW_UART0, &ch) == 0) && (ch == 'D' || ch == 'd'))
        {
            BOOTLAODER_LOG("waitting for file...(do NOT press any key form NOW)\r");  
            break;
        }
        if(i > 2000)
        {
            BOOTLAODER_LOG("timeout! ");
            RunningApplication(PROGRAM_ADDR_BASE);
        }
        i++;
        DelayMs(1);
    }
    
    /* init transfer */
    UART_DMARevInit(HW_DMA_CH0);
    DMA_SetDestAddress(HW_DMA_CH0, (uint32_t)SwitchBuffer(Buf1, Buf2));
    UART_ITDMAConfig(HW_UART0, kUART_DMA_Rx, true);
    
    /* protrol: download = size(4) + data(n) + cs(2) */
    transfer_size = 4;
    DMA_SetMajorLoopCounter(HW_DMA_CH0, transfer_size);
    
    /* set DMA callback and initalize transfer */
    DMA_CallbackInstall(HW_DMA_CH0, DMA_ISR);
    DMA_ITConfig(HW_DMA_CH0, kDMA_IT_Major, true);
    DMA_EnableRequest(HW_DMA_CH0);
    
    msg_t *pMsg;
    while(1)
    {
        if(mq_msg_exist())
        {
            pMsg = mq_pop();
            /* first 4 byte indicate total transfer size(size(4) + data(n) + cs(2) ) */
            if(transfer_size == 4)
            {
                
                /* calc image file size */
                file_size = pMsg->p[0] + (pMsg->p[1]<<8) + (pMsg->p[2]<<16) + (pMsg->p[2]<<24) - 6;
                
                /* calc transfer block size */
                if(file_size < SECTER_SIZE)
                {
                    transfer_size = file_size; /* last transfer */
                }
                else
                {
                    transfer_size = SECTER_SIZE;
                }
                
                /* inject dma transfer counter and lunch dma */
                DMA_SetMajorLoopCounter(HW_DMA_CH0, transfer_size);
                DMA_EnableRequest(HW_DMA_CH0);
                BOOTLAODER_LOG("file size:%d\r\n", file_size);
            }
            else /* received file data */
            {
                file_counter += transfer_size;
                /* calc transfer size */
                if(file_size - file_counter < SECTER_SIZE)
                {
                    transfer_size = file_size - file_counter;
                }
                else
                {
                    transfer_size = SECTER_SIZE;
                }
                DMA_SetMajorLoopCounter(HW_DMA_CH0, transfer_size);
                DMA_EnableRequest(HW_DMA_CH0);
                /* program flash */
                BOOTLAODER_LOG("programing @0x%08X size:%04d ...", program_addr, transfer_size);
                DisableInterrupts();
                EraseSector (program_addr);
                ProgramPage (program_addr, SECTER_SIZE, pMsg->p);
                EnableInterrupts();
                BOOTLAODER_LOG("ok\r\n");
                program_addr += SECTER_SIZE;
                
                /* download complete, running */
                if(file_counter == file_size)
                {
                    RunningApplication(PROGRAM_ADDR_BASE);
                }
            }
        }
      //  GPIO_ToggleBit(HW_GPIOE, 6);
    }
}
