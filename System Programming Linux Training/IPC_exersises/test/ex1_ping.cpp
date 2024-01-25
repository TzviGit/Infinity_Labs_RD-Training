#include <fcntl.h> //O_CREATE
#include <sys/stat.h> // PERMISSIONS MACROS
#include <semaphore.h> //sem_open(), sem_wait(), sem_close,sem_unlink
#include <stdlib.h> //exit()
#include <stdio.h> //perror()
#include <stdexcept>
#include <iostream> //cout
#include <unistd.h> //fork()

static void CloseSem(sem_t *sem) throw(std::exception);

static const int PING_PONG_CYCLES = 10;
class SemaphoreObject
{
public:
    SemaphoreObject( std::string name, int flag, int mode,
                    unsigned int val)throw(std::exception)
    :m_name(name)
    {
        if (SEM_FAILED == (m_semPointer = sem_open(name.c_str(), flag, mode, val)))
        {
            throw std::exception();
        }
    }
    ~SemaphoreObject()
    {
        try
        {
            CloseSem(m_semPointer);
        }
        catch (std::exception &e)
        {
            perror("FAILD TO CLOSE SEMAPHORE");
        }

        m_semPointer = NULL;
        sem_unlink(m_name.c_str());
    }

    operator sem_t *()
    {
        return m_semPointer;
    }

private:
    sem_t *m_semPointer;
    std::string m_name;
};

static void CloseSem(sem_t *sem) throw(std::exception)
{
    if (-1 == sem_close(sem))
    {
        throw(std::exception());
    }
}

void Ping(SemaphoreObject &semPing, SemaphoreObject &semPong);
void Pong(SemaphoreObject &semPing, SemaphoreObject &semPong);

int main()
{
    SemaphoreObject pingSem("/pingsem", O_CREAT, S_IRWXU | S_IRWXO, 1);
    SemaphoreObject pongSem("/pongsem", O_CREAT, S_IRWXU | S_IRWXO, 0);

    pid_t pid = fork();
    if (pid < 0) 
    {
        exit(-1);
    }
    
    if (0 == pid) //child
    {
        Pong(pingSem, pongSem);
    }
    else
    {
        Ping(pingSem, pongSem);
    }

    
    return 0;
}
void Ping(SemaphoreObject &semPing, SemaphoreObject &semPong)
{
    //open semaphore
    

    for (int i = 0; i < PING_PONG_CYCLES; ++i)
    {
        sem_wait(semPing);
        std::cout << "\033[1;36mPing --->\033[1;0m" << std::endl;
        sem_post(semPong);
    }

}


void Pong(SemaphoreObject &semPing, SemaphoreObject &semPong)
{
    //open semaphore
    
    for (int i = 0; i < PING_PONG_CYCLES; ++i)
    {
        sem_wait(semPong);
        std::cout << "\033[1;32m             <--- Pong \033[1;0m" << std::endl;
        sem_post(semPing);
    }

}
