#include <stdio.h>  /*printf*/
#include <stdlib.h> /*malloc*/
#include "fsa.h"

#define VERIFY(bool) TestCondition( __LINE__,  (bool))

void TestCondition( int line, int bool)
{
    if (!bool)
    {
        printf("The Test on line %d FAILED!!!\n\n", line);
    }
}


int main()
{
    void *mem_pool = NULL;
    size_t pool_size = FSASuggestSize(3, 50);

    fsa_t fsa = NULL;
    void *block1 = NULL, *block2 = NULL, *block3 = NULL, *block4 = NULL;
    printf("POOLSIZE IS:  %lu\n\n",pool_size);

    VERIFY(176 == pool_size);
    mem_pool = malloc(pool_size);
    fsa = FSAInit(mem_pool, pool_size, 50);
    block1 = FSAAlloc(fsa);

    VERIFY(2 == FSACountFree(fsa));
    block2 = FSAAlloc(fsa);

    VERIFY(1 == FSACountFree(fsa));
    block3 = FSAAlloc(fsa);

    VERIFY(0 == FSACountFree(fsa));
    block4 = FSAAlloc(fsa);
    VERIFY(0 == FSACountFree(fsa));
    VERIFY(!block4);

    FSAFree(fsa, block1);

    block4 = FSAAlloc(fsa);
    VERIFY(block4 == block1);





    free(mem_pool);
    mem_pool = NULL;
    return 0;
}
