/**********************************************************
 * 
 * dispatcher.hpp - Dispatcher API
 * 
 * Author : OLRD 110
 * 
 * version : 0.1.0
 *
 * Reviewed and approved by : Binyamin
 * 
 * 01-02-22
 * 
 * OLRD 110 Ramat Gan
 * 
 * *******************************************************/

#ifndef __ILRD_RD110_DISPATCHER_HPP__
#define __ILRD_RD110_DISPATCHER_HPP__

#include <exception> // std::exception
#include <vector> //std::vector


#include <boost/core/noncopyable.hpp> // boost::noncopyable
#include <boost/function.hpp>         // boost::function

namespace ilrd
{
    
    
template <typename EVENT>
class Dispatcher;

template <typename EVENT>
class CallBack : public boost::noncopyable
{
public:
    explicit CallBack(const boost::function<void(EVENT)> &notify_,
                        const boost::function<void(void)> &on_destruct_);
    ~CallBack(); // unsubscribe

//private:
    boost::function<void(EVENT)> m_notify;
    boost::function<void(void)> m_on_destruct;
    Dispatcher<EVENT> &m_dispatcher;
};

/***************************  CALL_BACK IMPLE  ******************/

template <typename EVENT>
CallBack<EVENT>::CallBack(const boost::function<void(EVENT)> &notify_,
                            const boost::function<void(void)> &on_destruct_)
:m_notify(notify_),
m_on_destruct(on_destruct_),
m_dispatcher()
{
    //empty
}

template <typename EVENT>
CallBack<EVENT>::~CallBack()
{
    m_dispatcher.Unsubscribe(*this);
}


class KeyNotFound : public std::exception
{
    explicit KeyNotFound(const char *str_) throw(std::bad_alloc);
    virtual const char *what() const throw();
};

/*********************************************************************
*               DISPATCHER CLASS
**********************************************************************/

template <typename EVENT>
class Dispatcher
{
public:
    ~Dispatcher(); // notify with CallBack.error_func
    void Subscribe(CallBack<EVENT>* callback_) throw(std::bad_alloc);
    void Unsubscribe(CallBack<EVENT>* callback_) throw(KeyNotFound);
    void Notify(EVENT val); 
    // inside the loop, if std::bad_function_call is thrown, unsubscribe it.

private:
    // implementation details
    std::vector<CallBack<EVENT>* > m_observers;
};


/***********  DISPATCHER IMPLE  **************************************/

template <typename EVENT>
Dispatcher<EVENT>::~Dispatcher()
{
    std::vector<CallBack<EVENT>* >::iterator iter = m_observers.begin();

    for (; iter != m_observers.end(); ++iter)
    {
        (*iter)->m_on_destruct();
    }
}

template <typename EVENT>
void Dispatcher<EVENT>::Subscribe(CallBack<EVENT> * callback_) throw(std::bad_alloc)
{
    m_observers.push_back(callback_);
}


template <typename EVENT>
void Dispatcher<EVENT>::Unsubscribe(CallBack<EVENT> * callback_) throw(KeyNotFound)
{
    std::vector<CallBack<EVENT> *>::iterator iter = std::find(m_observers.begin(), m_observers.end(), callback_);
    if (m_observers.end() == iter)
    {
        throw KeyNotFound b;
    }

    m_observers.erase(iter);
}

template <typename EVENT>
void Dispatcher<EVENT>::Notify(EVENT val_)
{
    std::vector<CallBack<EVENT>* >::iterator iter = m_observers.begin();

    for (; iter != m_observers.end(); ++iter)
    {
        (*iter)->m_notify(val_);
    }
}

} // namespace ilrd

#endif //__ILRD_RD110_DISPATCHER_HPP__

