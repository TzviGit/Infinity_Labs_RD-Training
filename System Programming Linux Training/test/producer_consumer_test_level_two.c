/**************************************************************************
 * Auther: Tzvi Moskowitz
 * 
 * 
 * 
 * 
 * 
 * **********************************************************************/

#include <semaphore.h> /*semaphores*/
#include <stdatomic.h>  /*atomic variables*/
#include <pthread.h> /*threads*/
#include <stdlib.h> /*exit*/
#include <stdio.h> /*printf*/
#include "queue.h" /*API for queue data structure*/

static const size_t MAX_ITERATIONS = 1000;
static const size_t MAX_CAPACITY = 50;

/*control how many production and consumer threads 
*  wil be created --> {MAX_THREADS for each} */
#define MAX_THREADS 8

pthread_mutex_t CONSUMER_LOCK = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t PRODUCER_LOCK = PTHREAD_MUTEX_INITIALIZER;

sem_t num_of_products = {0};

sem_t capacity = {0};

void *AssemblyLine(void *arg);
void ProduceNewData();
void ConsumeData();


static queue_t *data_container = NULL;


atomic_uint production_iterations = 0;
atomic_size_t consumer_iterations = 0;


int main(void)
{
    pthread_t producer[MAX_THREADS], consumer[MAX_THREADS];
    int i = 0;

    sem_init(&num_of_products, 0, 0);
    sem_init(&capacity, 0, MAX_CAPACITY);

    data_container = QCreate();
    for (i = 0; i < MAX_THREADS; ++i)
    {
        pthread_create(&producer[i], NULL, AssemblyLine, (void *)'P');
        pthread_create(&consumer[i], NULL, AssemblyLine, (void *)'C');
    }
   
    
    for (i = 0; i < MAX_THREADS; ++i)
    {
        pthread_join(producer[i], NULL);
        pthread_join(consumer[i], NULL);
    }
        
    return 0;
}

void *AssemblyLine(void *arg)
{
    if ('P' == (char)(size_t)arg)
    {
        ProduceNewData();
    }
    else
    {
        ConsumeData();
    }

return NULL;

}

void ProduceNewData()
{
   while (consumer_iterations < MAX_ITERATIONS) 
   {
        sem_wait(&capacity);
        pthread_mutex_lock(&PRODUCER_LOCK);

        QEnqueue(data_container, (void *)(size_t)++production_iterations);
        

        sem_post(&num_of_products);

        pthread_mutex_unlock(&PRODUCER_LOCK);
   }

    pthread_exit(0);
    

}


void ConsumeData()
{
   while (consumer_iterations < MAX_ITERATIONS)
   {
        sem_wait(&num_of_products);
        
        pthread_mutex_lock(&CONSUMER_LOCK);

        ++consumer_iterations;
        
        printf("%ld -> ", (long)QDequeue(data_container));

        sem_post(&capacity);

        pthread_mutex_unlock(&CONSUMER_LOCK);
   } 

    pthread_exit(0);
    
}


