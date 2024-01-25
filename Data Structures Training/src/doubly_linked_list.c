#include <stdlib.h> /*malloc ,free*/
#include <assert.h> /*assert*/
#include "doubly_linked_list.h"


#define GET_NODE_P(iter) (iter).internal

typedef struct dl_node dl_node_t;

struct dl_node
{
    dl_node_t *prev;
    dl_node_t *next;
    void *data;
};

struct dlist
{
    dl_node_t head;
    dl_node_t tail;
};


dlist_t *DLCreate(void)
{
    dlist_t *dlist = (dlist_t *)malloc(sizeof(*dlist));
    if (!dlist)
    {
        return NULL;
    }

    dlist->head.prev = NULL;
    dlist->head.data = NULL;
    dlist->head.next = &dlist->tail;
    dlist->tail.prev = &dlist->head;
    dlist->tail.next = NULL;
    dlist->tail.data = NULL;
    return dlist;

}

void DLDestroy(dlist_t *dlist)
{
    if (dlist)
    {
        dl_iter_t iter = DLBegin(dlist);

        while (!DLIsIterEqual(iter, DLEnd(dlist)))
        {
            iter = DLRemove(iter);
        }
        free(dlist);
    }
}

int DLIsEmpty(const dlist_t *dlist)
{
    return dlist->head.next == &dlist->tail;
}

dl_iter_t DLBegin(const dlist_t *dlist)
{
    dl_iter_t iter = {0};
    assert(dlist);

    iter.internal = dlist->head.next;
    return iter;
}

dl_iter_t DLEnd(const dlist_t *dlist)
{
    dl_iter_t iter = {0};
    assert(dlist);

    iter.internal = (dl_node_t *)&dlist->tail;
    return iter;
}

dl_iter_t DLNext(dl_iter_t dl_iter)
{
    dl_iter_t iter = {0};
    assert(dl_iter.internal);
    iter.internal = GET_NODE_P(dl_iter)->next;
    return iter;
}

dl_iter_t DLPrev(dl_iter_t dl_iter)
{
    dl_iter_t iter = {0};
    assert(dl_iter.internal);
    iter.internal = GET_NODE_P(dl_iter)->prev;
    return iter;
}

int IncrementCount(void *data, void *param)
{
    (void)data;
    *(size_t *)param += 1;
    return 0;
}
size_t DLSize(const dlist_t *dlist)
{
        size_t count = 0;
        assert(dlist);

        DLForEach(DLBegin(dlist),DLEnd(dlist),IncrementCount,(void *)&count);
        return count;

}

void *DLGetData(dl_iter_t dl_iter)
{
    assert(GET_NODE_P(dl_iter));
    return GET_NODE_P(dl_iter)->data;
}

void DLSetData(dl_iter_t dl_iter, void *data)
{
    assert(GET_NODE_P(dl_iter));
    GET_NODE_P(dl_iter)->data = data;
}

int DLIsIterEqual(dl_iter_t dl_i1, dl_iter_t dl_i2)
{
    assert(GET_NODE_P(dl_i1) && GET_NODE_P(dl_i2));
    return GET_NODE_P(dl_i1) == GET_NODE_P(dl_i2);
}

static dl_iter_t DLgetEOL(dl_iter_t dl_iter)
{
    while (GET_NODE_P(dl_iter)->next)
    {
        dl_iter = DLNext(dl_iter);
    }
    return dl_iter;
}

dl_iter_t DLInsert(dl_iter_t dl_iter, void *data)
{
    dl_node_t *prev_node = NULL, *new_node = NULL;
    assert(GET_NODE_P(dl_iter));

    prev_node = GET_NODE_P(dl_iter)->prev;
    new_node = (dl_node_t *)malloc(sizeof(*new_node));
    if (!new_node)
    {
        return DLgetEOL(dl_iter);
    }

    new_node->data = data;
    new_node->prev = prev_node;
    new_node->next = GET_NODE_P(dl_iter);
    prev_node->next = new_node;
    GET_NODE_P(dl_iter)->prev = new_node;

    return DLPrev(dl_iter);
}

dl_iter_t DLRemove(dl_iter_t dl_iter)
{
    dl_node_t *next_node = NULL , *prev_node = NULL;
    assert(GET_NODE_P(dl_iter));

    next_node = GET_NODE_P(dl_iter)->next;
    prev_node = GET_NODE_P(dl_iter)->prev;

    next_node->prev = prev_node;
    prev_node->next = next_node;
    free(GET_NODE_P(dl_iter));

    GET_NODE_P(dl_iter) = next_node;
    return dl_iter;
}

dl_iter_t DLPushBack(dlist_t *dlist, void *data)
{
    assert(dlist);
    return DLInsert(DLEnd(dlist), data);
}

dl_iter_t DLPushFront(dlist_t *dlist, void *data)
{
    assert(dlist);
    return DLInsert(DLBegin(dlist),data);
}

void *DLPopBack(dlist_t *dlist)
{
    void *data = NULL;
    dl_iter_t last_node = DLPrev(DLEnd(dlist));
    assert(dlist);

    data = DLGetData(last_node);
    DLRemove(last_node);

    return data;
}

void *DLPopFront(dlist_t *dlist)
{
    void *data = NULL;
    assert(dlist);

    data = DLGetData(DLBegin(dlist));
    DLRemove(DLBegin(dlist));

    return data;
}


void DLSplice(dl_iter_t dest, dl_iter_t src_from, dl_iter_t src_to)
{
    dl_node_t *dest_prev = NULL, *before_src = NULL, *src_last = NULL;
    assert(GET_NODE_P(dest) && GET_NODE_P(src_from) && GET_NODE_P(src_to));

    dest_prev = GET_NODE_P(DLPrev(dest));
    before_src = GET_NODE_P(DLPrev(src_from));
    src_last = GET_NODE_P(DLPrev(src_to));

    /*FIRST WE CUT FROM SOURCE*/
    before_src->next = GET_NODE_P(src_to);
    GET_NODE_P(src_to)->prev = before_src;

    /*NOW WE ATTACH TO DEST*/
    dest_prev->next = GET_NODE_P(src_from);
    GET_NODE_P(src_from)->prev = dest_prev;
    src_last->next = GET_NODE_P(dest);
    GET_NODE_P(dest)->prev = src_last;
}

int DLForEach(dl_iter_t from, dl_iter_t to,int (*action)(void *data, void *param), void *param)
{
    int flag = 0;
    dl_iter_t temp_iter = from;
    assert(GET_NODE_P(from) && GET_NODE_P(to) && action);

    while (!(DLIsIterEqual(temp_iter, to)))
    {
        flag = action(DLGetData(temp_iter), param);
        if(flag)
        {
            break;
        }

        temp_iter = DLNext(temp_iter);
    }

    return flag;
}

dl_iter_t DLFind(dl_iter_t from, dl_iter_t to, const void *data,
                 int (*is_match)(const void *, const void *))
{
        dl_iter_t temp_iter = from;
        assert(GET_NODE_P(from) && GET_NODE_P(to) && is_match);

        while (!DLIsIterEqual(temp_iter,to))
        {
            if (is_match(DLGetData(temp_iter), data))
            {
                break;
            }
            temp_iter = DLNext(temp_iter);
        }

            return temp_iter;
}

int DLMultiFind(dl_iter_t from, dl_iter_t to, dlist_t *dest,
                const void *data, int(*is_match)(const void *, const void *))
{
    int flag = 0;
    dl_iter_t temp_iter = from;
    assert(GET_NODE_P(from) && GET_NODE_P(to) && is_match);

    temp_iter = DLFind(temp_iter, to, data, is_match);
    while (!flag && !DLIsIterEqual(temp_iter,to))
    {
        flag = (NULL == DLGetData(DLPushBack(dest, DLGetData(temp_iter))));
        temp_iter = DLFind(DLNext(temp_iter), to, data, is_match);
    }

    return (-1 * flag);

}
