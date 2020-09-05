#ifndef _URL_H__
#define _URL_H__

/* 枚举方案类型 */
typedef enum {
    HTTP,
    HTTPS,
    FTP,
    UNKNOWN = 99,
} scheme_t;

/* 定义 URL 组成 */
typedef struct {
    scheme_t scheme;
    char    *netloc;
    int     port;
    char    *path;
    char    *query;
    char    *fragment;
} URL;

/* 返回指向 URL 结构体的指针 */
URL *urlsplit(URL *rv_url, char *input_url);

/* 返回编码后的字符串 */
char *urlencode(char *str);

/* 释放内存 */
void urlfree(URL *url);

#endif  /* _URL_H__ */
