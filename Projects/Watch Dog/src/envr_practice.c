
#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main (int argc, char **argv)
{
    int i = 0;
    char row1[20] = {0};
    char row2[20] = {0};
    char row3[20] = {0};
    char row4[20] = {0};

    char *envr[10] = {0};

    sprintf(row1, "%d", 1);
    sprintf(row2, "%d", 2);
    sprintf(row3, "%d", 3);
    sprintf(row4, "%d", 4);

    envr[0] = row1;
    envr[1] = row2;
    envr[2] = row3;
    envr[3] = row4;


    for (; i < 4; ++i)
    {
        printf("%d --> ", atoi(envr[i]));
        
    }
    fflush(stdout);

    setenv("prog_interval", "3", 0);
    setenv("prog_allowance", "2", 0);


    execvp("./envr2", argv);
    return 0;
}