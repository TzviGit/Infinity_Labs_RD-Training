#define _XOPEN_SOURCE 600

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define threds_num (5)
static pthread_key_t key;
static pthread_barrier_t bar;
static pthread_mutex_t lock;


int global1 = 1;
int global2 = 2;
int global3 = 3;

static int g_stat1 = 1;
static int g_stat2 = 2;
static int g_stat3 = 3;

void *PrintAdrresses(void *value)
{
    int local1 = 0, local2 = 1, local3 = 2;
    static int stat1 = 1, stat2 = 2, stat3 = 3;
    void *tls = NULL;

    int *dynamic1 = (int *)malloc(2 * (sizeof(int)));
    int *dynamic2 = (int *)malloc(2 * (sizeof(int)));
    int *dynamic3 = (int *)malloc(2 * (sizeof(int)));

    pthread_setspecific(key, value); 

    pthread_barrier_wait(&bar);

    printf("\n");
    pthread_barrier_wait(&bar);
    tls = pthread_getspecific(key);
    printf("in thread num %ld, thread local storage address is %lu\n", (size_t)tls, (void *)&tls);

    pthread_barrier_wait(&bar);

    printf("\n");
    pthread_barrier_wait(&bar);
    pthread_mutex_lock(&lock);
    printf("\n");
    printf("in thread num %ld:\n", (size_t)tls);
    printf("local1 address is %p\n", (void *)&local1);
    printf("local2 address is %p\n", (void *)&local2);
    printf("local3 address is %p\n", (void *)&local3);
    pthread_mutex_unlock(&lock);

    pthread_barrier_wait(&bar);

    printf("\n");
    pthread_barrier_wait(&bar);
    pthread_mutex_lock(&lock);
    printf("\n");
    printf("in thread num %ld:\n", (size_t)tls);
    printf("dynamic1 address is %p\n", (void *)dynamic1);
    printf("dynamic2 address is %p\n", (void *)dynamic2);
    printf("dynamic3 address is %p\n", (void *)dynamic3);
    pthread_mutex_unlock(&lock);

    pthread_barrier_wait(&bar);


    pthread_barrier_wait(&bar);

    printf("\n");
    pthread_barrier_wait(&bar);
    printf("global1 address is %p\n", (void *)&global1);
    pthread_barrier_wait(&bar);
    printf("global2 address is %p\n", (void *)&global2);
    pthread_barrier_wait(&bar);
    printf("global3 address is %p\n", (void *)&global3);
    pthread_barrier_wait(&bar);


    pthread_barrier_wait(&bar);

    printf("\n");
    pthread_barrier_wait(&bar);
    printf("g_stat1 address is %p\n", (void *)&g_stat1);
    pthread_barrier_wait(&bar);
    printf("g_stat2 address is %p\n", (void *)&g_stat2);
    pthread_barrier_wait(&bar);
    printf("g_stat3 address is %p\n", (void *)&g_stat3);


    pthread_barrier_wait(&bar);

    printf("\n");
    pthread_barrier_wait(&bar);
    printf("stat1 address is %p\n", (void *)&stat1);
    pthread_barrier_wait(&bar);
    printf("stat2 address is %p\n", (void *)&stat2);
    pthread_barrier_wait(&bar);
    printf("stat3 address is %p\n", (void *)&stat3);
    

	return NULL;
}


int MultiThreadProgram(void)
{
	
    size_t i = 0;
    int status = 0;
    pthread_t thread_id[threds_num] = {0};
    status = pthread_key_create(&key, NULL);
     if (status != 0)
    {
        return status;
    }

    /*initialize a barrier*/

    status = pthread_barrier_init(&bar, NULL, threds_num); 
    if (0 != status)
    {
        printf("barrier init has failed\n");
        return status;
    }

    /*initialize a mutex*/
    status = pthread_mutex_init(&lock, NULL);
    if (0 != status)
    {
        printf("mutex init has failed\n");
        return status;
    }

    /*create threads*/
  	for (i = 0; i < threds_num; i++)
	{
        status = pthread_create(&thread_id[i], NULL, PrintAdrresses, (void *)i);
        if (status != 0)
        {
            return status;
        }
	}
        
    /*join threads*/
    for (i = 0; i < threds_num; ++i)
    {
        pthread_join(thread_id[i], NULL);
    }

    status = pthread_key_delete(key);
    if (status != 0)
    {
        return status;
    }

    /*destroy barrier*/
    status = pthread_barrier_destroy(&bar);
    if (0 != status)
    {
        printf("failed to desrtoy barrier\n");
        return status;
    }

    /*destroy mutex*/
    pthread_mutex_destroy(&lock);

	return 0;
}


int main(void)
{
	return MultiThreadProgram();
}