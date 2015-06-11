#include <rtthread.h>
#include <lwip/api.h>

#include <finsh.h>
#include <shell.h>

#define TELNET_PORT         23
#define TELNET_RX_BUFFER    256
#define TELNET_TX_BUFFER    4096

#define ISO_nl              0x0a
#define ISO_cr              0x0d

#define STATE_NORMAL        0
#define STATE_IAC           1
#define STATE_WILL          2
#define STATE_WONT          3
#define STATE_DO            4
#define STATE_DONT          5
#define STATE_CLOSE         6

#define TELNET_IAC          255
#define TELNET_WILL         251
#define TELNET_WONT         252
#define TELNET_DO           253
#define TELNET_DONT         254

#define NW_RX               0x01
#define NW_TX               0x02
#define NW_CLOSED           0x04
#define NW_MASK             (NW_RX | NW_TX | NW_CLOSED)

struct rb
{
    rt_uint16_t read_index, write_index;
    rt_uint8_t *buffer_ptr;
    rt_uint16_t buffer_size;
};

struct telnet_session
{
    struct rb rx_ringbuffer;
    struct rb tx_ringbuffer;

    rt_sem_t rx_ringbuffer_lock;
    rt_sem_t tx_ringbuffer_lock;

    struct rt_device device;
    rt_event_t nw_event;
    struct netconn* conn;

    /* telnet protocol */
    rt_uint8_t state;
    rt_uint8_t echo_mode;

};
struct telnet_session* telnet;

/* 一个环形buffer的实现 */
/* 初始化环形buffer，size指的是buffer的大小。注：这里并没对数据地址对齐做处理 */
static void rb_init(struct rb* rb, rt_uint8_t *pool, rt_uint16_t size)
{
    RT_ASSERT(rb != RT_NULL);

    /* 对读写指针清零*/
    rb->read_index = rb->write_index = 0;

    /* 环形buffer的内存数据块 */
    rb->buffer_ptr = pool;
    rb->buffer_size = size;
}

/* 向环形buffer中写入数据 */
static rt_size_t rb_put(struct rb* rb, const rt_uint8_t *ptr, rt_uint16_t length)
{
    rt_size_t size;

    /* 判断是否有足够的剩余空间 */
    if (rb->read_index > rb->write_index)
        size = rb->read_index - rb->write_index;
    else
        size = rb->buffer_size - rb->write_index + rb->read_index;

    /* 没有多余的空间 */
    if (size == 0) return 0;

    /* 数据不够放置完整的数据，截断放入 */
    if (size < length) length = size;

    if (rb->read_index > rb->write_index)
    {
        /* read_index - write_index 即为总的空余空间 */
        memcpy(&rb->buffer_ptr[rb->write_index], ptr, length);
        rb->write_index += length;
    }
    else
    {
        if (rb->buffer_size - rb->write_index > length)
        {
            /* write_index 后面剩余的空间有足够的长度 */
            memcpy(&rb->buffer_ptr[rb->write_index], ptr, length);
            rb->write_index += length;
        }
        else
        {
            /*
             * write_index 后面剩余的空间不存在足够的长度，需要把部分数据复制到
             * 前面的剩余空间中
             */
            memcpy(&rb->buffer_ptr[rb->write_index], ptr,
                   rb->buffer_size - rb->write_index);
            memcpy(&rb->buffer_ptr[0], &ptr[rb->buffer_size - rb->write_index],
                   length - (rb->buffer_size - rb->write_index));
            rb->write_index = length - (rb->buffer_size - rb->write_index);
        }
    }

    return length;
}

/* 向环形buffer中写入一个字符 */
static rt_size_t rb_putchar(struct rb* rb, const rt_uint8_t ch)
{
    rt_uint16_t next;

    /* 判断是否有多余的空间 */
    next = rb->write_index + 1;
    if (next >= rb->buffer_size) next = 0;

    if (next == rb->read_index) return 0;

    /* 放入字符 */
    rb->buffer_ptr[rb->write_index] = ch;
    rb->write_index = next;

    return 1;
}

/* 从环形buffer中读出数据 */
static rt_size_t rb_get(struct rb* rb, rt_uint8_t *ptr, rt_uint16_t length)
{
    rt_size_t size;

    /* 判断是否有足够的数据 */
    if (rb->read_index > rb->write_index)
        size = rb->buffer_size - rb->read_index + rb->write_index;
    else
        size = rb->write_index - rb->read_index;

    /* 没有足够的数据 */
    if (size == 0) return 0;

    /* 数据不够指定的长度，取环形buffer中实际的长度 */
    if (size < length) length = size;

    if (rb->read_index > rb->write_index)
    {
        if (rb->buffer_size - rb->read_index > length)
        {
            /* read_index的数据足够多，直接复制 */
            memcpy(ptr, &rb->buffer_ptr[rb->read_index], length);
            rb->read_index += length;
        }
        else
        {
            /* read_index的数据不够，需要分段复制 */
            memcpy(ptr, &rb->buffer_ptr[rb->read_index],
                   rb->buffer_size - rb->read_index);
            memcpy(&ptr[rb->buffer_size - rb->read_index], &rb->buffer_ptr[0],
                   length - rb->buffer_size + rb->read_index);
            rb->read_index = length - rb->buffer_size + rb->read_index;
        }
    }
    else
    {
        /*
         * read_index要比write_index小，总的数据量够（前面已经有总数据量的判
         * 断），直接复制出数据。
         */
        memcpy(ptr, &rb->buffer_ptr[rb->read_index], length);
        rb->read_index += length;
    }

    return length;
}

static rt_size_t rb_available(struct rb* rb)
{
    rt_size_t size;

    if (rb->read_index > rb->write_index)
        size = rb->buffer_size - rb->read_index + rb->write_index;
    else
        size = rb->write_index - rb->read_index;

    /* 返回ringbuffer中存在的数据大小 */
    return size;
}

/* RT-Thread Device Driver Interface */
static rt_err_t telnet_init(rt_device_t dev)
{
    return RT_EOK;
}

static rt_err_t telnet_open(rt_device_t dev, rt_uint16_t oflag)
{
    return RT_EOK;
}

static rt_err_t telnet_close(rt_device_t dev)
{
    return RT_EOK;
}

static rt_size_t telnet_read(rt_device_t dev, rt_off_t pos, void* buffer, rt_size_t size)
{
    rt_size_t result;

    /* read from rx ring buffer */
    rt_sem_take(telnet->rx_ringbuffer_lock, RT_WAITING_FOREVER);
    result = rb_get(&(telnet->rx_ringbuffer), buffer, size);
    rt_sem_release(telnet->rx_ringbuffer_lock);

    return result;
}

static rt_size_t telnet_write (rt_device_t dev, rt_off_t pos, const void* buffer, rt_size_t size)
{
    const rt_uint8_t *ptr;

    ptr = (rt_uint8_t*)buffer;

    rt_sem_take(telnet->tx_ringbuffer_lock, RT_WAITING_FOREVER);
    while (size)
    {
        if (*ptr == '\n')
            rb_putchar(&telnet->tx_ringbuffer, '\r');

        if (rb_putchar(&telnet->tx_ringbuffer, *ptr) == 0)  /* overflow */
            break;
        ptr ++; size --;
    }
    rt_sem_release(telnet->tx_ringbuffer_lock);

    /* send to network side */
    rt_event_send(telnet->nw_event, NW_TX);

    return (rt_uint32_t)ptr - (rt_uint32_t)buffer;
}

static rt_err_t telnet_control(rt_device_t dev, rt_uint8_t cmd, void *args)
{
    return RT_EOK;
}

/* netconn callback function */
void rx_callback(struct netconn *conn, enum netconn_evt evt, rt_uint16_t len)
{
    if (evt == NETCONN_EVT_RCVPLUS)
    {
        rt_event_send(telnet->nw_event, NW_RX);
    }
    
    if (conn->state == NETCONN_CLOSE)
    {
        rt_event_send(telnet->nw_event, NW_CLOSED);
    } 
}

/* send telnet option to remote */
static void telnet_send_option(struct telnet_session* telnet, rt_uint8_t option, rt_uint8_t value)
{
    rt_uint8_t optbuf[4];

    optbuf[0] = TELNET_IAC;
    optbuf[1] = option;
    optbuf[2] = value;
    optbuf[3] = 0;

    rt_sem_take(telnet->tx_ringbuffer_lock, RT_WAITING_FOREVER);
    rb_put(&telnet->tx_ringbuffer, optbuf, 3);
    rt_sem_release(telnet->tx_ringbuffer_lock);

    /* trigger a tx event */
    rt_event_send(telnet->nw_event, NW_TX);
}

/* process tx data */
void telnet_process_tx(struct telnet_session* telnet, struct netconn *conn)
{
    rt_size_t length;
    rt_uint8_t tx_buffer[32];

    while (1)
    {
        rt_memset(tx_buffer, 0, sizeof(tx_buffer));
        rt_sem_take(telnet->tx_ringbuffer_lock, RT_WAITING_FOREVER);
        /* get buffer from ringbuffer */
        length = rb_get(&(telnet->tx_ringbuffer), tx_buffer, sizeof(tx_buffer));
        rt_sem_release(telnet->tx_ringbuffer_lock);

        /* do a tx procedure */
        if (length > 0)
        {
            netconn_write(conn, tx_buffer, length, NETCONN_COPY);
        }
        else break;
    }
}

/* process rx data */
void telnet_process_rx(struct telnet_session* telnet, rt_uint8_t *data, rt_size_t length)
{
    rt_size_t rx_length, index;

    for (index = 0; index < length; index ++)
    {
        switch(telnet->state)
        {
        case STATE_IAC:
            if (*data == TELNET_IAC)
            {
                /* take semaphore */
                rt_sem_take(telnet->rx_ringbuffer_lock, RT_WAITING_FOREVER);
                /* put buffer to ringbuffer */
                rb_putchar(&(telnet->rx_ringbuffer), *data);
                /* release semaphore */
                rt_sem_release(telnet->rx_ringbuffer_lock);

                telnet->state = STATE_NORMAL;
            }
            else
            {
                /* set telnet state according to received package */
                switch (*data)
                {
                case TELNET_WILL: telnet->state = STATE_WILL; break;
                case TELNET_WONT: telnet->state = STATE_WONT; break;
                case TELNET_DO:   telnet->state = STATE_DO; break;
                case TELNET_DONT: telnet->state = STATE_DONT; break;
                default: telnet->state = STATE_NORMAL; break;
                }
            }
            break;
        
        /* don't option */
        case STATE_WILL:
        case STATE_WONT:
            telnet_send_option(telnet, TELNET_DONT, *data);
            telnet->state = STATE_NORMAL;
            break;

        /* won't option */
        case STATE_DO:
        case STATE_DONT:
            telnet_send_option(telnet, TELNET_WONT, *data);
            telnet->state = STATE_NORMAL;
            break;

        case STATE_NORMAL:
            if (*data == TELNET_IAC) telnet->state = STATE_IAC;
            else if (*data != '\r') /* ignore '\r' */
            {
                rt_sem_take(telnet->rx_ringbuffer_lock, RT_WAITING_FOREVER);
                /* put buffer to ringbuffer */
                rb_putchar(&(telnet->rx_ringbuffer), *data);
                rt_sem_release(telnet->rx_ringbuffer_lock);
            }
            break;
        }

        data ++;
    }

    rt_sem_take(telnet->rx_ringbuffer_lock, RT_WAITING_FOREVER);
    /* get total size */
    rx_length = rb_available(&telnet->rx_ringbuffer);
    rt_sem_release(telnet->rx_ringbuffer_lock);

    /* indicate there are reception data */
    if ((rx_length > 0) && (telnet->device.rx_indicate != RT_NULL))
        telnet->device.rx_indicate(&telnet->device, 
            rx_length);

    return;
}

/* process netconn close */
void telnet_process_close(struct telnet_session* telnet, struct netconn *conn)
{
    /* set console */
    rt_console_set_device("uart1");
    /* set finsh device */
    finsh_set_device("uart1");

    /* close connection */
    netconn_close(conn);

    /* restore shell option */
    finsh_set_echo(telnet->echo_mode);

    rt_kprintf("resume console to uart0\n");
}

/* telnet server thread entry */
void telnet_thread(void* parameter)
{
    rt_err_t result,err;
    rt_uint32_t event;
    struct netbuf *buf;
    struct netconn *conn, *newconn;

    /* Create a new connection identifier. */
    conn = netconn_new(NETCONN_TCP);

    /* Bind connection to well known port number 7. */
    netconn_bind(conn, NULL, TELNET_PORT);

    /* Tell connection to go into listening mode. */
    netconn_listen(conn);

    /* register telnet device */
    telnet->device.type     = RT_Device_Class_Char;
    telnet->device.init     = telnet_init;
    telnet->device.open     = telnet_open;
    telnet->device.close    = telnet_close;
    telnet->device.read     = telnet_read;
    telnet->device.write    = telnet_write;
    telnet->device.control  = telnet_control;

    /* no private */
    telnet->device.user_data = RT_NULL;

    /* register telnet device */
    rt_device_register(&telnet->device, "telnet",
                       RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_STREAM);

    while (1)
    {
        rt_kprintf("telnet server waiting for connection\n");

        /* Grab new connection. */
        err = netconn_accept(conn, &newconn);
        //if (err == RT_EOK) continue;

        /* set network rx call back */
        newconn->callback = rx_callback;

        rt_kprintf("new telnet connection, switch console to telnet...\n");

        /* Process the new connection. */
        /* set console */
        rt_console_set_device("telnet");
        /* set finsh device */
        finsh_set_device("telnet");

        /* set init state */
        telnet->state = STATE_NORMAL;

        telnet->echo_mode = finsh_get_echo();
        /* disable echo mode */
        finsh_set_echo(0);

        while (1)
        {
            /* try to send all data in tx ringbuffer */
            telnet_process_tx(telnet, newconn);

            /* receive network event */
            result = rt_event_recv(telnet->nw_event,
                                   NW_MASK, RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,
                                   RT_TICK_PER_SECOND, &event);
            if (result == RT_EOK)
            {
                /* get event successfully */
                if (event & NW_RX)
                {
                    /* do a rx procedure */
                    err= netconn_recv(newconn, &buf);
                    if (buf != RT_NULL)
                    {
                        rt_uint8_t *data;
                        rt_uint16_t length;

                        /* get data */
                        netbuf_data(buf, (void**)&data, &length);
                        telnet_process_rx(telnet, data, length);
                        /* release buffer */
                        netbuf_delete(buf);
                    }
                    else
                    {
                        if (newconn->last_err == ERR_CLSD)
                        {
                            /* close connection */
                            telnet_process_close(telnet, newconn);
                            break;
                        }
                    }
                }

                if (event & NW_TX)
                    telnet_process_tx(telnet, newconn);

                if (event & NW_CLOSED)
                {
                    /* process close */
                    telnet_process_close(telnet, newconn);
                    break;
                }
            }
            else if (result == -RT_ETIMEOUT)
            {
                if (newconn->state == NETCONN_CLOSE)
                {
                    telnet_process_close(telnet, newconn);
                    break;
                }
            }
        }
    }
}

/* telnet server */
void telnet_srv()
{
    rt_thread_t tid;

    if (telnet == RT_NULL)
    {
        rt_uint8_t *ptr;

        telnet = rt_malloc (sizeof(struct telnet_session));
        if (telnet == RT_NULL)
        {
            rt_kprintf("telnet: no memory\n");
            return;
        }

        /* init ringbuffer */
        ptr = rt_malloc (TELNET_RX_BUFFER);
        rb_init(&telnet->rx_ringbuffer, ptr, TELNET_RX_BUFFER);
        /* create rx ringbuffer lock */
        telnet->rx_ringbuffer_lock = rt_sem_create("rxrb", 1, RT_IPC_FLAG_FIFO);
        ptr = rt_malloc (TELNET_TX_BUFFER);
        rb_init(&telnet->tx_ringbuffer, ptr, TELNET_TX_BUFFER);
        /* create tx ringbuffer lock */
        telnet->tx_ringbuffer_lock = rt_sem_create("txrb", 1, RT_IPC_FLAG_FIFO);

        /* create network event */
        telnet->nw_event = rt_event_create("telnet", RT_IPC_FLAG_FIFO);
    }

    tid = rt_thread_create("telnet", telnet_thread, RT_NULL,
                           2048, 25, 5);
    if (tid != RT_NULL)
        rt_thread_startup(tid);
}

#ifdef RT_USING_FINSH
#include <finsh.h>
FINSH_FUNCTION_EXPORT(telnet_srv, startup telnet server);
#endif
