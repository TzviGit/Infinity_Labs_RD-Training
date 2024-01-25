/* ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ *
 ~ Author        Tzvi Moskowitz                                      ~
 * Reviewer      --------                                            *
 ~ Description   Unrelated processes Ping Pong with pipe             ~
 * Group         OL110                                               *
 ~ Company       ILRD Ramat Gan                                      ~
 * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ */

#include <fcntl.h> // O_RDONLY, O_WRONLY, open()
#include <unistd.h> // write(), read()
#include <iostream> // std::cout, std::endl
#include <cstring> //strcpy()
#include <cstdlib>
#include <stdio.h>

static const int PING_PONG_CYCLES = 10;

static const char *pingPipeName = "/tmp/pingfifo";
static const char *pongPipeName = "/tmp/pongfifo";

typedef enum
{
   TERMINATE,
    CONTINUE
}err_status_t;


void HandleError(std::string err, err_status_t flag)
{
    perror(err.c_str());

    if (TERMINATE == flag)
    {
        std::exit(-1);
    }
}

int Pong(int write_fd, int read_fd);

int main()
{
    int read_fd = open(pingPipeName, O_RDONLY);
    if (-1 == read_fd)
    {
        unlink(pingPipeName);
        unlink(pongPipeName);
        HandleError("failure in open pingfifo", TERMINATE); 
    }

    int write_fd = open(pongPipeName, O_WRONLY);
    if (-1 == write_fd)
    {
        unlink(pingPipeName);
        unlink(pongPipeName);
        HandleError("failure in open pingfifo", TERMINATE); 
    }

    Pong(write_fd, read_fd);

    close(write_fd);
    close(read_fd);

    unlink(pingPipeName);
    unlink(pongPipeName);

    return 0;
}

int Pong(int write_fd, int read_fd)
{
    for (int i = 0; i < PING_PONG_CYCLES; ++i)
    {
        char pipeBuff[5] = {0};
        
        read(read_fd, pipeBuff, sizeof(pipeBuff));
        std::cout << "\033[1;36m" << pipeBuff << " --->\033[1;0m" << std::endl;

        strcpy(pipeBuff, "Pong");
        write(write_fd, pipeBuff, 5);
    }

    
    
    return 0;

}
