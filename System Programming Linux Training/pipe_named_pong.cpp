#include <stdio.h>  /* printf   */
#include <unistd.h> /* sleep    */
#include <fcntl.h>  /* open     */
#include <sys/stat.h>  /* mkfifo */

#define SIZE 5


static int fd1 = 0;
static int fd2 = 0;

static const char *pipe_ping = "/tmp/pipe_ping";
static const char *pipe_pong = "/tmp/pipe_pong";

static int PongEx3(void);

static int Init(void);
static void Clear(void);


int main()
{
    PongEx3();
    
    return 0;
}


static int PongEx3(void)
{
    int status = 0;
    size_t i = 0;
    char input[SIZE] = "";
    const char *output = "pong";
    
    status = Init();
    if (0 != status)
    {
        perror("");
        Clear();
        return -1;
    }
    
    for (i = 0; i < 6; i++)
    {
        read(fd1, input, SIZE);
        printf("%s\n", input);
        
        write(fd2, output, SIZE);
        sleep(1);
    }
    
    Clear();
    
    return 0;
}


static int Init()
{
    fd1 = open(pipe_ping, O_RDONLY);
    if (-1 == fd1)
    {
        return -1;
    }
    
    fd2 = open(pipe_pong, O_WRONLY);
    if (-1 == fd2)
    {
        return -1;
    }
    
    return 0;
}


static void Clear(void)
{
    close(fd1);
    close(fd2);
}

