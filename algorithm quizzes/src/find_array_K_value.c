/******************************************************************************
* Author        Tzvi Moskowitz
* Reviewer      --------
* Description   Algorithm to find Kth smallest value in int array.
* Group         OL110
* Company       ILRD Ramat Gan
* Date
* ****************************************************************************/

#include <assert.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include "sort.h"
#include "test_util.h"

#define SIZE 100

/*******************************************************************************
*   Finds Kth Smallest value of int array.
*   Assumes that there are no doubles in the list.
*   ###### K_VALUE MUST BE ==SMALLER== THAN ARRAY_SIZE. ######
*
*   Time Complexity: O(n * d) -- (d being how many sort slices per key.
                                sort slices is implementation defined)
*    space COmplexity: O(n + d)
*******************************************************************************/
int ArrayK_Value(int *arr, size_t size, size_t k_value)
{
    assert(arr);

    RadixSort(arr,size);

    return arr[k_value];
}

/******============= NOW LETS TEST IT!!! ========*****************/

void TestArrayK_Value(void)
{
    int flag = 0;
    int arr[SIZE] = {0};
    size_t i = 0;
    size_t wanted_K_value = 5;
    int found_K_value = 0;


    for (i = 0; i < SIZE; ++i)
    {
        arr[i] = 2 * (SIZE - i);
    }

    found_K_value = ArrayK_Value(arr, SIZE, wanted_K_value);

    VERIFY(found_K_value = 10);

    CHECK_SUCCESS(flag, "ArrayK_Value");
}


int main(void)
{
    TestArrayK_Value();
    return 0;
}
