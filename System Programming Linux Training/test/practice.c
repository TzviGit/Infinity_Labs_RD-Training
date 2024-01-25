#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h> 
#include <time.h>


#define  MAX_SIZE 21
pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;

static int global_array[MAX_SIZE] = {0};
 
void *FillMyIndex(void *vargp)
{
    global_array[(size_t)vargp] = (size_t)vargp;

    pthread_exit (NULL);
}

int main()
{
    time_t before, after;
	int i, j;
	pthread_t tid_array[MAX_SIZE] = {0};
    pthread_attr_t tattr;
    
    int *arr = malloc(20);

    arr[0] = 49;
    arr[2] = 50;

    printf("hello");
    pthread_attr_init(&tattr);
    pthread_attr_setdetachstate(&tattr,PTHREAD_CREATE_DETACHED);

	time(&before);
	for (i = 0; i < MAX_SIZE; ++i)
    {
        /*global_array[i] = i;*/
       pthread_create(&tid_array[i], &tattr, FillMyIndex, (void *)(size_t)i);

    }
      
    
   sleep(1);
    

    
    for (i = 0; i < MAX_SIZE; ++i)
    {
        if (!(i % 10000))
        {
            printf("%d -> ", global_array[i]); 
        }

        
        
        if (i != global_array[i])
        {
            printf("\n\nwhoops,  that didn't work!!\n\n");
        }

    }
		/*printf("%d,  %d", arr[1000] + arr[1001], arr[200000]- arr[10033]);*/

	return 0;
}
