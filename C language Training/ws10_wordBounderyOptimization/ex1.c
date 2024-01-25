#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include <string.h>
#include "lib_memfuncs.h"

void TestMemMove(char *alloc, size_t offset_src,size_t offset_dest,size_t n)
{
    void *src = NULL;
    void *dest = NULL;
    while((size_t)alloc % sizeof(size_t))
    {
        ++alloc;
    }
    src = (void *)(alloc + offset_src);
    dest = (void *)(alloc + offset_dest);
    assert(src);
    assert(dest);

    if (0 == memcmp(MemMove(dest, src, n), memmove(dest, src, n), n))
    {
        printf("The MemMove function passed\n");
    }
    else
    {
        printf("SORRY, MemMove failed\n");
    }
}

void TestMemCpy(char *alloc, size_t offset_src,size_t offset_dest,size_t n)
{
    void *src = NULL;
    void *dest = NULL;
    while((size_t)alloc % sizeof(size_t))
    {
        ++alloc;
    }
    src = (void *)(alloc + offset_src);
    dest = (void *)(alloc + offset_dest);

    assert(src);
    assert(dest);

    if (0 == memcmp(MemCpy(dest, src, n), memcpy(dest, src, n), n))
    {
        printf("The MemCpy function passed\n");
    }
    else
    {
        printf("SORRY, MemCpy failed\n");
    }
}

void TestMemSet(char *alloc, size_t offset,int c, size_t n)
{

    void *dest = NULL;
    while((size_t)alloc % sizeof(size_t))
    {
        ++alloc;
    }

    dest = (void *)(alloc + offset);

    assert(alloc);


    if (0 == memcmp(MemSet(dest, c, n), memset(dest, c, n), n))
    {
        printf("The MemSet function passed\n");
    }
    else
    {
        printf("SORRY, MemSet failed\n");
    }
}
int main()
{
    
    char *newarr = ( char *)malloc(300);
     strcpy(newarr,"012345678901234567890");
     TestMemCpy(newarr,3,7,6);
     TestMemMove(newarr,4,7,34);
     TestMemSet(newarr,6,'A',34);

    return 0;
}
