/**************************************************************************
 * Auther: Tzvi Moskowitz
 * 
 * 
 * 
 * 
 * 
 * **********************************************************************/


#include <stdatomic.h>  /*atomic variables*/
#include <pthread.h> /*threads*/
#include <stdlib.h> /*exit*/
#include <stdio.h> /*printf*/

static const size_t MAX_ITERATIONS = 1000000;

void *AssemblyLine(void *arg);
void ProduceNewData();
void ConsumeData();



enum 
{
    PRODUCER,
    CONSUMER
}run_state_t;

static int data_container = 0;

atomic_uint run_state = PRODUCER;
atomic_uint production_iterations = 0;
atomic_size_t consumer_iterations = 0;


int main(void)
{
    pthread_t producer, consumer;

    pthread_create(&producer, NULL, AssemblyLine, (void *)'P');
    pthread_create(&consumer, NULL, AssemblyLine, (void *)'C');

    pthread_join(producer, NULL);
    pthread_join(consumer, NULL);


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
   while (production_iterations < MAX_ITERATIONS) 
   {
        while(CONSUMER == run_state);

        ++data_container;
        ++production_iterations;

        if (1 != data_container)
        {
            printf("\n Looks like your threads aren't following the traffic laws!!\n\n");
        }

        run_state = CONSUMER;
   }

    exit (0);
    

}


void ConsumeData()
{
   while (consumer_iterations < MAX_ITERATIONS)
   {
        while(PRODUCER == run_state);

        --data_container;
        ++consumer_iterations;

        if (0 != data_container)
        {
            printf("\n Looks like your threads aren't following the traffic laws!!\n\n");
        }

        run_state = PRODUCER;
   } 

    exit (0);
    
}


