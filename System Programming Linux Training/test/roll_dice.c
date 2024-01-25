
#define _POSIX_C_SOURCE 200112L
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define NUM_THREADS 16

static int diceValues[NUM_THREADS] = {0};
static int status_arr[NUM_THREADS] = {0};

pthread_barrier_t dice_barrier = {0};
pthread_barrier_t calc_barrier = {0};

void *RollDice(void *args)
{
    int index = (long)args;
    diceValues[index] = (rand() % 6) + 1;

    pthread_barrier_wait(&dice_barrier);
    pthread_barrier_wait(&calc_barrier);

    if (1 == status_arr[index])
    {
        printf("(%d rolled %d) -- I won\n,", index, diceValues[index]);
    }
    else
    {
        printf("(%d rolled %d) -- I lost\n,", index, diceValues[index]);
    }

    return NULL;
}

int main(void)
{
    pthread_t threads[NUM_THREADS] = {0};
    size_t i = 0;
    int max_dice = 0;

    pthread_barrier_init(&dice_barrier, NULL, NUM_THREADS + 1);
    pthread_barrier_init(&calc_barrier, NULL, NUM_THREADS + 1);

    for (i = 0; i <NUM_THREADS; ++i)
    {
       if(pthread_create(&threads[i], NULL, RollDice, (void *)i))
       {
           perror("failed to create thread\n");
       }
    }

    pthread_barrier_wait(&dice_barrier);

    for (i = 0; i <NUM_THREADS; ++i)
    {
       if(diceValues[i] > max_dice)
       {
           max_dice = diceValues[i];
       }
       
    }

    for (i = 0; i <NUM_THREADS; ++i)
    {
        status_arr[i] = diceValues[i] == max_dice ? 1 : 0;
    }

    pthread_barrier_wait(&calc_barrier);

    for (i = 0; i <NUM_THREADS; ++i)
    {
       if(pthread_join(threads[i], NULL))
       {
           perror("failed to join thread\n");
       }
    }


    pthread_barrier_destroy(&dice_barrier);
    pthread_barrier_destroy(&calc_barrier);

    return 0;
}