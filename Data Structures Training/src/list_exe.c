
#include <stddef.h> /*NULL*/
#include <assert.h> /*ASSERT*/
#include <stdio.h>

#include "list_exe.h"

struct node
{
    void *data;
    struct node *next;
};

typedef struct list_exe
{
    node_t *head;
}list_t;

void PrintList(node_t *list)
{
    node_t *curr = list;
    while (curr)
    {
        printf("%d -> ", *(int *)curr->data);
        curr = curr->next;
    }
    printf("%s\n\n", "NULL");
}


node_t *Flip(node_t *head)
{
    node_t *prev = NULL, *curr = head, *next =NULL;
    assert(head);
    while (curr)
    {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }

    return prev;
}
static node_t *NodeJump(node_t *node, size_t jump)
{
    while (jump-- && node)
    {
        node = node->next;
    }
    return node;
}
int HasLoop(const node_t *head)
{

    node_t *jumper = NULL;
    assert(head);

    jumper = head->next;
    while (jumper && jumper != head)
    {
        jumper = NodeJump(jumper, 2);
        head = head->next;
    }

    return !!jumper;

}

static size_t ListSize(node_t *head)
{
    size_t size = 0;
    while (head)
    {
        ++size;
        head = head->next;
    }
    return size;
}
node_t *FindIntersection(node_t *head_1, node_t *head_2)
{
    node_t *long_list = NULL, *short_list = NULL;
    size_t size1 = ListSize(head_1);
    size_t size2 = ListSize(head_2), advance = 0, i = 0;
    assert(head_1 && head_2);

    if (size1 > size2)
    {
        advance = size1 - size2;
        long_list = head_1;
        short_list = head_2;
    }
    else
    {
        advance = size2 - size1;
        long_list = head_2;
        short_list = head_1;
    }

    for (i = 0; i < advance; ++i)
    {
        long_list = long_list->next;
    }

    while (long_list)
    {
        if (long_list == short_list)
        {
            break;
        }
        long_list = long_list->next;
        short_list = short_list->next;
    }

    return long_list;
}
