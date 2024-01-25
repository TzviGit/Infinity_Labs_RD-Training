

#include <stdio.h>
#include <string.h>
#include "test_util.h"

int DecimalConvert(char roman);


int RomanToDecimal(char *roman)
{
    size_t size = strlen(roman) - 1;
    size_t i = 0;
    int prev = 0;
    int curr =  0;
    int result = 0;

    for (i = 0; i <= size; ++i)
    {
        int flag = 0;
        curr = DecimalConvert(roman[ size - i]);
        if (curr == -1)
        {
            return -1;
        }
        if (curr >= prev)
        {
            flag = 0;
            result += curr;
        }
        else
        {
            if (1 == flag)
            {
                return -1;
            }
            result -= curr;
            flag = 1;
        }

        prev = curr;
    }

    return result;
}

int DecimalConvert(char roman)
{
    switch (roman)
    {
        case 'I' : return 1;
        case 'V' : return 5;
        case 'X' : return 10;
        case 'L' : return 50;
        case 'C' : return 100;
        case 'D' : return 500;
        case 'M' : return 1000;

        default :
                return (-1);

    }
}

int main (void)
{
    RESET_FLAG;
    char numeral1[] = "MCDXCIV";
    VERIFY(1494 == RomanToDecimal(numeral1));
    printf("%d\n\n", RomanToDecimal("IIX"));
    VERIFY(309 == RomanToDecimal("CCCIX"));
    CHECK_SUCCESS(RomanToInt);

    return 0;


}
