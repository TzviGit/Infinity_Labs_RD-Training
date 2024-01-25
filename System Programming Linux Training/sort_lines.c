/******************************************************************************
* Author        Tzvi Moskowitz
* Description   sorts lines of file
* Group         OL110
* Company       ILRD Ramat Gan
* ****************************************************************************/

#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>



typedef struct q_sort_params
{
    void *base;
    size_t n_items;
    size_t size;
    int (*compare)(const void *, const void *);
}q_sort_params_t;

#define NUM_THREADS 12
static size_t curr_random_generator_size = 0;
static struct stat file_stats;
static unsigned char **lines_array = NULL;

static int *random_generator_arr = NULL;
static unsigned char *file_ptr = NULL;
static size_t line_count = 0;
static int fd = 0;

static void PrepFileFromStdin(FILE *fd);

static size_t MyStrLen(const char *str);
static void FillLinesArray();
static void SetWordCountAndSwitchToNullTerm();
static void CloseFilesAndFreeAllocs();
int VoidStrCmp(const void *v1, const void *v2);
static void SortLinesArray(char **lines_array, size_t lines);
static void PrintSorted();
static void *ThreadQuickSort(void *args);
static int MergeAllChunks(char **lines_table, size_t lines);
static void Merge(char **arr, char **help_arr,
                  size_t left, size_t middle, size_t right);

int main(int argc, char *argv[])
{
    time_t t;
    char *file_name = NULL;
    char **start = NULL;
    srand((unsigned)time(&t));

    if (argc > 1)
    {
        file_name = argv[1];
    }
    else
    {
        FILE *file = NULL;
        file_name = "tmp.txt";

        file = fopen(file_name, "a");
        if (NULL == file)
        {
            perror("failed in open file: ");
            return -1;
        }
        
        PrepFileFromStdin(file);

    }
    fd = open(file_name, O_RDONLY, S_IRUSR | S_IWUSR);

    if (-1 == fstat(fd, &file_stats))
    {
        return -1;
    }

    file_ptr = mmap(NULL,  file_stats.st_size , PROT_WRITE, MAP_PRIVATE, fd, 0);
    file_ptr[file_stats.st_size - 1] = 0;
    SetWordCountAndSwitchToNullTerm();

    printf("HOW MANY LINES? %ld\n", line_count);

    if (argc < 2)
    {
        remove(file_name);
    }

    lines_array = (unsigned char **)malloc(sizeof(char *) * line_count);

  
    if (NULL == lines_array)
    {
        CloseFilesAndFreeAllocs();
    }

    start = (char **)lines_array;
    FillLinesArray();

    if (argc < 2)
    {
        qsort(lines_array, line_count, sizeof(char **), VoidStrCmp);
    }
    else
    {
        SortLinesArray((char **)lines_array, line_count);
    }

    lines_array = (unsigned char **)start;

    PrintSorted();

    CloseFilesAndFreeAllocs();

    return 0;
}

static void PrepFileFromStdin(FILE *fd)
{
    
    char buffer[1000] = "";

    
    while (NULL != fgets(buffer, 50, stdin))
    {
        fputs(buffer, fd);
    }

    fclose(fd);
}

static  void SetWordCountAndSwitchToNullTerm()
{
    size_t l_count = 0;
    unsigned char *ptr = file_ptr;

    while ('\0' != *ptr)
    {
        if ('\n' == *ptr)
       {
           ++l_count;
         *ptr = '\0';
       }
       
       ++ptr;
    }
    
    line_count = l_count + 1;
}



static void FillLinesArray()
{
    size_t i = 0;
    unsigned char *ptr = file_ptr;
    
    for (i = 0; i < line_count; ++i)
    {
        lines_array[i] = file_ptr;
        file_ptr += strlen((const char *)file_ptr) + 1;
    }

    file_ptr = ptr;
}


static void PrintSorted()
{
    size_t i = 0;

    for (i = 0; i < line_count; ++i)
    {
        printf("word %ld: --> %s\n", line_count - (i + 1), lines_array[line_count - (i + 1)]);
    }
}



static void CloseFilesAndFreeAllocs()
{
    munmap(file_ptr, file_stats.st_size);
    close(fd);

    if(lines_array)
    {
        free(lines_array);
        lines_array = NULL;
    }

}

static size_t MyStrLen(const char *str)
{
    size_t count = 0;

    while('\0' != *str && '\n' != *str)
    {
        ++count;
        ++str;
    }

    return count;
}

 

static void *ThreadQuickSort(void *args)
{
    q_sort_params_t params = *(q_sort_params_t *)args;

    qsort(params.base, params.n_items, params.size, params.compare);

    return NULL;
}



/******************************************************************************
 *      CREDIT FOR THIS ALGORITHM -- AMATZIA SAFRA
 * 
 * ******************************************************************************/
static void SortLinesArray(char **l_array, size_t lines)
{
    pthread_t tid[NUM_THREADS] = {0};
    size_t i = 0;
    size_t chunk_size = lines / NUM_THREADS;
    q_sort_params_t params[NUM_THREADS] = {0};

    for (i = 0; i < NUM_THREADS; i++)
    {
        size_t curr_offset = i * chunk_size;

        params[i].size = sizeof(char *);
        params[i].compare = VoidStrCmp;
        params[i].base = l_array + curr_offset;
        params[i].n_items = chunk_size;

        if (NUM_THREADS - 1 == i)
        {
            params[i].n_items = lines - curr_offset;
        }

        pthread_create(tid + i, NULL, ThreadQuickSort, &params[i]);
        
    }

    for (i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(tid[i], NULL);
    }

    if (NUM_THREADS > 1)
    {
        if (MergeAllChunks(l_array, lines) != 0)
        {
            printf("\nmalloc error"); exit(4);
        }
    }
}

static int MergeAllChunks(char **l_array, size_t lines)
{
    size_t chunk_size = lines / NUM_THREADS;
    size_t i = 0;
    size_t part1_end = chunk_size - 1;
    size_t part2_end = 2 * chunk_size - 1;
    char **help_arr = (char **)malloc(lines * sizeof(char *));
    if (NULL == help_arr)
    {
        return -1;
    }

    for (i = 0; i < NUM_THREADS - 1; i++)
    {
        part1_end = (i + 1) * chunk_size - 1;
        part2_end = (i + 2) * chunk_size - 1;
        
        Merge(l_array, help_arr, 0, part1_end, part2_end);
    }

    Merge(l_array, help_arr, 0, part2_end, lines - 1);

    free(help_arr); help_arr = NULL;

    return 0;
}

static void Merge(char **arr, char **help_arr,
                  size_t left, size_t middle, size_t right)
{
    size_t left_index = left;
    size_t right_index = middle + 1;
    size_t helper_index = 0;

    while (left_index <= middle && right_index <= right)
    {
        if (VoidStrCmp(&arr[left_index], &arr[right_index]) < 0)
        {
            help_arr[helper_index++] = arr[left_index++];
        }
        else
        {
            help_arr[helper_index++] = arr[right_index++];
        }
    }

/* copies rest of left-side, if there is */
    memcpy(help_arr + helper_index, arr + left_index,
        		(middle - left_index + 1) * sizeof(char *));

/* copies rest of right-side, if there is */
/* (never it will be the left -and- right, but one of them) */
    memcpy(help_arr + helper_index, arr + right_index,
        		(right - right_index + 1) * sizeof(char *));

/* copies help_arr back into arr */
    memcpy(arr + left, help_arr, (right - left + 1) * sizeof(char *));
}




int VoidStrCmp(const void *v1, const void *v2)
{
    return strcmp(*(const char **)v1, *(const char **)v2);
}