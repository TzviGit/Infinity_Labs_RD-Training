#include <stdio.h>
#include <ctype.h>
#include <assert.h>


void TF(int num)
{
    int i = 1;
    for (i = 1; i <= num; ++i)
    {
        if (!(i % 3) || !(i % 5))
        {
            printf("%s",!(i % 3)? "T": "");
            printf("%s",!(i % 5)? "F": "");
        }
        else
        {
            printf("%d",i);
        }
        printf(" ");

    }
     printf("\n");
}

void SwapChars(char *a, char *b)
{
    char temp = *a;
    *a = *b;
    *b = temp;
}
void ReverseToLower(char *str)
{
    char *ptr = str;

    assert(str);

    while(*ptr)
    {
        *ptr = tolower(*ptr);
        ++ptr;
    }

    ptr -= 1;
    while(!(ptr == str))
    {
        SwapChars(str++, ptr--);
    }

}

int main()
{
    char str[] = "HelLo wORLd";
    TF(345);
    ReverseToLower(str);
    printf("%s\n",str);
    return 0;
}
