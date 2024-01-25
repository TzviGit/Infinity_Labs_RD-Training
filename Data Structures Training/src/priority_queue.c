/******************************************************************************
* Description   Priority_Queue module
* Author        Tzvi Moskowitz
* Reviewer      ---
* date 20/09/21
* Group         OL110
* Company       ILRD Ramat Gan
* ****************************************************************************/

#include <assert.h> /* assert() */

#include <stdlib.h> /* malloc() */

#include "priority_queue.h"

#ifdef SL
	#include "sorted_list.h"
#else
	#include "heap.h"
#endif /* SL */



struct priority_q
{
#ifdef SL
	sr_list_t *queue;
#else
	heap_t *queue;
#endif /* SL */

};


pq_t *PQCreate(int (*prioritize)(const void *data1, const void *data2))
{
    pq_t *priority_q = NULL;
    assert(prioritize);

    priority_q = (pq_t *)malloc(sizeof(*priority_q));

    if (NULL == priority_q)
    {
        return NULL;
    }

#ifdef SL
	priority_q->queue = SortedLCreate(prioritize);
#else
	priority_q->queue = HeapCreate(prioritize);
#endif /* SL */

    if (NULL == priority_q->queue)
    {
        free(priority_q);
        priority_q = NULL;
    }

    return priority_q;

}



void PQDestroy(pq_t *priority_q)
{
    assert(priority_q);

#ifdef SL

    SortedLDestroy(priority_q->queue);
#else

    HeapDestroy(priority_q->queue);
#endif /*SL*/

    priority_q->queue = NULL;

    free(priority_q);
    priority_q = NULL;
}

int PQIsEmpty(const pq_t *priority_q)
{
    assert(priority_q);

#ifdef SL

    return SortedLIsEmpty(priority_q->queue);
#else

    return HeapIsEmpty(priority_q->queue);
#endif  /*SL*/
}

size_t PQSize(const pq_t *priority_q)
{
    assert(priority_q);

#ifdef SL

    return SortedLSize(priority_q->queue);
#else

    return HeapSize(priority_q->queue);
#endif /* SL */


}

int PQEnqueue(pq_t *priority_q, void *data)
{

    assert(priority_q);

#ifdef SL

    return SortedLIsIterEqual(SortedLInsert(priority_q->queue, data),
                                            SortedLEnd(priority_q->queue));
#else

    return HeapPush(priority_q->queue, data);
#endif /*SL*/
}


void *PQDequeue(pq_t *priority_q)
{
    assert(priority_q);

#ifdef SL

    return SortedLPopBack(priority_q->queue);
#else

	return HeapPop(priority_q->queue);
#endif /* SL */

}

void *PQPeek(const pq_t *priority_q)
{
    assert(priority_q);

#ifdef SL

    return SortedLGetData(SortedLPrev(SortedLEnd(priority_q->queue)));
#else

    return HeapPeek(priority_q->queue);
#endif /* SL */

}

void *PQErase(pq_t *priority_q, const void *data)
{
#ifdef SL

    sr_iter_t from = {0}, to = {0}, iter = {0};
    void *ret_val = NULL;

    assert(priority_q);

    from = SortedLBegin(priority_q->queue);
    to = SortedLEnd(priority_q->queue);
    iter = SortedLFind(from, to ,data, priority_q->queue);

    if (!SortedLIsIterEqual(iter, to))
    {
        ret_val = SortedLGetData(iter);
        SortedLRemove(iter);
    }

    return ret_val;

#else

    return HeapRemove(priority_q->queue, data);

#endif /*SL*/
}

void *PQEraseIf(pq_t *priority_q, const void *criteria,
    int (*is_match)(const void *data, const void *criteria))
{
#ifdef SL

    sr_iter_t from = {0}, to = {0}, iter = {0};
    void *ret_val = NULL;

    assert(priority_q);
    assert(is_match);

    from = SortedLBegin(priority_q->queue);
    to = SortedLEnd(priority_q->queue);
    iter = SortedLFindIf(from, to ,criteria, is_match);

    if (!SortedLIsIterEqual(iter, to))
    {
        ret_val = SortedLGetData(iter);
        SortedLRemove(iter);
    }

    return ret_val;

#else

    return HeapRemoveIf(priority_q->queue, criteria, is_match);

#endif /*SL*/

}


void PQClear(pq_t *priority_q)
{
    assert (priority_q);

    while (!PQIsEmpty(priority_q))
    {
        PQDequeue(priority_q);
    }

}
