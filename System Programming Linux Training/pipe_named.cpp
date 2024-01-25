#include <stdio.h>  /* printf   */
#include <unistd.h> /* sleep    */
#include <fcntl.h>  /* open     */
#include <sys/stat.h>  /* mkfifo */

#define SIZE 5


static int fd1 = 0;
static int fd2 = 0;

static const char *pipe_ping = "/tmp/pipe_ping";
static const char *pipe_pong = "/tmp/pipe_pong";

static int PingEx3(void);

static int Init(void);
static void Clear(void);


int main()
{
    PingEx3();
    
    return 0;
}


static int PingEx3(void)
{
    int status = 0;
    size_t i = 0;
    char input[SIZE] = "";
    const char *output = "ping";
    
    status = Init();
    if (0 != status)
    {
        perror("");
        Clear();
        return -1;
    }
    
    for (i = 0; i < 6; i++)
    {
        write(fd1, output, SIZE);
        
        read(fd2, input, SIZE);
        printf("%s\n", input);
        sleep(1);
    }
    
    Clear();
    
    return 0;
}


static int Init()
{
    int status = 0;
    
    status = mkfifo(pipe_ping, 0666);
    if (0 != status)
    {
        return -1;
    }
    
    status = mkfifo(pipe_pong, 0666);
    if (0 != status)
    {
        return -1;
    }
    
    fd1 = open(pipe_ping, O_WRONLY);
    if (-1 == fd1)
    {
        return -1;
    }
    
    fd2 = open(pipe_pong, O_RDONLY);
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
    remove("/tmp/pipe_ping");
    remove("/tmp/pipe_pong");
}



