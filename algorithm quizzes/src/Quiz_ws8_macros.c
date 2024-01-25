#include <stdio.h>

typedef struct
{
    double d;
    unsigned int bit1 : 2;
    unsigned int bit2 : 3;
    unsigned int bit3 : 1;
    int h;
}test_t;

#define MAX2(a,b) (((a) >= (b))? (a): (b))
#define MAX3(a,b,c) (MAX2(MAX2((a),(b)), (c)))
#define TOLOWER(c) ((((c) >= 'A') && ((c) <= 'Z'))? ((c) + 32): (c))
#define OFFSET(s,f) (size_t)((char *)(&((s *)10)->f) - (char *)(s *)10)
#define IS_LITTLE_ENDIAN ((*(unsigned int *)"\xFF\0\0\0" == 0xff ? 1 : 0))

int main()
{
    int a = -23, b = 45, c = -343;
    char ch = 'G';
    printf("the offset of the field bit3 is: %lu\n\n",OFFSET(test_t,h));
    printf("the lower case of ch is: %c\n\n", TOLOWER(ch));
    printf("the max val between a, b, and c is: %d\n\n", MAX3(a,b,c));
    printf("this processor %s little Endian\n\n", (IS_LITTLE_ENDIAN ? "IS" : "IS NOT"));
    return 0;
}
