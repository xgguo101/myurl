#ifndef _STRINGX_H__
#define _STRINGX_H__

/* 字符串切片实现 */
char *strcut(char *string, int start, int end);

/* 类似 python 中的 split */
char **strsplit(char *string, char *delim);

/* 类似 python 中 strip */
char *strstrip(char *string, char *flag);

/* 去除左边字符串指定字符 */
char *strlstrip(char *string, char *flag);

/* 去除右边字符串指定字符 */
char *strrstrip(char *string, char *flag);


#endif  /* _STRINGX_H__ */
