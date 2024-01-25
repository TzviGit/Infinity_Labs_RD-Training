#include <stdlib.h>
#include <string.h>
#include <stdio.h>


int IsRotation(const char *s1, const char *s2)
{
    int bool = 0;
    char *spread = (char *)malloc(2 * strlen(s1) + 5);
    char *ptr = spread;
    char *sptr = (char *)s1;
    while (*sptr)
    {
        *ptr++ = *sptr++;
    }
    sptr -= strlen(s1);
    while (*sptr)  /*again*/
    {
        *ptr++ = *sptr++;
    }
    *ptr = '\0';

     bool = (NULL != strstr(spread, s2));
     free(spread); spread = NULL;

    return bool;
}

int main()
{
    char arr1[] = "1234512";
    char arr2[] = "2345121";
    printf("%d\n\n",IsRotation(arr1,arr2));
    return 0;
}
