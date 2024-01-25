#include <stddef.h>
#include <stdio.h>
#include "list_exe.h"

struct node
{
    void *data;
    struct node *next;
};

void PrintList(node_t *list);
node_t *Flip(node_t *head);

void testFlip()
{
    int a = 2, b = 3, c = 5, d = 6;
    node_t one , two, three, four, *result = NULL;
    one.data = &a;
    one.next = &two;
    two.data = &b;
    two.next = &three;
    three.data = &c;
    three.next = &four;
    four.data = &d;
    four.next = NULL;
    PrintList(&one);
    result = Flip(&one);
    PrintList(result);

}

void TestHasLoop()
{
    int a = 1, b = 2, c = 3, d = 4, e = 5, result = 0;
    node_t one , two, three, four,five;
    one.data = &a;
    one.next = &two;
    two.data = &b;
    two.next = &three;
    three.data = &c;
    three.next = &four;
    four.data = &d;
    four.next = &five;
    five.data = &e;
    five.next = &two;


    result = HasLoop(&one);

    if (result)
    {
        printf("HasLoop PASSED the test!\n\n");
    }
    else
    {
        printf("HasLoop FAILED the test!\n\n");
    }
}

void TestFindIntersection()
{
    int a = 1, b = 2, c = 3, d = 4, e = 5;
    node_t one , two, three, four, five, six, seven, *result = NULL;
    one.data = &a;
    one.next = &two;
    two.data = &b;
    two.next = &three;
    three.data = &c;
    three.next = &four;
    four.data = &d;
    four.next = &five;
    five.data = &e;
    five.next = NULL;

    six.data = &a;
    six.next = &seven;
    seven.data = &e;
    seven.next = &three;

    PrintList(&one);
    PrintList(&six);

    result = FindIntersection(&one, &six);


    if (result == &three)
    {
        printf("FindIntersection PASSED the test!\n\n");
    }
    else
    {
        printf("FindIntersection FAILED the test!\n\n");
    }
}

int main()
{
    testFlip();
    TestHasLoop();
    TestFindIntersection();
    return 0;
}
