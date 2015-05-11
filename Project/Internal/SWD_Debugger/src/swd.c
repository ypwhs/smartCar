#include "swd.h"

#define DCRDR 0xE000EDF8
#define DCRSR 0xE000EDF4
#define DHCSR 0xE000EDF0
#define REGWnR (1 << 16)

typedef struct
{
    uint32_t r[16];
    uint32_t xpsr;
} DEBUG_STATE;

void SWJ_Sequence(uint32_t count, uint8_t *data)
{
    uint32_t val;
    uint32_t n;

    val = 0;
    n = 0;
    while(count--)
    {
        if (n == 0)
        {
            val = *data++;
            n = 8;
        }
    
        SW_WRITE_BIT(val & 1);
        val >>= 1;
        n--;
    }
}

uint8_t SWD_Transfer (uint32_t request, uint32_t *data)
{
    uint32_t ack;
    uint32_t bit;
    uint32_t val;
    uint32_t parity;
  
    uint32_t n;  
  
    /* Packet Request */      
    parity = 0;  
    SW_WRITE_BIT(1);         /* Start Bit */
    bit = request >> 0;        
    SW_WRITE_BIT(bit);       /* APnDP Bit */
    parity += bit;
    bit = request >> 1;        
    SW_WRITE_BIT(bit);       /* RnW Bit */  
    parity += bit;
    bit = request >> 2;        
    SW_WRITE_BIT(bit);       /* A2 Bit */   
    parity += bit;
    bit = request >> 3;        
    SW_WRITE_BIT(bit);       /* A3 Bit */   
    parity += bit;
    SW_WRITE_BIT(parity);    /* Parity Bit */            
    SW_WRITE_BIT(0);         /* Stop Bit */ 
    SW_WRITE_BIT(1);         /* Park Bit */ 
   
    /* Turnaround */           
    TMS_RD();   
    for (n = 1; n; n--)
    {     
        SW_CLOCK_CYCLE();        
    }
   
    /* Acknowledge response */ 
    bit = SW_READ_BIT();          
    ack  = bit << 0;           
    bit = SW_READ_BIT();          
    ack |= bit << 1;           
    bit = SW_READ_BIT();          
    ack |= bit << 2;           
   
    switch(ack)
    {
        case DAP_TRANSFER_OK:
            if (request & DAP_TRANSFER_RnW) /* read data */
            {
                val = 0;  
                parity = 0;            
                for (n = 32; n; n--)
                { 
                    bit = SW_READ_BIT();  /* Read RDATA[0:31] */      
                    parity += bit;       
                    val >>= 1;           
                    val  |= bit << 31;   
                }         
                bit = SW_READ_BIT();    /* Read Parity */           
                if ((parity ^ bit) & 1)
                {           
                    ack = DAP_TRANSFER_ERROR;         
                }               
                if (data) *data = val;
            
                /* Turnaround */       
                for (n = 1; n; n--)
                { 
                    SW_CLOCK_CYCLE();    
                }         
                TMS_WR();
            }
            else    /* write data */
            {
                /* Turnaround */       
                for (n = 1; n; n--) {    SW_CLOCK_CYCLE(); }
           
                TMS_WR();
                /* Write data */       
                val = *data;           
                parity = 0;            
                for (n = 32; n; n--)
                { 
                    SW_WRITE_BIT(val); /* Write WDATA[0:31] */     
                    parity += val;       
                    val >>= 1;           
                }         
                SW_WRITE_BIT(parity);/* Write Parity Bit */      
            }
            
            /* Idle cycles */               
            PIN_SWDIO_OUT(1);        
            return (ack);   
        case DAP_TRANSFER_WAIT:
        case DAP_TRANSFER_FAULT:
            /* WAIT or FAULT response */          
            if (0 && ((request & DAP_TRANSFER_RnW) != 0))
            {   
                for (n = 32+1; n; n--)
                {            
                    SW_CLOCK_CYCLE();  /* Dummy Read RDATA[0:31] + Parity */    
                }         
            }
            /* Turnaround */         
            for (n = 1; n; n--) { SW_CLOCK_CYCLE();}      
            TMS_WR();  
            if (0 && ((request & DAP_TRANSFER_RnW) == 0))
            {   
                PIN_SWDIO_OUT(0);      
                for (n = 32+1; n; n--) {    SW_CLOCK_CYCLE();  /* Dummy Write WDATA[0:31] + Parity */   };           
            }           
            PIN_SWDIO_OUT(1);        
            return (ack);  
        default:
            break;
    }
    
    /* Protocol error */       
    for (n = 1 + 32 + 1; n; n--)
    {         
        SW_CLOCK_CYCLE();      /* Back off data phase */   
    }
    PIN_SWDIO_OUT(1);          
    return (ack); 
}




//uint8_t target_unlock_sequence(void) {
//    uint32_t val;

//    // read the device ID
//    if (!swd_read_ap(MDM_IDR, &val)) {
//        return 0;
//    }
//    // verify the result
//    if (val != MCU_ID) {
//        return 0;
//    }

//    if (!swd_read_ap(MDM_STATUS, &val)) {
//        return 0;
//    }

//    // flash in secured mode
//    if (val & (1 << 2)) {
//        // hold the device in reset
//    //    target_set_state(RESET_HOLD);
//        // write the mass-erase enable bit
//        if (!swd_write_ap(MDM_CTRL, 1)) {
//            return 0;
//        }
//        while (1) {
//            // wait until mass erase is started
//            if (!swd_read_ap(MDM_STATUS, &val)) {
//                return 0;
//            }

//            if (val & 1) {
//                break;
//            }
//        }
//        // mass erase in progress
//        while (1) {            
//            // keep reading until procedure is complete
//            if (!swd_read_ap(MDM_CTRL, &val)) {
//                return 0;
//            }

//            if (val == 0) {
//                break;
//            }
//        }
//    }

//    return 1;
//}

// uint8_t swd_init_debug(void) {
//    uint32_t tmp = 0;

//    // call a target dependant function
//    // this function can do several stuff before really
//    // initing the debug
//   // target_before_init_debug();

//    if (!JTAG2SWD()) {
//        return 0;
//    }

//    if (!swd_write_dp(DP_ABORT, STKCMPCLR | STKERRCLR | WDERRCLR | ORUNERRCLR)) {
//        return 0;
//    }

//    // Ensure CTRL/STAT register selected in DPBANKSEL
//    if (!swd_write_dp(DP_SELECT, 0)) {
//        return 0;
//    }

//    // Power up
//    if (!swd_write_dp(DP_CTRL_STAT, CSYSPWRUPREQ | CDBGPWRUPREQ)) {
//        return 0;
//    }

//    do {
//        if (!swd_read_dp(DP_CTRL_STAT, &tmp)) {
//            return 0;
//        }
//    } while ((tmp & (CDBGPWRUPACK | CSYSPWRUPACK)) != (CDBGPWRUPACK | CSYSPWRUPACK));

//    if (!swd_write_dp(DP_CTRL_STAT, CSYSPWRUPREQ | CDBGPWRUPREQ | TRNNORMAL | MASKLANE)) {
//        return 0;
//    }

//    // call a target dependant function:
//    // some target can enter in a lock state
//    // this function can unlock these targets
//    target_unlock_sequence();

//    if (!swd_write_dp(DP_SELECT, 0)) {
//        return 0;
//    }

//    return 1;
//}
// 




/********************************************************************************************/

#define PULSE()             \
        DELAY();            \
        TCK_HIGH();         \
        DELAY();            \
        TCK_LOW();          \


static void SWJ_SendClock(uint32_t count, uint8_t swdio_logic)
{
    (swdio_logic)?(TMS_HIGH()):(TMS_LOW());

    while(count--)
    {
        PULSE();
    }
}

static void SWJ_SendData(uint16_t data)
{
    uint8_t i;
    
    for(i = 0; i < 16; i++)
    {
        ((data & 0x1) == 1) ? (TMS_HIGH()) : (TMS_LOW());
		PULSE();
	    data>>=1;
    }
}

static uint8_t SWJ_JTAG2SWD(void)
{
    SWJ_SendClock(51, 1);
    SWJ_SendData(0xE79E);
    SWJ_SendClock(51, 1);
    SWJ_SendClock(3, 0);
    return 0;
}

uint8_t SWJ_ReadDP(uint8_t adr, uint32_t *val)
{
    uint32_t tmp_in;
    uint8_t ack;
    uint8_t err;

    //SWJ_SendClock(8, 0);
    
    tmp_in = SWD_REG_DP | SWD_REG_R | SWD_REG_ADR(adr);
    ack = SWD_Transfer(tmp_in, val);

    (ack == DAP_TRANSFER_OK)?(err = 0):(err = 1);
    return err;
}

uint8_t SWJ_WriteDP(uint8_t adr, uint32_t val)
{
    uint32_t req;
    uint8_t ack;
    uint8_t err;
    
    req = SWD_REG_DP | SWD_REG_W | SWD_REG_ADR(adr);
    ack = SWD_Transfer(req, &val);

    (ack == DAP_TRANSFER_OK)?(err = 0):(err = 1);
    return err;
}

/* Read access port register. */
uint8_t SWJ_ReadAP(uint32_t adr, uint32_t *val)
{
    uint8_t tmp_in, ack, err;

    uint32_t apsel = adr & APSEL;
    uint32_t bank_sel = adr & APBANKSEL;

    if(SWJ_WriteDP(DP_SELECT, apsel | bank_sel))
    {
        return 1;
    }

    tmp_in = SWD_REG_AP | SWD_REG_R | SWD_REG_ADR(adr);

    /* first dummy read */
    ack = SWD_Transfer(tmp_in, val);
    ack = SWD_Transfer(tmp_in, val);

    (ack == DAP_TRANSFER_OK)?(err = 0):(err = 1);
    return err;
}


uint8_t SWJ_WriteAP(uint32_t adr, uint32_t val)
{
    uint8_t req, ack, err;
    
    uint32_t apsel = adr & APSEL;
    uint32_t bank_sel = adr & APBANKSEL;

    /* write DP select */
    if(SWJ_WriteDP(DP_SELECT, apsel | bank_sel))
    {
        return 1;
    }

    /* write AP data */
    req = SWD_REG_AP | SWD_REG_W | SWD_REG_ADR(adr);
    ack = SWD_Transfer(req, &val);

    /* read DP buff */
    req = SWD_REG_DP | SWD_REG_R | SWD_REG_ADR(DP_RDBUFF);
    ack = SWD_Transfer(req, NULL);

    (ack == DAP_TRANSFER_OK)?(err = 0):(err = 1);
    return err;
}

/* Read 32-bit word from target memory. */
// AP CSW register, base value
#define CSW_VALUE (CSW_RESERVED | CSW_MSTRDBG | CSW_HPROT | CSW_DBGSTAT | CSW_SADDRINC)


// Write target memory.
static uint8_t SWJ_WriteData(uint32_t addr, uint32_t data)
{
    uint8_t req, ack, err;

    SWJ_WriteAP(AP_TAR, addr);

    /* write data */
    req = SWD_REG_AP | SWD_REG_W | AP_DRW;
    ack = SWD_Transfer(req, &data);
    
    /* read DP buff */
    req = SWD_REG_DP | SWD_REG_R | SWD_REG_ADR(DP_RDBUFF);
    ack = SWD_Transfer(req, NULL);

    (ack == DAP_TRANSFER_OK)?(err = 0):(err = 1);
    return err;
}

static uint8_t SWJ_WriteMem8(uint32_t addr, uint8_t val)
{
    uint32_t tmp;
    uint8_t err;
    
    SWJ_WriteAP(AP_CSW, CSW_VALUE | CSW_SIZE8);

    tmp = val << ((addr & 0x03) << 3);
    err = SWJ_WriteData(addr, tmp);
    return err;
}

static uint8_t SWJ_WriteMem32(uint32_t addr, uint32_t val)
{
    uint8_t err;
    
    SWJ_WriteAP(AP_CSW, CSW_VALUE | CSW_SIZE32);
    err = SWJ_WriteData(addr, val);
    return err;
}

// Write 32-bit word aligned values to target memory using address auto-increment.
// size is in bytes.
static uint8_t SWJ_WriteBlock(uint32_t addr, uint8_t *buf, uint32_t len)
{
    uint8_t err, req;
    uint32_t size_in_words;
    uint32_t i, ack;

    if (len == 0)  return 0;
    err = 0;
    size_in_words = len/4;

    
    err += SWJ_WriteAP(AP_CSW, CSW_VALUE | CSW_SIZE32);
    err += SWJ_WriteAP(AP_TAR, addr);
    
    if(err) return err;

    /* DRW write */
    req = SWD_REG_AP | SWD_REG_W | (3 << 2);
    for (i = 0; i < size_in_words; i++)
    {
        if (SWD_Transfer(req, (uint32_t *)buf) != DAP_TRANSFER_OK)
        {
            return 1;
        }
        buf += 4;
    }

    /* read DP buff */
    req = SWD_REG_DP | SWD_REG_R | SWD_REG_ADR(DP_RDBUFF);
    ack = SWD_Transfer(req, NULL);
    
    (ack == DAP_TRANSFER_OK)?(err = 0):(err = 1);
    return err;
}

// Read target memory.
static uint8_t SWJ_ReadData(uint32_t addr, uint32_t *val)
{
    uint8_t req, ack, err;

    SWJ_WriteAP(AP_TAR, addr);

    /* read data */
    req = SWD_REG_AP | SWD_REG_R | AP_DRW;
    ack = SWD_Transfer(req, val);

    /* dummy read */
    req = SWD_REG_DP | SWD_REG_R | SWD_REG_ADR(DP_RDBUFF);
    ack = SWD_Transfer(req, val);
    
    (ack == DAP_TRANSFER_OK)?(err = 0):(err = 1);
    return err;
}

static uint8_t SWJ_ReadMem32(uint32_t addr, uint32_t *val)
{
    uint8_t err;
    SWJ_WriteAP(AP_CSW, CSW_VALUE | CSW_SIZE32);
    err = SWJ_ReadData(addr, val);
    return err;
}

static uint8_t SWJ_ReadMem8(uint32_t addr, uint8_t *val)
{
    uint32_t tmp;
    uint8_t err;
    
    SWJ_WriteAP(AP_CSW, CSW_VALUE | CSW_SIZE8); 
    
    err = SWJ_ReadData(addr, &tmp);
    
    *val = (uint8_t)(tmp >> ((addr & 0x03) << 3));
    
    return err;
}

// Read 32-bit word aligned values from target memory using address auto-increment.
// size is in bytes.
static uint8_t SWJ_ReadBlock(uint32_t addr, uint8_t *buf, uint32_t len)
{
    uint8_t err, req;
    uint32_t size_in_words;
    uint32_t i, ack;

    if (len == 0)  return 0;
    err = 0;
    size_in_words = len/4;

    
    err += SWJ_WriteAP(AP_CSW, CSW_VALUE | CSW_SIZE32);
    err += SWJ_WriteAP(AP_TAR, addr);
    
    if(err) return err;

    req = SWD_REG_AP | SWD_REG_R | (3 << 2);
    
    /* dummy read */
    if (SWD_Transfer(req, (uint32_t *)buf) != 0x01)
    {
        return 1;
    }
    
    for (i = 0; i < size_in_words; i++)
    {
        if (SWD_Transfer(req, (uint32_t *)buf) != DAP_TRANSFER_OK)
        {
            return 1;
        }
        buf += 4;
    }

    /* read DP buff */
    req = SWD_REG_DP | SWD_REG_R | SWD_REG_ADR(DP_RDBUFF);
    ack = SWD_Transfer(req, NULL);
    
    (ack == DAP_TRANSFER_OK)?(err = 0):(err = 1);
    return err;   
}

#define TARGET_AUTO_INCREMENT_PAGE_SIZE    (0x400)

// Write unaligned data to target memory.
// size is in bytes.
uint8_t swd_write_memory(uint32_t address, uint8_t *data, uint32_t size)
{
    uint32_t n;
    // Write bytes until word aligned
    while ((size > 0) && (address & 0x3))
    {
        SWJ_WriteMem8(address, *data);
        address++;
        data++;
        size--;
    }

    // Write word aligned blocks
    while (size > 3) {
        // Limit to auto increment page size
        n = TARGET_AUTO_INCREMENT_PAGE_SIZE - (address & (TARGET_AUTO_INCREMENT_PAGE_SIZE - 1));
        if (size < n) {
            n = size & 0xFFFFFFFC; // Only count complete words remaining
        }

        if (SWJ_WriteBlock(address, data, n))
        {
            return 1;
        }

        address += n;
        data += n;
        size -= n;
    }

    // Write remaining bytes
    while (size > 0) {
        SWJ_WriteMem8(address, *data);

        address++;
        data++;
        size--;
    }

    return 0;
}

// Read unaligned data from target memory.
// size is in bytes.
uint8_t swd_read_memory(uint32_t address, uint8_t *data, uint32_t size)
{
    uint32_t n;

    // Read bytes until word aligned
    while ((size > 0) && (address & 0x3))
    {
        SWJ_ReadMem8(address, data);
        address++;
        data++;
        size--;
    }

    // Read word aligned blocks
    while (size > 3)
    {
        // Limit to auto increment page size
        n = TARGET_AUTO_INCREMENT_PAGE_SIZE - (address & (TARGET_AUTO_INCREMENT_PAGE_SIZE - 1));
        if (size < n)
        {
            n = size & 0xFFFFFFFC; // Only count complete words remaining
        }

        if (SWJ_ReadBlock(address, data, n))
        {
            return 1;
        }

        address += n;
        data += n;
        size -= n;
    }

    // Read remaining bytes
    while (size > 0)
    {
        SWJ_ReadMem8(address, data);
        address++;
        data++;
        size--;
    }

    return 0;
}

uint8_t SWJ_ReadMem(uint32_t addr, uint8_t *buf, uint32_t len)
{
    return swd_read_memory(addr, buf, len);
}

uint8_t SWJ_WriteMem(uint32_t addr, uint8_t *buf, uint32_t len)
{
    return swd_write_memory(addr, buf, len);
}


#define MAX_TIMEOUT 100
static uint8_t SWJ_WaitUntilHalted(void)
{
    // Wait for target to stop
    uint32_t val, i, timeout = MAX_TIMEOUT;
    for (i = 0; i < timeout; i++)
    {

        if (SWJ_ReadMem32(DBG_HCSR, &val))
        {
            return 1;
        }

        if (val & S_HALT)
        {
            return 0;
        }
    }
    printf("SWJ_WaitUntilHalted reach max\r\n");
    return 1;
}

uint8_t SWJ_SetTargetState(TARGET_RESET_STATE state)
{
    switch (state)
    {
        case RESET_HOLD:
            TRST_LOW();
            break;
        case RESET_RUN:
            TRST_LOW();
            DelayMs(2);
            TRST_HIGH();
            DelayMs(2);
            break;
        case RESET_PROGRAM:
           // Use hardware reset (HW RESET)
            // First reset
            TRST_LOW();
            DelayMs(2);
            TRST_HIGH();
            DelayMs(2);

            SWJ_InitDebug();
        
            // Enable debug
            SWJ_WriteMem32(DBG_HCSR, DBGKEY | C_DEBUGEN);

            // Enable halt on reset
            SWJ_WriteMem32(DBG_EMCR, VC_CORERESET);
        
            // Reset again
            TRST_LOW();
            DelayMs(2);
            TRST_HIGH();
        
            break;
        default:
            printf("unknow stats\r\n");
            break;
    }
    return 0;
}

static uint8_t SWJ_WriteCoreReg(uint32_t n, uint32_t val)
{
    int i = 0, timeout = 100;
    
    SWJ_WriteMem32(DCRDR, val);

    SWJ_WriteMem32(DCRSR, n | REGWnR);

    // wait for S_REGRDY
    for (i = 0; i < timeout; i++)
    {
        SWJ_ReadMem32(DHCSR, &val);

        if (val & S_REGRDY)
        {
            return 0;
        }
    }

    return 1;
}

static uint8_t SWJ_ReadCoreReg(uint32_t n, uint32_t *val)
{
    int i = 0, timeout = 100, err;
    if (SWJ_WriteMem32(DCRSR, n))
        return 1;
    
    // wait for S_REGRDY
    for (i = 0; i < timeout; i++)
    {

        if (SWJ_ReadMem32(DHCSR, val))
            return 1;

        if (*val & S_REGRDY)
            break;
    }

    if (i == timeout)
        return 1;

    err = SWJ_ReadMem32(DCRDR, val);

    return err;
}

uint8_t swd_write_debug_state(DEBUG_STATE *state)
{
    uint32_t i, status;

    SWJ_WriteDP(DP_SELECT, 0);

    // R0, R1, R2, R3
    for (i = 0; i < 4; i++)
    {
        SWJ_WriteCoreReg(i, state->r[i]);
    }
    // R9
    SWJ_WriteCoreReg(9, state->r[9]);
    // R13, R14, R15
    for (i=13; i<16; i++)
    {
        SWJ_WriteCoreReg(i, state->r[i]);
    }

    // xPSR
    SWJ_WriteCoreReg(16, state->xpsr);
    SWJ_WriteMem32(DBG_HCSR, DBGKEY | C_DEBUGEN);

    // check status
    SWJ_ReadDP(DP_CTRL_STAT, &status);
    if (status & (STICKYERR | WDATAERR))
    {
        printf("write debug states failed\r\n");
        return 1;
    }

    return 0;
}

uint8_t swd_flash_syscall_exec(const FLASH_SYSCALL *sysCallParam, uint32_t entry, uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4)
{
    DEBUG_STATE state;

    // Call flash algorithm function on target and wait for result.
    state.xpsr     = 0x01000000;          // xPSR: T = 1, ISR = 0
    state.r[0]     = arg1;                   // R0: Argument 1
    state.r[1]     = arg2;                   // R1: Argument 2
    state.r[2]     = arg3;                   // R2: Argument 3
    state.r[3]     = arg4;                   // R3: Argument 4

    state.r[9]     = sysCallParam->static_base;    // SB: Static Base

    state.r[13]    = sysCallParam->stack_pointer;  // SP: Stack Pointer
    state.r[14]    = sysCallParam->breakpoint;       // LR: Exit Point
    state.r[15]    = entry;                           // PC: Entry Point

    if (swd_write_debug_state(&state))
    {
        printf("swd_write_debug_status failed\r\n");
        return 1;
    }
        
    if(SWJ_WaitUntilHalted())
    {
        printf("SWJ_WaitUntilHalted failed\r\n");
        return 1;
    }
        
    if(SWJ_ReadCoreReg(0, &state.r[0]))
    {
        printf("SWJ_ReadCoreReg failed\r\n");
        return 1;
    }
        

    // Flash functions return 0 if successful.
    if (state.r[0] != 0)
    {
        printf("resutlt failed:0x%X\r\n", state.r[0]);
        return 1;
    }
    
    return 0;
}


uint8_t SWJ_InitDebug(void)
{
    uint32_t tmp = 0;
    uint32_t val;
    
    SWJ_JTAG2SWD();
    
    if(SWJ_ReadDP(DP_IDCODE, &val))
    {
        return 1;
    }
    
    SWJ_WriteDP(DP_ABORT, STKCMPCLR | STKERRCLR | WDERRCLR | ORUNERRCLR);

    /* Ensure CTRL/STAT register selected in DPBANKSEL */
    SWJ_WriteDP(DP_SELECT, 0);

    /* Power ups */
    SWJ_WriteDP(DP_CTRL_STAT, CSYSPWRUPREQ | CDBGPWRUPREQ);

    do
    {
        if(!SWJ_ReadDP(DP_CTRL_STAT, &tmp))
        {
            return 0;
        }
    } while ((tmp & (CDBGPWRUPACK | CSYSPWRUPACK)) != (CDBGPWRUPACK | CSYSPWRUPACK));

    SWJ_WriteDP(DP_CTRL_STAT, CSYSPWRUPREQ | CDBGPWRUPREQ | TRNNORMAL | MASKLANE);
    return 0;
}

