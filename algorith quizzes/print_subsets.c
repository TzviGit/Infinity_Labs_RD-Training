#include <stdio.h>
#include <math.h>

static void PrintArray(int *arr, size_t end_index);
static void PrintSubsetsImple(int *arr, size_t size);


void PrintSubsets(int *arr, size_t size)
{
    printf("{0} -- the NULL set \n");

    PrintSubsetsImple(arr, size);
}

static void PrintSubsetsImple(int *arr, size_t size)
{
    long i = size - 1;

    if (0 == size)
    {
        return;
    }

    for (i = size - 1; i >= 0; --i)
    {
        PrintArray(arr, i);
    }

    PrintSubsetsImple(arr + 1, size - 1);
}

static void PrintArray(int *arr, size_t end_index)
{
    size_t i = 0;

    for (i = 0; i <= end_index; ++i)
    {
        if (0 == i)
        {
            printf("{");
        }
        printf("%d", arr[i]);

        printf("%c", end_index == i ? '}' : ',' );
    }

    printf("\n");
}

void PrintSubsetsBitwise(int *arr, size_t size)
{
    size_t combos = pow(2, size);
    size_t i, j;

    for (i = 0; i < combos; ++i)
    {
        for (j = 0; j < size; ++j)
        {


            if (1 == ((i >> j) & (size_t)1))
            {
                printf("%d", arr[size- 1 -j]);
            }
            
        }

        printf("\n");
    }
}

int main()
{
    int arr[3] = {1,2,3};

    PrintSubsetsBitwise(arr, 3);
    return 0;
}
