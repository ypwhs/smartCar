/* RT-Thread config file */
#ifndef __RTTHREAD_CFG_H__
#define __RTTHREAD_CFG_H__

#define RT_NAME_MAX                 8
#define RT_ALIGN_SIZE               4
#define RT_THREAD_PRIORITY_MAX      64
#define RT_TICK_PER_SECOND          100

/* SECTION: RT_DEBUG */
/* Thread Debug */
#define RT_DEBUG
//#define RT_USING_COMPONENTS_INIT
//#define RT_DEBUG_INIT   1
//#define RT_USING_MODULE

#define RT_USING_OVERFLOW_CHECK
#define RT_USING_HOOK
#define IDLE_THREAD_STACK_SIZE      512

/* Using Software Timer */
#define RT_USING_TIMER_SOFT 
#define RT_TIMER_THREAD_PRIO		4
#define RT_TIMER_THREAD_STACK_SIZE	512
#define RT_TIMER_TICK_PER_SECOND	10

/* SECTION: IPC */
#define RT_USING_SEMAPHORE
#define RT_USING_MUTEX
#define RT_USING_EVENT
#define RT_USING_MAILBOX
#define RT_USING_MESSAGEQUEUE


/* Using Dynamic Heap Management */
#define RT_USING_HEAP
#define RT_USING_MEMPOOL
//#define RT_USING_MEMHEAP_AS_HEAP
/* Using Small MM */
#define RT_USING_SMALL_MEM

/* SECTION: Device System */
/* Using Device System */
#define RT_USING_DEVICE
#define RT_USING_SERIAL
#define RT_USING_SPI
#define RT_USING_I2C
//#define RT_I2C_BIT_DEBUG
//#define RT_I2C_DEBUG
#define RT_USING_I2C_BITOPS
/* SECTION: Console options */
#define RT_USING_CONSOLE
/* the buffer size of console*/
#define RT_CONSOLEBUF_SIZE	255

/* SECTION: finsh, a C-Express shell */
#define RT_USING_FINSH
#define RT_CONSOLE_DEVICE_NAME "uart0"
/* Using symbol table */
#define FINSH_USING_SYMTAB
#define FINSH_USING_DESCRIPTION
#define FINSH_USING_MSH
#define FINSH_USING_MSH_DEFAULT
#define FINSH_THREAD_STACK_SIZE   (1024*4)

/* SECTION: device filesystem */
#define RT_USING_DFS 
#define RT_USING_DFS_ROMFS
#define RT_USING_DFS_ELMFAT
#define RT_DFS_ELM_WORD_ACCESS
#define DFS_USING_WORKDIR
/* Reentrancy (thread safe) of the FatFs module.  */
#define RT_DFS_ELM_REENTRANT
/* Number of volumes (logical drives) to be used. */
#define RT_DFS_ELM_DRIVES           2
#define RT_DFS_ELM_USE_LFN          3 
#define RT_DFS_ELM_MAX_LFN			255
/* Maximum sector size to be handled. */
#define RT_DFS_ELM_MAX_SECTOR_SIZE  4096

#define RT_USING_DFS_NFS
#define RT_NFS_HOST_EXPORT "10.193.32.65:/"

/* the max number of mounted filesystem */
#define DFS_FILESYSTEMS_MAX			4
/* the max number of opened files 		*/
#define DFS_FD_MAX					4





/* SECTION: lwip, a lighwight TCP/IP protocol stack */
#define RT_USING_LWIP
/* LwIP uses RT-Thread Memory Management */
//#define RT_LWIP_USING_RT_MEM
#define RT_LWIP_ICMP
#define RT_LWIP_UDP
#define RT_LWIP_TCP
#define RT_LWIP_DNS
#define RT_LWIP_DEBUG
#define RT_LWIP_DHCP
/* the number of simulatenously active TCP connections*/
#define RT_LWIP_TCP_PCB_NUM	5

/* ip address of target*/
#define RT_LWIP_IPADDR0	192
#define RT_LWIP_IPADDR1	168
#define RT_LWIP_IPADDR2	0
#define RT_LWIP_IPADDR3	201

/* gateway address of target*/
#define RT_LWIP_GWADDR0	192
#define RT_LWIP_GWADDR1	168
#define RT_LWIP_GWADDR2	0
#define RT_LWIP_GWADDR3	1

/* mask address of target*/
#define RT_LWIP_MSKADDR0	255
#define RT_LWIP_MSKADDR1	255
#define RT_LWIP_MSKADDR2	255
#define RT_LWIP_MSKADDR3	0

/* tcp thread options */
#define RT_LWIP_TCPTHREAD_PRIORITY		5
#define RT_LWIP_TCPTHREAD_MBOX_SIZE		8
#define RT_LWIP_TCPTHREAD_STACKSIZE		1048

/* ethernet if thread options */
#define RT_LWIP_ETHTHREAD_PRIORITY		6
#define RT_LWIP_ETHTHREAD_MBOX_SIZE		8
#define RT_LWIP_ETHTHREAD_STACKSIZE		1048

/* TCP sender buffer space */
#define RT_LWIP_TCP_SND_BUF	8192
/* TCP receive window. */
#define RT_LWIP_TCP_WND		8192


//#define CHECKSUM_CHECK_TCP              0
//#define CHECKSUM_CHECK_IP               0
//#define CHECKSUM_CHECK_UDP              0

//#define CHECKSUM_GEN_TCP                0
//#define CHECKSUM_GEN_IP                 0
//#define CHECKSUM_GEN_UDP                0

#endif
