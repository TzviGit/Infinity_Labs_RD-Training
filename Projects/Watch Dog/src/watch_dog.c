/*******************************************************
*   Author: Tzvi Moskowitz
*   Reviewer: TODO (Tuvia)
*   Description: Implementation of Watchdog to revive a
*   proccess if it runs into trouble.
*
*   Date November 29, 2021
*   Company ILRD OL110 -- Ramat Gan
*
**********************************************************/

#define _POSIX_C_SOURCE  200112L

#include <string.h> /* atoi */
#include <stdlib.h>
#include <stdio.h> /* sprintf */
#include <sys/types.h> /* fork */
#include <unistd.h>  /*fork */
#include <stdatomic.h>  /* atomic_int */
#include <signal.h> /* for signals */
#include <fcntl.h>           /* For O_CREAT* constants */
#include <sys/stat.h>   /* For semaphore mode constants */
#include <semaphore.h> /* semaphores */
#include <pthread.h> /* threads */
#include <time.h> /* time_t */
#include "uid.h" /* for uuid */
#include "sched.h"  /* API for the scheduler which we use */

#ifndef NDEBUG

    #define _BLUE_   "\x1B[1;34m"
    #define _PURPLE_ "\033[1;35m"
    #define _CYAN_   "\033[1;36m"
    #define _WHITE_  "\033[1;0m"
    #define _RESET_   "\033[1;0m"
    #define _GREEN_  "\033[1;32m"

#endif 

#define ERROR_HANDLE(expression)  if (0 != ((expression)))                  \
                                 {                                          \
                                    FreeAllocations();                     \
                                    return -1;                             \
                                 }

atomic_int proc_sig_flag = 0;

sem_t local_traffic_sem = {0};

char **global_argv = {0};

static void RestartProccess();
static int PrepareScheduler(size_t proc_interval,
                     size_t wd_interval, size_t wd_attempts);

static schedule_t *sched = NULL;

void HandleSignalFromProccess(int signum)
{
    printf(_CYAN_ "just recieved signal from proc \n");
    fflush(stdout);
    sem_post(&local_traffic_sem);
    ++proc_sig_flag;
    (void)signum;
}


int CheckProccess(void *param)
{
    size_t wd_attempts = (size_t)param;
    static atomic_size_t grace_counter = 0;
    if (0 != proc_sig_flag)
    {
        proc_sig_flag = 0;
    }
    else
    {
        ++grace_counter;
    }

    if (grace_counter > wd_attempts)
    {
        grace_counter = 0;
        printf(_CYAN_"\nWD  # %d NEEDS TO RESTART PROC!!!\n", getpid());
        RestartProccess();
        return 0;
    }

    (void)param;
    return 0;
}

static void RestartProccess()
{
    execvp(global_argv[0], global_argv);

    printf("WELL THAT DIDNT WORK!!\n");
}

int SignalToProccess(void *param)
{
    printf(_CYAN_ " WD # %d is still alive\n", getpid());
    kill(getppid(), SIGUSR1);

    (void)param;
    return 0;
}

int KillTask(void *params)
{
    (void)params;
    return 0;
}

int KillProccess(void *args)
{
    (void)args;
    kill(getppid(), SIGINT);
    return 1;
}

static void FreeAllocations()
{
    if (NULL != sched)
    {
        SchedDestroy(sched);
        sched = NULL;
    }

    sem_destroy(&local_traffic_sem);
}


int main (int argc, char *argv[])
{
    size_t wd_interval = atoi(getenv("wd_sig_inteval"));
    size_t proc_interval = atoi(getenv("proc_sig_inteval"));
    size_t wd_attempts = atoi(getenv("wd_attempts"));

    struct sigaction handle_proc_sig = {0};
    ERROR_HANDLE(PrepareScheduler(proc_interval, wd_interval, wd_attempts));
    
    global_argv = argv;

   
    /* printf(_CYAN_ "  kill -9  %d     -- and my parents pid is: %d\n", getpid(), getppid()); */

    /* printf("argv[0] is %s \n\n",global_argv[0]);
    fflush(stdout); */
    handle_proc_sig.sa_handler = HandleSignalFromProccess;
    sigaction(SIGUSR1, &handle_proc_sig, NULL);
  
    SchedAddTask(sched, time(NULL), proc_interval, CheckProccess, (void *)wd_attempts, KillTask, NULL);
    SchedAddTask(sched, time(NULL), wd_interval, SignalToProccess, NULL, KillTask, NULL);
    SchedAddTask(sched, time(NULL) + 4, 10, KillProccess, NULL, KillTask, NULL);
    
    ERROR_HANDLE(sem_init(&local_traffic_sem, 0, 0));
    (void)argc;
    /* envr = argv; */

    
    ERROR_HANDLE(kill(getppid(), SIGUSR1));
    
    sem_wait(&local_traffic_sem);
    
    proc_sig_flag = 0;

    SchedRun(sched);

    return 0;
}

static int PrepareScheduler(size_t proc_interval, size_t wd_interval, size_t wd_attempts)
{
    uuid_t uid1 = BAD_UID;
    uuid_t uid2 = BAD_UID;

    sched = SchedCreate();
    if (NULL == sched)
    {
        return -1;
    }

    uid1 =  SchedAddTask(sched, time(NULL), proc_interval, CheckProccess, (void *)wd_attempts, KillTask, NULL);
    uid2 = SchedAddTask(sched, time(NULL), wd_interval, SignalToProccess, NULL, KillTask, NULL);

    if (IsEqualUID(BAD_UID, uid1) || IsEqualUID(BAD_UID, uid2))
    {
        return -1;
    }

    return 0;
}
