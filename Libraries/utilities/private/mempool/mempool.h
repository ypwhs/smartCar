/**
  ******************************************************************************
  * @file    mempool.h
  * @author  YANDLD
  * @version V2.5
  * @date    2013.12.25
  * @brief   www.beyondcore.net   http://upcmcu.taobao.com 
  ******************************************************************************
  */


#ifndef __MEMPOOL_H__
#define __MEMPOOL_H__

#include <stdint.h>
#include <stdio.h>


/**
 * @ingroup BasicDef
 *
 * @def RT_ALIGN(size, align)
 * Return the most contiguous size aligned at specified width. RT_ALIGN(13, 4)
 * would return 16.
 */
#define RT_ALIGN(size, align)           (((size) + (align) - 1) & ~((align) - 1))

/**
 * @ingroup BasicDef
 *
 * @def RT_ALIGN_DOWN(size, align)
 * Return the down number of aligned at specified width. RT_ALIGN_DOWN(13, 4)
 * would return 12.
 */
#define RT_ALIGN_DOWN(size, align)      ((size) & ~((align) - 1))


typedef struct mempool
{

    void            *start_address;                     /**< memory pool start */
    size_t          size;                              /**< size of memory pool */

    size_t          block_size;                        /**< size of memory blocks */
    uint8_t         *block_list;                        /**< memory blocks list */

    size_t          block_total_count;                 /**< numbers of memory block */
    size_t          block_free_count;                  /**< numbers of free memory block */

}mempool,*mp_t;


//!< API functions
int mp_init(struct mempool *mp, const char *name, void *start, size_t size, size_t block_size);
void *mp_alloc(mp_t mp);
void mp_free(void *block);

#endif

