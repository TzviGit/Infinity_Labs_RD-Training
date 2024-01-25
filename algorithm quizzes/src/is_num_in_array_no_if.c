#include "test_util.h"

int ArrayHasNum(int *arr, size_t size, int num)
{
    int result = 1;
    size_t i = 0;

    for (i = 0; i < size; ++i)
    {
        result *= ((arr[i] - num) != 0);
    }

    return 0 == result;
}


int main (void)
{
    RESET_FLAG;

    int arr[] = {3,5,4,6,6,4,6,6,78,32,6,21,9,8,43,235,2453245,3243,345,948};
    VERIFY(ArrayHasNum(arr, 20, 6));
    VERIFY(!ArrayHasNum(arr, 20 , 999));
    VERIFY(ArrayHasNum(arr,20, 3243));

    CHECK_SUCCESS(ArrayHasNum);

    return 0;
}
