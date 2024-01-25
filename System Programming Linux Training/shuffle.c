/******************************************************************************
* Author        Tzvi Moskowitz
* Description   Shuffles lines of file
* Group         OL110
* Company       ILRD Ramat Gan
* ****************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

static size_t curr_random_generator_size = 0;
static struct stat file_stats;
static unsigned char **word_array = NULL;

static int *random_generator_arr = NULL;
static unsigned char *file_ptr = NULL;
static size_t word_count = 0;
static int fd = 0;

static size_t MyStrLen(const char *str);
static void FillWordArray();
static void SetWordCountAndSwitchToNullTerm();
static void InitRandGenerator();
static void PrintShuffledWords();
static void CloseFilesAndFreeAllocs();
static size_t GetNextRandom();
static void PrepFileFromStdin(FILE *fd);

int main(int argc, char *argv[])
{
    time_t t;
    char *file_name = NULL;
    srand((unsigned) time(&t));
    
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

    file_ptr = mmap(NULL, file_stats.st_size, PROT_WRITE, MAP_PRIVATE, fd, 0);

    if (argc < 2)
    {
        remove(file_name);
    }

    SetWordCountAndSwitchToNullTerm();

    printf("HOW MANY LINES? %ld\n", word_count);
    curr_random_generator_size = word_count;

    word_array = (unsigned char **)malloc(sizeof(char *) * word_count);
    random_generator_arr = (int *)malloc(sizeof(size_t) * word_count);
    if (NULL == word_array || NULL == random_generator_arr)
    {
        CloseFilesAndFreeAllocs();
    }

    FillWordArray();

    InitRandGenerator();

    PrintShuffledWords();

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
    size_t w_count = 0;
    unsigned char *ptr = file_ptr;

    while ('\0' != *ptr)
    {
        if ('\n' == *ptr)
       {
           ++w_count;
         *ptr = '\0';
       }
       
       ++ptr;
    }
    
    word_count = w_count;
}



static void FillWordArray()
{
    size_t i = 0;
    unsigned char *ptr = file_ptr;
    
    for (i = 0; i < word_count; ++i)
    {
        word_array[i] = file_ptr;
        file_ptr += strlen((const char *)file_ptr) + 1;
    }

    file_ptr = ptr;
}

static void InitRandGenerator()
{
    size_t i = 0;
    for (i = 0; i < word_count; ++i)
    {
        random_generator_arr[i] = i;
    }
}

static void PrintShuffledWords()
{
    
    
    
   
   
    size_t curr_index = 0;
    size_t i = 0;
    
    for (i = 0; i < word_count; ++i)
    {
        curr_index = GetNextRandom();
        printf("word %ld: --> %s\n", i + 1, word_array[curr_index]);
    }

}

static size_t GetNextRandom()
{
    size_t rand_num = rand();
    size_t index = rand_num % curr_random_generator_size;
    size_t num = random_generator_arr[index];

    random_generator_arr[index] = random_generator_arr[curr_random_generator_size - 1];
    --curr_random_generator_size;

    return num;
}

static void CloseFilesAndFreeAllocs()
{
    munmap(file_ptr, file_stats.st_size);
    close(fd);

    if(word_array)
    {
        free(word_array);
        word_array = NULL;
    }

    if (random_generator_arr)
    {
        free(random_generator_arr);
        random_generator_arr = NULL;
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