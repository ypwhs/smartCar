//http.c
//??:??
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <lwip/api.h>
#include <lwip/sockets.h>


#define  URL_LEN  128
#define  FILENAME_LEN  128
#define  PATH_LEN      128


int Connect(int fd, struct sockaddr *addr, socklen_t len)
{
    rt_kprintf("%d\r\n", addr->sa_family);
    rt_kprintf("len%d\r\n", len);
    
    int result;
    if ((result = connect(fd, addr, len)) == -1) {
        rt_kprintf("connect");
        rt_kprintf("exit\r\n");
        while(1);
    }
    return 0;
}

int Socket_Connect(char *ip,char *port)
{
    struct sockaddr_in addr;

    int sockfd;
    int len;

    
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);//127.0.0.1???ip
    
    rt_kprintf("prot:%d\r\n", atoi(port));
    rt_kprintf("addr.sin_addr.s_addr:%X\r\n", addr.sin_addr.s_addr);
    
    addr.sin_port = htons(atoi(port));
    len = sizeof(addr);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1)
    {
        rt_kprintf("Socket error\n");
    }        
            
    Connect(sockfd, (struct sockaddr*)&addr, len);
    return sockfd;

}


/*
??:?socketfd????,?????????len??
??:
sockfd ???????????????????
msg ?????,?????????????
len ???????????
flags ??????????0
??:
0-------- ??
??---??
??:
??:
??
*/
int Send(int sockfd, char *sendbuf, int len, int flags)
{
    int sendlen = 0;
    int ret = -1;

    while(sendlen < len)
    {
        ret = send(sockfd, sendbuf+sendlen, len-sendlen, flags); 
        if(-1 == ret)
        {
            rt_kprintf("send");
        rt_kprintf("exit\r\n");
        }
        else
            sendlen += ret;

    }

    return 0;

}

int Close(int sockfd)
{
    return close(sockfd);
    

}
int Recv(int sockfd, char *recvbuf, int len, int flags)
{
    int recv_len;

    if ((recv_len = recv(sockfd, recvbuf, len, flags)) < 0)
    {
        rt_kprintf("recv error");
        rt_kprintf("exit!!\r\n");
    }

    return recv_len;
}



#define MAX_RECV_SIZE    1440//????????????
char g_host[URL_LEN];
char g_ip[URL_LEN+1];//ip/??
char g_port[5+1];

char g_buf_send[4*1024];//???????
char g_buf_recv[10*1024];//???????

//BreakPoint_ST g_break_point;

/*
??:???????,????url????
??:
??:
>0---------??,???????
-1----------??
*/
//int Get_Breakpoint_Available(BreakPoint_ST *breakpoint,char *url,char *file_crc)
//{
//    
//    //????????,?????????
//    if((memcmp(breakpoint->url,url,strlen(url))== 0)&&(breakpoint->recv_size== MAX_RECV_SIZE))
//        return breakpoint->download_size;
//    else
//    {
//        return -1;
//    }
//    
//}


/*
??:?????????????
??:
filename---???????
file_crc----?????????????
??:
0---------???
>0--------???,???????
*/
int Get_Breakpoint(char *url,char *filename,char *file_crc)
{
//    char filename_bp[64];
//    int fd = -1;
//    int ret;
//    BreakPoint_ST break_point;
//    
//    //????? filename+bp
//    sprintf(filename_bp,"%s.bp",filename);

//    //??????filename????
//    fd = open(filename_bp,O_RDONLY,S_IRUSR|S_IWUSR);
//    if(fd == -1)
//    {    
//        #ifdef DEBUG_HTTP
//       rt_kprintf("no exsit %s\n",filename_bp);
//        #endif
//        return 0;
//    }

//    //????
//    ret = read(fd,&break_point,sizeof(break_point));
//    if(ret != sizeof(break_point))
//    {
//        rt_kprintf("ERR:Get_Breakpoint read");
//        exit(-1);
//    }

//    close(fd);

//    //????????
//    ret = Get_Breakpoint_Available(&break_point,url,file_crc);
//    if(ret > 0)
//        return ret;
//    else
//    {
//        
//       rt_kprintf("%s not available\n",filename_bp);
//        remove(filename);
//        remove(filename_bp);
//        return 0;
//        
//    }
}

/*
??:??????,???filename.bp
??:
filename---???????
file_crc----?????????????
??:
0---------??
>0--------???,???????
*/
int Save_Breakpoint(char *url,char *filename,int download_size,char *file_crc)
{
//    int fd;
//    BreakPoint_ST breakpoint;
//    char filename_bp[128];//???????,????

//    sprintf(filename_bp,"%s.bp",filename);
//    /* ?????? */
//    if((fd=open(filename_bp,O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR))==-1)
//    {
//        fprintf(stderr,"Open %s Error:%s\n",filename_bp,strerror(errno));
//        exit(1);
//    }
//    memset(&breakpoint,0x0,sizeof(breakpoint));
//    strcpy(breakpoint.url,url);
//    //strcpy(breakpoint.crc,file_crc);
//    strcpy(breakpoint.filename,filename);
//    breakpoint.download_size = download_size;
//    breakpoint.recv_size= MAX_RECV_SIZE;
//    
//    //xu tioa zheng wei fen ci xie ru
//    if(write(fd,&breakpoint,sizeof(breakpoint)) != sizeof(breakpoint))
//    {
//        rt_kprintf("ERR:Save_Breakpoint");
//        exit(1);
//    }

//    close(fd);

//    return 0;



}

/*
??:????,???
??:
??:
0---------??
*/

int Save_File(char *filebuf,int filelength,char *filename)
{
    rt_kprintf("Save_File\r\n");
//    int fd;
//    /* ????????? */
//    if((fd=open(filename,O_WRONLY|O_CREAT|O_APPEND,S_IRUSR|S_IWUSR))==-1)
//    {
//        fprintf(stderr,"Open %s Error:%s\n",filename,strerror(errno));
//        exit(1);
//    }
//    //xu tioa zheng wei fen ci xie ru
//    if(write(fd,filebuf,filelength) != filelength)
//    {
//        rt_kprintf("ERR:Save_File");
//        exit(1);
//    }

//    close(fd);

//    return 0;


}

 
int HTTP_GetResponseCode(void)
{
    return 0;
}

 /*
??:??http???????????
??:
revbuf--------???????
???:
>=0---------??(????)???
-1-----------??????
*/
int HTTP_GetRecvLength(char *revbuf)
{
    char *p1 = NULL;
    int HTTP_Body = 0;//?????
    int HTTP_Head = 0;//HTTP ?????


    HTTP_Body = HTTP_GetContentLength(revbuf);
    if(HTTP_Body == -1)
        return -1;

    p1=strstr(revbuf,"\r\n\r\n");
    if(p1==NULL)
        return -1;
    else
    {
        HTTP_Head = p1- revbuf +4;// 4?\r\n\r\n???
        return HTTP_Body+HTTP_Head;
    }


}


/*
??:??http???Content-Length??
??:
revbuf--------??????
???:
>=0---------Content-Length??
-1-----------??????
*/
int HTTP_GetContentLength(char *revbuf)
{
    char *p1 = NULL, *p2 = NULL;
    int HTTP_Body = 0;//?????

    p1 = strstr(revbuf,"Content-Length");
    if(p1 == NULL)
        return -1;
    else
    {
        p2 = p1+strlen("Content-Length")+ 2; 
        HTTP_Body = atoi(p2);
        return HTTP_Body;
    }

}

 /*
 ??:
 ??:
 sockfd--------???????
 ???:
 >0---------?????
 -1----------??
 =0---------???????
 ?:??????10k
 */

int HTTP_Recv(int sockfd,char *buf_recv)
{
    int ret;
    int recvlen=0;
    int downloadlen = 0;
    //int contentlen=0;
    char buf_recv_tmp[10*1024+1];
    
    memset(buf_recv_tmp,0x0,sizeof(buf_recv_tmp)); 
    while(1)
    {
        ret = Recv(sockfd,buf_recv_tmp+recvlen,sizeof(buf_recv_tmp)-1,0);

        if(ret <= 0)//????
        {
            rt_kprintf("ERR:recv fail");
            return ret;
        }
    
    
        if(recvlen == 0)
        {
            #ifdef DEBUG_HTTP_RECV
           rt_kprintf("recv len = %d\n", ret);
            rt_kprintf("recv = %s\n", buf_recv_tmp);
            #endif
            //????????;
            downloadlen = HTTP_GetRecvLength(buf_recv_tmp);


            #ifdef DEBUG_HTTP_RECV
           rt_kprintf("downloadlen = %d\n",downloadlen);
            #endif
        }

        recvlen += ret;
        #ifdef DEBUG_HTTP_RECV
       rt_kprintf("total recvlen = %d\n",recvlen);
        #endif

        if(downloadlen == recvlen)//????
            break;


    }
    memcpy(buf_recv,buf_recv_tmp,downloadlen);
    return recvlen;

}

/*
??:????url?????,????/????
??:
???:
0-----------??
-1----------??
?:??????10k
*/

int HTTP_GetFileName(char *url,char *filename)
{
    //??url?????/????
    int len;
    int i;

    len = strlen(url);
    for(i=len-1;i>0;i--)
    {
        if(url[i] == '/')
            break;
    }
    if(i == 0)//??????
    {
       rt_kprintf("url not contain '/'\n");
        return -1;
    }
    else
    {
    
        strcpy(filename,url+i+1);
        #ifdef DEBUG_HTTP
       rt_kprintf("filename=%s\n",filename);
        #endif
        return 0;
    }
}

/*
??:????url????,???/????
??:
???:
0-----------??
-1----------??
?:url ex "http://host:port/path"
*/
int HTTP_GetPath(char *url,char *path)
{
    char *p;

    p = strstr(url,"http://");
    if(p == NULL)
    {
        p = strchr(url,'/');
        if(p == NULL)
            return -1;
        else
        {
            strcpy(path,p);
            return 0;
        }
    }
    else
    {
        p = strchr(url+strlen("http://"),'/');
        if(p == NULL)
            return -1;
        else
        {
            strcpy(path,p);
            return 0;
        }
    }

}
/*
??:????url??ip?port,ip????,?????80
??:
???:
1-----------???
2-----------ip port?
-1----------??
?:url ex "http://host:port/path"
*/

int HTTP_Get_IP_PORT(char *url,char *ip,char *port)
{
    char *p = NULL;
    int offset = 0;
    char DOMAIN_NAME[128];

    p = strstr(url,"http://");
    if(p == NULL)
    {
        offset = 0;
    }
    else
    {
        offset = strlen("http://");
    }

    p = strchr(url+offset,'/');
    if(p == NULL)
    {
       rt_kprintf("url:%s format error\n",url);
        return -1;
        
    }
    else
    {

        memset(DOMAIN_NAME,0x0,sizeof(DOMAIN_NAME));
        memcpy(DOMAIN_NAME,url+offset,(p-url-offset));
        p = strchr(DOMAIN_NAME,':');
        if(p == NULL)
        {
            strcpy(ip,DOMAIN_NAME);
            strcpy(port,"80");
            //printf("ip %p,port %p\n",ip,port);
            
            #ifdef DEBUG_HTTP
           rt_kprintf("ip=%s,port=%s\n",ip,port);//debug info
            #endif
            return 1;

        }
        else
        {    
            *p = '\0';

            strcpy(ip,DOMAIN_NAME);
            strcpy(port,p+1);
            
            #ifdef DEBUG_HTTP
           rt_kprintf("ip=%s,port=%s\n",ip,port);//debug info
            #endif
            return 2;

        }
        return 0;
    }
    
}
void Package_Url_Get_File(char *path, char *range)
{
    char buf[64];
    memset(g_buf_send,0x0,sizeof(g_buf_send));         
    sprintf(g_buf_send, "GET %s",path);

    
    //HTTP/1.1\r\n ????????
    strcat(g_buf_send," HTTP/1.1\r\n");
    strcat(g_buf_send, "Host: ");
    strcat(g_buf_send, g_host);
    //strcat(g_buf_send, ":");
    //strcat(g_buf_send, PORT);
    
    sprintf(buf, "\r\nRange: bytes=%s",range);
    strcat(g_buf_send,buf);
    strcat(g_buf_send, "\r\nKeep-Alive: 200");
    strcat(g_buf_send,"\r\nConnection: Keep-Alive\r\n\r\n");
    

}

int Package_Url_Get_FileSize(char *url)
{
    
    memset(g_buf_send,0x0,sizeof(g_buf_send));         
    sprintf(g_buf_send, "HEAD %s",url);

        //HTTP/1.1\r\n ????????
    strcat(g_buf_send," HTTP/1.1\r\n");
    strcat(g_buf_send, "Host: ");
    strcat(g_buf_send, g_host);
    //strcat(g_buf_send, ":");
    //strcat(g_buf_send, PORT);
    strcat(g_buf_send,"\r\nConnection: Keep-Alive\r\n\r\n");

    return 0;
}


int HTTP_GetFileSize(int sockfd,char *path)
{
    int ret = -1;
    char buf_recv_tmp[10*1024+1];

    Package_Url_Get_FileSize(path);
#ifdef DEBUG_HTTP
   rt_kprintf("send = %s \n",g_buf_send);
#endif

    Send(sockfd, g_buf_send, strlen(g_buf_send), 0);

    memset(buf_recv_tmp,0x0,sizeof(buf_recv_tmp));                                                 
    ret = Recv(sockfd,buf_recv_tmp,sizeof(buf_recv_tmp)-1,0);
#ifdef DEBUG_HTTP
   rt_kprintf("recv len = %d\n", ret);
   rt_kprintf("recv = %s\n", buf_recv_tmp);
#endif
    if(ret <= 0)
    {
        rt_kprintf("ERR:recv fail GetFileSize()");
        return -1;

    }
    ret = HTTP_GetContentLength(buf_recv_tmp);
    if(ret <= 0)
        return -1;
    else
        return ret;


}




/*
??:??????
??:
???:
>0----------???????(???????)
-1----------??
*/
int HTTP_GetFile(int sockfd,char *path,int filelength,int download_size,char *filebuf)
{
    int count;
    char range[32];
    int i;
    int j = 0;//??????
    int ret = -1;
    char *p = NULL;
    int download_index;//??????

    count = (filelength%MAX_RECV_SIZE)?(filelength/MAX_RECV_SIZE +1):(filelength/MAX_RECV_SIZE);

    download_index = download_size/MAX_RECV_SIZE;

    for(i=download_index;i<count;i++)
    {
        //if(i == 20)//????
            //break;


        if((i == (count-1))&&(filelength%MAX_RECV_SIZE))
            sprintf(range,"%d-%d",i*MAX_RECV_SIZE,filelength-1);
        else
            sprintf(range,"%d-%d",i*MAX_RECV_SIZE,(i+1)*MAX_RECV_SIZE-1);


        Package_Url_Get_File(path,range);
        #ifdef DEBUG_HTTP
        rt_kprintf("send = %s \n",g_buf_send);
        #endif
         Send(sockfd, g_buf_send, strlen(g_buf_send), 0);

        /*?????http ???????????,??????*/
        memset(g_buf_recv,0x0,sizeof(g_buf_recv));                                              
        ret = HTTP_Recv(sockfd,g_buf_recv);
        if(ret < 0)
            break;
        if(ret == 0 )//???????
        {
            sockfd = Socket_Connect(g_ip,g_port);
             i--;
            continue;
        }
        /*???????,???filebuf?*/
        p = strstr(g_buf_recv,"\r\n\r\n");
        if(p == NULL)//jia ru duan dian baocun
        {
           rt_kprintf("ERR:g_buf_recv not contain end flag\n");
            break;
        }
         else
         {
             memcpy(filebuf+j*MAX_RECV_SIZE,p+4,MAX_RECV_SIZE);
             j++;

         }
    }
    if(i == count)
        return (filelength-download_size);
    else
        return (i*MAX_RECV_SIZE-download_size);
}

/*
??:HTTP????
??:
???:
0----------????
-1---------??
-2---------??????
?:?????bin????
*/
int download(char *url,char *save_path)
{
    
    int ret;
    int sockfd;
    int filesize;
    int download_size;
    char filename[FILENAME_LEN+1];
    char filename_bp[FILENAME_LEN+3+1];
    char *filebuf;
    char save_file_path[FILENAME_LEN+1];//?????????+???

    char path[PATH_LEN+1];//url??path

    url = "202.108.22.5/img/bdlogo.gif";
    save_path = "/SD/logo.gif";
    
    //??ip?port?url(url ????,??gethostbyname linux)
    ret = HTTP_Get_IP_PORT(url,g_ip,g_port);
    rt_kprintf("%s %s\r\n", g_ip, g_port);
    if(ret == -1)
        return -1;
    else
    {
        sprintf(g_host,"IP %s PORT %s",g_ip,g_port);
    }
    //???????
    ret = HTTP_GetFileName(url,filename);
    rt_kprintf("Name:%s\r\n", filename);
    if(ret == -1)
        return -1;

    ret = HTTP_GetPath(url,path);
    rt_kprintf("path:%s\r\n", path);
    if(ret == -1)
        return -1;
    //sleep(3);//debug info
    //????
    sockfd = Socket_Connect(g_ip,g_port);
    //?????????
    filesize = HTTP_GetFileSize(sockfd,path);
    rt_kprintf("size:%d\r\n", filesize);
    if(filesize == -1)
        return -1;
    //#ifdef DEBUG_HTTP
   rt_kprintf("http need download size %d\n",filesize);
    while(1);
    //#endif
    //malloc????????
    filebuf = (char *)malloc(filesize);
    if(filebuf == NULL)
    {
        rt_kprintf("malloc filebuf fail");
        return -1;
    }
    else
        memset(filebuf,0x0,filesize);

    download_size = Get_Breakpoint(url,filename,NULL);
    #ifdef DEBUG_HTTP
   rt_kprintf("breakpoint download_size=%d\n",download_size);//debug info
    sleep(3);//debug info
    #endif
    //??????
    ret = HTTP_GetFile(sockfd,path,filesize,download_size,filebuf);
    Close(sockfd);
    if(ret < 0)
    {
        free(filebuf);
        return -1;
    }
    else
    {

        sprintf(save_file_path,"%s%s",save_path,filename);
        
        #ifdef DEBUG_HTTP
       rt_kprintf("save_path=%s\n",save_path);
       rt_kprintf("filename=%s\n",filename);
       rt_kprintf("save_file_path=%s\n",save_file_path);
       rt_kprintf("download_size = %d\n",ret);
        #endif
        Save_File(filebuf,ret,save_file_path);
        free(filebuf);
        if((ret+download_size) == filesize)//??????
        {
            sprintf(filename_bp,"%s.bp",filename);
            remove(filename_bp);
            
    rt_kprintf("download success\n");
            return 0;
        }
        else//??????
        {
           rt_kprintf("part download success\n");
            //??????
            Save_Breakpoint(url,save_file_path,ret+download_size,NULL);
            return -2;
        }
    }

}

void wget(void)
{
	rt_thread_t tid;

	tid = rt_thread_create("websrv", download, RT_NULL,
		1024*30, 25, 5);
	if (tid != RT_NULL) rt_thread_startup(tid); 
}




#ifdef RT_USING_FINSH
#include <finsh.h>
/* ??tcpserv???finsh shell? */
FINSH_FUNCTION_EXPORT(wget, HTTP_Downlo1adFile);
#endif










