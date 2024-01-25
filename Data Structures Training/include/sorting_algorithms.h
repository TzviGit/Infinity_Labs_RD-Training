/* ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ *
 ~ Version       1.0.0                                           ~
 *                                                               *
 ~ Author        OL110 Students                                  ~
 * Reviewer      Yinon Yishay                                    *
 ~ Description   Sorting Algorithm - Header_File   				 ~
 * Group         OL110                                           *
 ~ Company       ILRD Ramat Gan                                  ~
 * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ */

#ifndef __OL110SORT_H__
#define __OL110SORT_H__

#include <stddef.h> /* size_t */


/*************************************************************************
*           ******BUBBLESORT*******      Time Complexity: O(n^2)
*                                        space COmplexity: O(1)
************************************************************************/
/**********CANNOT FAIL*******/
void BubbleSort(int *arr, size_t size);

/*************************************************************************
*           ******INSERTION_SORT*******      Time Complexity: O(n^2)
*                                        space COmplexity: O(1)
************************************************************************/
/**********CANNOT FAIL*******/
void InsertionSort(int *arr, size_t size);

/*************************************************************************
*           ******SELECTION_SORT*******      Time Complexity: O(n^2)
*                                        space COmplexity: O(1)
************************************************************************/
/**********CANNOT FAIL*******/
void SelectionSort(int *arr, size_t size);

/*************************************************************************
*           ******COUNTING_SORT*******      Time Complexity: O(n * R)
*        space COmplexity: O(n + R)    (R is the range of the array)
*
**********RETURNS 1 IF IT FAILED TO SORT, OR ELSE 0 ON SUCCESS*******/
int CountingSort(int *arr, size_t size);

/*************************************************************************
* ******RADIX_SORT*******   Time Complexity: O(n * d)
* (d being how many sort slices per key. sort slices is implementation defined)
*                         space COmplexity: O(n + d)
**********RETURNS 1 IF IT FAILED TO SORT, OR ELSE 0 ON SUCCESS*******/
int RadixSort(int *arr, size_t size);

void AVL_TreeSort(int *arr, size_t size);


/*A recursive binary search function. It returns
// location of num in given array arr[l..r] iprf esent,
// otherwise -1*/
int BinarySearchRecursive(const int *arr, size_t size, int num);

/*// A iterative binary search function. It returns
// location of x in given array arr is present,
// otherwise -1*/
int BinarySearchIterative(const int *arr, size_t size, int num);

int MergeSort(int *arr_to_sort, size_t num_elements);

void QuickSort(void *base, size_t nitems, size_t size,
                                    int (*compar)(const void *, const void*));

#endif /* __SORT_OL_110_H__ */
