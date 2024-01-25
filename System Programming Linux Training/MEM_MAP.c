#include <stdio.h>   /* printf() */
#include <stdlib.h>  /* malloc() */
#include <pthread.h> /* threads  */


#define NUM_OF_THREADS 4


static int static_non_const_global_var = 8;
static const int static_const_global_var = 4;

/*static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;*/

__thread int TLS_i1 = 0;
__thread int TLS_i2 = 0;

static void *Inc(void *arg)
{	
    int j = 0;
    
    for (TLS_i1 = 0; TLS_i1 < 5; TLS_i1++)
    {
		static_non_const_global_var++;
    }
    
    for (TLS_i2 = 0; TLS_i2 < 5; TLS_i2++)
    {
		static_non_const_global_var++;
    }
    
    printf("TLS1   var: %p\n", (void *)&TLS_i1);
    printf("TLS2   var: %p\n", (void *)&TLS_i2);
    printf("local  var: %p\n", (void *)&j);
    printf("global var: %p\n", (void *)&static_non_const_global_var);
	
    (void)arg;
    return NULL;
}


int main(int argc, char **argv, char **envp)
{
    void *heap_var = NULL;
    int non_const_local_var = 3;
    const int const_local_var = 9;
    static int static_non_const_local_var = 38;
    static const int static_const_local_var = 99;
    char *str_literal_ptr = "string_literal";
    size_t i = 0;
    
    pthread_t tid[NUM_OF_THREADS] = {0};
	
	for (i = 0; i < NUM_OF_THREADS; i++)
	{
		pthread_create(&tid[i], NULL, Inc, NULL);
	}
	
	for (i = 0; i < NUM_OF_THREADS; i++)
	{
		pthread_join(tid[i], NULL);
	}
	
    heap_var = malloc(1);
	
	printf("\n");
	printf("var: %d\n", static_non_const_global_var);
	
    (void)heap_var;
    (void)non_const_local_var;
    (void)const_local_var;
    (void)static_non_const_local_var;
    (void)static_const_local_var;
    (void)static_non_const_global_var;
    (void)static_const_global_var;
    (void)str_literal_ptr;
    (void)argc;
    (void)argv;
    (void)envp;
	
    free(heap_var); heap_var = NULL;
	
    return 0;
}

