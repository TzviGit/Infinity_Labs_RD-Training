#include <stdio.h> /*printf*/
#include "dynamic_vector.h"


char fail[8] = "FAILED";
char pass[8] = "PASSED";

void PrintStatus(char *function,int boolean)
{
    char *status = (boolean)? pass : fail;
    printf("%s has %s the testing!\n\n",function, status);
}

void TestVectorFuncs()
{
    size_t TEST_CAPAC = 3, i = 0;
    int a = 65, b = 25, c = -343, d = 99;
    double db = 5.55;
    int flag_Create = 0, flag_capacity = 1, flag_size = 1, flag_Get = 1, flag_Set = 1, flag_push = 1, flag_pop = 1, flag_reserve = 1, flag_Shrink = 1;
    vector_t * vector = DVectorCreate(TEST_CAPAC);

    flag_capacity *= (DVectorGetCapacity(vector) == TEST_CAPAC);
    flag_size *= (0 == DVectorGetSize(vector));
    DVectorPushBack(vector, &a);
    DVectorPushBack(vector, &b);
    DVectorPushBack(vector, &c);
    DVectorPushBack(vector, &d);
    DVectorPushBack(vector, &c);


    flag_Create = flag_capacity *= (DVectorGetCapacity(vector) == 2 * TEST_CAPAC);
    flag_size *= (5 == DVectorGetSize(vector));
    flag_Get *= (a == *(int *)DVectorGet(vector, 0)) && (c == *(int *)DVectorGet(vector, DVectorGetSize(vector) - 1));
    flag_push *= (a == *(int *)DVectorGet(vector, 0)) && (c == *(int *)DVectorGet(vector, DVectorGetSize(vector) - 1));
    flag_pop *= (c == *(int *)DVectorPopBack(vector)) && (4 == DVectorGetSize(vector));
    DVectorSet(vector, (DVectorGetSize(vector) -1), &db);
    flag_Set *= (db == *(double *)DVectorPopBack(vector));

    DVectorReserve(vector,2);
    flag_reserve *= (DVectorGetSize(vector) == DVectorGetCapacity(vector)) && (b == *(int *)DVectorGet(vector, DVectorGetCapacity(vector) - 1));
    for (i = 1; i < 10000; ++i)
    {
        DVectorPushBack(vector,&i);
    }
    DVectorPushBack(vector,&a);
    DVectorShrinkToFit(vector);
    flag_Shrink *= (DVectorGetSize(vector) == DVectorGetCapacity(vector)) && (*(int *)DVectorPopBack(vector) == a);

    DVectorDestroy(vector); /*check valgrind*/

    PrintStatus("DVectorCreate", flag_Create);
    PrintStatus("DVectorGetCapacity", flag_capacity);
    PrintStatus("DVectorGetSize", flag_size);
    PrintStatus("DVectorGet", flag_Get);
    PrintStatus("DVectorSet", flag_Set);
    PrintStatus("DVectorPushBack", flag_push);
    PrintStatus("DVectorPopBack", flag_pop);
    PrintStatus("DVectorReserve", flag_reserve);
    PrintStatus("DVectorShrinkToFit", flag_Shrink);



}

int main()
{
    TestVectorFuncs();
    return 0;
}
