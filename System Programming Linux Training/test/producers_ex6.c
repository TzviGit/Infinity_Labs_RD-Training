


#define _POSIX_C_SOURCE 200112L
#include <semaphore.h> /*semaphores*/
#include <stdatomic.h>  /*atomic variables*/
#include <pthread.h> /*threads*/
#include <stdlib.h> /*exit*/
#include <stdio.h> /*printf*/



#define NUM_OF_READERS 10
static size_t rounds = 10;
static pthread_barrier_t barrier = {0};
static int data_container = 0;
pthread_mutex_t MY_LOCK = PTHREAD_MUTEX_INITIALIZER;

sem_t read_semaphore = {0};

void *ReadAndRegroup(void *arg);
void *WriteData(void *args);

int main(void)
{
    pthread_t readers[NUM_OF_READERS] = {0};
    pthread_t writer = {0};
    size_t i = 0;
    pthread_barrierattr_t atr = {0};
    pthread_barrierattr_init(&atr);
    pthread_barrier_init(&barrier, &atr, NUM_OF_READERS + 1);
    sem_init(&read_semaphore, 0, 0);

    for (i = 0; i < rounds; ++i)
    {
        pthread_create(&readers[i], NULL, ReadAndRegroup, NULL);
    }

    pthread_create(&writer, NULL, WriteData, NULL);

    for (i = 0; i < rounds; ++i)
    {
        pthread_join(readers[i], NULL);
    }
    
    pthread_join(writer, NULL);

    sem_destroy(&read_semaphore);
    pthread_barrier_destroy(&barrier);
    pthread_mutex_destroy(&MY_LOCK);

    return 0;
}

void *WriteData(void *args)
{
    size_t i = 0;

    for (i = 0; i < rounds; ++i)
    {
        size_t j = 0;
        data_container = (long)i;
        
        for (j = 0; j < NUM_OF_READERS; ++j)
        {
            sem_post(&read_semaphore);
        }

        pthread_barrier_wait(&barrier);
        
        printf("\n");
    }

    (void)args;
    return NULL;
}


void *ReadAndRegroup(void *arg)
{
    size_t i = 0;

    for (i = 0; i < rounds; ++i)
    {
        sem_wait(&read_semaphore);

        printf("%d ", data_container);
		
        pthread_barrier_wait(&barrier);
    }

    (void)arg;
    return NULL;
}