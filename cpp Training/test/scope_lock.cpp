#include "scope_lock.hpp" // API for Scopelock object
#include <boost/thread/mutex.hpp>
#include "util.hpp"
#include <iostream>
 
using namespace ilrd;

class Pthread
{
public:
    explicit Pthread() throw(std::bad_alloc);
    ~Pthread() throw();

    int lock() throw();
    int unlock() throw();

private:
    pthread_mutex_t mutex;
};

Pthread::Pthread() throw(std::bad_alloc)
{
    mutex = PTHREAD_MUTEX_INITIALIZER;
}

Pthread::~Pthread() throw()
{
    //delete (mutex);
    pthread_mutex_destroy(&mutex);
}

int Pthread::lock() throw()
{
    return pthread_mutex_lock(&mutex);
}

int Pthread::unlock() throw()
{
    return pthread_mutex_unlock(&mutex);
}

int main()
{
    boost::mutex mutex;
    {ScopeLock<boost::mutex> Lock1(mutex);}

    Pthread pthread_mutex;
    ScopeLock<Pthread> Lock2(pthread_mutex);

    return 0;
}


