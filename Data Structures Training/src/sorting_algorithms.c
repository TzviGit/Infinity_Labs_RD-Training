/******************************************************************************
* Author        Tzvi Moskowitz
* Reviewer      NADAV
* Description   Scheduler module
* Group         OL110
* Company       ILRD Ramat Gan
* ****************************************************************************/
#include <stdio.h>
#include <assert.h> /*assert*/
#include <stddef.h> /*size_t*/
#include <stdlib.h> /*malloc etc.*/
#include <string.h> /*mamcpy*/
#include "sorting_algorithms.h"
#include "avl.h" /*AVLTreeSort*/


int MyCompare(const void *d1, const void *d2)
{
    return *(int *)d1 - *(int *)d2;
}
int PutInIntArray(void *data, void *param);

#define ABS(num) ((num) > 0 ? (num) : -(num))

#ifndef CHAR_BIT
#define CHAR_BIT 8
#endif


static int BinarySearchRecursiveImple(const int *arr, size_t left_ind,
                                            size_t right_ind, int num);
static int*RadixCountingSort(int *dest , int *src, size_t size,
     size_t *histogram, size_t hist_size, unsigned int byte_mask);
static void ArrayAddLeftCell(size_t *arr, size_t size);
static void ClearBuffer(size_t *buf, size_t size);
static int ArrayGetMax(int *arr, size_t size);
static int ArrayGetMin(int *arr, size_t size);
static void IntSwap(int *p1, int *p2);
static size_t FindIndexOfMin(int *arr, size_t size);
static void FillHistogram(int *arr, size_t arr_size,
                size_t *histogram, long offset_of_min);
static void BubbleNextLargest(int *arr, size_t end, int *flag);
static int *MergeSortImple(int *arr_ptr, int *buffer_ptr, size_t size);
static void MergeToBuffer(int *left_arr, size_t left_arr_size, int *right_arr,
                                        size_t right_arr_size, int *buffer_ptr);

size_t PartionOneElement(void *base, size_t n_items, size_t size,
                                    int (*compare)(const void *, const void*));

static void SwapBytes(void *data1, void *data2, size_t size_of_elem);

/*************************************************************************
*           ******BUBBLESORT*******      Time Complexity: O(n^2)
*                                        space COmplexity: O(1)
************************************************************************/
/**********CANNOT FAIL*******/
void BubbleSort(int *arr, size_t size)
{
    size_t i = 0;
    int not_done = 1;

    assert(arr);

    for (i = (size - 1); not_done && i > 0; --i)
    {
        BubbleNextLargest(arr, i, &not_done);
    }
}

static void BubbleNextLargest(int *arr, size_t end, int *flag)
{
    size_t i = 0;

    assert(arr);

    *flag = 0;
    for (i = 0; i < end; ++i)
    {
        if (arr[i] > arr[i + 1])
        {
            IntSwap(&arr[i], &arr[i + 1]);
            *flag = 1;
        }
    }
}

/*************************************************************************
*           ******INSERTION_SORT*******      Time Complexity: O(n^2)
*                                        space COmplexity: O(1)
************************************************************************/
/**********CANNOT FAIL*******/
void InsertionSort(int *arr, size_t size)
{
    size_t i = 0, j = 0;

    assert(arr);

    for (i = 1; i < size; ++i)
    {
        int curr_insert = arr[i];

        for (j = i; (j > 0) && (curr_insert < arr[j - 1]); --j)
        {
            arr[j] = arr[j - 1];
        }

        arr[j] = curr_insert;
    }
}

/*************************************************************************
*           ******SELECTION_SORT*******      Time Complexity: O(n^2)
*                                        space COmplexity: O(1)
************************************************************************/
/**********CANNOT FAIL*******/
void SelectionSort(int *arr, size_t size)
{
    size_t i = 0;

    assert(arr);

    for (i = 0; i < (size - 1) ;++i)
    {
        size_t curr_min_index = i + FindIndexOfMin((arr + i), (size - i));
        IntSwap((arr + i), (arr + curr_min_index));
    }
}



/*************************************************************************
*           ******COUNTING_SORT*******      Time Complexity: O(n)
*                                        space COmplexity: O(n)
************************************************************************/
/**********RETURNS 1 IF IT FAILED TO SORT, OR ELSE 0 ON SUCCESS*******/

int CountingSort(int *arr, size_t size)
{
    int *temp_arr = NULL;
    int min = 0, max = 0;
    long offset = 0;
    size_t range = 0;
    size_t i = 0;
    size_t *histogram = NULL;
    assert(arr);

    temp_arr = malloc(size * sizeof(*temp_arr));
    if (!temp_arr)
    {return 1;}

    min = ArrayGetMin(arr, size);
    max = ArrayGetMax(arr, size);
    offset = min;
    range = (max - min) + 1;

    histogram = calloc(1, (range * sizeof(*histogram)));
    if (!histogram)
    {return 1;}

    FillHistogram(arr, size, histogram, offset);
    ArrayAddLeftCell(histogram, range);

    for (i = (size - 1); (long)i >= 0; --i)
    {
        int place_value = arr[i] - offset;

        size_t new_ind = --histogram[place_value];
        memcpy((temp_arr + new_ind), (arr + i), sizeof(int));
    }

    memcpy(arr, temp_arr, (sizeof(int) * size));

    free(temp_arr); temp_arr = NULL;
    free(histogram); histogram = NULL;

    return 0;

}

static void FillHistogram(int *arr, size_t arr_size,
                size_t *histogram, long offset_of_min)
{
    size_t i = 0;
    assert(arr);
    assert(histogram);

    for (i = 0; i < arr_size; ++i)
    {
        size_t offset_index = arr[i] - offset_of_min;
        histogram[offset_index] += 1;
    }

}



/*************************************************************************
*           ******RADIX_SORT*******      Time Complexity: O(n)
*       uses counting sort Algorithm     space COmplexity: O(n)
************************************************************************/
/**********RETURNS 1 IF IT FAILED TO SORT, OR ELSE 0 ON SUCCESS*******/

int RadixSort(int *arr, size_t size)
{
    size_t shift = 0;
    size_t iterations = 4, hist_size = 256;
    size_t *histogram = NULL;
    int *temp_arr = NULL;
    assert(arr);

    histogram = calloc(1, (hist_size * sizeof(*histogram)));
    if (!histogram){return 1;}

    temp_arr = malloc(size * sizeof(*temp_arr));
    if (!temp_arr){return 1;}

    while (iterations--)
    {
        int *ptr = arr;
        arr = RadixCountingSort(temp_arr, arr, size, histogram, hist_size, shift);
        temp_arr = ptr;

        shift += CHAR_BIT;
    }

    free(temp_arr); temp_arr = NULL;
    free(histogram); histogram = NULL;

    return 0;
}


static int *RadixCountingSort(int *dest , int *src, size_t size,
     size_t *histogram, size_t hist_size, unsigned int shift)
{
    size_t i = 0;
    int byte_mask = 0xFF;

    for(i = 0; i < size; ++i)
    {
        size_t index = ((src[i] >> shift) & byte_mask);
        ++histogram[index];
    }

    ArrayAddLeftCell(histogram, hist_size);

    for (i = (size - 1); (long)i >= 0; --i)
    {
        int hist_ind = ((src[i] >> shift) & byte_mask);

        size_t new_ind = --histogram[hist_ind];
        memcpy((dest + new_ind), (src + i), sizeof(int));
    }

    ClearBuffer(histogram, hist_size);

    return dest;
}



/***************************************************************************
*               ****GENERAL UTILITY FUNCTIONS****
*
****************************************************************************/


static void ClearBuffer(size_t *buf, size_t size)
{
    size_t i = 0;
    assert(buf);

    for (i = 0; i < size; ++i)
    {
        buf[i] = 0;
    }
}

static void ArrayAddLeftCell(size_t *arr, size_t size)
{
        size_t i = 1;
        assert(arr);

        for (i = 1; i < size; ++i)
        {
            arr[i] += arr[i - 1];
        }
}


static void IntSwap(int *p1, int *p2)
{
    int temp = 0;
    assert(p1 && p2);

    temp = *p1;
    *p1 = *p2;
    *p2 = temp;
}

static size_t FindIndexOfMin(int *arr, size_t size)
{
    size_t i = 0;
    size_t ind_of_min = 0;
    assert(arr);


    for (i = 0; i < size; ++i)
    {
        if (arr[i] < arr[ind_of_min])
        {
            ind_of_min = i;
        }
    }

    return ind_of_min;
}

static int ArrayGetMax(int *arr, size_t size)
{
    size_t i = 0;
    int max = 0;
    assert(arr);

    max = arr[0];
    for(i = 0; i < size; ++i)
    {
        if (arr[i] > max)
        {
            max = arr[i];
        }
    }

    return max;
}

static int ArrayGetMin(int *arr, size_t size)
{
    size_t i = 0;
    int min = 0;
    assert(arr);

    min = arr[0];
    for(i = 0; i < size; ++i)
    {
        if (arr[i] < min)
        {
            min = arr[i];
        }
    }

    return min;
}


void AVL_TreeSort(int *arr, size_t size)
{
    avl_t *avl_tree = AVLCreate(MyCompare);
    int *buffer = malloc (8 *size);

    size_t i = 0;

    if (NULL == avl_tree)
    {
        return ;
    }


    for (i = 0; i < size; ++i)
    {

        AVLInsert(avl_tree, arr + i);
    }


    AVLForEach(avl_tree, PutInIntArray, (void *)buffer, IN);
    printf("\n\n");
    for (i = 0; i < size; ++i)
    {
        arr[i] = buffer[i] ;
    }

    /*printf("\n\n");
    AVLForEach(avl_tree, PrintNode, (void *)ptr, IN_ORDER);
    printf("\n\n");*/
    AVLDestroy(avl_tree);
    free(buffer);


}



int BinarySearchRecursive(const int *arr, size_t size, int num)
{
    return BinarySearchRecursiveImple(arr, 0, size - 1, num);
}


static int BinarySearchRecursiveImple(const int *arr, size_t left_ind,
                                            size_t right_ind, int num)
{
    if (left_ind > right_ind)
    {
        return -1;
    }
    else
    {
        size_t mid_ind = left_ind + (right_ind - left_ind) / 2;

        if (arr[mid_ind] == num)
        {
            return mid_ind;
        }
        else if (arr[mid_ind] > num)
        {
            return BinarySearchRecursiveImple(arr, left_ind, mid_ind - 1, num);
        }
        else
        {
            return BinarySearchRecursiveImple(arr, mid_ind + 1, right_ind, num);
        }
    }
}

int BinarySearchIterative(const int *arr, size_t size, int num)
{
    int left_ind = 0;
    int right_ind = size - 1;


    assert (arr);


    while (left_ind != right_ind)
    {
        int mid_ind = left_ind + ((right_ind - left_ind) / 2);

        if (num == arr[mid_ind])
        {
            return mid_ind;
        }
        else if (arr[mid_ind] > num)
        {
            right_ind = mid_ind - 1;
        }
        else
        {
            left_ind = mid_ind + 1;
        }

    }

    return (num == arr[left_ind]) ? left_ind : -1;
}


int MergeSort(int *arr_to_sort, size_t num_elements)
{
    int *buffer = (int *)malloc(num_elements * sizeof(buffer));
    if (NULL == buffer)
    {
        return 1;
    }

    MergeSortImple(arr_to_sort, buffer, num_elements);

    free(buffer); buffer = NULL;

    return 0;
}

static int *MergeSortImple(int *arr_ptr, int *buffer_ptr, size_t size)
{
    size_t half = size / 2;
    size_t left_arr_size = half;
    size_t right_arr_size = size - half;
    int *left_arr = NULL;
    int *right_arr = NULL;
    size_t i = 0;

    if (1 == size)
    {
        return arr_ptr;
    }

    left_arr = MergeSortImple(arr_ptr, buffer_ptr, left_arr_size);
    right_arr = MergeSortImple(arr_ptr + half, buffer_ptr + half, right_arr_size);

    MergeToBuffer(left_arr, left_arr_size, right_arr, right_arr_size, buffer_ptr);

    for (i = 0 ; i < size; ++i)
    {
        arr_ptr[i] = buffer_ptr[i];
    }

    return arr_ptr;
    /*return (int *)memcpy(arr_ptr, (int *)buffer_ptr, (size * sizeof(int)));*/
}


static void MergeToBuffer(int *left_arr, size_t left_arr_size, int *right_arr,
                                        size_t right_arr_size, int *buffer_ptr)
{
    if (0 == left_arr_size)
    {
        memcpy(buffer_ptr, (int *)right_arr, (right_arr_size * sizeof(int)));
        return;
    }
    else if (0 == right_arr_size)
    {
        memcpy(buffer_ptr, (int *)left_arr, (left_arr_size * sizeof(int)));
        return;
    }
    else
    {
        if (*left_arr <= *right_arr)
        {
            *buffer_ptr = *left_arr++;
            --left_arr_size;
        }
        else
        {
            *buffer_ptr = *right_arr++;
            --right_arr_size;
        }

        ++buffer_ptr;

        MergeToBuffer(left_arr, left_arr_size, right_arr,
                                right_arr_size, buffer_ptr);
    }

}


void QuickSort(void *base, size_t n_items, size_t size,
                                    int (*compare)(const void *, const void*))
{
    size_t partition_index = 0;
    void * right_arr = NULL;

    if (n_items < 2)
    {
        return;
    }

    partition_index = PartionOneElement(base, n_items, size, compare);

    right_arr = (void *)((char *)base + (size * (partition_index + 1)));

    QuickSort(base, partition_index, size,compare);

    

    QuickSort(right_arr, ((n_items - 1) - partition_index), size, compare);
}

size_t PartionOneElement(void *base, size_t n_items, size_t elem_size,
                                    int (*compare)(const void *, const void*))
{
    size_t partition_index = 0;
    size_t iterator = 1;
    void * pivot = (char *)base + (elem_size * (rand() % n_items));
    void *partition_data = NULL;

    SwapBytes(base, pivot, elem_size);

    pivot = base;

    for (; iterator < n_items; ++iterator)
    {
        void *iter_data = (void *)((char *)base + (iterator * elem_size));


        if (compare(pivot, iter_data) >= 0)
        {
            ++partition_index;
            partition_data = ((char *)base + (partition_index * elem_size));

            SwapBytes(iter_data, partition_data, elem_size);
        }
    }

    SwapBytes(base, (void *)((char *)base + (partition_index * elem_size)),
                                                                    elem_size);

    return partition_index;

}

static void SwapBytes(void *data1, void *data2, size_t elem_size)
{
    char *ptr1 = (char *)data1;
    char *ptr2 = (char *)data2;
    char temp = '\0';

    size_t i = 0;

    for (i = 0; i < elem_size; ++i)
    {
        temp = *ptr1;
        *ptr1++ = *ptr2;
        *ptr2++ = temp;
    }

}
