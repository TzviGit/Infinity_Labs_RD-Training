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

#include "util.hpp" //nulptr

namespace ilrd
{
    
/***************************************************************
*       KEY_NOT_FOUND EXCEPTION
***************************************************************/

class KeyNotFound : public std::exception
{
public:
    inline KeyNotFound(const std::string & = "") throw();
    inline ~KeyNotFound() throw(){}
    inline virtual const char *what() const throw();

private:
    std::string m_message;
};

KeyNotFound::KeyNotFound(const std::string& message_) throw()
:m_message(message_)
{
    //empty
}


const char *KeyNotFound::what() const throw()
{
    return m_message.c_str();
}

//////////////////////////////////////////////////////////////////////////////
//   CALL_BACK CLASS
////////////////////////////////////////////////////////////////////////////
    
template <typename EVENT>
class Dispatcher;

template <typename EVENT>
class CallBack : public boost::noncopyable
{
public:
    explicit CallBack(Dispatcher<EVENT> &dispatcher_,
                        const boost::function<void(EVENT)> &notify_,
                        const boost::function<void(void)> &on_destruct_ = nullptr)
                        throw();
    virtual ~CallBack() throw(); // unsubscribe

private:
    boost::function<void(EVENT)> m_notify;
    boost::function<void(void)> m_on_destruct;
    Dispatcher<EVENT> *m_dispatcher;

    friend void Dispatcher<EVENT>::Notify(EVENT val);
    friend Dispatcher<EVENT>::~Dispatcher();
};

/***************************  CALL_BACK IMPLE  ******************/

template <typename EVENT>
CallBack<EVENT>::CallBack(Dispatcher<EVENT> &dispatcher_, const boost::function<void(EVENT)> &notify_,
                            const boost::function<void(void)> &on_destruct_) throw()
:m_notify(notify_),
m_on_destruct(on_destruct_),
m_dispatcher(&dispatcher_)
{
    m_dispatcher->Subscribe(this);
}

template <typename EVENT>
CallBack<EVENT>::~CallBack() throw()
{
    if (nullptr != m_dispatcher)
    {
        try
        {
            m_dispatcher->Unsubscribe(this);
        }
        catch(const KeyNotFound& e)
        {
            //std::cerr << e.what() << '\n';
        }
    }

}
    



/*********************************************************************
*               DISPATCHER CLASS
**********************************************************************/

template <typename EVENT>
class Dispatcher
{
public:
    virtual ~Dispatcher(); // notify with CallBack.error_func
    
    
    void Notify(EVENT val); 
    // inside the loop, if std::bad_function_call is thrown, unsubscribe it.

private:
    // implementation details
    void Subscribe(CallBack<EVENT>* callback) throw(std::bad_alloc);
    void Unsubscribe(CallBack<EVENT>* callback) throw(KeyNotFound);
    std::vector<CallBack<EVENT>* > m_observers;
    
    friend class CallBack<EVENT>;
};

/***********  DISPATCHER IMPLE  **************************************/

template <typename EVENT>
Dispatcher<EVENT>::~Dispatcher()
{
    typename std::vector<CallBack<EVENT>* >::iterator iter = m_observers.begin();

    for (; iter != m_observers.end(); ++iter)
    {
        try
        {
            (*iter)->m_on_destruct();
        }
        catch(const boost::bad_function_call&)
        {
            //std::cerr << e.what() << '\n';
        }
        
        (*iter)->m_dispatcher = NULL;
    }
}

template <typename EVENT>
void Dispatcher<EVENT>::Subscribe(CallBack<EVENT>* callback_) throw(std::bad_alloc)
{
    m_observers.push_back(callback_);
}


template <typename EVENT>
void Dispatcher<EVENT>::Unsubscribe(CallBack<EVENT>* callback_) throw(KeyNotFound)
{
    typename std::vector<CallBack<EVENT>* >::iterator iter = std::find(m_observers.begin(), m_observers.end(), callback_);
    if (m_observers.end() == iter)
    {
        throw KeyNotFound();
    }

    m_observers.erase(iter);
}

template <typename EVENT>
void Dispatcher<EVENT>::Notify(EVENT val_)
{
    typename std::vector<CallBack<EVENT>* >::iterator iter = m_observers.begin();

    for (; iter != m_observers.end(); ++iter)
    {
        try
        {
            (*iter)->m_notify(val_);
        }
        catch(...)
        {
            //std::cerr << e.what() << '\n';
        }
        
        
    }
}

} // namespace ilrd

#endif //__ILRD_RD110_DISPATCHER_HPP__

