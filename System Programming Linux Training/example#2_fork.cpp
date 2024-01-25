#include <stdio.h>  /* printf   */
#include <string.h> /* strlen   */
#include <unistd.h> /* sleep    */
#include <sys/types.h> /* pid_t */

#define SIZE 5


static int fd1[2] = {0};
static int fd2[2] = {0};

static int PingPongEx2(void);

static int Init(int fd1[], int fd2[]);
static void Clear(void);


int main()
{
    PingPongEx2();
    
    return 0;
}


static int PingPongEx2(void)
{
    int status = 0;
    pid_t pid = 0;
    
    status = Init(fd1, fd2);
    if (0 != status)
    {
        perror("bad pipe.\n");
        return -1;
    }
    
    pid = fork();
    
    if (pid < 0)
    {
        perror("bad fork.\n");
        return -1;
    }
    else if (pid > 0) /* parent */
    {
        size_t i = 0;
        char input[SIZE] = "";
        const char *output = "ping";
        
        for (i = 0; i < 6; i++)
        {
            write(fd1[1], output, SIZE);
            
            read(fd2[0], input, SIZE);
            printf("%s\n", input);
            sleep(1);
        }
        
        Clear();
    }
    else /* child */
    {
        size_t i = 0;
        char input[SIZE] = "";
        const char *output = "pong";
        
        for (i = 0; i < 6; i++)
        {
            read(fd1[0], input, SIZE);
            printf("%s\n", input);
            
            write(fd2[1], output, SIZE);
        }
        
        Clear();
    }
    
    return 0;
}


static int Init(int fd1[], int fd2[])
{
    int status = 0;
    
    status = pipe(fd1);
    if (0 != status)
    {
        return -1;
    }
    
    status = pipe(fd2);
    if (0 != status)
    {
        return -1;
    }
    
    return 0;
}


static void Clear(void)
{
    close(fd1[0]);
    close(fd1[1]);
    close(fd2[0]);
    close(fd2[1]);
}


