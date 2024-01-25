/**********************************************************
 * 
 * future.hpp - Future API
 * 
 * Author : OLRD 110
 * 
 * version : 0.1.0
 * 
 * 23-01-22
 * 
 * OLRD 110 Ramat Gan
 * 
 * *******************************************************/

#ifndef __ILRD_RD110_FUTURE_HPP__
#define __ILRD_RD110_FUTURE_HPP__

#include <boost/thread/condition_variable.hpp> // condition_variable, mutex

namespace ilrd
{
template <typename RETURN_TYPE> 
class Future
{
public:
    explicit Future() throw(); 
    ~Future() throw();      // do we need it?
    RETURN_TYPE Get() const throw(boost::thread_resource_error); //blocking
    Future<RETURN_TYPE> &Set(RETURN_TYPE val) throw();
    bool IsValid() const throw(); //valid from when value was inserted till forever
    void Wait() const throw(boost::thread_resource_error); //blocking

private:
    RETURN_TYPE m_value;
    bool m_valid;
    mutable boost::condition_variable m_condition_var;
    mutable boost::mutex m_mutex;

    class IsValidFunc
    {
    public:
        explicit IsValidFunc(const Future& future_);
        bool operator()();        

    private:
        const Future& m_future;
    };

};

template<>   
class Future<void>
{
public:
    explicit Future() throw();
    ~Future() throw();
    void Get() const throw(boost::thread_resource_error); //blocking
    Future<void> &Set() throw();
    bool IsValid() const throw(); 
    void Wait() const throw(boost::thread_resource_error); //blocking
private:
    bool m_valid;
    mutable boost::condition_variable m_condition_var;
    mutable boost::mutex m_mutex;

    class IsValidFunc
    {
    public:
        explicit IsValidFunc(const Future& future_);
        bool operator()();        

    private:
        const Future& m_future;
};

};

/*************** REGULAR_IMPL ************************************/
template <typename RETURN_TYPE>
Future<RETURN_TYPE>::Future() throw()
: m_value(), m_valid(false), m_condition_var(), m_mutex()
{
    // empty
}

template <typename RETURN_TYPE>
Future<RETURN_TYPE>::~Future() throw()
{
    // empty
}

template <typename RETURN_TYPE>
RETURN_TYPE Future<RETURN_TYPE>::Get() const throw(boost::thread_resource_error)
{
    Wait();

    return m_value;
}

template <typename RETURN_TYPE>
Future<RETURN_TYPE> &Future<RETURN_TYPE>::Set(RETURN_TYPE val) throw()
{
    m_value = val;
    m_valid = true;
    
    boost::unique_lock<boost::mutex> lock(m_mutex);
    m_condition_var.notify_all();

    return *this;
}

template <typename RETURN_TYPE>
bool Future<RETURN_TYPE>::IsValid() const throw()
{
    return m_valid;
}

template <typename RETURN_TYPE>
void Future<RETURN_TYPE>::Wait() const throw(boost::thread_resource_error) 
{
    boost::unique_lock<boost::mutex> lock(m_mutex);

    m_condition_var.wait(lock, IsValidFunc(*this));
}

template<typename RETURN_TYPE>
Future<RETURN_TYPE>::IsValidFunc::IsValidFunc(const Future& future_)
: m_future(future_)
{
    // empty
}

template<typename RETURN_TYPE>
bool Future<RETURN_TYPE>::IsValidFunc::operator()()
{
    return m_future.IsValid();
}


} // namespace ilrd

#endif //__ILRD_RD110_FUTURE_HPP__
