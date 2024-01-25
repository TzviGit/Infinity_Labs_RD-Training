 /******************************************************************************
* Author        Tzvi Moskowitz
* Description  SOrt Funcs TESTER
* Group         OL110
* Company       ILRD Ramat Gan
* ****************************************************************************/

#define ABS(num) ((num) > 0 ? (num) : -(num))

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sorting_algorithms.h"
#include "test_util.h"
#define SIZE 600


int IsSorted(int *arr, size_t size)
{
    size_t i = 0;
    for (; i < size - 5; ++i)
    {
        if (arr[i] > arr[i + 1])
        {return 0;}
    }

    return 1;
}

int Compare(const void *p1, const void *p2)
{
    return *(int *)p1 - *(int *)p2;
}




void TestBubbleSort()
{
    int flag = 0;
    clock_t before = {0};
    clock_t after = {0};

    int arr[SIZE] = {0};
    size_t i = 0;
    for (; i < SIZE; ++i)
    {
        arr[i] = rand();
    }

    printf("*******BubbleSort---Random*******\n\n");
    before = clock();
    BubbleSort(arr, SIZE);
    after = clock();

    printf("BubbleSort -- Random-- took %lu clock tics\n\n", (after - before));

    VERIFY(IsSorted(arr, SIZE));

    printf("*******BubbleSort---sorted backwords (worst case)*******\n\n");

    for (i = 0; i < SIZE; ++i)
    {
        arr[i] = SIZE - i;
    }

    before = clock();
    BubbleSort(arr, SIZE);
    after = clock();

    printf("BubbleSort **worst case** took %lu clock tics\n\n", (after - before));

    VERIFY(IsSorted(arr, SIZE));

    CHECK_SUCCESS("BUBBLESORT");

}

void TestInsertionSort()
{
    int flag = 0;
    clock_t before = {0};
    clock_t after = {0};

    int arr[SIZE] = {0};
    size_t i = 0;
    for (; i < SIZE; ++i)
    {
        arr[i] = rand();
    }

    printf("\n\n*******InsertionSort---Random*******\n\n");
    before = clock();
    InsertionSort(arr, SIZE);
    after = clock();

    printf("InsertionSort -- Random-- took %lu clock tics\n\n", (after - before));

    VERIFY(IsSorted(arr, SIZE));

    printf("*******InsertionSort---sorted backwords (worst case)*******\n\n");

    for (i = 0; i < SIZE; ++i)
    {
        arr[i] = SIZE - i;
    }

    before = clock();
    InsertionSort(arr, SIZE);
    after = clock();

    printf("InsertionSort **worst case** took %lu clock tics\n\n", (after - before));

    VERIFY(IsSorted(arr, SIZE));

    CHECK_SUCCESS( "INSERTION_SORT");

}

void TestSelectionSort()
{
    int flag = 0;
    clock_t before = {0};
    clock_t after = {0};

    int arr[SIZE] = {0};
    size_t i = 0;
    for (; i < SIZE; ++i)
    {
        arr[i] = rand();
    }

    printf("\n\n\n*******SelectionSort---Random*******\n\n");
    before = clock();
    SelectionSort(arr, SIZE);
    after = clock();

    printf("SelectionSort -- Random-- took %lu clock tics\n\n", (after - before));

    VERIFY(IsSorted(arr, SIZE));

    printf("*******SelectionSort---sorted backwords (worst case)*******\n\n");

    for (i = 0; i < SIZE; ++i)
    {
        arr[i] = SIZE - i;
    }

    before = clock();
    SelectionSort(arr, SIZE);
    after = clock();

    printf("SelectionSort **worst case** took %lu clock tics\n\n", (after - before));

    VERIFY(IsSorted(arr, SIZE));

    CHECK_SUCCESS( SELECTION_SORT);

}

void Testqsort()
{
    int flag = 0;
    clock_t before = {0};
    clock_t after = {0};

    int *arr = malloc(SIZE*4);
    size_t i = 0;
    for (; i < SIZE; ++i)
    {
        arr[i] = rand();
    }

    printf("\n\n\n*******qsort---Random*******\n\n");
    before = clock();
    qsort(arr, SIZE, 4,Compare);
    after = clock();

    printf("qsort -- Random-- took %lu clock tics\n\n", (after - before));

    VERIFY(IsSorted(arr, SIZE));

    printf("*******qsort---sorted backwords (worst case)*******\n\n");

    for (i = 0; i < SIZE; ++i)
    {
        arr[i] = SIZE -1 - i;
    }

    before = clock();
    qsort(arr, SIZE, 4,Compare);
    after = clock();

    printf("qsort **worst case** took %lu clock tics\n\n", (after - before));

    VERIFY(IsSorted(arr, SIZE));

    CHECK_SUCCESS( "QSORT");
    free(arr);
}

void TestCountingSort()
{
    int flag = 0;
    clock_t before = {0};
    clock_t after = {0};

    int arr[SIZE] = {0};
    size_t i = 0;
    for (; i < SIZE; ++i)
    {
        arr[i] = (rand() % (39999999999)) - (3948340983 / 2);
    }

    printf("\n\n\n*******CountingSort---Random*******\n\n");
    before = clock();
    CountingSort(arr, SIZE);
    after = clock();

    printf("CountingSort -- Random-- took %lu clock tics\n\n", (after - before));

    VERIFY(IsSorted(arr, SIZE));

    printf("*******CountingSort---sorted backwords (worst case)*******\n\n");

    for (i = 0; i < SIZE; ++i)
    {
        arr[i] = SIZE - i;
    }

    before = clock();
    CountingSort(arr, SIZE);
    after = clock();

    printf("CountingSort **worst case** took %lu clock tics\n\n", (after - before));

    VERIFY(IsSorted(arr, SIZE));

    CHECK_SUCCESS( "COUNTING_SORT");

}


void TestRadixSort(void)
{
    int flag = 0;
    clock_t before = {0};
    clock_t after = {0};

     int arr[SIZE] = {0};
    size_t i = 0;
    for (; i < SIZE; ++i)
    {
        arr[i] = ABS(rand());
    }

    printf("\n\n\n*******RadixSort---Random*******\n\n");
    before = clock();
    RadixSort(arr, SIZE);
    after = clock();

    printf("RadixSort -- Random-- took %lu clock tics\n\n", (after - before));

    VERIFY(IsSorted(arr, SIZE));

    printf("*******RadixSort---sorted backwords (worst case)*******\n\n");

    for (i = 0; i < SIZE; ++i)
    {
        arr[i] = SIZE - i;
    }

    before = clock();
    RadixSort(arr, SIZE);
    after = clock();

    printf("RadixSort **worst case** took %lu clock tics\n\n", (after - before));

    VERIFY(IsSorted(arr, SIZE));

    CHECK_SUCCESS( "RADIX_SORT");

}

void TestAVLTreeSort()
{
    int flag = 0;
    clock_t before = {0};
    clock_t after = {0};

    int *arr1 = malloc(4 * SIZE);
    int *arr2 = malloc(4 * SIZE);
    size_t i = 0;

    printf("*******AVL_TreeSort---sorted backwords (worst case)*******\n\n");

    for (i = 0; i < SIZE; ++i)
    {
        arr2[i] = rand() + 3;
    }

    before = clock();
    AVL_TreeSort(arr2, SIZE);
    after = clock();

    printf("AVL_TreeSort **worst case** took %lu clock tics\n\n", (after - before));

    VERIFY(IsSorted(arr2, SIZE));

    for (i = 0; i < SIZE; ++i)
    {
        arr1[i] = rand();
    }

    printf("\n\n\n*******AVL_TreeSort---Random*******\n\n");
    before = clock();
    AVL_TreeSort(arr1, SIZE);
    after = clock();

    printf("AVL_TreeSort -- Random-- took %lu clock tics\n\n", (after - before));

    VERIFY(IsSorted(arr1, SIZE));


    CHECK_SUCCESS( "AVL_TreeSort");

    free(arr1);
    free(arr2);
}

void TestBinarySearch(void)
{
    RESET_FLAG;
    int size = 20;
    int i = 0;
    int arr[20] = {-10,1,5,7,14,25,26,37,38,42,50,51,58,63,69,72,79,104,115,200};

    for (i = 0; i < size; ++i)
    {
        VERIFY(i == BinarySearchIterative(arr,size, arr[i]));
        VERIFY(i == BinarySearchRecursive(arr,size, arr[i]));
        
    }
        CHECK_SUCCESS(Both BinarySearches);
}

void TestMergeSort(void)
{
    RESET_FLAG;

    clock_t before = {0};
    clock_t after = {0};

    int arr[SIZE] = {0};
    size_t i = 0;

    for (; i < SIZE; ++i)
    {
        arr[i] = (rand());
    }

    printf("\n\n\n*******MergeSort---Random*******\n\n");
    before = clock();
    MergeSort(arr, SIZE);
    after = clock();

    printf("MergeSort -- Random-- took %lu clock tics\n\n", (after - before));

    VERIFY(IsSorted(arr, SIZE));

    printf("*******MergeSort---sorted backwords (worst case)*******\n\n");

    for (i = 0; i < SIZE; ++i)
    {
        arr[i] = SIZE - i;
    }

    before = clock();
    MergeSort(arr, SIZE);
    after = clock();

    printf("MergeSort **worst case** took %lu clock tics\n\n", (after - before));

    VERIFY(IsSorted(arr, SIZE));

    CHECK_SUCCESS( "MERGE_SORT");
}

void TestMyQuickSortVsGnuQsort(void)
{
    RESET_FLAG;
    clock_t before1 = {0};
    clock_t after1 = {0};

    clock_t before2 = {0};
    clock_t after2 = {0};
    int *arr1 = malloc(SIZE*4);
    int *arr2 = malloc(SIZE*4);
    size_t i = 0;
    for (; i < SIZE; ++i)
    {
        arr1[i] = rand() % SIZE;
        arr2[i] = rand() % SIZE;
    }

    printf("\n\n\n*******MyQuickSort---Random*******\n\n");
    before1 = clock();
    QuickSort(arr1, SIZE, 4,Compare);
    after1 = clock();

    before2 = clock();
    qsort(arr2, SIZE, 4,Compare);
    after2 = clock();
    printf("Relative difference between my QuickSort and GNU qsort is: %f\n\n", ((double)(after1 - before1) / (double)(after2 - before2)));

    VERIFY(IsSorted(arr1, SIZE));

    printf("*******MyQuickSort---sorted backwords (worst case)*******\n\n");

    for (i = 0; i < SIZE; ++i)
    {
        arr1[i] = SIZE - i;
        arr2[i] = SIZE - i;
    }

    before1 = clock();
    QuickSort(arr1, SIZE, 4,Compare);
    after1 = clock();

    before2 = clock();
    qsort(arr2, SIZE, 4,Compare);
    after2 = clock();
    printf("Relative difference between my QuickSort and GNU qsort is: %f\n\n", ((double)(after1 - before1) / (double)(after2 - before2)));

    VERIFY(IsSorted(arr1, SIZE));

    CHECK_SUCCESS( "MyQuickSort");
    free(arr1);
    free(arr2);
}


int main(void)
{
    /*Testqsort();*/
    /*TestCountingSort();
    TestRadixSort();

    printf("\n\n\n\nNOW HERE COME THE SLOW ALGO.............\n\n\n\n\n");
    /*TestBubbleSort();
    TestInsertionSort();
    TestSelectionSort();*/
    /*TestAVLTreeSort();*/

    /*TestMergeSort();*/

 TestBinarySearch();

    TestMyQuickSortVsGnuQsort();

    return 0;
}
