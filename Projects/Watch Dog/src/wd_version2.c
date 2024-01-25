/*******************************************************
*   Author: Tzvi Moskowitz
*   Reviewer: Tuvia Resnik

*   Description: Implementation of Watchdog to revive a
*   proccess if it runs into trouble.
*
*   Date November 30, 2021
*   Company ILRD OL110 -- Ramat Gan
********************************************************/

#define _POSIX_C_SOURCE  200112L  /* getenv and threads */
#include <string.h> /* atoi */
#include <stdlib.h> /* setenvr  getenvr */
#include <stdio.h> /* sprintf */
#include <sys/types.h> /* fork */
#include <unistd.h> /*fork */
#include <stdatomic.h> /* atomic vars */
#include <signal.h> /* kill_func_for signals */
#include <fcntl.h>           /* For O_CREAT* constants */
#include <sys/stat.h>   /* For semaphore mode constants */
#include <semaphore.h> /* semaphores */
#include <pthread.h> /* threads */
#include <time.h>   /* time_t */
#include "wd.h"  /* API for this module */
#include "sched.h"  /* API for the scheduler which we use */
#include "uid.h"    /* for uuid */

#ifndef NDEBUG
    #define _RED_    "\033[1;31m"
    #define _GREEN_  "\033[1;32m"
    #define _BLUE_   "\x1B[1;34m"
    #define _PURPLE_ "\033[1;35m"
    #define _CYAN_   "\033[1;36m"
    #define _WHITE_  "\033[1;0m"
    #define _RESET_   "\033[1;0m"

#endif

/***********************************************************************
 ***  ERROR HANDLE MACROS  ********************************************/

#define ERROR_HANDLE(expression)  if (0 != ((expression)))                  \
                                 {                                          \
                                    FreeAllocations();                     \
                                    return -1;                             \
                                 }

#define POST_FORK_ERROR_HANDLE(expression) if (0 != ((expression)))        \
                                    {                                       \
                                        WDStop();                            \
                                        return -1;                            \
                                    }                                          


/***********    global and static vars *******************/
atomic_int wd_activated = 0;
atomic_int wd_signal_flag = 0;
pid_t g_pid_wd = 0;
static struct sigaction handle_wd_sig = {0};
static schedule_t *sched = NULL;

/* environment vars */
static int gl_prog_attempts = 0;
static int gl_prog_interval = 0;
static int gl_wd_attempts = 0;
static int gl_wd_interval = 0;
static char *gl_wd_name = "";
char **gl_argv = {0};

/* trafic semaphore - to syncronize wd with proc */
static sem_t traffic_sem = {0};


/*********  FUNCTION PROTOTYPES  ************************/
static void ConfirmInitialWatchDogContact(int signum);
static void HandleSignalFromWatchDog(int signum);
static int CheckWatchDog(void *param);
static int SignalWatchDog(void *param);
static void *RunScheduler(void *args);
static int PrepareScheduler();
static int KillTask(void *params);
static int SetSigHanlerToInitMode();
static int SetSigHandlerToStandardMode();
static int ExecuteWatchDogFork();
static int SetEnvironment();
static int StartWDInteracttionThread();
static int SignalInitlHandshakeToWatchDog();
static void FreeAllocations();
static void SetGlobalVariables(char *argv[], const char *wd_name,
		unsigned int prog_interval, unsigned int prog_attempts,
		unsigned int wd_interval, unsigned int wd_attempts);


/**********************************************************
 *****  THE SIGNAL HANDLERS  *****************************/

static void ConfirmInitialWatchDogContact(int signum)
{
    sem_post(&traffic_sem);
    (void)signum;
}

static void HandleSignalFromWatchDog(int signum)
{
    
#ifndef NDEBUG
     printf(_RED_ "just recieved signal from WD \n");
#endif

	++wd_signal_flag;
	(void)signum;
}



/**********************************************************************
 *  The WDStart functions and all its helper functions
 * 
 * ********************************************************************/

int WDStart(char *argv[], const char *wd_name,
            unsigned int prog_attempts, unsigned int prog_sig_interval,
            unsigned int wd_attempts, unsigned int wd_sig_interval)
{
    wd_activated = 1;

    

    SetGlobalVariables(argv, wd_name, prog_sig_interval, prog_attempts,
                       wd_sig_interval, wd_attempts);

    
    ERROR_HANDLE(sigaction(SIGUSR1, &handle_wd_sig, NULL));
    
    ERROR_HANDLE(sem_init(&traffic_sem, 0, 0));
   
    ERROR_HANDLE(PrepareScheduler());
    ERROR_HANDLE(SetEnvironment());
    
    ExecuteWatchDogFork();

    POST_FORK_ERROR_HANDLE(StartWDInteracttionThread());
    
    POST_FORK_ERROR_HANDLE(SignalInitlHandshakeToWatchDog());
    
    return 0;

   

    
}

/* **********************************************
******    SetGlobalVariabes   ***************** */

static void SetGlobalVariables(char *argv[], const char *wd_name,
		unsigned int prog_interval, unsigned int prog_attempts,
		unsigned int wd_interval, unsigned int wd_attempts)
{
	gl_argv = argv;
	gl_wd_name = (char *)wd_name;
	gl_prog_attempts = prog_attempts;
	gl_prog_interval = prog_interval;
	gl_wd_attempts = wd_attempts;
	gl_wd_interval = wd_interval;
}


/* **********************************************
******    SetEnvironment   *****************/

static int SetEnvironment()
{
    char buffer[20] = {0};
    int success_boolean = 1;

    success_boolean *=  (-1 < sprintf(buffer, "%d", gl_wd_interval));
    success_boolean *= (0 == setenv("wd_sig_inteval", buffer, 0));

    success_boolean *= (-1 < sprintf(buffer, "%d", gl_wd_attempts));
    success_boolean *= (0 == setenv("wd_attempts", buffer, 0));

    success_boolean *= (-1 < sprintf(buffer, "%d", gl_prog_interval));
    success_boolean *= (0 == setenv("proc_sig_inteval", buffer, 0));

    return !success_boolean;
}

/* **********************************************
******    ExecuteWatchDogFork   *****************/

static int ExecuteWatchDogFork()
{
    pid_t child_pid = 0;

    ERROR_HANDLE(SetSigHanlerToInitMode());

    child_pid = fork();
    if (-1 == child_pid)
    {
        ERROR_HANDLE(-1);
    }
    if (0 == child_pid)  
    {    
     
     POST_FORK_ERROR_HANDLE(execvp(gl_wd_name, gl_argv));  

    }
    else if (child_pid > 0)
    {
        g_pid_wd = child_pid;

        sem_wait(&traffic_sem);

        POST_FORK_ERROR_HANDLE(SetSigHandlerToStandardMode());
    }

     return 0;
}


/* ************* RunScheduler ********************
******    StartWDInteracttionThread   ***********/

static int StartWDInteracttionThread()
{
    pthread_t tid = 0;

    pthread_create(&tid, NULL, RunScheduler, NULL);

    return 0;
}

/****** The Threads starting point **************/

static void *RunScheduler(void *args)
{
    (void)args;

	pthread_detach(pthread_self());

    SchedRun(sched);

    return NULL;
}

/****** SignalInitlHandshakeToWatchDog   ******/

static int SignalInitlHandshakeToWatchDog()
{
   POST_FORK_ERROR_HANDLE(kill(g_pid_wd, SIGUSR1));

    return 0;
}


/************************************************************
 * ~ The Scheduler and its Tasks -- run at chronic intervals
 *
 ***********************************************************/
static int PrepareScheduler()
{
    uuid_t uid1 = BAD_UID;
    uuid_t uid2 = BAD_UID;

    sched = SchedCreate();
    if (NULL == sched)
    {
        return -1;
    }

    uid1 =  SchedAddTask(sched, time(NULL), gl_wd_interval,
                             CheckWatchDog, NULL, KillTask, NULL);
    
    uid2 = SchedAddTask(sched, time(NULL), gl_prog_interval,
                            SignalWatchDog, NULL, KillTask, NULL);

    if (IsEqualUID(BAD_UID, uid1) || IsEqualUID(BAD_UID, uid2))
    {
        return -1;
    }

    return 0;
}

 
/*******    CHECK ON THE WD  ITSELF   ********************/
static int CheckWatchDog(void *param)
{
    static atomic_int grace_counter = 0;
    
    if (!wd_activated)
    {
        pthread_exit(0);
    }

    if (0 != wd_signal_flag)
    {
        wd_signal_flag = 0;
        grace_counter = 0;
    }
    else
    {
        ++grace_counter;
    }

    if (grace_counter > gl_prog_attempts)
    {
        /* WD has fell and we will now revive him */
       
#ifndef NDEBUG
    printf(_RED_"\nTid #%ld NEEDS TO RESTART WD!!!\n", pthread_self());
#endif
 
        grace_counter = 0;

        ExecuteWatchDogFork();
     }

    (void)param;
    return 0;
}

/**************** SignalWatchDog  *************************
**** SEND SIGS TO WD TO VERIFY OUR EXISTENCE  ************/

static int SignalWatchDog(void *param)
{
   
#ifndef NDEBUG
     printf(_GREEN_ " PROC # %d is sending sig to WD\n", getpid());
#endif

    POST_FORK_ERROR_HANDLE(kill(g_pid_wd, SIGUSR1));

    (void)param;
    return 0;
}

/*******************************************************
 ***  DUMMY FUNC TO SATISFY SCHED API ******************/

static int KillTask(void *params)
{
    (void)params;
    
    return 0;
}


/* **********************************************
******   the 2 Set signal handler functions   ******/

static int SetSigHanlerToInitMode()
{
    handle_wd_sig.sa_handler = ConfirmInitialWatchDogContact;
    sigaction(SIGUSR1, &handle_wd_sig, NULL);

    return 0;
}

static int SetSigHandlerToStandardMode()
{
    handle_wd_sig.sa_handler = HandleSignalFromWatchDog;
    sigaction(SIGUSR1, &handle_wd_sig, NULL);

    return 0;
}


/* **********************************************
*       WDStop function -- stops the WD
*           and frees up the resources
************************************************/

int WDStop()
{
    int status = 0;
    wd_activated = 0;
    status = kill(g_pid_wd, SIGTERM);
    FreeAllocations();

    return status;
}

/* **********************************************
******    FreeAllocations   *******************/

static void FreeAllocations()
{
    if (NULL != sched)
    {
        SchedDestroy(sched);
        sched = NULL;
    }

    sem_destroy(&traffic_sem);
}
