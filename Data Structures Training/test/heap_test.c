
#include "heap.h"
#include "test_util.h"

#define MAX (7)


int Compare(const void *p1, const void *p2)
{
    return *(int *)p1 - *(int *)p2;
}

int IsSame(const void *p1, const void *p2)
{
    return *(int *)p1 == *(int *)p2;
}

int PrintVector(heap_t *heap);
int IsMaxHeap(heap_t *heap);

void TestHeap(void)
{
    RESET_FLAG;

    size_t i = 0;
    int arr[MAX] = {0};
    heap_t *heap = HeapCreate(Compare);

    VERIFY(HeapIsEmpty(heap));
    VERIFY(0 == HeapSize(heap));

    for (;i < MAX; ++i)
    {
        arr[i] = (i + 1);
        HeapPush(heap, (arr + i));
        VERIFY(IsMaxHeap(heap));
        VERIFY(((int)i + 1) == *(int *)HeapPeek(heap));
    }

    VERIFY(!HeapIsEmpty(heap));
    VERIFY(MAX == HeapSize(heap));


    VERIFY(IsMaxHeap(heap));
    for (i = 0;i < MAX -1; ++i)
    {

        HeapRemoveIf(heap, arr + i, IsSame);
        PrintVector(heap);
        VERIFY((MAX) == *(int *)HeapPeek(heap));
       VERIFY(IsMaxHeap(heap));


    }

    HeapDestroy(heap);
    heap = NULL;

    CHECK_SUCCESS(Heap_FUNCS_BASIC);
}
int main(void)
{

    TestHeap();
    return 0;
}
