
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>
#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <cstdlib>
#include <cstdio>

static const mode_t all_r = (1 << 2) | (1 << 5) | (1 << 8);
static const mode_t all_w = (1 << 1) | (1 << 4) | (1 << 7);
static const mode_t all_r_w = all_r | all_w;

static void Pong(sem_t *ping_sem, sem_t *pong_sem);
void *CheckSem(void *args);
sem_t *ping_sem = NULL;
sem_t *pong_sem = NULL;

int main()
{
    // sem_unlink("./tzvi");
    // sem_unlink("./moskowitz");
    
    ping_sem = sem_open("/ping_ex1", O_CREAT, all_r_w,0);
    pong_sem =  sem_open("/pong_ex1", O_CREAT, all_r_w, 0);

    if (SEM_FAILED == ping_sem )
    {
        std::cout << "failed to open ping semaphore\n";
         perror(NULL);
        exit(1);
    }

    if (SEM_FAILED == pong_sem)
    {
        std::cout << "failed to open pong semaphore\n";
        perror(NULL);
        exit(1);
    }

    Pong(ping_sem, pong_sem);
    return 0;
}

static void Pong(sem_t *ping_sem, sem_t *pong_sem)
{
    pthread_t tid;
    pthread_create(&tid, NULL, CheckSem, reinterpret_cast<void *>(pong_sem));
    int i = 0; 
    while (i < 10)
    {
        sem_post(ping_sem);
        
        sem_wait(pong_sem);
        std::cout << "pong\n";
        
    }

   pthread_join(tid, NULL);
}

void *CheckSem(void *args)
{
    int i = 0;
    int sem_val;
    sem_t *sem = reinterpret_cast<sem_t *>(args);
    while(1)
    {
        sem_getvalue(sem, &sem_val);
        i += (sem_val == 0) ? 1 : 0;
        if (i == 3)
        {
            exit(2);
        }
        sleep(1);
    }
}