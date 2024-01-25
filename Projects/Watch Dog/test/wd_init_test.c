#include <stdio.h>  /* printf   */
#include <fcntl.h>  /* O_CREAT  */
#include <semaphore.h> /* sem_t */


#define _RED_    "\033[1;31m"
#define _GREEN_  "\033[1;32m"
#define _WHITE_  "\033[0m"


typedef enum {
	SUCCESS,
	ERR_FORK,
	ERR_SEM_OPEN,
	ERR_SEM_CLOSE,
	ERR_SEM_POST,
	ERROR
} status_t;


/* semaphore permissions */
const mode_t all_r = (1 << 2) | (1 << 5) | (1 << 8);
const mode_t all_w = (1 << 1) | (1 << 4) | (1 << 7);
const mode_t all_r_w = all_r | all_w;


int InitSem(void);


int main()
{
	status_t status = SUCCESS;
	
	status = InitSem();
	
	if (SUCCESS != status)
	{
		printf("init semaphore error\n");
	}
	
	return status;
}


int InitSem()
{
	status_t status = SUCCESS;
	sem_t *sem_prog = NULL;
	const char *sem_name_prog = "/watchdog_test";
	int val = 0;
	
	sem_prog = sem_open(sem_name_prog, O_CREAT, all_r_w, 0);
	if (NULL == sem_prog)
	{
		return ERR_SEM_OPEN;
	}
	
	status = sem_post(sem_prog);
	if (SUCCESS != status)
	{
		return ERR_SEM_POST;
	}
	
	sem_getvalue(sem_prog, &val);
	printf("sem size (tester) : %d\n", val);
	
	status = sem_close(sem_prog);
	if (SUCCESS != status)
	{
		return ERR_SEM_CLOSE;
	}
	
	return SUCCESS;
}



