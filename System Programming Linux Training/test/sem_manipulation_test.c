
#include <semaphore.h>
#include <fcntl.h>      /* For O_* constants */
#include <sys/stat.h>  /*for mode constants*/
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

static char sem_name[20] = {'\0'}; 
static int undo_counter = 0;
sem_t *curr_semaphore = NULL;
static int undo_flag = 0;

int ProccessUndoesBeforeExit(int sig);
int ProccessInput(char **argv, sem_t **semaphore, unsigned int *change_amount, int *undo_flag);


int main(int argc, char **argv)
{
    unsigned int sem_change_amount = 0;
    
   
    int error_status = 0;
    struct sigaction interupt_catcher;

    interupt_catcher.sa_handler = ProccessUndoesBeforeExit;
    interupt_catcher.sa_flags = 0;

    sigaction(SIGINT, &interupt_catcher,NULL);

    if (argc < 4)
    {
        printf("\nplease enter the following: <I/V> <number> <[undo]>\n");
        return 1;
    }
    
    error_status = ProccessInput(&argv[1],&curr_semaphore, &sem_change_amount, &undo_flag);
    if (0 != error_status)
    {
        return -1;
    }

    ProccesSemaphoreChange(curr_semaphore, sem_change_amount);
}

int ProccessInput(char **argv, sem_t **semaphore, unsigned int *change_amount, int *undo_flag)
{
    char mode = argv[1];
    strcpy(sem_name, "/");
    
    strcat(sem_name, argv[0]);

    *semaphore = sem_open("/coolsemaphore",O_CREAT,S_IWUSR | S_IWOTH,0);
     if (SEM_FAILED == *semaphore)
    {
        kill(getpid(), SIGINT);
    }

    if (!("I" == mode || "D" == mode))
    {
        printf("\nplease enter the following: <I/V> <number> <[undo]>\n");
        return 1;
    }

    undo_flag = 
    {
        printf("\nplease enter the following: <I/V> <number> <[undo]>\n");
        return 1;
    }
    else
    {

    }
}


int ProccessUndoesBeforeExit(int sig)
{
    while (undo_counter > 0)
    {
        sem_wait(curr_semaphore);
    }

    while (undo_counter < 0)
    {
        sem_post(curr_semaphore);
    }

    exit (0);

}

    
    
    
    if (argc < 2)
        {
            printf("\nplease enter the following: <I/V> <number> <[undo]>\n");
            return 1;
        }

    
    = sem_open("/coolsemaphore",O_CREAT,S_IWUSR | S_IWOTH,0);
    if (SEM_FAILED == my_semaphore)
    {
        return -1;
    }

        
    char change_type = argv[1];

    if (NULL != argv[2] && argv[2] == "undo")



   
    return 0;
}


