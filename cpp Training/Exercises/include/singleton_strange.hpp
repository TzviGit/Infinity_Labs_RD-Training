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

#include <boost/core/noncopyable.hpp> // boost::noncopyable
#include "scope_lock.hpp"
#include "util.hpp"
#include <boost/thread/mutex.hpp>
#include <iostream>

namespace ilrd
{
template <typename T>
static void SetBoolean(bool &b, T *validPointer);
template <typename T>
static void CreateInstance(T **instPointer) throw(std::bad_alloc);

/**************************************************************
********  Singleton Class ************************************/

template <typename T>
class Singleton : private boost::noncopyable
{
public:
    static T* GetInstance() throw(std::bad_alloc);

private:
    Singleton(); //private Ctor
    static void SingletonDestroy() throw();
    
    static T* m_instance;

};

template <typename T>
T *Singleton<T>::m_instance = nullptr;

template <typename T>
void Singleton<T>::SingletonDestroy() throw()
{
    delete m_instance;
    m_instance = nullptr;
    std::cout << "INSTANCE DESTROYED ......"
              << std::endl;
}

template <typename T>
T* Singleton<T>::GetInstance() throw(std::bad_alloc)
{
    static boost::mutex s_lock;
    static boost::atomic_bool isConstructed(false);

    if (false == isConstructed.load(boost::memory_order_consume))
    {
        { //scope of lock
            boost::mutex::scoped_lock Lock(s_lock);
            if (false == isConstructed.load(boost::memory_order_consume))
            {
                m_instance = new T;
                std::cout << "INSTANCE CREATED\n";
                isConstructed.store(true, boost::memory_order_release);

                atexit(SingletonDestroy);
            }
        }// end of scope_lock
    }
    return m_instance;
}



}//ilrd
#endif // __ILRD_RD110_SINGLETON_HPP__
