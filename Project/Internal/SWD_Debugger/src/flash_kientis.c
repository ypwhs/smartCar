#include "swd.h"
#include "flash_kinetis.h"

#define MDM_STATUS              (0x01000000)
#define MDM_CTRL                (0x01000004) 
#define MDM_IDR                 (0x010000FC)
#define SIM_SDID_BASE           (0x40048024)
#define KINETIS_KK_MCU_ID       (0x001C0000)
#define KINETIS_KL_MCU_ID       (0x001C0020)

#define FLASH_SECTOR_SIZE                  (1024)

#define TARGET_AUTO_INCREMENT_PAGE_SIZE    (0x400)

uint32_t TFlashGetMemID(void)
{
    uint32_t id;
    SWJ_ReadAP(MDM_IDR, &id);
    return id;
}

uint32_t TFlashGetSDID(void)
{
    uint32_t id;
    SWJ_ReadMem(SIM_SDID_BASE, (uint8_t*)&id, 4);
    return id;
}

uint8_t TFlash_UnlockSequence(void)
{
    uint32_t val;

    /* read the device ID */
    if(SWJ_ReadAP(MDM_IDR, &val))
    {
        printf("readID failed:0x%08X\r\n", val);
        return 1;
    }

    /* verify the result */
    if ((val != KINETIS_KK_MCU_ID) && (val != KINETIS_KL_MCU_ID))
    {
        printf("MCU_ID does not match!0x%X\r\n", val);
        return 1;
    }

    if(SWJ_ReadAP(MDM_STATUS, &val))
    {
        printf("read mem_status error\r\n");
        return 3;
    }

    printf("MEMSTATUS:0x%X\r\n", val);
    
    /* flash in secured mode */
   // if (val & (1 << 2))
    {
        printf("system is secured mode\r\n");
        
        /* hold the device in reset */
        //SWJ_SetTargetState(RESET_PROGRAM);
        SWJ_SetTargetState(RESET_HOLD);
        /* write the mass-erase enable bit */
        if(SWJ_WriteAP(MDM_CTRL, 1))
        {
            printf("write enable failed\r\n");
            return 1;
        }
        
        while (1)
        {
            //SWJ_WriteAP(MDM_CTRL, 1);
            /* wait until mass erase is started */
            if(SWJ_ReadAP(MDM_STATUS, &val))
            {
                printf("read satatus failed\r\n");
                return 2;
            }
            
            if (val & 1) break;
        }
        
        /* mass erase in progress */
        while (1)
        {            
            /* keep reading until procedure is complete */
            if(SWJ_ReadAP(MDM_CTRL, &val))
                return 3;

            if (val == 0)
                break;
        }
    }
    return 0;
}


uint8_t TFlash_Init(const TARGET_FLASH* flash)
{
    uint8_t err;
    
    /* Download flash programming algorithm to target and initialise. */
    err = SWJ_WriteMem(flash->algo_start, (uint8_t *)flash->image, flash->algo_size);
    if(err)
    {
        printf("write flash algo to RAM failed\r\n");
    }
    
    err = swd_flash_syscall_exec(&flash->sys_call_param, flash->init, 0, 0 /* clk value is not used */, 0, 0);
    
    return err;
}

uint8_t target_flash_erase_sector(const TARGET_FLASH* flash, unsigned int sector)
{
    uint8_t err;
    err = swd_flash_syscall_exec(&flash->sys_call_param, flash->erase_sector, sector * FLASH_SECTOR_SIZE, 0, 0, 0);
    return err;
}

uint8_t target_flash_erase_chip(const TARGET_FLASH* flash)
{
    uint8_t err;
    err = swd_flash_syscall_exec(&flash->sys_call_param, flash->erase_chip, 0, 0, 0, 0);
    return err;
}


uint8_t target_flash_program_page(const TARGET_FLASH* flash, uint32_t addr, uint8_t * buf, uint32_t size)
{
    uint32_t bytes_written = 0;

    // call a target dependent function to check if
    // we don't want to write specific bits (flash security bits, ...)
//    if (!check_security_bits(addr, buf)) {
//        return 0;
//    }

    // Program a page in target flash.
    if(SWJ_WriteMem(flash->program_buffer, buf, size))
    {
        printf("SWJ_WriteMem failed\r\n");
        return 1;
    }

    while(bytes_written < size) {
        if (swd_flash_syscall_exec(&flash->sys_call_param,
                                    flash->program_page,
                                    addr,
                                    flash->ram_to_flash_bytes_to_be_written,
                                    flash->program_buffer + bytes_written, 0))
        {
                                        printf("call error\r\n");
            return 1;
        }

        bytes_written += flash->ram_to_flash_bytes_to_be_written;
        addr += flash->ram_to_flash_bytes_to_be_written;
    }

    return 0;
}

