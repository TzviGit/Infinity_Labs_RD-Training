/********************************************************
 *
 * priority_q.c - PRIORITY_Q implementation
 *
 * Author : Tzvi Moskowitz
 *
 * Reviewed and approved by : ???
 *
 * 19/09/2021
 *
 * OLRD 110 Ramat Gan
 *
 * *********************************************************/

#include <stdlib.h> /***MALLOC  FREE***/
#include <assert.h> /*assert*/
#include <stdio.h>
#include "sorted_list.h"

#define GET_DL_ITER(sr_iter) (sr_iter.dl_iter)
#define GET_DLIST_PTR(sr_list) (sr_list)->dlist


struct sorted_list
{
    dlist_t *dlist;
    int (*compare)(const void *d1, const void *d2);
};

static sr_iter_t FindInsertPoint(const sr_list_t *sr_list_t, const void *data);

sr_list_t *SortedLCreate(int (*compare)(const void *d1, const void *d2))
{
    sr_list_t *srlist = (sr_list_t *)malloc(sizeof(*srlist));
    if (srlist)
    {
        dlist_t *dlist = DLCreate();

        if (! dlist)
        {
            free(srlist); srlist = NULL;
            return NULL;
        }

        srlist->dlist = dlist;
        srlist->compare = compare;
    }

    return srlist;
}

void SortedLDestroy(sr_list_t *srlist)
{
        assert(srlist);
        DLDestroy(srlist->dlist);
        srlist->dlist = NULL;
        free(srlist); srlist = NULL;
}

void *SortedLGetData(sr_iter_t iter)
{
    return DLGetData(GET_DL_ITER(iter));
}


sr_iter_t SortedLBegin(const sr_list_t *srlist)
{
    sr_iter_t iter = {0};
    assert(srlist);

    iter.dl_iter = DLBegin(srlist->dlist);

    #ifndef NDEBUG
    iter.origin_list = srlist;
    #endif

    return iter;
}

sr_iter_t SortedLEnd(const sr_list_t *srlist)
{
    sr_iter_t iter = {0};
    assert(srlist);

    iter.dl_iter = DLEnd(srlist->dlist);

    #ifndef NDEBUG
    iter.origin_list = srlist;
    #endif

    return iter;
}

sr_iter_t SortedLNext(sr_iter_t iter)
{
    iter.dl_iter = DLNext(GET_DL_ITER(iter));
    return iter;
}

sr_iter_t SortedLPrev(sr_iter_t iter)
{
    iter.dl_iter = DLPrev(GET_DL_ITER(iter));
    return iter;
}

int SortedLIsIterEqual(sr_iter_t i1, sr_iter_t i2)
{
    assert(i1.origin_list == i2.origin_list);
    return DLIsIterEqual(GET_DL_ITER(i1), GET_DL_ITER(i2));
}

size_t SortedLSize(const sr_list_t *srlist)
{
    assert(srlist);
    return DLSize(GET_DLIST_PTR(srlist));
}

int SortedLIsEmpty(const sr_list_t *srlist)
{
    assert(srlist);
    return DLIsEmpty(GET_DLIST_PTR(srlist));
}


sr_iter_t SortedLInsert(sr_list_t *srlist, void *data)
{
    sr_iter_t iter = {0};
    assert(srlist);

    iter = FindInsertPoint(srlist, data);
    GET_DL_ITER(iter) = DLInsert(GET_DL_ITER(iter), data);



    return iter;
}

sr_iter_t SortedLRemove(sr_iter_t iter)
{
    GET_DL_ITER(iter) = DLRemove(GET_DL_ITER(iter));

    return iter;
}

void *SortedLPopBack(sr_list_t *srlist)
{
    assert(srlist);
    return DLPopBack(GET_DLIST_PTR(srlist));
}

void *SortedLPopFront(sr_list_t *srlist)
{
    assert(srlist);
    return DLPopFront(GET_DLIST_PTR(srlist));
}


sr_iter_t SortedLFindIf(sr_iter_t from, sr_iter_t to,
    const void *data, int (*is_match)(const void *, const void *))
{
    assert(from.origin_list == to.origin_list);
    GET_DL_ITER(from) = DLFind(GET_DL_ITER(from), GET_DL_ITER(to), data, is_match);
    return from;
}

sr_iter_t SortedLFind(sr_iter_t from, sr_iter_t to,
    const void *data, sr_list_t *srlist)
{
    sr_iter_t iter = from;
    assert(srlist);
    assert(from.origin_list == to.origin_list);

    while (!SortedLIsIterEqual(iter, to))
	{
        if (0 == srlist->compare(data, SortedLGetData(iter)))
		{
			break;
		}

		iter = SortedLNext(iter);
    }

    return iter;

}

int SortedLForEach(sr_iter_t from, sr_iter_t to,
    int (*action)(void *data, void *param), void *param)
{
    assert(from.origin_list == to.origin_list);
    return DLForEach(GET_DL_ITER(from), GET_DL_ITER(to),action, param);
}


void SortedLMerge(sr_list_t *dest, sr_list_t *src)
{
    sr_iter_t insert_point = {0};
    sr_iter_t src_from = {0};
    sr_iter_t src_to = {0};

    assert(dest);
    assert(src);

    insert_point = SortedLBegin(dest);

    while (!SortedLIsEmpty(src))
    {

        src_from = SortedLBegin(src);
        src_to = SortedLEnd(src);
        insert_point = FindInsertPoint(dest, SortedLGetData(src_from));
        src_to = FindInsertPoint(src, SortedLGetData(insert_point));

        DLSplice( GET_DL_ITER(insert_point), GET_DL_ITER(src_from), GET_DL_ITER(src_to));
    }

}

static sr_iter_t FindInsertPoint(const sr_list_t *srlist, const void *data)
{
    sr_iter_t iter = {0};
    assert(srlist);
    iter = SortedLBegin(srlist);
    while (!SortedLIsIterEqual(iter, SortedLEnd(srlist)))
    {
        if (srlist->compare(data, SortedLGetData(iter)) <= 0)
        {
            break;
        }

        iter = SortedLNext(iter);
    }



    return iter;
}
