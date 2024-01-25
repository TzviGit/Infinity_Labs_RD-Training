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
#include <semaphore.h>
#include <fcntl.h>      /* For O_* constants */
#include <sys/stat.h>  /*for mode constants*/

#define _RED_    "\033[1;31m"
#define _GREEN_  "\033[1;32m"

enum
{
    OFF,
    ON
};

atomic_int state = OFF;

void SignalHandler(int sig)
{
    (void)sig;

    state = ON;
}
int main ()
{
    sem_t *semaphore = sem_open("/coolsemaphore",O_CREAT,S_IWUSR | S_IWOTH,0);
    struct sigaction sigact = {0};
    sigact.sa_handler = SignalHandler;
    sigaction(SIGUSR1, &sigact, NULL);

    sem_wait(semaphore);
    
    while (1)
    {
        while (OFF == state);

        printf(_RED_"PONG\n");

        state = OFF;
        sleep(1);
        kill(getppid(), SIGUSR2);
        sleep(1);

    }

    return 0;
}