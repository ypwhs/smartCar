/*----------------------------------------------------------------------------
 *      RL-ARM - USB
 *----------------------------------------------------------------------------
 *      Name:    usbd_user_msc.c
 *      Purpose: Mass Storage Device Class User module
 *      Rev.:    V4.70
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2013 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

//#include <RTL.h>
#include <rl_usb.h>
#include <string.h>

extern const unsigned char DiskImage[0x1000];

U8 Memory[1024*4];                       /* MSC Memory in RAM                  */
U8 BlockBuf[512];

void usbd_msc_init () {
  USBD_MSC_MemorySize = 1024*4;
  USBD_MSC_BlockSize  = 512;
  USBD_MSC_BlockGroup = 1;
  USBD_MSC_BlockCount = USBD_MSC_MemorySize / USBD_MSC_BlockSize;
  USBD_MSC_BlockBuf   = BlockBuf;

  memcpy(Memory, DiskImage, sizeof(DiskImage));
  USBD_MSC_MediaReady = __TRUE;
}

void usbd_msc_read_sect (U32 block, U8 *buf, U32 num_of_blocks) {
  if (USBD_MSC_MediaReady) {
    memcpy(buf, &Memory[block * USBD_MSC_BlockSize], num_of_blocks * USBD_MSC_BlockSize);
  }
}

void usbd_msc_write_sect (U32 block, U8 *buf, U32 num_of_blocks) {
  if (USBD_MSC_MediaReady) {
    memcpy(&Memory[block * USBD_MSC_BlockSize], buf, num_of_blocks * USBD_MSC_BlockSize);
  }
}
