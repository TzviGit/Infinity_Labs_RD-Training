#include <stdio.h>
#include <stddef.h>

int Increment(int num)
{
    size_t mask = 1;
    while (mask & num)
    {
        num ^= mask;
        mask <<= 1;
    }

    return num ^= mask;
}

int main()
{
    int a =  -2147483648; /*MIN_INT*/
    int b = -1;
    printf("%d plus one = %d\n\n%d plus one = %d\n\n", a, Increment(a), b, Increment(b));
    return 0;
}
