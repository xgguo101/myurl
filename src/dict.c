/** 
 * 字典数据结构实现
 * TODO: 完成当字典大小超过当前大小的 70% 自动扩展字典大小。
 *       （ 现在字典初始化大小为 131，不会自动扩展，同一个索引挂载值太多的话，
 *         操作字典的平均时间复杂度会退化成使用链表的平均时间复杂度。）
 * 
 * 使用链表存储值，解决 hash 冲突，哈希值一样则挂载在同一个索引中
 * 
 * 
 *                     + - - - - - - - - - - - - - - - - - - - - +
 *                     .  index0)                                .
 *                     . +--------+                              .
 *                     . | key1   |         +--------+           .
 *                     . | value1 |         | key0   |           .
 *                     . | next   |-------> | value0 |           .
 *                     . +--------+         | next   |----> ...  .
 *                     .                    +--------+           .
 *    dict             . (index1)                                .
 *  +---------+        . +--------+                              .          
 *  | array   | -----> . | key2   |                              .             
 *  |         |        . | value2 |                              .
 *  | length  |        . | next   |                              .
 *  +---------+        . +--------+                              .
 *                     .      .                                  .
 *                     .      .                                  .
 *                     . (index2)                                .
 *                     . +--------+                              .
 *                     . | key_n  |                              .
 *                     . | value_n|                              .
 *                     . | next   |                              .
 *                     . +--------+                              .
 *                     + - - - - - - - - - - - - - - - - - - - - +
 * 
 * 
 */

#include "linked_list.h"
#include "dict.h"

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>     /* strlen */

static int hash(const char *str);


/* 获取字符串的哈希值 */
static int
hash(const char *str)
{
    unsigned int hash_code = 0;
    unsigned int seed = 131;
    
	while (*str) {
        hash_code = hash_code * seed + (*str++);
    }
    return hash_code % MAXELEMENTS;
}


/* 初始化字典 */
Dict *
dict_init()
{
    Dict *d = (Dict *) malloc(sizeof(Dict));
    assert(d != NULL);

    d->length = 0;
    for (int i = 0; i < MAXELEMENTS; i++) {
        d->array[i] = NULL;
    }
    return d;
}


/**
 *  添加 key, value 键值对
 *  如果 key 已经存在就更新 key 的值为 value
 */
Dict *
dict_set(Dict *d, char *key, char *value)
{
    int index = hash(key);

    if (d->array[index] != NULL) {      /* 表示此槽位已经被占 */
        Linked_list *lst = d->array[index];
        char *rv_value = linked_list_find(lst, key);
        if (rv_value != NULL)       /* 表示此 key 已经存在, 更新该值 */
            d->array[index] = linked_list_set(lst, key, value);
        else {
            d->array[index] = linked_list_appendleft(lst, key, value);
            d->length += 1;
        }
    } else {
        Linked_list *lst = linked_list_init();
        d->array[index] = linked_list_appendleft(lst, key, value);
        d->length += 1;
    }
    return d;
}


/* 获取 key 的 value */
char *
dict_get(Dict *d, char *key)
{
    int index = hash(key);
    Linked_list *lst = d->array[index];
    return linked_list_find(lst, key);
}


/* 获取所有的 keys */
char **
dict_keys(char **rv_keys, Dict *d)
{
    char **p = rv_keys;

    if (d->length == 0)
        return NULL;
    
    int index = 0;
    for (int i = 0; i < MAXELEMENTS; i++) {
        if (d->array[i] != NULL) {  /* 槽位为空的话，会返回 NULL */
            Linked_list *lst = d->array[i];

            /* 不同的 key 哈希值一样，挂载在同一个槽位 */
            for (; lst->next != NULL; lst = lst->next) {
                p[index++] = lst->key;
            }
        }
    }
    return rv_keys;
}


/* 获取所有的 values */
char **
dict_values(char **rv_values, Dict *d)
{
    char **p = rv_values;

    if (d->length == 0)
        return NULL;
    
    int index = 0;
    for (int i = 0; i < MAXELEMENTS; i++) {
        if (d->array[i] != NULL) {  /* 槽位为空的话，会返回 NULL */
            Linked_list *lst = d->array[i];

            /* 不同的 key 哈希值一样，挂载在同一个槽位 */
            for (; lst->next != NULL; lst = lst->next) {
                p[index++] = lst->value;
            }
        }
    }
    return rv_values;
}


/* 从字典 d2 中取值更新字典 d1 */
void
dict_update(Dict *d1, Dict *d2)
{
    int len = d2->length;
    char **keys = (char **) calloc(len, sizeof(char *));
    keys = dict_keys(keys, d2);
    
    char **p = keys;
    for (int i = 0; i < len; i++) {
        d1 = dict_set(d1, keys[i], dict_get(d2, keys[i]));
    }
    free(keys);
}


/* 打印字典值 */
void 
dict_dispaly(Dict *d)
{
    if (d->length == 0 || d == NULL)
        printf("{}\n");
    else {
        printf("{");
        int count = MAXELEMENTS;
        for (int i = 0; count-- > 0; i++) {
            Linked_list *lst = d->array[i];
            if (lst != NULL)
                linked_list_display(lst);
        }
        printf("\n}\n");
    }
}


/* 删除字典并释放内存 */
void 
dict_free(Dict *d)
{
    if (d != NULL) {
        /* 先释放链表的空间 */
        // 链表释放需要构造一个二重指针
        Linked_list *lst = linked_list_init();
        for (int i = 0; i < MAXELEMENTS; i++) {
            if (d->array[i] != NULL)
                lst = d->array[i];
                linked_list_free(&lst);
        }

        free(lst);
        /* 释放字典空间 */
        free(d);
    }
}
