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
static unsigned char *start = NULL;

static pthread_mutex_t hist_lock = PTHREAD_MUTEX_INITIALIZER;
static int length = 0;

static atomic_int global_histogram[256] = {0};
static unsigned char *global_ptr = NULL;

static atomic_int running = 1;

void *FillHistogram(void *args)
{
    int i = 0;
    
    unsigned char *curr_ptr = NULL;

    
    while (running)
    {
        pthread_mutex_lock(&hist_lock);
        curr_ptr = global_ptr;
        global_ptr += SWATH;
        pthread_mutex_unlock(&hist_lock);

        

        for (i = 0; i < SWATH;++i)
        {
            if ((curr_ptr - start) > (length))
            {
                atomic_store(&running, 0);
                break;
            }

        
            ++global_histogram[(int)*curr_ptr];
            
            ++curr_ptr;
        }
    }

    return NULL;
}

int main()
{
    clock_t before = {0};
    clock_t after = {0};
    
    int fd = open("/usr/share/dict/words", O_RDONLY, S_IRUSR | S_IWUSR);
    struct stat file_stats;
    if (-1 == fd)
    {
        return -1;
    }

    pthread_t t_arr[NUM_THREADS] = {0};
    int i = 0;

     if (-1 == fstat(fd, &file_stats))
    {
        return -1;
    }

    global_ptr = mmap(NULL, file_stats.st_size, PROT_WRITE, MAP_PRIVATE, fd, 0);
    
    length = file_stats.st_size;

    start = global_ptr;

    before = clock();

    for (; i < NUM_THREADS; ++i)
    {
         pthread_create(&t_arr[i],NULL, FillHistogram, NULL);
    }

     for (i = 0; i < NUM_THREADS; ++i)
    {
        pthread_join(t_arr[i], NULL);
    }

     after = clock();
    printf("shared histogram -- took %lu clock tics\n\n\n", (after - before));


    for (i = 'A'; i <= 'z'; ++i)
    {
        printf("%c: %d -- ", i, global_histogram[i]);
    }

    puts("\n");

    munmap(ptr, file_stats.st_size);
    close(fd);
    return 0;

}