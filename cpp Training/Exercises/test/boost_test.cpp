

#include "thread_pool.hpp"
#include <algorithm>
#include <pthread.h>
#include <iostream> //for DeBug
#include <exception>
boost::condition_variable cond;
pthread_mutex_t tmm = PTHREAD_MUTEX_INITIALIZER;
static boost::mutex mutex;
static boost::mutex mutex2;
void Bar()
{
    boost::mutex::scoped_lock lock(mutex2);
    
    cond.wait(lock);
}
int Foo()
{
    
     boost::mutex::scoped_lock lock(mutex);
    // Bar();

    // std::cout << "GOT TO HERE\n";
    // boost::condition_variable cond;
    // cond.wait(lock);
    // while (1)
    // {
    // }
    // pthread_mutex_lock(&tmm);
    // pthread_mutex_unlock(&tmm);
    boost::condition_variable dest;
    Bar();
    //dest.wait(lock);
}

int main()
{

    boost::thread thread(Foo);
    usleep(10000);
    cond.notify_all();
    usleep(100);
    thread.detach();
    usleep(3000);
    thread.join();
    thread.join();
    

    //    // = PTHREAD_MUTEX_INITIALIZER;
    //     boost::mutex tt;
    //     //tt.lock();
    //     tt.unlock();
    //     Foo();
    //     if (EBUSY == pthread_mutex_destroy(&tmm))
    //     {
    //         std::cout << "what!!\n\n";
    //     }
    //     //tt.~mutex();
    // sleep(100);
    ;
    //pthread::pthread_mutex_destroy()
        // pthread_cancel(thread.native_handle());
        //  S thread.~thread();
        return 0;
}