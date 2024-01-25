#include <stdio.h> /*for printf*/
#include <stdlib.h> /*for malloc*/

int main()
{
    int *a ;
    int *b;
    int j = 0;
    int i = 0;
    int m = 0;
    int n = 0;
    (void)m;
    a = (int *)malloc(10 * sizeof(int));

    for (i = 0; i < 10; ++i)
    {
        *a = 2;
    }
    m = *(a + 11);



    b = (int *)malloc(10 * sizeof(int));

    for (j = 0; j <= 10; ++j)
    {
        *b = 2;
    }
    free(b);

    *(b + 8) = n;
	b = NULL;

    return 0;
}
