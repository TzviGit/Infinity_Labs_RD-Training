#include <sys/ipc.h>            /* ftok */
#include <sys/shm.h>            /* shmget, shmat, shmdt, shmctl */
#include <sys/types.h>          /* key_t */
#include <iostream>
#include <cstring>
#include <unistd.h>

static const int MAGIC = 65;

class SharedMem
{
public:
    SharedMem(const std::string& filePath_, size_t size_ = 1024, int flags_ = 0666 | IPC_CREAT)
    {
        int magic = MAGIC;
        key_t key = ftok(filePath_.c_str(), magic);
        m_shmid = shmget(key, size_, flags_);
        
    }

    void ShrMemReader()
    {
        try
        {
            char *memPtr = static_cast<char *>(shmat(m_shmid, (const void*)0, 0));
            if (NULL == memPtr)
            {
                throw "shmat error!";
            }
            std::cout << "the message was: " << "\033[1;32m" << memPtr << "\033[1;0m\n";
            shmdt(memPtr);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            
            throw;
        }
    }

    void ShrMemWriter(std::string mesg)
    {
        try
        {
            char *memPtr = (char *)shmat(m_shmid, NULL, 0);
            if (NULL == memPtr)
            {
                throw "shmat error!";
            }
            strcpy(memPtr, mesg.c_str());
            strcat(memPtr, "\n");

            shmdt(memPtr);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            
            throw;
        }
    }

    void ShrMemRemove()
    {
        shmctl(m_shmid, IPC_RMID, NULL);
    }

    ~SharedMem()
    {
        close(m_shmid);
    }

private:
    int m_shmid;
};

