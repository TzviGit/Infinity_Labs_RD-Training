#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h> 
#include <time.h>
#include <stddef.h>
#include <stddef.h>



pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
static const size_t NUM_THREADS = 30;
static const size_t number = 10000000;
static size_t partial_sums_arr[NUM_THREADS] = {0};
static const long jump = 200;

static size_t sum_of_divisors = 0;

typedef struct range
{
    size_t lower_range;
    size_t Upper_range;
}range_t;

size_t PartialSumOfDivisors(size_t num, size_t lower, size_t upper)
{
    size_t partial_sum_of_divisors = 0;
    size_t i = 0;

    for (i = lower; i <= num && i <= upper; ++i)
    {
        if (0 == num % i)
        {
            partial_sum_of_divisors += i;
        }
    }

    return partial_sum_of_divisors;
}

void *AddPartialSum(void *arg)
{
    
    size_t partial_sum = 0;
    size_t lower = (size_t)arg + 1;
    size_t upper = lower + jump;

    partial_sum = PartialSumOfDivisors( lower, upper);

    pthread_mutex_lock(&lock1);

    sum_of_divisors += partial_sum;

    pthread_mutex_unlock(&lock1);

    return NULL;
    
}


int main(int argc, char const *argv[])
{
    pthread_t *tids_arr = (pthread_t *)malloc(NUM_THREADS * sizeof(*tids_arr));
    size_t i = 0;
    size_t j = 0;

    for (i = 0; i < number; i += (NUM_THREADS * jump))
    {   
        tids_arr = realloc(tids_arr, NUM_THREADS * sizeof(*tids_arr));
        for (j = 0; j < NUM_THREADS; j += jump)
        {

            pthread_create(&tids_arr[i], NULL, AddPartialSum, (void *)(size_t)j);
        }


        for (j = 0; j < NUM_THREADS; ++j)
        {
            pthread_join(tids_arr[j], NULL);
        }
    }


    printf("\n\nFinal result is:   %lu \n\n", sum_of_divisors);

    free(tids_arr); tids_arr = NULL;
    
    return 0;
}