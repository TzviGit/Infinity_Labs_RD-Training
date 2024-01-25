  #include <assert.h>
#include <stdlib.h>
#include "stack.h"

struct stack
{
   size_t capacity;
   size_t size;
   void **stack_element;
};

/******APPROVED BY FANIA********/

stack_t *StackCreate(size_t capacity)
{

    stack_t *stackp = (stack_t *)malloc(sizeof(stack_t) + (capacity * sizeof(void *)));
    if (NULL == stackp)
    {
        return NULL;
    }
    stackp->stack_element = (void **)(stackp + 1);
    stackp->capacity = capacity;
    stackp->size = 0;


    return stackp;
}

void StackDestroy(stack_t *stack)
{
    free(stack); stack = NULL;
}

void StackPush(stack_t *stack, void *elem)
{
    assert(stack && (stack->size != stack->capacity));
    stack->stack_element[(stack->size)++] = elem;
}

void StackPop(stack_t *stack)
{
    assert(stack && stack->size);
    stack->stack_element[--(stack->size)] = NULL;
}

void *StackPeek(const stack_t *stack)
{
    assert(stack);
    return stack->stack_element[stack->size - 1];
}

size_t StackSize(const stack_t *stack)
{
    return stack->size;
}

size_t StackCapacity(const stack_t *stack)
{
    return stack->capacity;
}

int StackIsEmpty(const stack_t *stack)
{
    return !(stack->size);
}
