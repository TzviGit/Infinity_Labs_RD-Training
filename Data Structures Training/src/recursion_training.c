/******************************************************************************
* Author        Tzvi Moskowitz
* Reviewer      --------
* Description   Recursive Implementation of some Popular functions
* Date          October 19, 2021
* Group         OL110
* Company       ILRD Ramat Gan
* ****************************************************************************/

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "recursion_training.h"


#define MIN(num1, num2) (num1) < (num2) ? (num1) : (num2)
#define MAX(num1, num2) (num1) > (num2) ? (num1) : (num2)

static stack_t *StackBubbleMax(stack_t *stack);
static int MyStackIntPop(stack_t *stack);
static node_t *ReverseListRecursive(node_t *curr, node_t *prev);
static void MyStackIntPush(stack_t *stack, int val);


/****************************************************************************
*   ~~~~~~~ Recursive  & Iterative Implementation of Fibonacci Series ~~~~~
*****************************************************************************/

int FibonacciRec(int ind)
{
    assert(ind >= 0);

    if (ind < 2)
    {
        return ind;
    }

    return FibonacciRec(ind - 1) + FibonacciRec(ind - 2);
}

int FibonacciIter(int ind)
{
    int prevs_prev = 0, prev = 1, curr = 0;
    int i = 0;

    assert(ind >= 0);

    if (ind < 2)
    {
        return ind;
    }
    for (i = 1; i < ind; ++i)
    {
        curr = prevs_prev + prev;
        prevs_prev = prev;
        prev = curr;
    }

    return curr;
}


/****************************************************************************
*   ~~~~~~~~~~  Recursive Implementation To Reverse Singly Linked List ~~~~~
*****************************************************************************/

node_t *FlipList(node_t *node)
{
    assert(node);
    return ReverseListRecursive(node, NULL);
}

static node_t *ReverseListRecursive(node_t *curr, node_t *prev)
{
    node_t *next = NULL;
    if (NULL == curr)
    {
        return prev;
    }

    assert(curr);


    next = curr->next;

    curr->next = prev;

    return ReverseListRecursive(next, curr);
}



/****************************************************************************
*   ~~~~~~~~~~  Recursive Implementation of Stack sort ~~~~~~~~
*****************************************************************************/

void SortIntStack(stack_t *stack)
{
    int max = 0;;

    assert(stack);

    if (StackSize(stack) < 2)
    {
        return;
    }

    max = MyStackIntPop(StackBubbleMax(stack));

    SortIntStack(stack);
    MyStackIntPush(stack, max);

}

static stack_t *StackBubbleMax(stack_t *stack)
{
    int first = 0;
    int second = 0;

    assert(stack);

    if (StackSize(stack) < 2)
    {
        return stack;
    }

     first = MyStackIntPop(stack);
    second = MyStackIntPop(StackBubbleMax(stack));

    MyStackIntPush(stack, MIN(first, second));
    MyStackIntPush(stack, MAX(first, second));

    return stack;
}

/******** STACK UTIL FUNCS ***********************/

static int MyStackIntPop(stack_t *stack)
{
    void *top = NULL;

    assert(stack);
    assert(!StackIsEmpty(stack));
    top = StackPeek(stack);
    StackPop(stack);
    return (int)top;
}

static void MyStackIntPush(stack_t *stack, int val)
{
    assert(stack);

    StackPush(stack, (void *)(long)val);
}




/******=======**** STRING FUNCTIONS *****=====************/


size_t StrLen(const char *str)
{
    assert(str);

    if ('\0' == *str)
    {
        return 0;
    }

    return 1 + StrLen(str + 1);
}


int StrCmp(const char *s1, const char *s2)
{
    assert(s1);
    assert(s2);

    if ('\0' == *s1 || *s1 != *s2)
    {
        return *s1 - *s2;
    }

    return StrCmp(s1 + 1, s2 + 1);
}

char *StrCpy(char *dest, const char *src)
{
	assert(src);
	assert(dest);

	*dest = *src;

	if ('\0' == *src)
	{
		return dest;
	}

	StrCpy(dest + 1, src + 1);

	return dest;
}



char *StrCat(char *dest, const char *src)
{
    char *cat_start = NULL;

    assert(dest);
    assert(src);

    cat_start = dest + StrLen(dest);
    StrCpy(cat_start, src);

    return dest;
}

char *StrStr(const char *haystack, const char *needle)
{
    size_t haystack_len = 0;
    size_t needle_len = 0;

    assert(haystack);
    assert(needle);

    haystack_len = StrLen(haystack);
    needle_len = StrLen(needle);

    if (haystack_len < needle_len)
    {
        return NULL;
    }

    if (0 == StrNCmp(haystack, needle, needle_len))
    {
        return (char *)haystack;
    }

    return StrStr(haystack + 1, needle);
}



int StrNCmp(const char *s1, const char *s2, size_t n_bytes)
{
    if (0 == n_bytes)
    {
        return 0;
    }

    if (*s1 != *s2)
    {
        return *s1 - *s2;
    }

    return StrNCmp(s1 + 1, s2 + 1, n_bytes - 1);
}
