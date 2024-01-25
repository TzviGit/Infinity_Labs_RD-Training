

#include <stdio.h>
#include <pthread.h>
#include <stdatomic.h>
 
atomic_long acnt;
int cnt;
long secondary = 0;

void *f(void* thr_data)
{
    int n = 0;
    for (n = 0; n < 100000; ++n)
    {
        ++cnt;
        cnt;
        secondary = ++acnt;
        secondary = --acnt;
        secondary = --acnt;
        secondary = ++acnt;
        /* for this example, relaxed memory order is sufficient, e.g.
         atomic_fetch_add_explicit(&acnt, 1, memory_order_relaxed); */
    }
    return (void *)0;
}
 
int main(void)
{
    int n = 0;
    pthread_t thr[10];
    for(n = 0; n < 10; ++n)
        pthread_create(&thr[n],NULL,f, NULL);
    for( n = 0; n < 10; ++n)
        pthread_join(thr[n], NULL);
 
    printf("The atomic counter is %lu\n", secondary);
    printf("The non-atomic counter is %u\n", cnt);

    return 0;
}