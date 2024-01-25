#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "wd.h"

#define _RED_    "\033[1;31m"

static void SleepEnoughTime(time_t run_time)
{
    int flag = 0;

    do
    {
        flag = sleep(difftime(run_time, time(NULL)));
    }
    while (0 != flag);
}


int main(
    int argc, char *argv[])
    {

        printf(_RED_"IM STARTING TO RUN\n");
        WDStart(argv, "./bin/wd_program", 3, 3, 3, 3);

        SleepEnoughTime(time(NULL) + 5);
        
        WDStop();
        return 0;
    }
