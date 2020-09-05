#include "url.h"
#include "stringx.h"        /* strsplit, strcut */

#include <stdio.h>          /* printf */
#include <stdlib.h>         /* malloc */
#include <string.h>         /* strlen, strcmp */
#include <ctype.h>          /* isalnum */

static char to_hex(char code);


/**
 * 返回指向 URL 结构体的指针
 */
URL *
urlsplit(URL *rv_url, char *input_url)
{
    URL *url = rv_url;
    char *sub_url = input_url;

    url->scheme = HTTP;
    
    // <scheme>://<sub_url>
    if (strstr(input_url, "://")!= NULL) {
        char *scheme = strsplit(input_url, "://")[0];
        sub_url = strsplit(input_url, "://")[1];
        scheme_t flag;

        if (strcmp(scheme, "http") == 0)
            flag = HTTP;
        else if (strcmp(scheme, "https") == 0)
            flag = HTTPS;
        else if (strcmp(scheme, "ftp") == 0)
            flag = FTP;
        else
            flag = UNKNOWN;
        url->scheme = flag;
    }

    // *host -> <host>:<port>
    char *host = strsplit(sub_url, "/")[0];
    if (strchr(host, ':') != NULL) {
        url->netloc = strsplit(host, ":")[0];
        url->port = atoi(strsplit(host, ":")[1]);
    } else {
        url->netloc = strsplit(host, ":")[0];
        if (url->scheme == HTTP)
            url->port = 80;
        else if (url->scheme == HTTPS)
            url->port = 443;
    }

    // *part_url -> <path>?<query>#<fragment>
    char *part_url = strsplit(sub_url, "/")[1];
    if (part_url != NULL) {
        char *path = strsplit(part_url, "?")[0];
        char *query = strsplit(part_url, "?")[1];
        char *fragment = NULL;

        if (query != NULL) {
            fragment = strsplit(query, "#")[1];
            query = strsplit(query, "#")[0];
        }
        url->path = path;
        url->query = query;
        url->fragment = fragment;
    } else {
        url->path = NULL;
        url->query = NULL;
        url->fragment = NULL;
    }
        
    return url;
}


/* to hex */
static char 
to_hex(char code) 
{
	static char hex[] = "0123456789abcdef";
	return hex[code & 15];
}


/* 返回编码后的 url 字符串 */
char *urlencode(char *str) 
{
	char *pstr = str;
    char *buf = (char*)malloc(strlen(str) * 3 + 1);
    char *pbuf = buf;
    
  	while (*pstr) 
	{
    	if (isalnum(*pstr) 
            || *pstr == '-' 
            || *pstr == '_' 
            || *pstr == '.' 
            || *pstr == '~'
        ) 
            *pbuf++ = *pstr;
    	else if (*pstr == ' ') 
      		*pbuf++ = '+';
    	else 
      		*pbuf++ = '%', *pbuf++ = to_hex(*pstr >> 4),
                           *pbuf++ = to_hex(*pstr & 15);
    	pstr++;
  	}
  	*pbuf = '\0';
  	return buf;
}


/* 释放在堆上分配的内存 */
void
urlfree(URL *url)
{
    free((void *) url->scheme);
    free(url->netloc);
    free(url->path);
    free(url->query);
    free(url->fragment);
}
