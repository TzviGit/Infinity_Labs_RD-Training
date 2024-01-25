#include <unistd.h> // pipe(), fork(), close(), read(), write()
#include <sys/types.h> // pid_t
#include <iostream> // std::cout, std::endl
#include <stdio.h> // perror()
#include <stdlib.h> // exit()
#include <cstring> //strlen


/******** THE GLOBAL CONSTANTS  ********/
static const int PIPE_FDS = 2;
static const int PING_PONG_CYCLES = 10;
static const int MSG_SIZE = strlen("ping") +1;

/********  VARIOUS ENUMS FOR READABILITY  *****/
typedef enum
{
   TERMINATE,
    CONTINUE
}err_status_t;

typedef enum
{
    PING,
    PONG
} ping_t;

enum
{
    READ_END,
    WRITE_END
};

/***************  UTILFUNCS  PROTOTYPES  ***************/
void HandleError(std::string err, err_status_t flag);
void PipeBounce(int write_fd, int read_fd, ping_t mode);


int main()
{
    //pipe returns 2 fd's
    int pingPipe[PIPE_FDS] = {0};
    int pongPipe[PIPE_FDS] = {0};

    pipe(pingPipe);
    pipe(pongPipe);

    pid_t pid = fork();
    if (pid < 0)
    {
        HandleError("FAILED TO FORK", TERMINATE);
    }

    if(pid == 0)
    {
        //child
       close(pingPipe[WRITE_END]);
        close(pongPipe[READ_END]);

        PipeBounce(pongPipe[WRITE_END], pingPipe[READ_END], PONG);


        exit(0);
    }
    else
    {
        close(pingPipe[READ_END]);
        close(pongPipe[WRITE_END]);

        PipeBounce(pingPipe[WRITE_END], pongPipe[READ_END], PING);
        
    }

    return 0;
}

void PipeBounce(int write_fd, int read_fd, ping_t mode)
{
    for (int i = 0; i < PING_PONG_CYCLES; ++i)
        {
            char Buff[MSG_SIZE] = {0};
            

            
            if (PING == mode)
            {
                write(write_fd, "Ping", MSG_SIZE); 
                read(read_fd, Buff, MSG_SIZE);
                std::cout << "\033[1;36m" << Buff << " --->\033[1;0m" << std::endl;
                
            }
            else
            {
                read(read_fd, Buff, MSG_SIZE);
                std::cout << "\033[1;32m             <--- " <<Buff << "\033[1;0m" << std::endl;
                write(write_fd, "Pong", MSG_SIZE);
            }
             
        }

        close(write_fd);
        close(read_fd);
}


void HandleError(std::string err, err_status_t flag)
{
    perror(err.c_str());

    if (TERMINATE == flag)
    {
        exit(-1);
    }
}