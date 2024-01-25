
/********************************************************
 * 
 * watchdog_test.c - Watch Dog testing
 * 
 * Author : Tuvia
 *
 * Reviewed and approved by : ???
 * 
 * 28-11-21
 * 
 * OLRD 110 Ramat Gan
 * 
 * *********************************************************/
#define _POSIX_C_SOURCE  200112L
#include <stdio.h>  /* printf   */
#include <stdlib.h> /* exit     */
#include <unistd.h> /* sleep    */
#include <signal.h> /* signals  */
#include <fcntl.h>  /* O_CREAT  */
#include <sys/ipc.h> /* shmget  */
#include <sys/shm.h> /* shmget  */
#include <pthread.h> /* threads */
#include <sys/types.h> /* pid_t */
#include <semaphore.h> /* sem_t */
#include <time.h>  /* timedwait */

#include "watch_dog.h"



#define _RED_    "\033[1;31m"
#define _GREEN_  "\033[1;32m"
#define _YELLOW_ "\033[1;33m"
#define _BLUE_   "\033[1;34m"
#define _PURPLE_ "\033[1;35m"
#define _CYAN_   "\033[1;36m"
#define _WHITE_  "\033[0m"

#define BLOCK_SIZE 1024

/* buffer size */
#define INT_STRING_LEN 30


/* semaphore permissions */
static const mode_t all_r = (1 << 2) | (1 << 5) | (1 << 8);
static const mode_t all_w = (1 << 1) | (1 << 4) | (1 << 7);
static const mode_t all_r_w = all_r | all_w;

static const size_t NUM_OF_INTERRUPTS = 3;
static const size_t NUM_OF_JOBS = 16;

static const unsigned int g_prog_attempts = 3;
static const unsigned int g_prog_interval = 1;
static const unsigned int g_wd_attempts = 2;
static const unsigned int g_wd_interval = 2;

extern pid_t g_pid_wd;
static pid_t g_pid_prog = 0;
static const char *g_wd_path = "./wd_app";
static const char *g_file_name_prog_pid = "worker_pid";
static const char *g_file_name_wd_pid = "wd_pid";


static int TestWD(int argc, char *argv[]);

static int RunInterrupter(void);
static void SendTerminations(void);

static int RunWorker(void);

static void TestWDStop(void);

static int SetPid(pid_t pid, const char *filename);
static pid_t GetPid(const char *filename);
static void Itoa(int num, char *buf);

static void PrintStatus(status_t status);


int main(int argc, char *argv[])
{
	status_t status = SUCCESS;
	
	status = TestWD(argc, argv);
	
	printf(_WHITE_ "\n");
	return status;
}


static int TestWD(int argc, char *argv[])
{
	status_t status = SUCCESS;
	
	printf(_CYAN_ "RESTART TEST PROGRAM" _WHITE_ "\n");
	
	g_pid_prog = fork();
	
	if (g_pid_prog < 0)
	{
		PrintStatus(ERR_FORK);
		return ERR_FORK;
	}
	else if (g_pid_prog > 0) /* parent */
	{	
		WDStart(argv, g_wd_path,
			g_prog_attempts, g_prog_interval,
			g_wd_attempts, g_wd_interval);
		
		status = RunWorker(); /* this is the saved section. */
		
		PrintStatus(status);
		
		printf(_CYAN_ "\nTest WD Stop" _WHITE_ "\n");
		WDStop();
		TestWDStop(); /* this isn't safe section. */
	}
	else
	{
		status = RunInterrupter();
		PrintStatus(status);
	}
	
	(void)argc;
	return status;
}


static int RunInterrupter()
{
	status_t status = SUCCESS;
	sem_t *sem_prog = NULL;
	const char *sem_name_prog = "/watchdog_test";
	
	sem_prog = sem_open(sem_name_prog, O_CREAT, all_r_w, 0);
	if (NULL == sem_prog)
	{
		return ERR_SEM_OPEN;
	}
	
	status = sem_trywait(sem_prog);
	if (SUCCESS != status)
	{
		exit(status);
	}
	
	sleep(1);
	SendTerminations();
	
	status = sem_close(sem_prog);
	if (SUCCESS != status)
	{
		printf(_RED_ "*** interrupter error ***\n" _WHITE_);
		PrintStatus(status);
	}
	
	exit(status);
}


static void SendTerminations()
{
	pid_t pid = 0;
	size_t i = 0;
	int delay = 0;
	
	/* few signals to program */
	for (i = 0; i < NUM_OF_INTERRUPTS; i++)
	{
		delay = g_prog_interval * (g_prog_attempts + 1) / 2;
		sleep(delay);
		printf(_PURPLE_ "-- SIGTERM ARRIVED to Program! --" _WHITE_ "\n");
		pid = GetPid(g_file_name_prog_pid);
		kill(pid, SIGTERM);
	}
	
	/* signal to watchdog */
	delay = g_prog_interval * (g_prog_attempts + 2);
	sleep(delay);
	printf(_PURPLE_ "-- SIGTERM ARRIVED to WatchDog! --" _WHITE_ "\n");
	pid = GetPid(g_file_name_wd_pid); /* need to change to wd */
	kill(pid, SIGTERM);
	
	/* 1 more signal to program */
	delay = g_wd_interval * (g_wd_attempts + 2);
	sleep(delay);
	printf(_PURPLE_ "-- SIGTERM ARRIVED to Program! --" _WHITE_ "\n");
	pid = GetPid(g_file_name_prog_pid);
	kill(pid, SIGTERM);
}


static int RunWorker()
{
	status_t status = SUCCESS;
	unsigned int i = 0;
	
	status = SetPid(getpid(), g_file_name_prog_pid);
	if (SUCCESS != status)
	{
		return status;
	}
	
	status = SetPid(g_pid_wd, g_file_name_wd_pid);
	if (SUCCESS != status)
	{
		return status;
	}
	
	for (i = 0; i < NUM_OF_JOBS; i++)
	{
		printf(_YELLOW_ "job %d\n" _WHITE_, i);
		sleep(1);
	}
	
	return SUCCESS;
}


static void TestWDStop()
{
	printf(_PURPLE_ "-- SIGTERM ARRIVED to Program! --" _WHITE_ "\n");
	printf(_GREEN_ "program abort successfully." _WHITE_ "\n");
	
	raise(SIGTERM);
	
	printf(_RED_ "Failed to Stop!" _WHITE_ "\n");
}


static int SetPid(pid_t pid, const char *filename)
{
	FILE *fd = NULL;
	char buf[INT_STRING_LEN] = "";
	
	fd = fopen(filename, "w");
	if (NULL == fd)
	{
		return ERR_FILE_OPEN;
	}
	
	Itoa(pid, buf);
	fputs(buf, fd);
	
	fclose(fd); fd = NULL;
	
	return SUCCESS;
}


static pid_t GetPid(const char *filename)
{
	FILE *fd = NULL;
	char buf[INT_STRING_LEN] = "";
	pid_t pid = 0;
	
	fd = fopen(filename, "r");
	if (NULL == fd)
	{
		return 0;
	}
	
	fgets(buf, INT_STRING_LEN, fd);
	pid = atoi(buf);
	
	fclose(fd); fd = NULL;
	
	return pid;
}





static void PrintStatus(status_t status)
{
	printf(_YELLOW_ "status: ");
	
	switch (status)
	{
		case SUCCESS:
			printf(_GREEN_ "SUCCESS");
			break;
		case ERR_ALLOC:
			printf(_RED_ "ALLOCATE MEMORY ERROR");
			break;
		case ERR_FORK:
			printf(_RED_ "FORK ERROR");
			break;
		case ERR_EXEC:
			printf(_RED_ "EXECUTE ERROR");
			break;
		case ERR_SEM_OPEN:
			printf(_RED_ "SEMAPHORE OPEN ERROR");
			break;
		case ERR_SEM_CLOSE:
			printf(_RED_ "SEMAPHORE CLOSE ERROR");
			break;
		case ERR_SEM_POST:
			printf(_RED_ "SEMAPHORE POST ERROR");
			break;
		case ERR_THRD:
			printf(_RED_ "THREAD CREATE ERROR");
			break;
		case ERR_KILL:
			printf(_RED_ "TERMINATION DELIVERY ERROR");
			break;
		case ERR_FILE_OPEN:
			printf(_RED_ "FILE OPEN ERROR");
			break;
		case ERR_SIG_HANDLER:
			printf(_RED_ "SIGNAL HANDLER ERROR");
			break;
		case ERR_MISCELLANEOUS:
			printf(_RED_ "OTHER ERROR");
			break;
		default:
			printf(_RED_ "UNKNOWN ERROR");
			break;
	}
	
	printf(_WHITE_ "\n");
}



static void Itoa(int num, char *buf_start)
{
	char *buf = buf_start;
	char *head = buf_start;
	char *tail = "";
	size_t size = 0;
	
	do {
		*buf++ = (num % 10) + '0';
		num /= 10;
		size++;
	} while (0 != num);
	
	tail = head + size - 1;
	
	/* reverse the reversed buf */
	while (head < tail)
	{
		char temp = *head;
		*head++ = *tail;
		*tail-- = temp;
	}
}





