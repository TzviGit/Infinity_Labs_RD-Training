
/*  ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~
~ Version       0.0.1                                           ~
* Date          17/01/2022                                      *
~ Author        RD110                                           ~
* Reviewer      Binyamin                                        *
~ Description   Singleton - API                                ~
* Group         R&D 110                                         *
~ Company       ILRD Ramat Gan                                  ~
* ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ */

#ifndef __ILRD_RD110_SINGLETON_HPP__
#define __ILRD_RD110_SINGLETON_HPP__

#include <boost/atomic.hpp>             // load, store, and enums
                                        // (memory_order_consume etc.)
#include <boost/thread/mutex.hpp>       // boost::mutex
#include <boost/core/noncopyable.hpp>   // boost::noncopyable
#include <cstdlib>                      // atexit

#include "util.hpp"                     // nullptr for C++ < 11

namespace ilrd
{

template <typename T>
class Singleton : private boost::noncopyable
{
public:
    static T* GetInstance() throw(std::bad_alloc);

private:
    Singleton();
    static void SingletonDestroy();
    static boost::atomic<T*> s_instance;
    static boost::mutex s_mutex;
};

template <typename T>
boost::atomic<T*> Singleton<T>::s_instance(nullptr);

template <typename T>
boost::mutex Singleton<T>::s_mutex;

template <typename T>
T* Singleton<T>::GetInstance() throw(std::bad_alloc)
{
    T* temp = s_instance.load(boost::memory_order_acquire);

    if (nullptr == temp)
    {
        boost::mutex::scoped_lock scoped_lock(s_mutex);
        
        temp = s_instance.load(boost::memory_order_acquire);
        if (nullptr == temp)
        {
            temp = new T;

            std::cout << "INSTANCE CREATED\n";
            s_instance.store(temp, boost::memory_order_release);

            atexit(&SingletonDestroy);
        }
    }

    return temp;
}

template <typename T>
void Singleton<T>::SingletonDestroy()
{
    delete s_instance;
    s_instance = nullptr;
    
}

} // ilrd

#endif // __ILRD_RD110_SINGLETON_HPP__
