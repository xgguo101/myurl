/**
 * 扩展字符串常用函数
 */

#include <stdlib.h>         /* malloc */
#include <string.h>         /* strlen, strcmp, strstr */
#include <ctype.h>          /* isspace */
#include <stdio.h>


/* 字符串切片实现, 左闭右开*/
char *
strcut(char *string, int start, int end) 
{
    char *str = string;

    if (str == NULL)
        return NULL;

    int len = strlen(str);
    if ((end - start > 0) && (start >=0) && (end <= len)) {
        int size = end - start;
        char *new_str = (char *) malloc(size * sizeof(char));
        char *p = new_str;
        int pos = 0;

        /* 指针移到起始点 */
        for (; pos < start; pos++) {
            str++;
        }

        /* 指针移到结束点 */
        for (; pos < end; pos++) {
            *p++ = *str++;
        }
        *p = '\0';
        return new_str;
    } else {
        // fprintf(stderr, "Index error: function `strcut`\n");
        return NULL;
    }
}


/* 类似 python 中的 split */
char **
strsplit(char *string, char *delim)
{
    char **data = (char **) malloc(sizeof(char *));
    char *sub_str = strstr(string, delim);

    if (sub_str == NULL || strcmp(delim, "") == 0 ) {
        data[0] = string;
        data[1] = NULL;
    } else {
        int len = strlen(string);
        int pos = sub_str - string; /* 索引位置 */
        if (pos == 0) {             /* 表示索引位置在首部 */
            data[0] = strcut(string, pos + strlen(delim), len);
            data[1] = NULL;
        } else {
            data[0] = strcut(string, 0, pos);
            data[1] = strcut(string, pos + strlen(delim), len);
        }
    }
    return data;
}


/* 去除左边字符串指定字符 */
char *
strlstrip(char *string, char *flag)
{  
    char *sub = strstr(string, flag);
    int pos = string - sub;
    
    if (sub == NULL)    /* 未找到指定字符串 */
        return string;
    
    int len = strlen(flag);
    if (pos == 0) {    /* 去除左边的指定字符串 */
        for (; len > 0; len--) {
            string++;
        }
    }

    return string;
}


/* 去除右边字符串指定字符 */
char *
strrstrip(char *string, char *flag)
{
    int flag_len = strlen(flag);
    int s_len = strlen(string);
    int index = strlen(string) - flag_len;

    char *dst = strcut(string, index, s_len);
    if (strcmp(dst, flag) == 0) {
        string[index] = '\0';
    }
    free(dst);
    return string;
}


/* 类似 python 中 strip */
char *
strstrip(char *string, char *flag)
{
    size_t s_len = strlen(string);
    size_t flag_len = strlen(flag);

    if (flag_len > s_len) {
        fprintf(stderr, "Invalid flag: function `strstrip`\n");
        return string;
    }

    if (strcmp(flag, "") == 0) {    /* 去除两边空格 */
        /* 去除左边空格 */
        while (isspace(*string)) {
            string++;
        }

        /* 去除右边空格 */
        int cur_len = strlen(string);
        while (isspace(string[cur_len-1]) && cur_len >= 0) {
            cur_len--;
        }
        string[cur_len] = '\0';
        return string;
    } else {    /* 去除指定字符串 */
        string = strlstrip(string, flag);
        string = strrstrip(string, flag);
    }

    return string;
}
