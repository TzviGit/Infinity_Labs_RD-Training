#define _POSIX_SOURCE

#include <stdio.h>  /* printf  */
#include <stdlib.h> /* exit    */

#include <signal.h> /* signals */

#include <sys/types.h> /* pid_t */




void ExecHandler(int signal)
{
    (void)signal;
}

int main(void)
{

    struct sigaction sig_act;
    sig_act.sa_handler = ExecHandler;

    sigaction(SIGUSR1, &sig_act, NULL);


    return 0;
}
