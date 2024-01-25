/* ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ *
 ~ Author        Tzvi Moskowitz                                  ~
 * Reviewer      -----                                               *
 ~ Description   Unrelated processes Ping Pong with named pipe             ~
 * Group         OL110                                               *
 ~ Company       ILRD Ramat Gan                                      ~
 * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ */
#include <unistd.h> // access(), unlink()
#include <sys/stat.h> // mkfifo()
#include <fcntl.h> // O_RDONLY, O_WRONLY, open()
#include <unistd.h> // write(), read()
#include <iostream> // std::cout, std::endl
#include <stdio.h> // perror()
#include <cstdlib> //exit()

static const int PING_PONG_CYCLES = 10;


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
        exit(-1);
    }
}

static const std::string pingPipeName = "/tmp/pingfifo";
static const std::string pongPipeName = "/tmp/pongfifo";

int Ping(int writeFd, int readFd);

int main()
{   
    if (-1 == mkfifo(pingPipeName.c_str(), O_CREAT| S_IRWXU) ||
                         -1 == mkfifo(pongPipeName.c_str(), O_CREAT|S_IRWXU))
    {
        HandleError("failure in mkfifo", TERMINATE);
    }
    
    int write_fd = open(pingPipeName.c_str(), O_CREAT| O_WRONLY);
    if (-1 == write_fd)
    {
        HandleError("failure in open pingfifo", TERMINATE); 
    }

    int read_fd = open(pongPipeName.c_str(), O_CREAT | O_RDONLY);
    if (-1 == read_fd)
    {
        HandleError("failure in open pongfifo", TERMINATE); 
    }


    Ping(write_fd, read_fd);

    return 0;
}

int Ping(int write_fd, int read_fd)
{
    for (int i = 0; i < PING_PONG_CYCLES; ++i)
    {
        char pipeBuff[5] = "Ping";

        if (-1 == write(write_fd, pipeBuff, 5))
        {
            HandleError("failure in write to pingfifo:", CONTINUE);
        }

        read(read_fd, pipeBuff, 5);
        std::cout << "\033[1;32m             <--- " << pipeBuff << "\033[1;0m" << std::endl;
    }

    close(write_fd);
    close(read_fd);

    return 0;

}


