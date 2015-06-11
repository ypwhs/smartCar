/*
 * File      : serial.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006 - 2012, RT-Thread Development Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2006-03-13     bernard      first version
 * 2012-05-15     lgnq         modified according bernard's implementation.
 * 2012-05-28     bernard      code cleanup
 * 2012-11-23     bernard      fix compiler warning.
 * 2013-02-20     bernard      use RT_SERIAL_RB_BUFSZ to define
 *                             the size of ring buffer.
 */

#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>

#include "uart.h"
#include "gpio.h"
#include "board.h"
#include "rtt_drv.h"
#include "board.h"




static struct rt_device uart_device;
static char ch;
static bool _rev_flag;

static void UART_ISR(uint16_t byteReceived)
{
    _rev_flag = true;
    ch = (uint8_t)byteReceived;
    uart_device.rx_indicate(&uart_device, 1);
}

static rt_err_t rt_uart_open(rt_device_t dev, rt_uint16_t oflag)
{
	return RT_EOK;
}

static rt_err_t rt_uart_close(rt_device_t dev)
{
	return RT_EOK;
}

static rt_err_t rt_uart_init (rt_device_t dev)
{
    uint32_t instance;
    instance = UART_QuickInit(BOARD_UART_DEBUG_MAP, BOARD_UART_BAUDRATE);
    UART_CallbackRxInstall(instance, UART_ISR);
    UART_ITDMAConfig(instance, kUART_IT_Rx, true);
    dev->user_data = (void*)instance;
    
    return RT_EOK;
}

static rt_size_t rt_uart_read(rt_device_t dev, rt_off_t pos, void* buffer, rt_size_t size)
{
    uint8_t *p;
    int ret;
    
    p = buffer;
    if(_rev_flag)
    {
        p[0] = ch;
        _rev_flag = false;
        return 1;
    }
    return 0;
}

static rt_size_t rt_uart_write(rt_device_t dev, rt_off_t pos, const void* buffer, rt_size_t size)
{
    volatile uint8_t *p;
    
    p = (uint8_t*)buffer;
    
    while(size--)
    {
        /*
         * to be polite with serial console add a line feed
         * to the carriage return character
         */
        if (*p == '\n' && (dev->open_flag & RT_DEVICE_FLAG_STREAM))
        {
            UART_WriteByte((uint32_t)dev->user_data, '\r');
        }
        UART_WriteByte((uint32_t)dev->user_data, *p++);
    }
    return size;
}

static rt_err_t rt_uart_control(rt_device_t dev, rt_uint8_t cmd, void *args)
{
    return RT_EOK; 
}

void rt_hw_uart_init(void)
{

	uart_device.type 		= RT_Device_Class_Char;
	uart_device.rx_indicate = RT_NULL;
	uart_device.tx_complete = RT_NULL;
	uart_device.init 		= rt_uart_init;
	uart_device.open		= rt_uart_open;
	uart_device.close		= rt_uart_close;
	uart_device.read 		= rt_uart_read;
	uart_device.write       = rt_uart_write;
	uart_device.control 	= rt_uart_control;
	uart_device.user_data	= RT_NULL;

    rt_device_register(&uart_device, "uart0", RT_DEVICE_FLAG_RDWR);
}
INIT_BOARD_EXPORT(rt_hw_uart_init);
