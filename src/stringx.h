#ifndef _STRINGX_H__
#define _STRINGX_H__

/* 字符串切片实现，索引左闭右开 */
char *strcut(char *string, int start, int end);

/* 
 * 类似 python 中列表的 split(delim, 1) 方法 
 * 返回 2 层指针，形如 2个元素的数组， *["abc", "def"]
 */
char **strsplit(char *string, char *delim);

/* 
 * 类似 python 中字符串的 strip 方法 
 * 去除 string 两边匹配 flag 的字符串
 * 当 flag 等于 “” 时去除两边所有空格
 */
char *strstrip(char *string, char *flag);

/* 去除左边字符串指定字符 */
char *strlstrip(char *string, char *flag);

/* 去除右边字符串指定字符 */
char *strrstrip(char *string, char *flag);


#endif  /* _STRINGX_H__ */
