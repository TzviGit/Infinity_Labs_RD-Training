/******************************************************************************
* Author        Tzvi Moskowitz
* Reviewer      --------
* Description   Algorithm to Reverse order of the word in a sentence. O(n)
* Group         OL110
* Company       ILRD Ramat Gan
* Date
* ****************************************************************************/

#include <assert.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "my_string.h"
#include "test_util.h"


/*******************************************************************************
*   Reverse order of the word in a sentence. Time_C = O(n) : Space_C = O(1)
*******************************************************************************/

char *ReverseWordsOrder(char *str)
{
    char *word_ptr = str;
    assert(word_ptr);

    /**first reverse entire string***/
    StrReverse(str);

    /**Then reverse each word***/
    while (*word_ptr)
    {
        ReverseWord(word_ptr);
        word_ptr = GetNextWord(word_ptr);
    }

    return str;
}


/******============= NOW LETS TEST IT!!! ========*****************/

void TestReverseWordsOrder(void)
{
    int flag = 0;
    char string[] = "Hello how are you";
    VERIFY(!strcmp(ReverseWordsOrder(string), "you are how Hello"));

    CHECK_SUCCESS(flag, "ReverseWordsOrder");
}

int main()
{
    TestReverseWordsOrder();
    return 0;
}
