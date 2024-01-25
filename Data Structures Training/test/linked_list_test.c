#include <stdio.h>
#include "linked_list.h"

#define TEST(line, function, bool) TestCondition( (line), #function, (bool))

typedef struct sl_node
{
    void *data;
    struct sl_node *next;

} sl_node_t;


struct slist
{
    sl_node_t *head;
    sl_node_t *end;
};

void PrintList(sl_node_t *list)
{
    sl_node_t *curr = list;
    while (curr)
    {
        printf("%d -> ", *(int *)curr->data);
        curr = curr->next;
    }
    printf("%s\n\n", "NULL");
}

int VoidCompare(const void *p1, const void *p2)
{
    return p1 == p2;
}


void TestCondition( int line, char *func, int bool)
{
    if (!bool)
    {
        printf("The Test for %s on line %d FAILED!!!\n\n", func, line);
    }
}

void TestS_list()
{
    int a = 3, b = 5, i = 0;
    slist_t *list = SLCreate();
    sl_iter_t head = list->head, end = list->end, current1= NULL, current2 = NULL;

    TEST(__LINE__, SLCreate,list->head == list->end);
    TEST(__LINE__, SLEnd, SLEnd(list) == list->end);
    TEST(__LINE__, SLBegin, SLBegin(list) == list->head);
    TEST(__LINE__, SLCount, (0 == SLCount(list)));
    TEST(__LINE__, SLIsEmpty, SLIsEmpty(list));

    current1= SLAddBefore(end, &a);

    TEST(__LINE__, SLAddBefore,(head == SLBegin(list) && end != SLEnd(list)) && SLGetValue(current1) == (void *)&a);
    TEST(__LINE__, SLCount, (1 == SLCount(list)));
    TEST(__LINE__, SLIsEmpty, !SLIsEmpty(list));
    TEST(__LINE__, SLNext, SLNext(current1) == SLEnd(list));
    TEST(__LINE__, SLBegin, SLBegin(list) == current1);

    end = SLEnd(list);
    current2 = SLAddAfter(current1, &b);
    TEST(__LINE__, SLAddAfter, ((void *)&b == SLGetValue(current2) && (end != SLEnd(list))));
    TEST(__LINE__, SLGetValue, (SLGetValue(current1) == (void *)&a));
    TEST(__LINE__, SLCount, (2 == SLCount(list)));
    TEST(__LINE__, SLIsEmpty, !SLIsEmpty(list));

    SLSetValue(current1, (void *)&b);

    TEST(__LINE__, SLIterIsEqual, !SLIterIsEqual(current1,current2));
    TEST(__LINE__, SLSetValue, (void *)&b == SLGetValue(current1));

    SLSetValue(current1, (void *)&a);
    end = SLEnd(list);
    SLRemove(current2);

    TEST(__LINE__, SLRemove, current2 == SLEnd(list) && 1 == SLCount(list));

    SLRemove(SLBegin(list));

    for (i = 0; i < 100; ++i)
    {
        SLAddBefore(SLEnd(list), (void *)&a);
    }

    current1 = SLAddBefore(SLEnd(list), (void *)&b);
    current2 = SLFind(list, (void *)&b, VoidCompare);

    SLDestroy(list);

    list = SLCreate();
    current1 = SLAddBefore(SLEnd(list), (void *)&a);
    current2 = SLAddAfter(current1, (void *)&b);
    SLRemoveAfter(current1);

    TEST(__LINE__, SLRemoveAfter, SLEnd(list) == SLFind(list, (void *)&b, VoidCompare) );

    SLDestroy(list);
}

int AddParam(void *data, void *param)
{
    int status = (data != NULL ? 0 : 1);
    if (!status)
    {
        *(int *)data += *(int *)param;
    }
    return status;
}

void TestForEach()
{   int arr[10] = {0,1,2,3,4,5,6,7,8,9}, parameter = 5;
     int test_arr[10] = {5,6,7,8,9,10,11,12,13,14};
    void *param = (void *)&parameter;
    int i = 0;
    slist_t *list = SLCreate();
    for (i = 0; i < 10; ++i)
    {
        SLAddBefore(SLEnd(list), (void *)(arr + i));
    }
    printf("HEREWE ARE");
    PrintList(SLBegin(list));
    SLForEach(SLBegin(list),SLEnd(list), AddParam, param);
    PrintList(SLBegin(list));
    for (i = 0; i < 10; ++i)
    {
        TEST(__LINE__, SLForEach, (test_arr[i] == (arr[i] )));
    }

    SLDestroy(list);

}

void TestAppend()
{
    size_t i = 0;
    int arr[10] = {1,2,3,4,5,6,7,8,9,10};
    slist_t *base = SLCreate(), *append = SLCreate();

    for (i = 0; i < 10; ++i)
    {
        SLAddBefore(SLEnd(base), (arr + i));
        SLAddBefore(SLEnd(append), (arr + (9 - i)));
    }

    PrintList(SLBegin(base));
    PrintList(SLBegin(append));

    SLAppend(base,append);
    PrintList(SLBegin(base));
    PrintList(SLBegin(append));

    SLDestroy(base); base = NULL;
    SLDestroy(append); append = NULL;

}


int main()
{
    slist_t *list = SLCreate();
    SLDestroy(list);
    TestForEach();
    TestS_list();
    TestAppend();
    printf("\n\nIF YOU GOT NO ERRORS THEN ALL TEST WERE PASSED.\n BUT MAKE SURE TO TRY VALGRIND!\n\n" );

    return 0;
}
