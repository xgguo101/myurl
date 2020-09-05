#ifndef _SOCKET_H__
#define _SOCKET_H__

#include <sys/types.h>  /* ssize_t */

/* 返回已建立好连接的 sockfd */
int socket_connect(const char *host, int port);

ssize_t socket_send(int sockfd, const char *send_buf);

ssize_t socket_recv(int sockfd, const char *recv_buf);

/* 关闭套接字 */
int socket_close(int sockfd);

#endif  /* _SOCKET_H__ */
