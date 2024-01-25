/*  ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~
~ Version       0.0.1                                           ~
* Date          16/01/2022                                      *
~ Author        RD110                                           ~
* Reviewer      Binyamin                                        *
~ Description   Scope lock - API                                ~
* Group         R&D 110                                         *
~ Company       ILRD Ramat Gan                                  ~
* ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ */

#ifndef __ILRD_RD110_SCOPE_LOCK_HPP__
#define __ILRD_RD110_SCOPE_LOCK_HPP__

#include <boost/core/noncopyable.hpp> // boost::noncopyable
#include <iostream>
namespace ilrd
{

class ScopeLock_Exception : public std::exception
{
public:
    explicit ScopeLock_Exception(const char *str = "lock fail") throw()
    : m_str(str) {}
    const char *what() const throw() { return m_str; }
private:
    const char *m_str;
};


template <typename T>
class ScopeLock : private boost::noncopyable
{
public:
    explicit ScopeLock(T &lock_) throw(ScopeLock_Exception);
    ~ScopeLock() throw();

private:
    T &m_lock;
};

template <typename T>
ScopeLock<T>::ScopeLock(T &lock_) throw(ScopeLock_Exception)
:m_lock(lock_)
{
    std::cout << "LOCKING .....\n";
    try
    {
        m_lock.lock();
    }
    catch(const std::exception& e)
    {
        throw(ScopeLock_Exception());
    }
}

template <typename T>
ScopeLock<T>::~ScopeLock() throw()
{
    std::cout << "UNLOCKING .....\n";
    m_lock.unlock();
}

} // ilrd


#endif // __ILRD_RD110_SCOPE_LOCK_HPP__