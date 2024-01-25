#include <stddef.h> /*size_t*/
#include <assert.h> /*assert*/
#include <ctype.h>  /*toupper*/
#include <stdlib.h> /*malloc*/
#include <string.h> /*string funcs*/
#include <stdio.h> /*printf*/
#include <limits.h> /*CHAR_BIT*/
#include "bit_array.h"

#define MAX_INDEX (sizeof(size_t) * CHAR_BIT)
#define BINARY 2
#define BYTE 8



/*HELPER FUNCS DECLARATIONS*/
char DigToChar(int );
unsigned int SizeInBase(size_t , int );
char *Itoa(size_t , char *, int  );
size_t ByteMirror(unsigned char );
size_t CountSetBits(size_t );
int CharToDigit(char , int );
int Atoi( const char *, unsigned int );

/*APROVED BY FANIA*/

/*BIT_ARR FUNCS*/
bit_arr_t SetAll(bit_arr_t arr)
{
    (void)arr;
    return  (bit_arr_t)~0;
}

bit_arr_t ResetAll(bit_arr_t arr)
{
    (void)arr;
    return  (bit_arr_t)0;
}

bit_arr_t SetOn(bit_arr_t arr, size_t index)
{
    return arr | ((size_t)1 << index);
}

bit_arr_t SetOff(bit_arr_t arr, size_t index)
{
    return arr & (~((size_t)1 << index));
}

bit_arr_t SetBit(bit_arr_t arr, size_t index, int set_as)
{
    arr &= (~(size_t)1 << index);
    arr |= ((size_t)set_as << index);
    return arr;
}


bit_arr_t FlipBit(bit_arr_t arr, size_t index)
{
    return arr ^ ((size_t)1 << index);
}

int GetBit(bit_arr_t arr, size_t index)
{
    return (arr >> index) & 1;
}

bit_arr_t RotR(bit_arr_t arr, size_t amount)
{
    return (arr << amount) | (arr >> ( MAX_INDEX - amount));
}

bit_arr_t RotL(bit_arr_t arr, size_t amount)
{
    return (arr >> amount) | (arr << ( MAX_INDEX - amount));
}

bit_arr_t Mirror(bit_arr_t arr)
{
    size_t mask = 0xFF, i = 0;
    bit_arr_t mirror = (size_t)ByteMirror((unsigned char)(arr & mask));

    for (i = 0; i < sizeof(size_t); ++i)
    {
        mirror  = RotR(mirror, BYTE);
        mirror |= (size_t)ByteMirror((unsigned char)(arr & mask));
        arr = RotL(arr, BYTE);
    }

    return mirror;
}

size_t CountOn(bit_arr_t arr)
{
    size_t count = 0;

 /*each iteration closes a bit so it will only run for as many bits are set*/
    while (arr)
    {
        arr = arr & (arr - 1);

        count++;
    }

    return count;
}


size_t CountOff(bit_arr_t arr)
{
    return MAX_INDEX - CountOn(arr);
}

char *ToString(size_t arr, char *buf) /*buffer must be at least 72 long*/
{
    size_t i = 0;
    char *ptr = buf;

    assert(buf);
    *ptr++ = '0' + (1 & (arr));

    for (i = 1; i < MAX_INDEX; ++i)
    {
        if (0 == (i % BYTE))
        {
            *ptr++ = '\n';
        }

        *ptr++ = '0' + (1 & (arr >> i));

    }
    *(ptr) ='\0';

    return buf;
}






/*HELPER FUNCTIONS*/
char DigToChar(int d)
{
    if (d < 0 || d > 36)
    {
        return '\0';
    }
    return ((d > 9) ? (d -10) + 'A' : d + '0');
}

unsigned int SizeInBase(size_t val, int base)
{
    unsigned int count = 1;

    while(val /= base)
    {
        ++count;
    }


    return count;
}

/*JOHHNY*/
char *Itoa(size_t value, char *dest, int base )
{
    char *end = NULL;

    assert(dest);

    if (base < 2 || base > 36)
    {
        return NULL;
    }

    end = dest + SizeInBase(value,base);


    *end-- = '\0';

    do
    {
        *end-- = DigToChar(value % base);
        value /= base;
    }
    while(value);

    return dest;
}


size_t ByteMirror(unsigned char num)
{
	num = (((num & 0xf0) >> 4) | ((num & 0x0f) << 4));
	num = (((num & 0xcc) >> 2) | ((num & 0x33) << 2));
	return ((num & 0xAA) >> 1) | ((num & 0x55) << 1);
}



int Atoi( const char *nptr, unsigned int base)
{
    int number = 0;
    int digit = 0;
    int flag = 0;
    if (base < 2 || base > 36)
    {
        return 0;
    }
    while(-1 != (digit = CharToDigit(*nptr, base)) )
    {
        if (32 == digit || 10 == digit)
        {
            ++nptr;
            continue;
        }
        if (45 == digit)
        {
            if (0 == flag && 0 == number && 10 == base)
            {
                ++flag;
                ++nptr;
                continue;
            }
            else
            {
                break;
            }
        }

        number = (number * base) + digit;
        ++nptr;
    }

    if (flag)
    {
        number *= -1;
    }

    return number;
}

int CharToDigit(char c, int base)
{
    c = toupper(c);

    if (32 == c || 45 == c || 10 == c)
    {
        return c;
    }

    if (base <= 0)
    {
        return -1;
    }
    if ( base <= 10)
    {
        return ((c >= '0') && (c <= ('0' + (base -1))) ? (c -48) : -1);
    }
    if (base <= 36)
    {
        if (c >= 'A' && c < ('A' + (base - 10)))
        {
            return c - 55;
        }

        return (c >= '0' && c <= '9') ? (c - 48) : -1;
    }
    return -1;

}
