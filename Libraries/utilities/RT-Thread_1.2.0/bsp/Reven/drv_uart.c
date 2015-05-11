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
#include "uart.h"
#include "gpio.h"
#include "board.h"
#include "drv_uart.h"
#include <rtdevice.h>

static struct rt_serial_device serial;

static uint16_t *gRevCh = RT_NULL;
static void UART_ISR(uint16_t byteReceived)
{
    static uint16_t ch;
    rt_interrupt_enter();
    ch = byteReceived;
    gRevCh = &ch;
    rt_hw_serial_isr(&serial, RT_SERIAL_EVENT_RX_IND);
    rt_interrupt_leave();
}

static rt_err_t _configure(struct rt_serial_device *serial, struct serial_configure *cfg)
{
	return RT_EOK;
}

static rt_err_t _control(struct rt_serial_device *serial, int cmd, void *arg)
{
    switch (cmd)
    {
        case RT_DEVICE_CTRL_SET_INT:
            UART_ITDMAConfig(serial->config.reserved, kUART_IT_Rx, true);
            break;
        case RT_DEVICE_CTRL_CLR_INT:
            UART_ITDMAConfig(serial->config.reserved, kUART_IT_Rx, false);
            break;
    }
    return RT_EOK;
}

static int _putc(struct rt_serial_device *serial, char c)
{
    UART_WriteByte(serial->config.reserved, c);
    return 1;
}

static int _getc(struct rt_serial_device *serial)
{
    int c;
    c = -1;
    
    if(gRevCh != RT_NULL)
    {
        c = *gRevCh;
        gRevCh = RT_NULL;
    }
    return c;
}

static const struct rt_uart_ops _ops =
{
    _configure,
    _control,
    _putc,
    _getc,
};

void rt_hw_uart_init(void)
{
    uint32_t instance;
    instance = UART_QuickInit(BOARD_UART_DEBUG_MAP, BOARD_UART_BAUDRATE);
    
    /* 启动FIFO加速 */
    UART_EnableTxFIFO(instance, true);
    UART_SetTxFIFOWatermark(instance, UART_GetTxFIFOSize(instance));
    
    struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;
    config.reserved  = instance;
    serial.config = config;
    serial.ops    = &_ops;
    
    /* 回调函数 */
    UART_CallbackRxInstall(instance, UART_ISR);
    rt_hw_serial_register(&serial, "uart0",
                          RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX | RT_DEVICE_FLAG_STREAM,
                          (void*)0);

}

INIT_BOARD_EXPORT(rt_hw_uart_init);
