#include <stdio.h>
#include "queue.h"


#define TEST(line, function, bool) TestCondition( (line), #function, (bool))

void TestCondition( int line, char *func, int bool)
{
    if (!bool)
    {
        printf("The Test for %s on line %d FAILED!!!\n\n", func, line);
    }
}

void TestQueue()
{
    queue_t * queue = QCreate();

    TEST(__LINE__, QGetSize, 0 == QGetSize(queue));

    QDestroy(queue);
}

int main()
{
    TestQueue();
    return 0;
}
