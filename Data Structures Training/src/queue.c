#include <assert.h> /*assert*/
#include <stddef.h> /*NULL*/
#include <stdlib.h> /*free, malloc*/
#include "queue.h"
#include "s_list.h"

 /*APPROVED bY JOHNNY*/

struct queue
{
    slist_t *q_list;
};

queue_t *QCreate()
{
    queue_t *queue = (queue_t *)malloc(sizeof(*queue));
    if (!queue)
    {
        return NULL;
    }
    queue->q_list = SLCreate();
    if(!queue->q_list)
    {
        free(queue); queue = NULL;
        return NULL;
    }
    return queue;
}

void QDestroy(queue_t *queue)
{
    assert(queue && queue->q_list);
    SLDestroy(queue->q_list); queue->q_list = NULL;
    free(queue);
}

int QIsEmpty(const queue_t *queue)
{
    assert(queue);
    return SLIsEmpty(queue->q_list);
}

size_t QGetSize(const queue_t *queue)
{
    assert(queue);
    return SLCount(queue->q_list);
}

int QEnqueue(queue_t *queue, void *data)
{
    assert(queue);
    return !SLIterIsEqual(SLEnd(queue->q_list),SLAddBefore(SLEnd(queue->q_list), data));
}

void *QDequeue(queue_t *queue)
{
    void *data = NULL;
    assert(queue);
    data = SLGetValue(SLBegin(queue->q_list));
    SLRemove(SLBegin(queue->q_list));
    return data;
}

void *QPeek(const queue_t *queue)
{
    assert(queue);
    return SLGetValue(SLBegin(queue->q_list));
}

void QAppend(queue_t *base, queue_t *append)
{
    assert(base && append);
    SLAppend(base->q_list, append->q_list);
}
