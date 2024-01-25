#define POSIX_SOURCE 


#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
//#include <stdatomic.h>
#include "scope_lock.hpp" // API for Scopelock object
#include <boost/thread/mutex.hpp>
#include "util.hpp"
//#include <iostream>
 

class Pthread
{
public:
    explicit Pthread() throw(std::bad_alloc);
    ~Pthread() throw();

    int lock() throw();
    int unlock() throw();

private:
    pthread_mutex_t mutex;
};

Pthread::Pthread() throw(std::bad_alloc)
{
    mutex = PTHREAD_MUTEX_INITIALIZER;
}

Pthread::~Pthread() throw()
{
    //delete (mutex);
    pthread_mutex_destroy(&mutex);
}

int Pthread::lock() throw()
{
    return pthread_mutex_lock(&mutex);
}

int Pthread::unlock() throw()
{
    return pthread_mutex_unlock(&mutex);
}

/***************************************************
 * **************     ****************************/
using namespace ilrd;
static const int SWATH = 50000;
#define NUM_THREADS 8
static  char *ptr = NULL;
static char *start = NULL;

boost::mutex my_boost_lock;
Pthread my_lock;
static int length = 0;

static int running = 1;

void *FillHistogram(void *args)
{
    int i = 0;
    static __thread int  t_histogram[256] = {0};
    char *curr_ptr = NULL;

    
    while (running)
    {
       {
           //ScopeLock<boost::mutex> BoostLock(my_boost_lock);
           ScopeLock<Pthread> Lock(my_lock);
           curr_ptr = ptr;
           ptr += SWATH;
        
       }

        if ((curr_ptr - start) > (length))
        {
            running = 0;
            break;
        }

        for (i = 0; i < SWATH;++i)
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
    int fd = open("/usr/share/dict/words", O_RDONLY, S_IRUSR | S_IWUSR);
    struct stat file_stats;

    static int total_histogram[256] = {0};
    pthread_t t_arr[NUM_THREADS] = {0};
    int i = 0;

    if (-1 == fstat(fd, &file_stats))
    {
        return -1;
    }

    ptr = (char *)mmap(NULL, file_stats.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    
    length = file_stats.st_size;

    start = ptr;

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

    for (i = 'A'; i <= 'z'; ++i)
    {
        printf("%c: %d -- ", i, total_histogram[i]);
    }

    munmap(ptr, file_stats.st_size);
    close(fd);
    return 0;
}


void FalseSharingMethod()
{
    
}