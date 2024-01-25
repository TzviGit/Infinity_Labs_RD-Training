/* #define POSIX_SOURCE */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdatomic.h>

static const int SWATH = 50000;
#define NUM_THREADS 8
static  char *ptr = NULL;
static char *start = NULL;

static pthread_mutex_t my_lock = PTHREAD_MUTEX_INITIALIZER;
static int length = 0;

static atomic_int running = 1;

void *FillHistogram(void *args)
{
    int i = 0;
    static __thread int  t_histogram[256] = {0};
    char *curr_ptr = NULL;
    /* printf("%p   %p   %p   %ld    %ld\n", (void *)&t_histogram[2], (void *)&i, (void *)&length, (long)((char *)&t_histogram[2] - (char *)&i), (long)((char *)&t_histogram[2] - (char *)&length)); */
    
    while (running)
    {
        pthread_mutex_lock(&my_lock);
        curr_ptr = ptr;
        ptr += SWATH;
        
        pthread_mutex_unlock(&my_lock);

        if ((curr_ptr - start) > (length))
        {
            running = 0;
            break;
        }
        
        for (i = 0; i < SWATH; ++i)
        {
            ++t_histogram[(int)*curr_ptr];

            if ('\0' == *curr_ptr)
            {
                running = 0;
                break;
            }

            ++curr_ptr;
        }
    }
    
    return (void *)((int *)t_histogram);
}

int main()
{
    clock_t before = {0};
    clock_t after = {0};

    int fd = open("/usr/share/dict/words", O_RDONLY, S_IRUSR | S_IWUSR);
    struct stat file_stats;

    static int total_histogram[256] = {0};
    pthread_t t_arr[NUM_THREADS] = {0};
    int i = 0;

    if (-1 == fstat(fd, &file_stats))
    {
        return -1;
    }

    ptr = mmap(NULL, file_stats.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    
    length = file_stats.st_size;

    start = ptr;

    before = clock();
    
    

    for (; i < NUM_THREADS; ++i)
    {
         pthread_create(&t_arr[i],NULL, FillHistogram, NULL);
    }

    for (i = 0; i < NUM_THREADS; ++i)
    {
        int j = 0;
        void *curr_hist;
        pthread_join(t_arr[i], &curr_hist);

        for (; j < 256; ++j)
        {
            total_histogram[j] += ((int *)curr_hist)[j];
        }
    }

    after = clock();
    printf("private histogram -- took %lu clock tics\n\n\n", (after - before));

    for (i = 'A'; i <= 'z'; ++i)
    {
        printf("%c: %d -- ", i, total_histogram[i]);
    }

    puts("\n");

    munmap(ptr, file_stats.st_size);
    close(fd);
    return 0;
}

