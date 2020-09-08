/**
 * 单向链表实现。
 * 
 * 函数平均时间复杂度如下：
 * 
 *      linked_list_append()        O(n)
 *      linked_list_appendleft()    O(1)
 *      linked_list_find()          O(n)
 *      linked_list_remove()        O(n)
 *      linked_list_pop()           O(1)
 *      linked_list_insert()        O(n)
 * 
 * 
 *  linked_list:
 *      root                                          end
 *    +-------+           +-------+                +-------+      
 *    | value |     ----> | value |      ----> ... | value |                
 *    | key   |     |     | key   |      |         | key   |          
 *    | next  | ----|     | next  | -----|         | next  |----> NULL
 *    +-------+           +-------+                +-------+
 * 
 */

#include "linked_list.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#define LL Linked_list *

static void check_input(LL linked_list, int index, char *func_name);


/* 初始化链表 */
LL
linked_list_init()
{
    LL root = (LL) malloc(sizeof(Linked_list));
    assert(root != NULL);
    root->next = NULL;
    return root;
}


/* 尾部追加数据 */
LL
linked_list_append(LL linked_list, char *key, char *value)
{
    LL p = linked_list;
    for (; p->next != NULL; p = p->next);
    p->key = key;
    p->value = value;
    LL new_node = linked_list_init();
    p->next = new_node;
    return linked_list;
}


/* 左边追加数据 */
LL
linked_list_appendleft(LL linked_list, char *key, char *value)
{
    LL new_node = linked_list_init();
    new_node->key = key;
    new_node->value = value;
    new_node->next = linked_list;
    return new_node;
}


/* 删除节点 */
LL
linked_list_remove(LL linked_list,  int index)
{
    check_input(linked_list, index, "Remove");
    LL p = linked_list;
    
    if (p->next == NULL)    /* 链表为空 */
        return linked_list;
    else if (index == 0)    /* 删除首节点 */
        return  linked_list->next;

    int i;
    for (i = 0; i < (index-1); i++) {  /* 指针移到待删除的节点的前一个节点 */
        if (p->next != NULL) {
            p = p->next;
        }
    }
    LL tmp = p->next->next;
    free(p->next);
    p->next = tmp;
    return linked_list;
}


/* 删除首节点并返回 value 值 */
char *
linked_list_pop(LL *linked_list)
{
    LL p = *linked_list;
    if (p->next !=  NULL) {
        char *key = p->key;
        char *value = p->value;
        LL tmp = p->next;
        free(*linked_list);
        *linked_list = tmp;
        return value;
    } 
    else 
        return NULL;
}


/* 查找 key 节点的 value, 失败则返回 NULL */
char *
linked_list_find(LL linked_list, char *key)
{   
    LL p = linked_list;
    int index = 0;
    for (; p->next != NULL; p = p->next) {
        if (strcmp(p->key, key) == 0) {
            return p->value;
        }
        index++;
    }
    return NULL;
}


/* 查找 key 节点的 index, 失败则返回 -1 */
int
linked_list_get_index(LL linked_list, char *key)
{   
    LL p = linked_list;
    int index = 0;
    for (; p->next != NULL; p = p->next) {
        if (strcmp(p->key, key) == 0) {
            return index;
        }
        index++;
    }
    return -1;
}


/* 修改 key 节点的值为 value */
LL
linked_list_set(LL linked_list, char *key, char *value)
{
    int index = linked_list_get_index(linked_list, key);
    if (index == -1)    /* 表示未找到此 key */
        return linked_list;
    else {
        LL p = linked_list;
        for (int i = 0; i < index; i++) {  /* 指针移到该节点 */
                p = p->next;
        }
        p->value = value;
        return linked_list;
    }
}


/* 内部函数，输入检查 */
static void
check_input(LL linked_list, int index, char *func_name)
{
    char abort_signal = 0;
    size_t len = linked_list_len(linked_list);

    char r_flag = ')';
    if (func_name[0] == 'I') { /* Insert 函数索引左边是闭区间，右边也是闭区间 */
        len += 1;
        r_flag = ']';
    }
    
    if (len > 0 && index > (len -1)) /* len 等于 0 时， len-1 会溢出 */
        abort_signal = 1;
    else if (len == 0 && index > 0) 
        abort_signal = 1;
    
    if (abort_signal) {
        if (func_name[0] == 'I')
            len -= 1;
        fprintf(stderr,
            "'%s' Func IndexError: linked_list index(%d) out of range. -> \
            [0, %lu%c\n", func_name, index, len, r_flag \
        );
        abort();
    }
}


/* 返回链表长度 */
size_t
linked_list_len(LL linked_list)
{
    LL p = linked_list;
    size_t len = 0;
    for (; p->next != NULL; p = p->next)
        len++;
    return len;
}


/* 删除链表释并放内存 */
void
linked_list_free(LL *linked_list)
{
    LL p = *linked_list;
    
    for (; p->next != NULL; p = p->next) {
        linked_list_pop(linked_list);
    }
}


/* 打印链表数据 */
void
linked_list_display(LL linked_list)
{   
    LL p = linked_list;
    printf("\n    [ ");
    for (; p->next != NULL; p = p->next)
        printf("{%s, %s}, ",  p->key, p->value);
    printf("],");
}


#undef LL
