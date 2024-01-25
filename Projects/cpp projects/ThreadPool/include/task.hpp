/**********************************************************
 * 
 * task.hpp - Task API
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

#ifndef __ILRD_RD110_TASK_HPP__
#define __ILRD_RD110_TASK_HPP__

//#include <boost/tr1/functional.hpp> // boost::tr1::function

//own libraries
#include "future.hpp" // Future

namespace ilrd
{

class TaskInterface
{
public:
    virtual ~TaskInterface() {} //empty
    // Purposely now throw list because can throw ANYTHING
    virtual void Run() = 0;
};


template <typename RETURN_TYPE, typename PARAM>
class Task : public TaskInterface
{
public:
    typedef RETURN_TYPE (*function_t)(PARAM);
    //typedef RETURN_TYPE (boost::tr1::function function_t)(PARAM);
    explicit Task(function_t func, PARAM param) throw(); 
    void Run(); // may throw any error that the underlying function may throw when called
    // RETURN_TYPE operator () (PARAM) const; // 
    
private:
    function_t m_func;
    PARAM m_param;
};

template <typename RETURN_TYPE, typename PARAM>
Task<RETURN_TYPE, PARAM>::Task(RETURN_TYPE (*func)(PARAM), PARAM param) throw()
:m_func(func),
m_param(param)
{
    //empty
}

template <typename RETURN_TYPE, typename PARAM>
void Task<RETURN_TYPE, PARAM>::Run()
{
    m_func(m_param);
}

/***************************************************************************************************
********************    TaskWithFuture  ***********************************************************/
template <typename RETURN_TYPE, typename PARAM>
class TaskWithFuture : public TaskInterface // with non void return value, and with void return value
{
public:
    typedef boost::shared_ptr<Future<int> > future_ptr;
    typedef RETURN_TYPE (*function_t)(PARAM);
    explicit TaskWithFuture(function_t func, PARAM param, boost::shared_ptr<Future<RETURN_TYPE> > future) throw(); 
    
    void Run() ; // may throw any error that the underlying function may throw when called
    // RETURN_TYPE operator () (PARAM) const; // 
    
private:
    function_t m_func;
    PARAM m_param;
    mutable boost::mutex m_mutex;
    boost::shared_ptr<Future<RETURN_TYPE> > m_future;
};


template <typename RETURN_TYPE, typename PARAM>
TaskWithFuture<RETURN_TYPE, PARAM>::TaskWithFuture(RETURN_TYPE (*func)(PARAM), PARAM param, 
                                            boost::shared_ptr<Future<RETURN_TYPE> > future) throw()
:m_func(func),
m_param(param),
m_future(future)
{
    //empty
}

template <typename RETURN_TYPE, typename PARAM>
void TaskWithFuture<RETURN_TYPE, PARAM>::Run()
{
    boost::mutex::scoped_lock lock(m_mutex);
    m_future->Set(m_func(m_param)); // m_func(m_param)
}

//******************************************************** with void return value
template <typename PARAM>
class TaskWithFuture<void,PARAM> : public TaskInterface 
{
public:
    typedef void (*function_t)(PARAM);
    explicit TaskWithFuture(function_t func, PARAM& param,           
                boost::shared_ptr<Future<PARAM> > future) throw(); 
    ~TaskWithFuture() throw();
    void Run(); // may throw any error that the underlying function may throw when called

private:
    function_t m_func;
    PARAM m_param;
    boost::shared_ptr<Future<PARAM> > m_future;
};

template<typename PARAM>
TaskWithFuture<void, PARAM>::TaskWithFuture(function_t func, PARAM& param,      
                            boost::shared_ptr<Future<PARAM> > future) throw()
: m_func(func), m_param(param), m_future(future)
{
    // empty
}

template<typename PARAM>
TaskWithFuture<void, PARAM>::~TaskWithFuture() throw()
{
    //empty
}

template<typename PARAM>
void TaskWithFuture<void, PARAM>::Run()
{
    m_func(m_param);
   // m_future->Set());
}

    // specialization of void

} // namespace ilrd

#endif //__ILRD_RD110_TASK_HPP__