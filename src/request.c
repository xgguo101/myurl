#include "request.h"
#include "socket.h"
#include "url.h"
#include "dict.h"
#include "stringx.h"

#include <stdlib.h>     /* malloc */
#include <string.h>     /* strlen */
#include <stdio.h>      /* sprintf */

#define URL_SIZE        2048        /* URL 最长值 2048 */
#define HEADERS_SIZE    10240       /* 请求头最大 10k */
#define PAYLOAD_SIZE    10240       /* payload 最大 10k */

static char *headers_encode(Dict *d, size_t size);
static char *form_encode(Dict *d, size_t size);
static char *recv_all_data_from_socket(int sockfd);
static Response *parse_http(Response *rv_resp, char *response);


/* 字典 d 转为 headers 字符串, size 表示字符串最大存储大小 */
static char *
headers_encode(Dict *d, size_t size)
{
    if (d == NULL)
        return NULL;

    int len = d->length;
    char **keys = (char **) calloc(len, sizeof(char *));
    char **values = (char **) calloc(len, sizeof(char *));
    char *data = (char *) calloc(1, size);
    
    keys = dict_keys(keys, d);
    values = dict_values(values, d);
    for (int i = 0; i < len; i++ ) {
        char buf[1024];
        sprintf(buf, "%s: %s\r\n", keys[i], values[i]);
        strcat(data, buf);
    }
    free(keys);
    free(values);

    return data;
}


/* 字典 d 转为 form 表单 payload 字符串, size 表示字符串最大存储大小 */
static char *
form_encode(Dict *d, size_t size)
{
    if (d == NULL)
        return NULL;
    
    int len = d->length;
    char **keys = (char **) calloc(len, sizeof(char *));
    char **values = (char **) calloc(len, sizeof(char *));
    char *data = (char *) calloc(1, size);

    keys = dict_keys(keys, d);
    values = dict_values(values, d);
    for (int i = 0; i < len; i++) {
        char buf[1024];
        sprintf(buf, "%s=%s&", keys[i], values[i]);
        strcat(data, buf);
    }
    data = strrstrip(data, "&");
    free(keys);
    free(values);

    return data;
}


/**
 * 从套接字接收所有数据
 * HTTP 请求头 connection 字段值必须为 close
 * 否则会阻塞，直至 timeout 才会有数据
 */
static char *
recv_all_data_from_socket(int sockfd)
{
    char *response = (char *) malloc(0);
    char recv_buf[4096];

    size_t recv_len = 0;
    while ((recv_len =  socket_recv(sockfd, recv_buf)) > 0) {
        recv_buf[recv_len] = '\0';
        size_t size = strlen(response) + strlen(recv_buf);
        response = (char *) realloc(response, size);
        sprintf(response, "%s%s", response, recv_buf);
    }
    if (recv_len < 0) {
        response = NULL;
    }
    return response;
}


/**
 * 解析 HTTP Response 
 * 返回指向 Response 结构体的指针
 */
static Response *
parse_http(Response *rv_resp, char *response)
{
    if (response == NULL)
        return NULL;
    
    char **data = strsplit(response, "\r\n");
    char *response_line = data[0];
    char *res = data[1];
    
    rv_resp->req = NULL;
    rv_resp->resp_line = response_line;

    char **rv = strsplit(res, "\r\n\r\n");
    rv_resp->headers = rv[0];
    rv_resp->body = rv[1];

    rv_resp->version = strsplit(response_line, " ")[0];
    char *tmp = strsplit(response_line, " ")[1];
    rv_resp->status_code = strsplit(tmp, " ")[0];
    rv_resp->status_msg = strrstrip(strsplit(tmp, " ")[1], "\r\n");

    return rv_resp;
}


/* 发送 HTTP 请求接口 */
Response *
http_request(
    char *url, 
    char *method, 
    Dict *headers, 
    Dict *form
)
{
    Response *resp = (Response *) malloc(sizeof(Response));
    Request *req = (Request *) malloc(sizeof(Request));
    URL *rv_url = (URL *) malloc(sizeof(URL));

    rv_url = urlsplit(rv_url, url);
    req->method = (method == NULL) ? "GET" : method;
    req->version = "HTTP/1.1";
    req->query = rv_url->query;
    
    if (rv_url->path == NULL) {
         req->path = "/";
    } else {
        size_t len = strlen(rv_url->path);
        req->path = (char *) malloc(len+1);
        sprintf(req->path, "/%s", rv_url->path);
        free(rv_url->path);
    }

    /* request line */
    char *request_line = malloc(URL_SIZE);
    if (req->query != NULL) {
        sprintf(request_line, "%s %s?%s %s\r\n", 
                req->method, 
                req->path, 
                req->query,
                req->version
        );
    } else {
        sprintf(request_line, "%s %s %s\r\n", 
                req->method, 
                req->path, 
                req->version
        );
    }
    req->req_line = request_line;
    
    /* headers */
    Dict *header_d = dict_init();
    header_d = dict_set(header_d, "Host", rv_url->netloc);
    header_d = dict_set(header_d, "Connection", "close");
    header_d = dict_set(header_d, "User-Agent", "myurl/0.1.0");
    header_d = dict_set(header_d, "Content-Type", "text/html");

    if (headers != NULL && headers->length > 0) {
        /* 更新 header 值 */
        dict_update(header_d, headers);
    }

    char *headers_str = headers_encode(header_d, HEADERS_SIZE);
    req->headers = headers_str;
    
    /* body */
    if (form != NULL && form->length > 0) {
        char *payload = form_encode(form, PAYLOAD_SIZE);
        req->body = payload;
    } else {
        req->body = (char *) malloc(0);
    }
    char *body = req->body;
    
    /* request data */
    size_t size = strlen(request_line) + strlen(headers_str) + strlen(body);
    char send_data[size + 3];
    sprintf(send_data, "%s%s\r\n%s", request_line, headers_str, body);
    // fprintf(stdout, ">>> SEND: \n%s\n", send_data);

    int sockfd = socket_connect(rv_url->netloc, rv_url->port);
    socket_send(sockfd, send_data);
    char *response = recv_all_data_from_socket(sockfd);
    // fprintf(stdout, ">>> RESPONSE\n%s\n", response);
    resp = parse_http(resp, response);
    resp->req = req;

    socket_close(sockfd);

    return resp;
}


/* HEAD 请求 */
Response *
http_head(char *url, Dict *headers)
{
    Response *resp = http_request(url, "HEAD", headers, NULL);
    return resp;
}


/* GET 请求 */
Response *
http_get(char *url, Dict *headers)
{
    Response *resp = http_request(url, "GET", headers, NULL);
    return resp;
}


/* POST 请求 */
Response *
http_post(char *url, Dict *headers, Dict *form)
{
    dict_set(headers, "Content-Type", "application/x-www-form-urlencoded");
    Response *resp = http_request(url, "POST", headers, form);
    return resp;
}


/* PUT 请求 */
Response *
http_put(char *url, Dict *headers, Dict *form)
{
    dict_set(headers, "Content-Type", "application/x-www-form-urlencoded");
    Response *resp = http_request(url, "PUT", headers, form);
    return resp;
}


/* 释放堆上分配的内存 */
void
http_free_response(Response *resp)
{
    free(resp->req->req_line);
    free(resp->req->headers);
    free(resp->req->body);
    free(resp->resp_line);
    free(resp->headers);
    free(resp->body);
    free(resp->req);
    free(resp);
}
