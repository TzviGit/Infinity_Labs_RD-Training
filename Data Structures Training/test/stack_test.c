#include <stdio.h> /*printf*/
#include "stack.h"
#include <assert.h>



char fail[8] = "FAILED";
char pass[8] = "PASSED";

void PrintStatus(char *function,int boolean)
{
    char *status = (boolean)? pass : fail;
    printf("%s has %s the testing!\n\n",function, status);
}

void TeststackFuncs()
{
    size_t TEST_CAPAC = 5;
    int a = 65, b = 25, c = -343, d = 99;

    int flag_Create = 1, flag_capacity = 1, flag_size = 1, flag_isEmpty = 1, flag_push = 1, flag_pop = 1, flag_peek = 1;
    stack_t * stack = StackCreate(TEST_CAPAC);
    flag_isEmpty *= StackIsEmpty(stack);
    flag_capacity *= (StackCapacity(stack) == TEST_CAPAC);
    flag_size *= (!StackSize(stack));
    flag_Create *= stack && (TEST_CAPAC == StackCapacity(stack)) && (0 == StackSize(stack));
    StackPush(stack, &a);
    StackPush(stack, &b);
    StackPush(stack, &c);
    StackPush(stack, &d);
    StackPush(stack, &c);
    flag_push *= (TEST_CAPAC == StackSize(stack)) && (c == *(int *)StackPeek(stack));
    flag_isEmpty *= !StackIsEmpty(stack);

    flag_size *= (TEST_CAPAC == StackSize(stack));
    flag_peek *= (c == *(int *)StackPeek(stack)) && (TEST_CAPAC == StackSize(stack));
    StackPop(stack);

    flag_pop *= (TEST_CAPAC -1 == StackSize(stack)) && (d == *(int *)StackPeek(stack));



    StackDestroy(stack); /*check valgrind*/

    PrintStatus("StackCreate", flag_Create);
    PrintStatus("StackCapacity", flag_capacity);
    PrintStatus("StackSize", flag_size);
    PrintStatus("StackPeek", flag_peek);

    PrintStatus("StackPush", flag_push);
    PrintStatus("StackPop", flag_pop);
    PrintStatus("StackIsEmpty", flag_isEmpty);




}

int main()
{
    TeststackFuncs();
    return 0;
}
