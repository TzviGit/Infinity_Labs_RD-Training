/**********************************************************
 * 
 * waitable_queue.hpp - waitable_queue API
 * 
 * Author : Tzvi Moskowitz
 *
 * Version : 0.1.0
 *
 * Reviewed and approved by : 
 * 
 * 20-01-22
 * 
 * OLRD 110 Ramat Gan
 * 
 * *******************************************************/

#ifndef __ILRD_RD110_WAITABLE_QUEUE_HPP__
#define __ILRD_RD110_WAITABLE_QUEUE_HPP__

#include <queue>      // std::queue
#include <stdexcept>  // std::bad_alloc
#include <boost/chrono.hpp> // boost::chrono
#include <boost/thread/condition_variable.hpp> // boost::condition_variable


namespace ilrd
{


// TYPE must be copyable
// CONTAINER must have push() & pop() methods
template < typename TYPE, typename CONTAINER = std::queue<TYPE> >
class WaitableQueue
{
public:
    // ~WaitableQueue()
    // {
    
    // }
    void Push(const TYPE& data_) throw(std::bad_alloc);
    
    // returns false if queue was empty till end of time, otherwise true
    bool Pop(TYPE &outparam, size_t milli_second);

    void Pop(TYPE &outparam);
    bool IsEmpty() const throw();
    size_t Size() const throw();
    TYPE Front() const;

private:
    CONTAINER m_queue;
    boost::mutex m_mutex;
    boost::condition_variable_any m_cond; // timed or not
    
    void PopAndReturn(TYPE &outparam) ;
};



template <typename TYPE, typename CONTAINER>
void WaitableQueue<TYPE, CONTAINER>::Push(const TYPE& data_)
    throw(std::bad_alloc)
{
    {   // scope of the lock
        boost::mutex::scoped_lock lock(m_mutex);
        m_queue.push(data_);
    }
    
    m_cond.notify_one();
}


template <typename TYPE, typename CONTAINER>
bool WaitableQueue<TYPE, CONTAINER>::Pop(TYPE &outparam, size_t millis)
   // throw(boost::thread_resource_error, boost::thread_interrupted)
{
    boost::chrono::milliseconds ms(millis);
    boost::mutex::scoped_lock lock(m_mutex);
    
    while (IsEmpty())
    {
        if (m_cond.wait_for(lock, ms) == boost::cv_status::timeout)
        {
            return false;
        }
    }
    
    PopAndReturn(outparam);
    
    return true;
}


template <typename TYPE, typename CONTAINER>
void WaitableQueue<TYPE, CONTAINER>::Pop(TYPE &outparam)
    //throw(boost::thread_resource_error, boost::thread_interrupted)
{
    boost::unique_lock<boost::mutex> lock(m_mutex);
    
    while (IsEmpty())
    {
        m_cond.wait(lock);
    }
    
    PopAndReturn(outparam);
}

template <typename TYPE, typename CONTAINER>
TYPE WaitableQueue<TYPE, CONTAINER>::Front() const
{
    return m_queue.front();
}

template <typename TYPE, typename CONTAINER>
bool WaitableQueue<TYPE, CONTAINER>::IsEmpty() const throw()
{
    return m_queue.empty();
}


template <typename TYPE, typename CONTAINER>
size_t WaitableQueue<TYPE, CONTAINER>::Size() const throw()
{
    return m_queue.size();
}

template <typename TYPE, typename CONTAINER>
void WaitableQueue<TYPE, CONTAINER>::PopAndReturn(TYPE &outparam) 
{
    outparam = m_queue.front();
    m_queue.pop();
}



} // ilrd

#endif /* __ILRD_RD110_WAITABLE_QUEUE_HPP__ */


