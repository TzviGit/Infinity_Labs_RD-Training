#include <assert.h>
#include <stdlib.h>
#include "test_util.h"
#include "recursion_training.h"
#include <time.h>

static void FreeAndNullify(void **pointer);

void PrintList(node_t *head);
void PrintListBackwords(node_t *head);
static node_t *CreateList(size_t n_nodes);

void TestFibonacci(void)
{
    int i = 0;


    printf(_CYAN_"\n\nHERES FOR"_GREEN_ " ITERATIVE"_CYAN_" METHOD\n\n"_WHITE_);
    for (i = 0; i < 10; ++i)
    {
        printf("%d -> ", FibonacciIter(i));

    }

    printf(_CYAN_"\n\nHERES FOR"_GREEN_ " Recursive"_CYAN_" METHOD\n\n"_WHITE_);
    for (i = 0; i < 10; ++i)
    {
        printf("%d -> ", FibonacciRec(i));

    }

}

void TestReverseList(void)
{
    node_t *list = CreateList(10);
    node_t *reverse_list = NULL;
    PrintList(list);
    printf("NULL\n\n" );
    PrintListBackwords(list);
    printf("NULL\n\n" );

    reverse_list = FlipList(list);
    printf("%d\n\n",(!!list));
    PrintList(reverse_list);
    printf("NULL\n\n" );
    PrintListBackwords(reverse_list);
    printf("NULL\n\n" );

    FreeAndNullify((void **)&list);

}

void TestStrlen(void)
{
    RESET_FLAG;
    char arr1[] = "12345678901234567890";
    char arr2[] = "";
    VERIFY(StrLen(arr1) == 20);
    VERIFY(StrLen(arr2) == 0);
    CHECK_SUCCESS(Strlen);
}

void TestStrCmp(void)
{
    int flag = 0;

    VERIFY(StrCmp("abcde", "abcdef") < 0);

    CHECK_SUCCESS(StrCmp);
}

void TestStrCpy(void)
{
    RESET_FLAG;
    char dest[40] = "hello";
    char src1[] = "hello world!!\0\0\0";
    char src2[] = "";
    StrCpy(dest, src1);
    VERIFY(StrCmp(dest, src1) == 0);

    StrCpy(dest, src2);
    VERIFY(StrCmp(dest, src2) == 0);

    CHECK_SUCCESS(StrCpy);
}

void TestStrCat(void)
{
    RESET_FLAG;
    char dest[40] = "hello";
    char src1[] = " world";
    char src2[] = "";
    StrCat(dest, src1);
    VERIFY(StrCmp(dest, "hello world") == 0);

    StrCpy(dest,"hello");
    StrCat(dest, src2);

    VERIFY(StrCmp(dest, "hello") == 0);

    CHECK_SUCCESS(StrCpy);
}

void TestStrStr(void)
{
    RESET_FLAG;
    const char haystack[] = "googloogloogoogleloogl";
    const char needle[] = "google";
    char *found = NULL;

    found = StrStr(haystack, needle);
    VERIFY(0 == StrCmp(found, "googleloogl"));

    CHECK_SUCCESS(StrStr);
}


void TestSortIntStack(void)
{
    size_t max = 500;
    clock_t before = {0};
    clock_t after = {0};
    RESET_FLAG;
    void *top = NULL, *next = NULL;
    size_t i = 0;
    stack_t *stack = StackCreate(max);


    for (i = 0; i < max; ++i)
    {
        StackPush(stack, (void *)(long)((rand() % 1000) - 500));
    }

    before = clock();
    SortIntStack(stack);
    after = clock();

    printf("My STACK SORT- took %lu clock tics\n\n", (after - before));


    top = StackPeek(stack);
    StackPop(stack);
    while (!StackIsEmpty(stack))
    {
        next = StackPeek(stack);

        VERIFY((long)top >= (long)next);

        top = next;
        StackPop(stack);
    }



    CHECK_SUCCESS(SortIntStack);

    StackDestroy(stack); stack = NULL;

}

int main()
{
    TestFibonacci();
    TestReverseList();
    TestStrlen();
    TestStrCmp();
    TestStrCpy();
    TestStrCat();
    TestStrStr();
    TestSortIntStack();

    return 0;
}


/***************************************************************************
*       UTILITY FUNC: INITIALIZE List with given length, setting their
*       respective data fiels to their placement in the list .
**************************************************************************/
static node_t *CreateList(size_t n_nodes)
{
    size_t i = 0;
    node_t *pool = (node_t *)malloc(n_nodes * sizeof(*pool));
    node_t *init_ptr = pool;
    if (!pool)
    {
        return NULL;
    }

    for (i = 1; i < n_nodes; ++i)
    {
        init_ptr->data = i;
        init_ptr->next = (init_ptr + 1);
        ++init_ptr;
    }

    init_ptr->data = i;
    init_ptr->next = NULL;

    return pool;

}

void PrintList(node_t *head)
{
    if (head == NULL)
    {
        return;
    }

    printf("%lu -> ", (size_t)head->data);

    PrintList(head->next);

}

void PrintListBackwords(node_t *head)
{
    if (head == NULL)
    {
        return;
    }

    PrintListBackwords(head->next);

    printf("%lu -> ", (size_t)head->data);

}


static void FreeAndNullify(void **pointer)
{
    assert(pointer);

    free((void *)(*(size_t **)pointer));
     *(size_t **)pointer = NULL;

    pointer = NULL;
}
