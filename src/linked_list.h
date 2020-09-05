#ifndef _LINKED_LIST_H__
#define _LINKED_LIST_H__

#include <stddef.h>     /* size_t */

#define LL Linked_list *

typedef struct Linked_list {
    char *key;
    char *value;
    struct Linked_list *next;
} Linked_list;


/* 初始化链表 */
LL linked_list_init();

/* 尾部追加数据 */
LL linked_list_append(LL linked_list, char *key, char *value);

/* 左边追加数据 */
LL linked_list_appendleft(LL linked_list, char *key, char *value);

/* 删除节点 */
LL linked_list_remove(LL linked_list,  int index);

/* 删除首节点并返回其值 */
char *linked_list_pop(LL *linked_list);

/* 查找 key 节点的 value 值, 失败则返回 NULL */
char *linked_list_find(LL linked_list, char *key);

/* 查找 key 节点的 index, 失败则返回 -1 */
int linked_list_get_index(LL linked_list, char *key); 

/* 修改 key 节点的值为 value */
LL linked_list_set(LL linked_list, char *key, char *value);

/* 在 index 处插入 value 节点  */
LL linked_list_insert(LL linked_list, char *value, int index);

/* 返回链表长度 */
size_t linked_list_len(LL linked_list);

/* 删除链表并释放内存*/
void linked_list_free(LL *linked_list);

/* 打印链表数据 */
void linked_list_display(LL linked_list);

#undef LL
#endif  /* _LINKED_LIST_H__ */
