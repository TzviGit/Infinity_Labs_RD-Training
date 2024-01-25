/* ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ *
 ~ Version       1.0.0                                           *
 ~ Author        Vadim                                           ~
 ~ Description   Resursion WS - Header file                      ~
 * Group         OL110                                           *
 ~ Company       ILRD Ramat Gan                                  ~
 * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ */
#ifndef __RECURSION_OL110_ILRD_H__
#define __RECURSION_OL110_ILRD_H__

#include "stack.h"

typedef struct node node_t;
struct node
{
    int data;
    node_t *next;
};


int FibonacciRec(int ind);

int FibonacciIter(int ind);

node_t *FlipList(node_t *node);


void SortIntStack(stack_t *stack);

/* string.h */

size_t StrLen(const char *s);

int StrCmp(const char *s1, const char *s2);

char *StrCpy(char *dest, const char *src);

char *StrCat(char *dest, const char *src);

char *StrStr(const char *haystack, const char *needle);

int StrNCmp(const char *s1, const char *s2, size_t n_bytes);

#endif /* __RECURSION_OL110_ILRD_H__ */
