
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "ws11_funcs.h"
#define CHARS 255


/*APROVED BY FANIA*/

void PrintLetFirst2Arr(char arr1[], size_t arr1_size, char arr2[], size_t arr2_size, char blocker[], size_t arr3_size)
{
    size_t i = 0;

    int *alight = (int *)calloc(sizeof(*alight),CHARS);
    if (!alight)
    {
        return;
    }
    assert(arr1 && arr2 && blocker);

    for (i = 0; i < arr1_size; ++i)
    {
        alight[(int)arr1[i]] = 1;

    }
    for (i = 0; i < arr2_size; ++i)
    {
        alight[(int)arr2[i]] += 2;

    }
    for (i = 0; i < arr3_size; ++i)
    {
        alight[(int)blocker[i]] = 0;

    }
    printf("The resulting chars are: ");
    for (i = 0; i < CHARS; ++i)
    {
        if (isalpha(i ) && (int)alight[i] % 2)
        {
            printf("%c ", (unsigned char)(i ));
       }
    }

    free(alight); alight = NULL;
    printf("\n");

}
