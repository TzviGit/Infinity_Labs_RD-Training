/********************************************************
 *
 * QUIZ QUESTION -- IMPLEMENTATION OF QUEUE WITH 2 STACK.
 *       ~~~ MY IMPLEMENTATION IS BSED ON THE GENIUS IDEA OF AMATZIA ~~
 *
 * Author : Tzvi Moskowitz
 *
 * Date:  October 25, 2021
 *
 * OLRD 110 Ramat Gan
 *
 * *********************************************************/

#include <stdlib.h> /*malloc*/
#include "stack.h"
#include "test_util.h"



typedef struct queue
{
    stack_t *enqueue_stack;
    stack_t *dequeue_stack;
}my_queue_t;

my_queue_t *CreateQueue(size_t stack_size);
void DestroyQueue(my_queue_t *queue);
static void TranferToDequeuStack(my_queue_t *queue);







/***************************************************************************
*       Heres the Implementation of a queue withe 2 stacks.
*
***************************************************************************/


void MyQueueEnqueue(my_queue_t *queue, void *data)
{
    StackPush(queue->enqueue_stack, data);
}

void *MyQueueDequeue(my_queue_t *queue)
{
    void *return_val = NULL;
    if (StackIsEmpty(queue->dequeue_stack))
    {
        TranferToDequeuStack(queue);
    }

    return_val = StackPeek(queue->dequeue_stack);
    StackPop(queue->dequeue_stack);

    return return_val;
}

static void TranferToDequeuStack(my_queue_t *queue)
{
    while (!StackIsEmpty(queue->enqueue_stack))
    {
        StackPush(queue->dequeue_stack, StackPeek(queue->enqueue_stack));
        StackPop(queue->enqueue_stack);
    }
}


/******************************************************************************
*              ~~~~~~~~~~~~~ TESTER  ~~~~~~~
****************************************************************************/
void TestMyQueue(void)
{
    int flag = 0;
    size_t i = 0;
    int arr[] = {1,2,3,4,5,6,7,8,9,10};
    my_queue_t *queue = CreateQueue(10);

    for (; i < 10; ++i)
    {
        MyQueueEnqueue(queue, arr + i);
    }

    MyQueueDequeue(queue);

    for (i = 0; i < 10; ++i)
    {
        MyQueueEnqueue(queue, arr + i);
    }

    for (i = 0; i < 19; ++i)
    {
        printf(_BLUE_ "%d"_CYAN_"->" _RESET_, *(int *)MyQueueDequeue(queue) );
    }

    DestroyQueue(queue);
}

int main(void)
{
    TestMyQueue();
    return 0;

}












my_queue_t *CreateQueue(size_t stack_size)
{
    my_queue_t *queue = (my_queue_t *)malloc(sizeof(*queue));
    if (NULL == queue)
    {
        return NULL;
    }

    queue->enqueue_stack = StackCreate(stack_size);
    queue->dequeue_stack = StackCreate(stack_size);

    if (NULL == queue->enqueue_stack || NULL == queue->dequeue_stack)
    {
        if (!queue->enqueue_stack)
        {
            StackDestroy(queue->dequeue_stack);
        }
        if (!queue->dequeue_stack)
        {
            StackDestroy(queue->enqueue_stack);
        }

        free(queue);
        return NULL;
    }
    return queue;
}

void DestroyQueue(my_queue_t *queue)
{
    StackDestroy(queue->enqueue_stack);
    StackDestroy(queue->dequeue_stack);

    free(queue); queue = NULL;
}
