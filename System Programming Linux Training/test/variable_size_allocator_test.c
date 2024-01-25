
/******************************************************************************
* Author        Tzvi Moskowitz
* Reviewer      Fania Volkof
* Description   varying size allocator (VSA) TESTER
* Group         OL110
* Company       ILRD Ramat Gan
* ****************************************************************************/


#include <stdio.h>  /*printf*/
#include <stdlib.h> /*malloc*/
#include "vsa.h"

#define IMP 0

#define VERIFY(bool) TestCondition( __LINE__,  (bool))

void TestCondition( int line, int bool)
{
    if (!bool)
    {
        printf("The Test on line %d FAILED!!!\n\n", line);
    }
}

void TestAllocAndFreeSTAGE_ONE(void);
void TestAllocAndFreeSTAGE_TWO(void);
void TestVSAInit(void);


int main (void)
{
    TestVSAInit();
    TestAllocAndFreeSTAGE_ONE();
    TestAllocAndFreeSTAGE_TWO();
    return 0;
}


void TestVSAInit(void)
{
    void *pool1 = malloc(109), *pool2 = malloc(23 + IMP), *pool3 = malloc(31 + IMP);

    vsa_t *vsa1 = VSAInit(pool1, 109), *vsa2 = VSAInit(pool2, 23 + IMP),
                                        *vsa3 = VSAInit(pool3, 31 + IMP);

    VERIFY(VSALargestBlockAvailable(vsa1) == 80 - IMP);

    VERIFY(!vsa2);
    VERIFY(0 == VSALargestBlockAvailable(vsa3));

    free(pool1); free(pool2); free(pool3);

}


void TestAllocAndFreeSTAGE_ONE(void)
{
        void *pool =malloc(56 + IMP), *alloc_ptr = NULL;
        vsa_t *vsa = VSAInit(pool, 56 + IMP);
        size_t count = 0;

        alloc_ptr = VSAAlloc(vsa, 8);
        while (alloc_ptr)
        {
            ++count;
            alloc_ptr = VSAAlloc(vsa, 8);
        }

        VERIFY(count == 2);
        free(pool);
}


void TestAllocAndFreeSTAGE_TWO(void)
{
    void *pool = malloc(147 + IMP);
    vsa_t *vsa = VSAInit(pool, 147 + IMP);

    void *b1 = NULL, *b2 = NULL,*b3 = NULL;

    VERIFY(VSALargestBlockAvailable(vsa) == 120);

    b1 = VSAAlloc(vsa, 53);
    VERIFY(VSALargestBlockAvailable(vsa) == 48);
    b2 = VSAAlloc(vsa, 45);
    VERIFY(VSALargestBlockAvailable(vsa) == 0);

    VSAFree(b1);
    VERIFY(VSALargestBlockAvailable(vsa) == 56);

    VSAAlloc(vsa, 10);
    VERIFY(!VSAAlloc(vsa, 25));

    b3 = VSAAlloc(vsa, 24);
    VERIFY(VSALargestBlockAvailable(vsa) == 0);

    VSAFree(b1); VSAFree(b2); VSAFree(b3);
    free(pool);
}
