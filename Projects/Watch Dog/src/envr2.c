#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main (int argc, char *argv[])
{
    int i = 0;

    int interval = atoi(getenv("prog_interval"));
    int allowance = atoi(getenv("prog_allowance"));
    /*  printf("\nNOW IN OTHER PROCCES\n");
    for (; i < 4; ++i)
    {
        printf("%d", atoi(envr[i]));
    }
 */

    printf("I got the number! it : %d\n", interval);

    printf("I got the allowance! it : %d\n", allowance);
    fflush(stdout);

    printf("%s\n", getcwd(NULL, 0));

    return 0;
}