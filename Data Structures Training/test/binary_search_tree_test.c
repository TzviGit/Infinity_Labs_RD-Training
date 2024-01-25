/******************************************************************************
* Author        Tzvi Moskowitz
* Reviewer      --------
* Description   Tester for recursive.c
* Date          October 19, 2021
* Group         OL110
* Company       ILRD Ramat Gan
* ****************************************************************************/

#include <stdio.h> /*printf*/
#include <stdlib.h> /*rand()*/
#include "binary_search_tree.h" /* API FOR BST */
#include "test_util.h" /* HEADER WHICH CONTAINS UTIL FUNCS FOR THIS FILE  */

int d = 10, e =15, c = 5, a = 11, b = 31, f = 3, g = 19, h = 8;
int PrintNode(void *data, void *param)
{
    (void)param;
    printf(_CYAN_"%d"_GREEN_ "->" _WHITE_, *(int *)data);
    return 0;
}
int IsBefore(const void *data1, const void *data2, void *param)
{
    (void)param;
    return (*(int *)data2 > *(int *)data1);
}


void TestUnit1()
{
    int flag = 0;
    bst_t *tree = BSTCreate(IsBefore, &d);

    VERIFY(BSTIsEmpty(tree));
    VERIFY(BSTSize(tree) == 0);

    BSTDestroy(tree);

    CHECK_SUCCESS( BST_CREATE);
}

void TestUnit2(void)
{
    int flag = 0;
    bst_t *tree = BSTCreate(IsBefore, &d);
    size_t i = 0;
    int arr[10000] = {0};
    bst_iter_t iter = NULL;
    BSTInsert(tree, &d);
    VERIFY(BSTSize(tree) == 1);

    BSTInsert(tree, &e);
    VERIFY(BSTSize(tree) == 2);


    BSTInsert(tree, &a);
    BSTInsert(tree, &b);
    BSTInsert(tree, &f);
    BSTInsert(tree, &c);
    BSTInsert(tree, &g);
    BSTInsert(tree, &h);
    VERIFY(BSTSize(tree) == 8);

    /*BSTForEach( BSTBegin(tree),BSTEnd(tree), PrintNode, NULL);*/

    VERIFY(!BSTIsEmpty(tree));

    iter = BSTFind(tree, &f);
    VERIFY(BSTIterIsEqual(iter, BSTBegin(tree)));
    VERIFY(BSTGetData(iter) == &f);

    iter = BSTFind(tree, &b);
    VERIFY(!!iter);
    VERIFY(BSTGetData(iter) == &b);
    VERIFY(BSTIterIsEqual(BSTEnd(tree), BSTNext(iter)));

    VERIFY(BSTGetData(BSTPrev(BSTEnd(tree))) == &b);

    for (; i < 10000; ++i)
    {
        arr[i] = (rand() % 10) - 10000;
    }

    for (i = 0; i < 10000; ++i)
    {
        BSTInsert(tree, arr + i);
    }

    VERIFY(BSTSize(tree) == 10008);



    iter = BSTFind(tree, &e);
    VERIFY(!!iter);

    BSTRemove(iter);

    iter  = BSTFind(tree, &e);
    VERIFY(BSTIterIsEqual(iter, BSTEnd(tree)));
/*    BSTForEach( BSTBegin(tree),BSTEnd(tree), PrintNode, NULL);*/

    BSTDestroy(tree);

    CHECK_SUCCESS(UNI2_2);

}


int main(void)
{



TestUnit1();
TestUnit2();


    return 0;
}
