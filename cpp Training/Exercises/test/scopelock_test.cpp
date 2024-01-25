/* ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ *
~ Author        Ron Matalon                                     ~
* Reviewer                                                *
~ Description                                             ~
* Group         OL110                                           *
~ Company       ILRD Ramat Gan                                  ~
* ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ */

#include <pthread.h> // mutex_t
#include <iostream>
#include <unistd.h>
#include <boost/thread.hpp>

#include "scope_lock.hpp"

using namespace ilrd;

class Pthread_Mutex
{
private:
    pthread_mutex_t mutex;
public:
    explicit Pthread_Mutex();
    ~Pthread_Mutex();

    void lock();
    void unlock();
};

Pthread_Mutex::Pthread_Mutex()
{
    pthread_mutex_init(&mutex, NULL);
}

Pthread_Mutex::~Pthread_Mutex()
{
    pthread_mutex_destroy(&mutex);
}

void Pthread_Mutex::lock()
{
    pthread_mutex_lock(&mutex);
    std::cout << "locked" << std::endl;
}

void Pthread_Mutex::unlock()
{
    pthread_mutex_unlock(&mutex);
    std::cout << "unlocked" << std::endl;
}

void TestPthread();
void *TestThread(void *arg);

void TestBoost();
void *TestThreadBoost(void *arg);

void TestException();

int main()
{
    TestPthread();
    TestBoost();

    try
    {
        TestException();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}

void TestException()
{

}

void TestBoost()
{
    pthread_t ids[3];
    boost::mutex b_mutex;

    for (size_t i = 0; i < 3; ++i)
    {
        pthread_create(&ids[i], NULL, TestThreadBoost, &b_mutex);
    }

    for (size_t i = 0; i < 3; ++i)
    {
        pthread_join(ids[i], NULL);
    }
}

void *TestThreadBoost(void *arg)
{
    static size_t num = 0;
    ScopeLock<boost::mutex> mut((*(boost::mutex *)arg));
    ++num;
    --num;
    ++num;
    std::cout << "Thread num: " << num << std::endl;

    return NULL;
}

void TestPthread()
{
    pthread_t ids[3];
    Pthread_Mutex mutex;

    for (size_t i = 0; i < 3; ++i)
    {
        pthread_create(&ids[i], NULL, TestThread, &mutex);
    }

    for (size_t i = 0; i < 3; ++i)
    {
        pthread_join(ids[i], NULL);
    }

}

void *TestThread(void *arg)
{
    static size_t num = 0;
    ScopeLock<Pthread_Mutex> mut((*(Pthread_Mutex *)arg));
    ++num;
    --num;
    ++num;
    std::cout << "Thread num: " << num << std::endl;

    return NULL;
}
