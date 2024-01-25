#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "circular_buffer.h"


#define TEST(function, bool) TestCondition( __LINE__, #function, (bool))


char *CBToString(cbuff_t *cbuff, char *);

void TestCondition( int line, char *func, int bool)
{
    if (!bool)
    {
        printf("The Test for %s on line %d FAILED!!!\n\n", func, line);
    }
}



void TestCircleBuffer(void)
{
    cbuff_t *cbuff = CBCreate(10);
    size_t count = 0;
    char buf1[10] = "hello day";
    char test_buffer[100] = "";
    char *buf2 = malloc(10);
    if (!buf2)return;

    strcpy(buf2,"abcdefghi");



    TEST(CBIsEmpty, CBIsEmpty(cbuff));
    TEST(CBIsEmpty, CBBufsiz(cbuff) == CBFreeSpace(cbuff));

    count = CBWrite(cbuff,(void *)buf1, 10);
    TEST(CBWrite, count == 10 && CBFreeSpace(cbuff) == 0);
    TEST(CBWrite, !strcmp(CBToString(cbuff, test_buffer), buf1));

    count = CBWrite(cbuff,(void *)buf2, 10);
    TEST(CBWrite, count == 0 && CBFreeSpace(cbuff) == 0);
    TEST(CBWrite, !strcmp(CBToString(cbuff, test_buffer), buf1));
    TEST(CBFreeSpace, !CBFreeSpace(cbuff));

    count = CBRead(cbuff,(void *)buf2, 10);
    TEST(CBRead, !strcmp(buf1,buf2) && !strcmp("" ,CBToString(cbuff, test_buffer)));

    strcpy(buf2,"abcdefghi");
    CBWrite(cbuff, buf1 , 5);
    CBWrite(cbuff, buf2, 4);

    TEST(CBFreeSpace, 1 == CBFreeSpace(cbuff));
    TEST(CBWrite, !strcmp("helloabcd", CBToString(cbuff, test_buffer)));

    CBDestroy(cbuff); cbuff = NULL;
    free(buf2); buf2 = NULL;
    

}

int main()
{
    TestCircleBuffer();
    return 0;
}
