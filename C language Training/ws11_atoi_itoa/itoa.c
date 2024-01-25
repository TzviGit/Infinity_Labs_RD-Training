#include "ws11_funcs.h"
#include <assert.h>


char DigToChar(int d)
{
    if (d < 0 || d > 36)
    {
        return '\0';
    }
    return ((d > 9) ? (d -10) + 'A' : d + '0');
}

unsigned int SizeInBase(int val, int base)
{
    unsigned int count = 1;

    while(val /= base)
    {
        ++count;
    }


    return count;
}

/*JOHHNY*/
char *Itoa(int value, char *dest, int base )
{
    char *end = NULL;
    int sign = 1;
    if (value < 0)
    {
        sign = -1;
        value *= sign;
    }

    assert(dest);

    if (base < 2 || base > 36)
    {
        return NULL;
    }

    end = dest ;

    if (-1 == sign)
    {
        ++end;
        *dest = '-';

    }
    end += SizeInBase(value,base);


    *end-- = '\0';

    do
    {
        *end-- = DigToChar(value % base);
        value /= base;
    }
    while(value);

    return dest;
}
