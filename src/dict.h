#ifndef _DICT_H__
#define _DICT_H__

#include <stddef.h>     /* size_t */

#define MAXELEMENTS 131     /* 字典默认大小 */

typedef struct {
    void *array[MAXELEMENTS];
    size_t  length;
} Dict;

/* 初始化字典，返回指向字典的指针 */
Dict *dict_init();

/* 添加 key, value 键值对, 如果 key 已经存在就更新 key 的值为 value */
Dict *dict_set(Dict *d, char *key, char *value);

/* 获取 key 的 value */
char *dict_get(Dict *d, char *key);

/* 获取所有的 keys */
char **dict_keys(char **rv_keys, Dict *d);

/* 获取所有的 values */
char **dict_values(char **rv_values, Dict *d);

/* 从字典 d2 中取值更新字典 d1 */
void dict_update(Dict *d1, Dict *d2);

/* 打印字典值 */
void dict_dispaly(Dict *d);

/* 删除字典并释放内存 */
void dict_free(Dict *d);

#endif  /* _DICT_H__ */
