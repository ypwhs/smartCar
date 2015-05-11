#include <rtthread.h>
#include <lwip/netdb.h>
#include <lwip/sockets.h>

#define BUFSZ	512


#define YL_HTTP_HEADER    "POST /v1.0/device/%d/sensor/%d/datapoints HTTP/1.1\r\n" \
"User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.7; rv:9.0.1) Gecko/20100101 Firefox/9.0.1\r\n" \
"Host: api.yeelink.net\r\n" \
"Accept: */*\r\n" \
"U-ApiKey: %s\r\n" \
"Content-Length: %d\r\n" \
"Content-Type: application/x-www-form-urlencoded\r\n\r\n" 


#define YL_DATA "{\r\n\"timestamp\":\"%s\",\r\n\"value\":%d\r\n}"


void yl_postdata(char *buf, char *key, int device_id, int sensor_id, char* time, int data)
{
    char data_buf[128];
    char heafer_buf[512];
    int len;
    
    len = rt_sprintf(data_buf, YL_DATA, time, data);
    rt_sprintf(heafer_buf, YL_HTTP_HEADER, device_id, sensor_id, key, len);
    
    strcpy(buf, heafer_buf);  
    strcat(buf, data_buf); 
}


void yl_post(void)
{
    char *send_data;
    struct hostent *host;
    int sock, bytes_received;
    struct sockaddr_in server_addr;

    char *url;
    int port;
    
    url = "10.193.21.27";
    port = 5000;
    url = "api.yeelink.net";
    port = 80;
    
    host = gethostbyname(url);

    send_data = rt_malloc(BUFSZ);
    if (send_data == RT_NULL)
    {
        rt_kprintf("No memory\n");
        return;
    }

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        rt_kprintf("Socket error\n");

        rt_free(send_data);
        return;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr = *((struct in_addr *)host->h_addr);
    rt_memset(&(server_addr.sin_zero), 0, sizeof(server_addr.sin_zero));

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
    {
        rt_kprintf("Connect fail!\n");
        lwip_close(sock);

        rt_free(send_data);
        return;
    }

    rt_kprintf("connection ok\r\n");

    yl_postdata(send_data, "6ef5232426a5dccc4883c0b6147ff279", 18318, 32280, "2012-03-18T16:13:14", 113);
    //rt_kprintf(send_data);
    send(sock, send_data, rt_strlen(send_data), 0);
    
    bytes_received = recv(sock, send_data, BUFSZ - 1, 0);
    if (bytes_received <= 0)
    {
        rt_kprintf("no reveived\r\n");
        lwip_close(sock);
        rt_free(send_data);
        return;
    }
    
    
    if(rt_strstr(send_data, "OK"))
    {
        rt_kprintf("post data OK\r\n");
    }
    else
    {
        rt_kprintf("post data failed %s\r\n", send_data);
    }
    
    lwip_close(sock);
    rt_free(send_data);
}

#ifdef RT_USING_FINSH
#include <finsh.h>

FINSH_FUNCTION_EXPORT(yl_post, startup tcp client);
#endif
