
#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "ws11_funcs.h"


/*APROVED BY AVIAD*/
int CharToDigit(char c, int base)
{
    if(isdigit(c) && (c - '0') < base)
    {
        return (c - '0');
    }
    else if(isalpha(c) && (toupper(c) - 'A' + 10) < base)
    {
        return (c - 'A' + 10);
    }

    return -1;
}

int Atoi(const char *str, int base)
{
    int result = 0;
    int sign = 1;
    int d = 0;

    while(isspace(*str))
    {
        ++str;
    }
    if ('-' == *str)
    {
       sign = -1;
       str++;
    }

    d = CharToDigit ((*str), base);
    while(-1 != d)
    {
        result = (result * base) + d;
        d = CharToDigit((*++str), base);
    }

    return result * sign;
}

int CompareAtoi( const char *nptr)
{
    return Atoi(nptr, 10) == atoi(nptr);
}

void TestAtoi()
{
    char str1[] = "0453", str2[] = "675g4", str3[] = "-12304";
    if (CompareAtoi(str1) && CompareAtoi(str2) && CompareAtoi(str3))
    {
        printf("Atoi PASSED!!\n\n");
    }
    else
    {
        printf("Atoi is a FAILURE\n\n");
    }
}

void TestItoa(int val, char *dest, int base, char *result)
{
    if(0 == strcmp(Itoa(val, dest,base), result))
    {
        puts("Itoa PASSED THE TEST!!\n\n");
    }
    else
    {
        puts("Itoa FAILED THE TEST\n\n");
    }
}
