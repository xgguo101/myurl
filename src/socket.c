#include <sys/socket.h>     /* socket, connect */
#include <unistd.h>         /* close */
#include <netinet/in.h>     /* socket_addr_in */
#include <arpa/inet.h>      /* inet_addr */
#include <netdb.h>          /* hostent, gethostbyname */

#include <stdlib.h>         /* NULL */
#include <string.h>         /* memcpy */
#include <stdio.h>          /* printf */

char G_DEBUG_ADDR_INFO[1024];


/* 返回已建立好连接的 sockfd */
int 
socket_connect(const char *host, int port)
{
    int sockfd;
    struct sockaddr_in server_addr;
    struct hostent *hp;
    
    // 填充 0
    memset(&server_addr, 0, sizeof(server_addr));
    in_addr_t addr = inet_addr(host);   /* IP 转整形 */

    if (addr != INADDR_NONE) {        
        memcpy(&server_addr.sin_addr, &addr, sizeof(addr));
    } else {    /* 说明输入是域名, 解析 DNS 得到 IP */ 
        hp = gethostbyname(host);
        if (hp == NULL) {
            fprintf(stderr, "Could not resolve host: %s\n", host);
            exit(-1);
        }
        memcpy(&server_addr.sin_addr, hp->h_addr, hp->h_length);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    sprintf(G_DEBUG_ADDR_INFO, "* Connected to %s port %d\n", 
            inet_ntoa(server_addr.sin_addr), port
    );
    
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        fprintf(stderr, "Can't create socket.\n");
        exit(-2);
    }

    if (connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr))) {
        fprintf(stderr, "Connection socket failed.\n");
        exit(-3);
    }
    
    return sockfd;
}


ssize_t 
socket_send(int sockfd, const char *send_buf)
{
    return send(sockfd, (void *) send_buf, strlen(send_buf), 0);
}


ssize_t
socket_recv(int sockfd, const char *recv_buf)
{
    return recv(sockfd, (void *) recv_buf, 4096, 0);
}


/* 关闭套接字 */
int
socket_close(int sockfd)
{
    return close(sockfd);
}
