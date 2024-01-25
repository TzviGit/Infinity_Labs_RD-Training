/******************************************************************************
* Author        Tzvi Moskowitz
* Reviewer      --------
* Description   Tester for AVL.c
* Date          October 19, 2021
* Group         OL110
* Company       ILRD Ramat Gan
* ****************************************************************************/

#include <stdlib.h>
#include "avl_tree.h" /* API FOR AVL */
#include "test_util.h" /* HEADER WHICH CONTAINS UTIL FUNCS FOR THIS FILE  */

typedef struct avl_node avl_node_t;
typedef int (*action_t)(void *data, void *param);


int MyCompare(const void *d1, const void *d2)
{
    return *(int *)d1 - *(int *)d2;
}

int PointerAsLongCompare(const void *d1, const void *d2)
{
    return (long)d1 - (long)d2;
}

int PutIntoArray(void *data, void *param);
void PrintTree(avl_t *node);
int PrintNode(void *data, void *param);
int IsBalanced(avl_node_t *node);
long RecSubTreeHeight(avl_node_t *node);
int VerifyHeightAndBalance(void *data, void *param);

int ForEachNodeOfTree(avl_t *avl, action_t action, void *param);

int SortIntArray(int *array, size_t size)
{
    avl_t *avl_tree = AVLCreate(PointerAsLongCompare);


    size_t i = 0;

    if (NULL == avl_tree)
    {
        return 1;
    }


    for (i = 0; i < size; ++i)
    {

        AVLInsert(avl_tree, (void *)(long)(int)array[i]);
    }


    ForEachNodeOfTree(avl_tree, PutIntoArray, (void *)array);
    printf("\n\n");

    /*printf("\n\n");
    ForEachNodeOfTree(avl_tree, PrintNode, (void *)ptr, IN_ORDER);
    printf("\n\n");*/
    AVLDestroy(avl_tree);


    return 0;
}
 avl_node_t *GetMaxSubTree(avl_node_t *root);




void TestAVLBasicFunctions(void)
{
    RESET_FLAG;

    avl_t *avl = AVLCreate(MyCompare);

    VERIFY(0 == AVLSize(avl));
    VERIFY(AVLIsEmpty(avl));

    AVLDestroy(avl);
    avl = NULL;

    CHECK_SUCCESS(AVLBasicFunctions);

}

void TestAVLInsertAndFind(void)
{
    RESET_FLAG;
    size_t max = 10, i = 0;
    int *arr = malloc(4 * max);
    int buf[] = {9,1,3,2,4,5,7,6,8,0,11,19,13,17,18,12,15,14,16,10};
    avl_t *avl = AVLCreate(MyCompare);

    VERIFY(NULL == AVLFind(avl, (void *)78787));

    for (i = 0; i < max; ++i)
    {   arr[i] = buf[i];

        AVLInsert(avl, arr + i);
    }



    VERIFY(max == AVLSize(avl));
    VERIFY(!AVLIsEmpty(avl));

    ForEachNodeOfTree(avl, PrintNode, NULL);

    for (i =0; i < 10; ++i)
    {
        VERIFY((arr + i)== AVLFind(avl, arr + i));
    }

    free(arr); arr = NULL;

    AVLDestroy(avl);
    avl = NULL;

    CHECK_SUCCESS(AVLBasicFunctions);
}

void TestAVLHeight(void)
{
    RESET_FLAG;
    size_t max = 50, i = 0;
    int *arr = malloc(4 * max);

    avl_t *avl = AVLCreate(MyCompare);
    VERIFY(-1 == AVLHeight(avl));


    for (i = 0; i < max; ++i)
    {
        arr[i] = i;

        AVLInsert(avl, arr + i);


    }

    /*ForEachNodeOfTree(avl, PrintNode, NULL, REVERSE_ORDER);*/
    free(arr); arr = NULL;

    AVLDestroy(avl);
    avl = NULL;

    CHECK_SUCCESS(AVLHeight);

}
int ABS(int num)
{
    return num < 0? -num : num;
}
void TestAVLRemove(void)
{
    RESET_FLAG;
    long max = 800, i = 0;
    int *arr = malloc(4 * max);

    avl_t *avl = AVLCreate(MyCompare);
    int count = max;

    for (i = 0; i < max; ++i)
    {   arr[i] = rand();

        AVLInsert(avl, arr + i);

        /*ForEachNodeOfTree(avl, VerifyHeightAndBalance, NULL, REVERSE_ORDER);*/
    }


VERIFY(count == (long)AVLSize(avl));
    while (!AVLIsEmpty(avl))
    {
        AVLRemove(avl, arr + ABS((rand() % (max ))) );

      ForEachNodeOfTree(avl, VerifyHeightAndBalance, NULL);
        VERIFY(NULL == AVLFind(avl, arr + i));
        VERIFY(NULL == AVLRemove(avl, arr + i));
    }

    free(arr); arr = NULL;

    AVLDestroy(avl);
    avl = NULL;

    CHECK_SUCCESS(AVLRemove);


}

void TestRemove2Children()
{
    RESET_FLAG;
	avl_t *avl = AVLCreate(MyCompare);
	int x12 = 12, x7 = 7, x20 = 20, x3 = 3, x11 = 11, x5 = 5, x6 = 6, x30 = 30, x25 = 25, x8 = 8, x18 = 18, x16 = 16, x13 = 13, x10 = 10;

	printf("\nRemove Test- two children\n");

	AVLInsert(avl, &x12);
	AVLInsert(avl, &x20);
	AVLInsert(avl, &x13);
	AVLInsert(avl, &x16);
	AVLInsert(avl, &x7);
	AVLInsert(avl, &x3);
	AVLInsert(avl, &x5);
	AVLInsert(avl, &x6);
	AVLInsert(avl, &x30);
	AVLInsert(avl, &x25);
	AVLInsert(avl, &x11);
	AVLInsert(avl, &x10);
	AVLInsert(avl, &x18);
	AVLInsert(avl, &x8);



printf("\nRemove with next which is not a right child\n");
	AVLRemove(avl, &x7);
	VERIFY(AVLSize(avl) ==  13);ForEachNodeOfTree(avl, VerifyHeightAndBalance, NULL);
	PrintTree(avl);

		AVLRemove(avl, &x8);
	VERIFY(AVLSize(avl) ==  12);ForEachNodeOfTree(avl, VerifyHeightAndBalance, NULL);
	PrintTree(avl);

	AVLRemove(avl, &x12);
	VERIFY(AVLSize(avl) ==  11);ForEachNodeOfTree(avl, VerifyHeightAndBalance, NULL);
	PrintTree(avl);

	AVLRemove(avl, &x13);
	VERIFY(AVLSize(avl) ==  10);ForEachNodeOfTree(avl, VerifyHeightAndBalance, NULL);
    PrintTree(avl);

	AVLDestroy(avl);
    CHECK_SUCCESS(newtests);
}
void TestRemove2Children1()
{
    RESET_FLAG;
	avl_t *avl = AVLCreate(MyCompare);
	int x12 = 12, x7 = 7, x20 = 20, x3 = 3, x11 = 11, x5 = 5, x6 = 6, x30 = 30, x25 = 25;

	printf("\nRemove Test- two children\n");

	AVLInsert(avl, &x12);
    ForEachNodeOfTree(avl, VerifyHeightAndBalance, NULL);

	AVLInsert(avl, &x20);ForEachNodeOfTree(avl, VerifyHeightAndBalance, NULL);

	AVLInsert(avl, &x7);ForEachNodeOfTree(avl, VerifyHeightAndBalance, NULL);

	AVLInsert(avl, &x3);ForEachNodeOfTree(avl, VerifyHeightAndBalance, NULL);

	AVLInsert(avl, &x5);ForEachNodeOfTree(avl, VerifyHeightAndBalance, NULL);

	AVLInsert(avl, &x6);ForEachNodeOfTree(avl, VerifyHeightAndBalance, NULL);

	AVLInsert(avl, &x30);ForEachNodeOfTree(avl, VerifyHeightAndBalance, NULL);

	AVLInsert(avl, &x25);ForEachNodeOfTree(avl, VerifyHeightAndBalance, NULL);

	AVLInsert(avl, &x11);ForEachNodeOfTree(avl, VerifyHeightAndBalance, NULL);


ForEachNodeOfTree(avl, VerifyHeightAndBalance, NULL);
printf("\nRemove with next as right child\n");
	AVLRemove(avl, &x12);
	VERIFY(AVLSize(avl) ==  8);
	PrintTree(avl);
	PrintTree(avl);
ForEachNodeOfTree(avl, VerifyHeightAndBalance, NULL);
	AVLRemove(avl, &x7);
	VERIFY(AVLSize(avl) ==  7);ForEachNodeOfTree(avl, VerifyHeightAndBalance, NULL);
	PrintTree(avl);

	AVLDestroy(avl);
    CHECK_SUCCESS(shani_tests);
}
void TestSortArray(void)
{
    RESET_FLAG;
    long max = 100, i = 0;

    int *arr = malloc(4 * max);

    for (i = 0; i < max; ++i)
    {   arr[i] = max - i;

    }


    SortIntArray(arr,max);
    for (i = 1; i < max; ++i)
    {
        VERIFY(arr[i] > arr[i - 1]);
    }
    CHECK_SUCCESS(SortIng);

    free(arr);
}

int main(void)
{
    TestAVLInsertAndFind();
    TestAVLBasicFunctions();
    TestAVLHeight();
    TestAVLRemove();
    TestRemove2Children();
    TestRemove2Children1();
    /*TestSortArray();*/
    return 0;
}
