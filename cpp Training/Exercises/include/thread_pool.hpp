/**********************************************************
 * 
 * thread_pool.hpp - ThreadPool API
 * 
 * Author : OLRD 110
 * 
 * version : 0.1.0
 *
 * Reviewed and approved by : Yinon
 * 
 * 23-01-22
 * 
 * OLRD 110 Ramat Gan
 * 
 * *******************************************************/
#ifndef __ILRD_RD110_THREAD_POOL_HPP__
#define __ILRD_RD110_THREAD_POOL_HPP__

#include <cstddef>   // size_t
#include <stdexcept> // std::bad_alloc
#include <vector>    // std::vector
#include <semaphore.h> //semephores


//third party libraries
#include <boost/thread/thread.hpp>
//#include <boost/thread.hpp>           // boost::thread
#include <boost/core/noncopyable.hpp> // boost::noncopyable
#include <boost/atomic.hpp> //Atomic vars
#include <boost/shared_ptr.hpp>


//own libraries
#include "task.hpp"           // TaskInterface
#include "waitable_queue.hpp" // WaitableQueue
#include "priority_queue.hpp"

namespace ilrd
{
class ThreadPool : private boost::noncopyable
{
public:

    typedef enum {LOW,NORMAL,HIGH}Priority_t;
    typedef std::pair<boost::shared_ptr<TaskInterface>, Priority_t> taskPair_t;
    typedef boost::shared_ptr<TaskInterface> taskPtr_t;
    typedef boost::shared_ptr<boost::thread> threadPtr_t;
    // init with hardware_concurrency
    explicit ThreadPool(size_t num_of_threads = boost::thread::hardware_concurrency(),
                        int niceness = 0) throw(std::bad_alloc, boost::thread_resource_error);
    ~ThreadPool() throw();

    bool ShutDown(size_t seconds) throw();
    void AddTask(boost::shared_ptr<TaskInterface> task, Priority_t prior_ = NORMAL) throw(std::bad_alloc);
    // shared_ptr!
    // typedef  for shared_ptr
    // ENUM priority - public:LOW, NORMAL, HIGH.   private:CRITICAL.
    void Pause() throw(); // should also stop threads in the middle of a task?
    void Resume() throw();
    void SetNumOfThread(size_t num) throw(std::bad_alloc, boost::thread_resource_error);

private: // impl details!!!!!
    static const int CRITICAL = HIGH + 1;
    static const int KILLER = HIGH + 2;
    WaitableQueue<taskPair_t, PriorityQueue<taskPair_t> > m_task_queue;
    
    std::vector<threadPtr_t> m_threads; // thread_group
    

    boost::atomic_bool m_isRunning;

    int m_niceness;
    boost::atomic<size_t> m_numThreads;
    boost::mutex m_runLock;
   

    boost::condition_variable m_runCond;
    boost::condition_variable m_shutDownTimer;
    
    bool IsRunning() const throw();
    void EventLoop() throw();
    void ExecuteNextTask();
    void GoToSleep();
    static void PoisonApple(int a) throw(std::string);
    static void PauseTrap(int a) throw(std::exception, boost::thread_interrupted);
    void ActualShutdown(); 
    void AddThread();
    void RemoveThread();
    void JoinFinishedThreads();
};

} // namespace ilrd

#endif //__ILRD_RD110_THREAD_POOL_HPP__