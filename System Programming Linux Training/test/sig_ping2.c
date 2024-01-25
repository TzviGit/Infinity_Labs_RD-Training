/* ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ *
 ~ Author        Tzvi Moskowitz                                      ~
 * Reviewer                                                      *
 ~ Description   PingPong - Ex 1                                 ~
 * Group         OL110                                           *
 ~ Company       ILRD Ramat Gan                                  ~
 * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ */

#define _POSIX_SOURCE

#include <signal.h> /*sig_atomic_t, SIGUSR1, SIGUSR2, sigaction*/
#include <sys/types.h> /*pid_t*/
#include <unistd.h> /*write, getppid*/
#include <stdio.h> /*printf*/
#include <sys/wait.h>
#include <stdatomic.h>

#define _RED_    "\033[1;31m"
#define _GREEN_  "\033[1;32m"

enum
{
    CHILD_TURN,
    PARENT_TURN
};

pid_t child_pid = 0;
atomic_int state = PARENT_TURN;


void HandleChildSignal(int sig)
{
    ++state;
    (void)sig;
}

void HandleParentSignal(int sig)
{
    --state;
    (void)sig;
}


int main(int argc, char *argv[])
{

    struct sigaction sigact_parent = {0};

    sigact_parent.sa_handler = HandleChildSignal;
    sigaction(SIGUSR2, &sigact_parent, NULL);
    
    child_pid = fork();

    if (0 == child_pid)
    {
        execv("./bin/sig_exec", argv);
    }
    if (child_pid > 0)
    {   /* parent proccess */
       
       while (1)
       {
            while (CHILD_TURN == state);   
            
            printf(_GREEN_"PING\n");
             sleep(1);
            kill(child_pid, SIGUSR1);
            sleep(1);
        }
    }

    (void)argc;
    return 0;
}
