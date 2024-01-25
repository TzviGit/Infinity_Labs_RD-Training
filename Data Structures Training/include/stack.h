#ifndef STACK_H
#define STACK_H
#include <stddef.h> /* size_t */

typedef struct stack stack_t;
/********************* ~FUNctions~ **********************/

/* Create new stack of given capacity */
stack_t *StackCreate(size_t capacity);

/* destroy the whole stack,  */
void StackDestroy(stack_t *stack);

/* add an element to the top of the stack */
void StackPush(stack_t *stack, void *elem);

/* removes top element from the stack */
void StackPop(stack_t *stack);

/* returns last element */
void *StackPeek(const stack_t *stack);

/* return number of elements in stack */
size_t StackSize(const stack_t *stack);

/* return stack capacity */
size_t StackCapacity(const stack_t *stack);

/* return if stack is empty or not (meaning contains any elements) */
int StackIsEmpty(const stack_t *stack);


#endif
