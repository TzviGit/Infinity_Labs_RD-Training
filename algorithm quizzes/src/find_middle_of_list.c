/*************************************************************************
*   auther: Tzvi Moskowitz

    date: 24/09/21
*
*************************************************************************/


#include <stddef.h> /*size_t*/



typedef struct node
{
    void *data;
    struct node *next;
}node_t;

typedef struct singly_linked_list
{
    node_t *head;
}linked_list;



node_t * MiddleOfList(const linked_list *list)
{
    node_t *ptr1 = list->head, *ptr2 = list->head;

    while (ptr1->next && ptr1->next->next)
    {
        ptr1 = ptr1->next->next;
        ptr2 = ptr2->next;
    }

    return ptr2;
}
