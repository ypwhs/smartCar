


#include <stdio.h>
#include "common.h"
#include "MT47H64M16HR.h"


//
// Input file: MT47H64M16.mem
//

// Copy and paste the following section of the code to c file. Or,
// attached the following file to the project.
//
// This tool will not generate the DDR clock, but use the pre-set ddr
// clock for DDR timing calculation to the optimal settings.
//
// DRAM size: 256MB
//




int ddr_init_script(void)
{
	if ((SIM->SCGC3 & SIM_SCGC3_DDR_MASK) != SIM_SCGC3_DDR_MASK)
	{
		// Enable DDR clock gate
		SIM->SCGC3 |= SIM_SCGC3_DDR_MASK;
	}
	else
	{
		// Check to if the DRAMC is already initialized
		if ((DDR->CR00 & 1) == 1)
			return 1;
	}

	// Setup drive strength & enable IO pad
	SIM->MCR &= 0xFFFFFF00;
	SIM->MCR |= 0x00000044;

	// Setup IO pad ODT, 700mV seperation & spare dly ctrl
#define DDR_PAD_CTRL	*((volatile unsigned long*)0x400AE1AC)
	DDR_PAD_CTRL = 0x01030203;

	SIM->MCR |= 0x100;	// DDR RCR reset
	SIM->MCR &= ~0x100;	// DDR RCR out of reset

	DDR->CR00 = 0x00000400;	// DDRCLS, start
	DDR->CR02 = 0x02007530;	// initaref, tinit
	DDR->CR03 = 0x02020707;	// tccd, wrlat, latgate, latlin
	DDR->CR04 = 0x07090202;	// trasmin, trc, trrd, tbint
	DDR->CR05 = 0x02020302;	// tmrd, trtp, trp, twtr
	DDR->CR06 = 0x00290402;	// intwbr, trasmax, tmod
	DDR->CR07 = 0x01010303;	// ccapen, ap, tckesr, clkpw
	DDR->CR08 = 0x06030301;	// tdal, twr, trasdi, tras 
	DDR->CR09 = 0x020000c8;	// bstlen, nocmd, tdll 
	DDR->CR10 = 0x02000808;	// trpab, tcpd, tfaw
	DDR->CR11 = 0x01000000;	// trefen, arefmode, aref, regdimm
	DDR->CR12 = 0x048a001e;	// tref, trfc
	DDR->CR13 = 0x00000005;	// pd, trefint
	DDR->CR14 = 0x00c70002;	// txsr, tpdex
	DDR->CR15 = 0x00000015;	// puref, sref, txsnr
	DDR->CR16 = 0x00000001;	// lpctrl, clkdly, qkref
	DDR->CR20 = 0x00030300;	// cksrx, cksre, lpre
	DDR->CR21 = 0x24040232;	// mr1dat0, mr0dat0
	DDR->CR25 = 0x0a010201;	// aprebit, colsiz, addpins, bnk8
	DDR->CR26 = 0x0101ffff;	// bnkspt, addcol, cmdage, agecnt
	DDR->CR27 = 0x00010101;	// swpen, rwen, prien, plen
	DDR->CR28 = 0x00000001;	// cmdlatr, bigend, reduc, csmap
	DDR->CR30 = 0x00000001;	// intack, rsyncrf
	DDR->CR34 = 0x00000101;	// odtwrcs, odtrdcs
	DDR->CR37 = 0x00000200;	// w2wsame, w2rsame, r2wsame, r2rsame
	DDR->CR38 = 0x00200000;	// p0wrcnt, pupcs, pdncs
	DDR->CR39 = 0x00000020;	// wp0, rp0, p0rdcnt
	DDR->CR40 = 0x00002000;	// p1wrcnt, p0typ
	DDR->CR41 = 0x01010020;	// wp1, rp1, p1rdcnt
	DDR->CR42 = 0x00002000;	// p2wrcnt, p1typ
	DDR->CR43 = 0x02020020;	// wp2, rp2, p2rdcnt
	DDR->CR45 = 0x00070b0f;	// p0pri3, p0pri2, p0pri1, p0pri0
	DDR->CR46 = 0x0f004000;	// p1pri0, p0prirlx, p0ord
	DDR->CR47 = 0x0100070b;	// p1ord, p1pri3, p1pri2, p1pri1
	DDR->CR48 = 0x0b0f0040;	// p2pri1, p2pri0, p1prirlx
	DDR->CR49 = 0x00020007;	// p2ord, p2pri3, p2pri2
	DDR->CR50 = 0x00000040;	// p2prirlx
	DDR->CR52 = 0x02000602;	// rddtenbas, phyrdlat, phywrltbs
	DDR->CR56 = 0x02030000;	// wrlatadj, rdlatadj
	DDR->CR57 = 0x01000000;	// odtalten

	DDR->CR00 |= 1;	// DDRCLS, start

	while ((DDR->CR30 & 0x400) != 0x400);

	// Setup memory address translation
	MCM->CR |= MCM_CR_DDRSIZE(2);

	return 0;
}

uint32_t DDR_SelfTest(void)
{
    uint32_t i;
    uint32_t address, size;
    address = 0x70000000;
    size = 256*1024*1024;
    printf("memtest,address: 0x%08X size: 0x%08X\r\n", address, size);
    
    /**< 8bit test */
    {
        uint8_t * p_uint8_t = (uint8_t *)address;
        for(i=0; i<size/sizeof(uint8_t); i++)
        {
            *p_uint8_t++ = (uint8_t)i;
             if(!(i%(1024*1024)))
             {
                 printf(">");
             }
        }
    
        printf("read...\r\n");
        
        p_uint8_t = (uint8_t *)address;
        for(i=0; i<size/sizeof(uint8_t); i++)
        {
            if( *p_uint8_t != (uint8_t)i )
            {
                printf("8bit test fail @ 0x%08X\r\nsystem halt!!!!!",(uint32_t)p_uint8_t);
                return 1;
            }
            if(!(i%(1024*1024)))
            {
                printf(">");
            }
            p_uint8_t++;
        }
        printf("8bit test pass!!\r\n");
    }
    
    /**< 16bit test */
    {
        uint16_t * p_uint16_t = (uint16_t *)address;
        for(i=0; i<size/sizeof(uint16_t); i++)
        {
            *p_uint16_t++ = (uint16_t)i;
        }

        p_uint16_t = (uint16_t *)address;
        for(i=0; i<size/sizeof(uint16_t); i++)
        {
            if( *p_uint16_t != (uint16_t)i )
            {
                printf("16bit test fail @ 0x%08X\r\nsystem halt!!!!!",(uint32_t)p_uint16_t);
                return 2;
            }
            p_uint16_t++;
        }
        printf("16bit test pass!!\r\n");
    }

    /**< 32bit test */
    {
        uint32_t * p_uint32_t = (uint32_t *)address;
        for(i=0; i<size/sizeof(uint32_t); i++)
        {
            *p_uint32_t++ = (uint32_t)i;
        }

        p_uint32_t = (uint32_t *)address;
        for(i=0; i<size/sizeof(uint32_t); i++)
        {
            if( *p_uint32_t != (uint32_t)i )
            {
                printf("32bit test fail @ 0x%08X\r\nsystem halt!!!!!",(uint32_t)p_uint32_t);
                return 3;
            }
            p_uint32_t++;
        }
        printf("32bit test pass!!\r\n");
    }

    /**< 32bit Loopback test */
    {
        uint32_t * p_uint32_t = (uint32_t *)address;
        for(i=0; i<size/sizeof(uint32_t); i++)
        {
            *p_uint32_t  = (uint32_t)p_uint32_t;
            *p_uint32_t++;
        }

        p_uint32_t = (uint32_t *)address;
        for(i=0; i<size/sizeof(uint32_t); i++)
        {
            if( *p_uint32_t != (uint32_t)p_uint32_t )
            {
                printf("32bit Loopback test fail @ 0x%08X", (uint32_t)p_uint32_t);
                printf(" data:0x%08X \r\n", (uint32_t)*p_uint32_t);
                return 4;
            }
            p_uint32_t++;
        }
        printf("32bit Loopback test pass!!\r\n");
    }
    return  0;
}

int MT47H64M16HR_Init(void)
{
    return ddr_init_script();
}


