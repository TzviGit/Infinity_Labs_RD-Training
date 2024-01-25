/******************************************************************************
* Author        Tzvi Moskowitz
* Reviewer      Tuvia Resnick
* Description   heap data structer --- *MAX_HEAP*  implementation
* Date:         October 27, 2021
* Group         OL110
* Company       ILRD Ramat Gan
* ****************************************************************************/

#include <stdio.h>
#include <assert.h> /*assert*/
#include <stdlib.h> /*malloc and free*/

#include "heap.h"      /*the API for this module*/
#include "dynamic_vector.h" /*API for the dvector data struct  we use for our impl*/

static const size_t INIT_SIZE = 2;




struct heap
{
    vector_t *vector;
    compare_t compare;
};

static void ProperlyRemove(heap_t *heap, size_t index);
static void BubbleDown(heap_t *heap, size_t index);
static void BubbleUp(heap_t *heap, size_t index);
static void HeapDataSwap(heap_t *heap, size_t ind_1, size_t ind_2);
static size_t GetLargestChildIndex(heap_t *heap, size_t index);
static void InitializeHeapStruct(heap_t *heap, vector_t *vec, compare_t comp);



/***************************************************************************
*    ~~~~~~  A slew of utility functions to make code more readable  ~~~
*                       try to understand them.
****************************************************************************/

static int HasNoChildren(heap_t *heap, size_t index)
{
    return  index >= DVectorGetSize(heap->vector) / 2 ;
}

static int HasParent(size_t index)
{
    /* even though index is insigned, this is just to
    *    stop the recursion BEFORE it reaches 0     */
    return  index > 0;
}

static int HasLeftSon(heap_t *heap, size_t index)
{
    return index < DVectorGetSize(heap->vector) / 2 ;
}

static int HasRightSon(heap_t *heap, size_t index)
{
    return index < (DVectorGetSize(heap->vector) - 1) / 2 ;
}

static size_t LeftSonIndex(size_t index)
{
    return (2 * index) + 1;
}

static size_t RightSonIndex(size_t index)
{
    return (2 *index) + 2;
}

static size_t ParentIndex(size_t index)
{
    return (index - 1) / 2;
}


/*************************************************************************
*~~~~~~~~~~~~~~~~~~      HEAP_CREATE  and util func ~~~~~~~~~          */
/*  O(1) */
/*  in: compare function (<,>,= 0 if val1 is <,>,= val2 respectively)   */
/*  Creates a max heap                                                  */
/*  ret: heap_t pointer on success, NULL on failure                     */

heap_t *HeapCreate(compare_t compare)
{
    heap_t *heap = NULL;
    vector_t *internal_vector = DVectorCreate(INIT_SIZE);
    if (NULL == internal_vector)
    {
        return NULL;
    }

    heap = (heap_t *)malloc(sizeof(*heap));
    if (NULL == heap)
    {
        DVectorDestroy(internal_vector);
        internal_vector = NULL;

        return NULL;
    }

    InitializeHeapStruct(heap, internal_vector, compare);

    return heap;
}


static void InitializeHeapStruct(heap_t *heap, vector_t *vec, compare_t comp)
{
    heap->vector = vec;
    heap->compare = comp;
}

/*************************************************************************
*~~~~~~~~~~~~~~~~~~      HEAP_DESTROY  ~~~~~~~~~~~~~~~~~~~~~~~~~          */
/*  O(1) */
/*  in: heap ptr to destroy */

void HeapDestroy(heap_t *heap)
{
    assert (heap);

    DVectorDestroy(heap->vector);

    heap->vector = NULL;
    heap->compare = NULL;
    free(heap);
    heap = NULL;
}


/*************************************************************************
*~~~~~~~~~~~~~~~~~~      HEAP_IS_EMPTY  ~~~~~~~~~~~~~~~~~~~~~~~~~        */
/* O(1) */
/*  in: binary heap                             */
/*  ret: 0 if not empty, other number otherwise */

int HeapIsEmpty(const heap_t *heap)
{
    assert (heap);

    return (0 == DVectorGetSize(heap->vector));
}


/*************************************************************************
*~~~~~~~~~~~~~~~~~~      HEAP_SIZE  ~~~~~~~~~~~~~~~~~~~~~~~~~          */
/* O(1) */
/*  in: binary heap                         */
/*  retrieve the heap's size (# elements)   */

size_t HeapSize(const heap_t *heap)
{
    assert (heap);

    return DVectorGetSize(heap->vector);
}

/*************************************************************************
*~~~~~~~~~~~~~~~~~~      HEAP_PUSH and BubbleUp  ~~~~~~~~~~~~~~~        */
/**************************************************************************/
/* O(log n) */
/*  in: heap to place data in                   */
/*  data to insert                              */
/*  ret: 0 on Success, other number on failure  */

int HeapPush(heap_t *heap, void *data)
{
    assert (heap);
    assert (data);

    if (0 != DVectorPushBack(heap->vector, data))
    {
        return 1;
    }

    BubbleUp(heap, DVectorGetSize(heap->vector) - 1);

    return 0;
}


/***************************************************************************
******************* BubbleUp functions  and Heap_Data_Swap *****************/

static void BubbleUp(heap_t *heap, size_t index)
{
    long parent_ind = 0;
    void *parent_data = NULL;
    void *curr_data = NULL;

    if (!HasParent(index))
    {
        return;
    }

    curr_data = DVectorGet(heap->vector, index);
    parent_ind = ParentIndex(index);
    parent_data = DVectorGet(heap->vector, parent_ind);

    if (heap->compare(parent_data, curr_data) < 0)
    {
        HeapDataSwap(heap, parent_ind, index);
        BubbleUp(heap, parent_ind);
    }
}


/***************************************************************************
******************* Heap_Data_Swap ****************************************/

static void HeapDataSwap(heap_t *heap, size_t ind_1, size_t ind_2)
{
    vector_t *vector = heap->vector;

    void *temp = DVectorGet(vector, ind_1);
    DVectorSet(vector, ind_1, DVectorGet(vector, ind_2));
    DVectorSet(vector, ind_2, temp);
}



/***************************************************************************
******************* GET_LARGEST_SON_INDEX  ************ *****************/

static size_t GetLargestChildIndex(heap_t *heap, size_t index)
{

    size_t left = LeftSonIndex(index);
    size_t right = RightSonIndex(index);

    if (!HasRightSon(heap, index))
    {
        return left;
    }
    else
    {
        void *left_data = DVectorGet(heap->vector, left);
        void *right_data = DVectorGet(heap->vector, right);

        return (heap->compare(left_data, right_data) >= 0) ? left : right;
    }
}


/*************************************************************************
*~~~~~~~~~~~~~~~~~~      HEAP_POP   and BubbleDown funcs  ~~~~~~        */
/**************************************************************************/
/* O(log n) */
/*  in: non-empty binary heap   */
/*  ret: removed min element    */

void *HeapPop(heap_t *heap)
{
    void *max =  NULL;
    void *end = NULL;
    assert (heap);

    max = DVectorGet(heap->vector, 0);
    end = DVectorPopBack(heap->vector);

    if (!HeapIsEmpty(heap))
    {
        DVectorSet(heap->vector, 0, end);

        BubbleDown(heap, 0);
    }


    return max;
}



/***************************************************************************
**********************  BubbleDown functions  ******************************/

static void BubbleDown(heap_t *heap, size_t index)
{
    size_t larger_child_ind = 0;
    void *larger_child_data = NULL;
    void *curr_data = DVectorGet(heap->vector, index);

    if (HasNoChildren(heap, index))
    {
        return;
    }

    larger_child_ind = GetLargestChildIndex(heap, index);
    larger_child_data = DVectorGet(heap->vector, larger_child_ind);

    if (heap->compare(curr_data, larger_child_data) <= 0)
    {
        HeapDataSwap(heap, larger_child_ind, index);
        BubbleDown(heap, larger_child_ind);
    }

}


/*************************************************************************
*~     ~~~~~~~~~~~~~~~~~      HEAP_PEEK  ~~~~~~        */
/**************************************************************************/

/* O(1)  */
/*  in: non-empty binary heap           */
/*  ret: data of min element from heap  */

void *HeapPeek(const heap_t *heap)
{
    assert (heap);
    assert (!HeapIsEmpty(heap));

    return DVectorGet(heap->vector, 0);
}


/*************************************************************************
*~~~~~~~~~~~~~~~   HEAP_REMOVE_If  and heap_find_index funcs  ~~~~~~        */
/**************************************************************************/

/* O(n) */
/*  find and remove first occurence of 'data' element in the heap   */
/*  in: binary heap                                                 */
/*      data of element to be removed                               */
/*  return : the value of the element removed, if found             */
/*           NULL if not found                                      */


void *HeapRemoveIf(heap_t *heap, const void *criteria,
            int (*is_match)(const void *data, const void *criteria))
{
    size_t i = 0;

    for (i = 0; i < DVectorGetSize(heap->vector); ++i)
    {
        void *data = DVectorGet(heap->vector, i);

        if (is_match(data, criteria))
        {
            ProperlyRemove(heap, i);
            return data;
        }
    }

    return NULL;
}


/*************************************************************************
*~~~~~~~~~~~~~~~   HEAP_REMOVE  and heap_find_index funcs  ~~~~~~       */
/**************************************************************************/


void *HeapRemove(heap_t *heap, const void *criteria)
{
    size_t i = 0;

    for (i = 0; i < DVectorGetSize(heap->vector); ++i)
    {
        void *data = DVectorGet(heap->vector, i);

        if (0 == heap->compare(data, criteria))
        {
            ProperlyRemove(heap, i);
            return data;
        }
    }

    return NULL;
}



/*****************************************************************************
*******************  util func ProperlyRemove   ****************************/

static void ProperlyRemove(heap_t *heap, size_t index)
{
    size_t end_ind = DVectorGetSize(heap->vector) - 1;
    HeapDataSwap(heap, index, end_ind);

    DVectorPopBack(heap->vector);

    BubbleUp(heap, index);
    BubbleDown(heap, index);
}

/*    ~~~~~~~~~~~~~~~~  END OF API FUNCTIONS   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
******************************************************************************/






/*************************************************************************
*~~~~~~~~~~~~~~~~~~      Function used for the TESTER  ~~~~~~        */
/**************************************************************************/


static int IsMaxHeapImple(heap_t *heap, size_t index )
{
    int bool1 = 1, bool2 = 1, bool3 = 1;
    vector_t *vector = heap->vector;
    void *larger_child_data = NULL;

    if (HasNoChildren(heap, index))
    {
        return 1;
    }

    larger_child_data = DVectorGet(vector, GetLargestChildIndex(heap, index));

    if (HasRightSon(heap, index))
    {
        bool1 = IsMaxHeapImple(heap, RightSonIndex(index));
    }

    bool2 = IsMaxHeapImple(heap, LeftSonIndex(index));

    bool3 = heap->compare(DVectorGet(vector, index), larger_child_data) >= 0;



    return bool1 && bool2 && bool3;
}

int IsMaxHeap(heap_t *heap)
{
    return IsMaxHeapImple(heap, 0);
}


/***************************************************************************
*******************     Print out the inner Vector   **********************/

int PrintVector(heap_t *heap)
{
    size_t i = 0;
    for (; i < DVectorGetSize(heap->vector); ++i)
    {
        printf("%d -> ",*(int *)DVectorGet(heap->vector, i));
    }

    printf("\n\n");
    return 0;
}

void HeapifySubTree(int *arr, size_t size, size_t index);
void HeapifyArray(int *arr, size_t size);
void Swap(int *d1, int *d2);


void HeapifyArray(int *arr, size_t size)
{
    long i = 0;
    for (i = (size / 2 - 1); i >= 0; i--)
      {
          HeapifySubTree(arr, size, i);
      }
}

void HeapifySubTree(int *arr, size_t size, size_t index)
{
  size_t largest = index;
  size_t left = 2 * index + 1;
  size_t right = 2 * index + 2;

  if (left < size)
    {
        if (right < size && arr[right] > arr[largest])
          {
              largest = right;
          }
          else if (arr[left] > arr[largest])
          {
              largest = right;
          }
      }

    if (largest != index)
    {
      Swap(&arr[index], &arr[largest]);
      HeapifySubTree(arr, size, largest);
    }
}

void Swap(int *d1, int *d2)
{
    int temp = *d1;
    *d1 = *d2;
    *d2 = temp;
}

void HeapSort(int *arr, size_t size)
{
    long i = 0;

    HeapifyArray(arr, size);

    for (i = size - 1; i > 0; --i)
    {
        Swap(&arr[i], &arr[0]);
        HeapifyArray(arr, --size);
    }
}
