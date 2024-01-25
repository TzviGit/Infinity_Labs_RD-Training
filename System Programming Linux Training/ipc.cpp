

#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
static void Ping(sem_t *ping_sem, sem_t *pong_sem);

static const mode_t all_r = (1 << 2) | (1 << 5) | (1 << 8);
static const mode_t all_w = (1 << 1) | (1 << 4) | (1 << 7);
static const mode_t all_r_w = all_r | all_w;

sem_t *ping_sem = NULL;
sem_t *pong_sem = NULL;

char **g_argv;
int main(int argc,char *argv[])
{
    (void)argc;
    g_argv = argv;
    ping_sem = sem_open("/ping_ex1", O_CREAT, all_r_w, 0);
    
    pong_sem = sem_open("/pong_ex1", O_CREAT, all_r_w, 0);

    if (SEM_FAILED == ping_sem || SEM_FAILED == pong_sem)
    {
        std::cout << "failed to open semaphore\n";
        perror(NULL);
        exit(1);
    }

    // int sem_val = 0;
    // sem_getvalue(ping_sem, &sem_val);
    // printf("SEMVAL NOW IS ... %d\n", sem_val);
    // while (sem_val < 1)
    // {
    //     sem_post(ping_sem);
    // }
       //printf("SEMVAL IS ... %d\n", sem_val);
    Ping(ping_sem, pong_sem);

    sem_unlink("/pong_ex1");
    sem_unlink("/ping_ex1");
    return 0;
}

static void Ping(sem_t *ping_sem, sem_t *pong_sem)
{
    pid_t pid = fork();
    if (0 == pid)
    {
        execvp("./ipc#2", g_argv);
    }

    
    
    int i = 0;
    while (i++ < 10)
    {
        //  int sem_val;
        // sem_getvalue(ping_sem, &sem_val);

        // printf("SEMVAL IS ... %d\n", sem_val);

        sem_wait(ping_sem);
        std::cout << "ping\n";
        sem_post(pong_sem);
        }
}