#include <stdio.h>

size_t CountBits(unsigned char byte)
{
    size_t count = 0;
    
    while (byte)
    {
        ++count;
        byte &= (byte - 1);
    }
    return count;
}

size_t CountBitCouples(unsigned char byte)
{
    return (size_t)CountBits((byte & (byte >> 1)));
}

int main()
{
    printf("%lu\n",CountBitCouples(0xFa));
    return 0;
}
