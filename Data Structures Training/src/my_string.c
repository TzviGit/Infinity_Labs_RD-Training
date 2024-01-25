
#include <string.h>
#include <ctype.h>
#include "my_string.h"


void StrReverse(char *str)
{
    ReverseChars(str, strlen(str));
}

void ReverseWord(char *word)
{
    ReverseChars(word, WordLen(word));
}

void ReverseChars(char *start, size_t how_many)
{
    size_t left = 0;
    size_t right = how_many - 1;

    for (; left < right; ++left, --right)
    {
        CharSwap(&start[left], &start[right]);
    }
}

size_t WordLen(char *word)
{
    char *char_ptr = word;
    size_t count = 0;

    while (isalpha(*char_ptr))
    {
        ++char_ptr;
        ++count;
    }

    return count;
}

char *GetNextWord(char *word)
{
    char *char_ptr = word;

    char_ptr += WordLen(char_ptr);

    while (isspace(*char_ptr))
    {
        ++char_ptr;
    }

    return char_ptr;

}

void CharSwap(char *c1, char *c2)
{
    char temp = *c1;
    *c1 = *c2;
    *c2 = temp;
}
