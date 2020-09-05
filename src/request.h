#ifndef _REQUEST_H__
#define _REQUEST_H__

#include "dict.h"

typedef struct Request {
    char *method;
    char *path;
    char *query;
    char *version;
    char *headers;
    char *body;
    char *req_line;
} Request;

typedef struct Rseponse {
    Request *req;
    char *version;
    char *status_code;
    char *status_msg;
    char *headers;
    char *body;
    char *resp_line;
} Response;

/* 发送 HTTP 请求接口 */
Response *http_request(
    char *url, 
    char *method, 
    Dict *headers, 
    Dict *form
);

/* HEAD 请求 */
Response *http_head(char *url, Dict *headers);

/* GET 请求 */
Response *http_get(char *url, Dict *headers);

/* POST 请求 */
Response *http_post(char *url, Dict *headers, Dict *form);

/* PUT 请求 */
Response *http_put(char *url, Dict *headers, Dict *form);

/* 释放内存 */
void http_free_response(Response *resp);

#endif  /* _REQUEST_H__ */
