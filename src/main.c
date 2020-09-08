#include "socket.h"         /* G_DEBUG_ADDR_INFO */
#include "request.h"
#include "dict.h"
#include "stringx.h"

#include <getopt.h>         /* getopt_long, optarg, optind */

#include <stdio.h>          /* printf */
#include <stdlib.h>         /* malloc */
#include <string.h>         /* memset, strlen, strcmp */

#define VERSION "myurl/0.1.0\n"

static void usage(void);
static Dict *headers_to_dict(char *str);
static Dict *form_to_dict(char *str);
static void display_request(Response *resp);
static void display_response(Response *resp, char debug);


static void
usage(void)
{
    printf(
"\n"
"Usage: myurl [options] <url>\n"
"\n"
"Options:            HTTP only\n"
"-h, --help          This help text\n"
"-I, --head          Show document info only\n"
"-d, --data DATA     HTTP POST data. example: id=1\n"
"-H, --header LINE   Pass custom header LINE to server\n"
// "-L, --location      Follow redirects\n"
"-X, --request COMMAND  Specify request command to use\n"
// "-o, --output FILE   Write to FILE instead of stdout\n"
"-v, --verbose       Make the operation more talkative\n"
"-V, --version       Show version number and quit\n"
"\n"
    );
}


/**
 * headers 字符串转换为字典 
 * 例；
 * input: "Host: dd.test"
 * output: {"Host": "dd.test"}
 */
static Dict * 
headers_to_dict(char *str)
{
    Dict *d = dict_init();
    
    str = strstrip(str, "");
    char **data = strsplit(str, ":");
    char *key = strstrip(data[0], "");
    char *value = strstrip(data[1], "");
    dict_set(d, key, value);

    return d;
}


/**
 * form 表单字符串转换为字典
 * 例：
 * input: "a=ff"
 * output: {"a": "ff"}
 **/
static Dict *
form_to_dict(char *str)
{
    Dict *d = dict_init();

    str = strstrip(str, "");;
    
    char **data = strsplit(str, "=");
    char *key = strstrip(data[0], "");
    char *value = strstrip(data[1], "");
    dict_set(d, key, value);

    return d;
}


/* 打印 request */
static void
display_request(Response *resp)
{   
    printf(">>>\n");
    printf("%s", resp->req->req_line);
    printf("%s", resp->req->headers);
    printf("\r\n");
    printf("%s", resp->req->body);
    if (strcmp(resp->req->body, "") == 0)
        printf(">>>\n");
    else
        printf("\n>>>\n");
}


/* 打印 response */
static void
display_response(Response *resp, char debug)
{   
    if (debug == 'Y')
        printf("<<<\n");
    
    printf("%s\r\n", resp->resp_line);
    printf("%s\r\n", resp->headers);
    printf("\r\n");

    if (resp->body == NULL)
        printf("%s", "");
    else
        printf("%s", resp->body);
    
    if (debug == 'Y')
        printf("<<<\n");
}


int
main(int argc, char *argv[])
{
    Response *resp;
    char *url;
    int opt;
    int opt_idx = 0;

    char *method = "GET";
    Dict *headers = dict_init();
    Dict *data = dict_init();
    char debug = 'N';

    char *optstr = "hId:H:LX:o:vV::"; 
    struct option long_optstr[] = {
        {"help", no_argument, NULL, 'h'},
        {"head", no_argument, NULL, 'I'},
        {"data", required_argument, NULL, 'd'}, 
        {"header", required_argument, NULL, 'H'},
        {"location", no_argument, NULL, 'L'},
        {"request", required_argument, NULL, 'X'},
        {"outfile", required_argument, NULL, 'o'},
        {"verbose", no_argument, NULL, 'v'},
        {"version", no_argument, NULL, 'V'},
    };

    while ((opt = getopt_long(argc, argv, optstr, long_optstr, &opt_idx))
            != EOF) 
    {
        switch (opt)
        {
        case '?':
            usage();
            return -1;
        case 'h':
            usage();
            return 0;
        case 'I':
            method = "HEAD";
            break;
        case 'd':
            method = "POST";
            dict_update(data, form_to_dict(optarg));
            break;
        case 'H':
            dict_update(headers, headers_to_dict(optarg));
            break;
        case 'L':
            fprintf(stdout, "Unsupported redirects\n");
            return -1;
        case 'X':
            method = optarg;
            break;
        case 'o':
            fprintf(stdout, "Unsupported download\n");
            return -1;
        case 'v':
            debug = 'Y';
            break;
        case 'V':
            fprintf(stdout, VERSION);
            return 0;
        }
    }

    if (argc < 2) {
        fprintf(stdout, "try 'myurl --help'\n");
        return 0;
    } else if ((argv[optind]) == NULL) {
        fprintf(stderr, "no URL specified!\n");
        return -1;
    }

    url = argv[optind];

    if (strcmp(method, "GET") == 0)
        resp = http_get(url, headers);
    else if (strcmp(method, "HEAD") == 0)
        resp = http_head(url, headers);
    else if (strcmp(method, "POST") == 0)
        resp = http_post(url, headers, data);
    else if (strcmp(method, "PUT") == 0)
        resp = http_put(url, headers, data);
    else
        resp = http_request(url, method, headers, data);
    
    if (debug == 'Y') {
        printf("%s", G_DEBUG_ADDR_INFO);
        display_request(resp);
    }
    
    display_response(resp, debug);
    http_free_response(resp);

    return 0;
}
