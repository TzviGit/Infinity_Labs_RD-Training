#include <stdlib.h> /*malloc free*/
#include <assert.h> /* assert*/

#include "linked_list.h"

/*******APROVED BY FANIA**********/

#define NODE_P_TO_ITER(node_pointer) (node_pointer)
#define ITER_TO_NODE_P(iter) (iter)
#define FIND_END(iter) while(ITER_TO_NODE_P(iter)->next){ iter = SLNext(iter);}

typedef struct sl_node

{
    void *data;
    struct sl_node *next;

} sl_node_t;


struct slist
{
    sl_node_t *head;
    sl_node_t *end;
};

sl_node_t *CreatNode()
{
    return (sl_node_t *)malloc(sizeof(sl_node_t));
}
slist_t *SLCreate()
{
    slist_t *list = (slist_t *)malloc(sizeof(slist_t));
    sl_node_t *end = CreatNode();
    if (!(list && end))
    {
        free(end); end = NULL;
        free(list); list = NULL;
        return NULL;
    }

    end->data = (void *)&(list->end);
    end->next = NULL;

    list->end = end;
    list->head = end;

    return list;

}

void SLDestroy(slist_t *list)
{
    /*WE NEED TO MIMIC THE BEHAVIOUR OF THE FREE FUNC*/
    if (list)
    {
        sl_iter_t iter = NULL;
        while(!SLIsEmpty(list))
        {
            iter = SLBegin(list);
            SLRemove(iter);
        }

        free(list->end); list->end = NULL;
        free(list); list = NULL;
    }
}

sl_iter_t SLBegin(const slist_t *list)
{
    assert(list);
    return NODE_P_TO_ITER(list->head);
}
sl_iter_t SLEnd(const slist_t *list)
{
    assert(list);
    return NODE_P_TO_ITER(list->end);
}

sl_iter_t SLNext(const sl_iter_t iter)
{
    sl_node_t *np = ITER_TO_NODE_P(iter);
    assert(iter);
    return NODE_P_TO_ITER(np->next);
}

int SLIterIsEqual(const sl_iter_t i1, const sl_iter_t i2)
{
    sl_node_t *p1 = ITER_TO_NODE_P(i1), *p2 = ITER_TO_NODE_P(i2);
    assert(p1 && p2);
    return p1 == p2;
}

int SLIsEmpty(const slist_t *list)
{
    assert(list);
    return SLBegin(list) == SLEnd(list);
}

size_t SLCount(const slist_t *list)
{
    sl_iter_t iter = NULL;
    size_t count = 0;
    assert(list);

    iter = SLBegin(list);
    while (iter != SLEnd(list))
    {
        iter = SLNext(iter);
        ++count;
    }

    return count;
}


sl_iter_t SLAddAfter(sl_iter_t iter, void *data)
{
    return SLAddBefore(SLNext(iter), data);
}


sl_iter_t SLAddBefore(sl_iter_t iter, void *data)
{
    sl_node_t *curr = NULL;
    sl_node_t *new_node = CreatNode();

    assert(iter);
    if (!new_node)
    {
        FIND_END(iter); /*MACRO*/
        return iter;
    }

    curr = ITER_TO_NODE_P(iter);

    new_node->data = curr->data;
    new_node->next = curr->next;
    curr->next = new_node;
    curr->data = data;

    if (!new_node->next)
    {
        *(sl_iter_t *)new_node->data = NODE_P_TO_ITER(curr->next); /*the data of the END hold a pointer to the variable "end" in the list*/
    }

    return NODE_P_TO_ITER(curr);
}

sl_iter_t SLRemoveAfter(sl_iter_t iter)
{
    assert(iter && SLNext(iter));
    return SLRemove(SLNext(iter));
}

sl_iter_t SLRemove(sl_iter_t iter)
{
    sl_node_t *curr = NULL, *to_free = NULL;

    assert(iter);
    assert( SLNext(iter));
    
    curr = ITER_TO_NODE_P(iter);
    to_free = ITER_TO_NODE_P(SLNext(iter));

    curr->data = to_free->data;
    curr->next = to_free->next;

    free(to_free); to_free = NULL;

    if (!curr->next)
    {
        /*the data of the END hold a pointer to the variable "end" in the list*/
        *(sl_iter_t *)curr->data = NODE_P_TO_ITER(curr);
    }
    return NODE_P_TO_ITER(curr);

}

void *SLGetValue(const sl_iter_t iter)
{
    assert(iter);
    return ITER_TO_NODE_P(iter)->data;
}

void SLSetValue(sl_iter_t iter, void *data)
{
    assert(iter);
    ITER_TO_NODE_P(iter)->data = data;
}


sl_iter_t SLFind(const slist_t *list, const void * data, int (*CompareTo)(const void *, const void *))
{
    sl_iter_t iter = SLBegin(list);
    assert(list && CompareTo);

    while (iter != SLEnd(list) && !CompareTo(data, SLGetValue(iter)))
    {
        iter = SLNext(iter);
    }

    return iter;
}

int SLForEach(sl_iter_t start, sl_iter_t end, int (*Action)(void * data, void *param), void *param) /* returns 0 on success and another number otherwise.  stop if not 0*/
{
    int flag = 0;
    sl_iter_t iter = start;
    assert(start && end);
    while (!flag && !(SLIterIsEqual(iter, end)))
    {
        if(Action(SLGetValue(iter), param))
        {
            flag = 1;
        }

        iter = SLNext(iter);
    }

    return flag;
}

void SLAppend(slist_t *base,  slist_t *append)
{
    sl_node_t *utiltity = NULL;
    assert(base && append);

    if (!SLIsEmpty(append))
    {
         utiltity = ITER_TO_NODE_P(SLBegin(append));

        ITER_TO_NODE_P(SLEnd(base))->next = utiltity->next;
        ITER_TO_NODE_P(SLEnd(base))->data = utiltity->data;


        base->end = SLEnd(append);
        ITER_TO_NODE_P(SLEnd(append))->data = (void *)&base->end;
        (base->end) = append->end;


        utiltity->data = (void *)&append->end;
        utiltity->next = NULL;
        append->end = utiltity;
    }
}
