/*****************************************************************************

Author:         Tzvi Moskowitz
Company:		Infinity
Reviewer:       RON
Date:           
Group:          OL110
Description:    

*****************************************************************************/


#include <algorithm>
#include <pthread.h>
#include <exception>

#ifndef NDEBUG
#include <iostream> //cout
#endif //NDEBUG

#include "thread_pool.hpp"


namespace ilrd
{



ThreadPool::ThreadPool(size_t numOfThreads, int niceness_) throw(std::bad_alloc, boost::thread_resource_error)
:m_task_queue(),
m_threads(),
m_isRunning(true),
m_niceness(niceness_),
m_numThreads(0),
m_runLock(),
m_runCond(),
m_shutDownTimer()
{


    for (size_t i = 0; i < numOfThreads; i++)
    {
        AddThread();
    }

    /*****  allow for global state to update ***/
    while (m_numThreads < numOfThreads)
    {}
    
}

/****************************************************
*******   ADD_THREAD TO POOL ********************/
void ThreadPool::AddThread()
{
    sched_param sch_params = {0};
    sch_params.sched_priority = m_niceness;
    

    threadPtr_t thread(new boost::thread(boost::bind(&ThreadPool::EventLoop, this)));
    pthread_setschedparam(thread->native_handle(), SCHED_FIFO, &sch_params);
    m_threads.push_back(thread);
}

/****************************************************
*******   REMOVE_THREAD FROM POOL ********************/
void ThreadPool::RemoveThread()
{
    boost::mutex::scoped_lock lock(m_runLock);
    
    ThreadPool::taskPtr_t t_ptr(new Task<void, int>(ThreadPool::PoisonApple, 6));
    AddTask(t_ptr,static_cast<Priority_t>(KILLER));
    bool currStatus = m_isRunning.load(boost::memory_order_acquire);

    if (false == currStatus)
    {
        m_isRunning.store(true, boost::memory_order_seq_cst);
        
        m_runCond.notify_one();
       
        m_isRunning.store(false, boost::memory_order_seq_cst);
    }

}

//---------------------------------------------------------------------------
//******       ADD_TASK  ****************************************************

void ThreadPool::AddTask(taskPtr_t task_, Priority_t prior_) throw(std::bad_alloc)
{
    std::pair<taskPtr_t, Priority_t> pair(task_ , prior_);
    
    {
        m_task_queue.Push(pair);
    }

}


/***************************************************************************************
******  SETNUMTHREADS --- control how many threads in pool at run time  ***************/

void ThreadPool::SetNumOfThread(size_t newNum_) throw(std::bad_alloc, boost::thread_resource_error)
{
    size_t currNum = m_numThreads.load();

    while (newNum_ > currNum)
    {
        AddThread();
        ++currNum;
    }

    while (currNum > newNum_)
    {
        RemoveThread();
        --currNum;
    }
}


/****************************************************************************************
*       HARSH DESTRUCTOR -> will stop anything and everything 
****************************************************************************************/
ThreadPool::~ThreadPool() throw()
{
    
    SetNumOfThread(0);
    usleep(1000);
    while (!m_threads.empty())
    {
        
        m_threads.back().get()->interrupt();
    

        m_threads.back().get()->try_join_for(boost::chrono::milliseconds(1));

        m_threads.back().get()->~thread();
        m_threads.pop_back();
    }
    
    //m_joinerThread.interrupt();
}


/****************************************************************************************
*       SHUT_DOWN -> will try to join anything it can within alloted time 
****************************************************************************************/
bool ThreadPool::ShutDown(size_t seconds_) throw()
{

    boost::thread tid(boost::bind(&ThreadPool::ActualShutdown, this));
    boost::chrono::seconds sec(seconds_);
    boost::mutex mutex;
    boost::mutex::scoped_lock lock(mutex);

    bool result = true;
    if (m_shutDownTimer.wait_for(lock, sec) == boost::cv_status::timeout)
    {
        result = false;

        while (!m_task_queue.IsEmpty() && 
                m_task_queue.Front().second == static_cast<Priority_t>(KILLER))
        {
            taskPair_t pair;
            m_task_queue.Pop(pair, 1);
        }
    }

    return result;
}

/*********  the thread func that will actually do the joining  ******/

void ThreadPool::ActualShutdown()
{
    SetNumOfThread(0);
    
    for (size_t i = 0; i < m_threads.size(); ++i)
    {
        m_threads[i].get()->join();
    }

    m_shutDownTimer.notify_all();

}


/****************************************************************************************
*       PAUSE -> will try to put the threads to sleep 
****************************************************************************************/
void ThreadPool::Pause() throw()
{
    m_isRunning.store(false, boost::memory_order_seq_cst);

    /** give some time for boolean to be globaly visible **/
    //usleep(1000);

    boost::mutex::scoped_lock lock(m_runLock);
    
    ThreadPool::taskPtr_t t_ptr(new Task<void, int>(ThreadPool::PauseTrap,6));

    size_t num = m_numThreads.load(boost::memory_order_acquire);
    for (size_t i = 0; i < num; i++)
    {
        AddTask(t_ptr,static_cast<Priority_t>(CRITICAL));
    }
    

}

/****************************************************************************************
*       RESUME -> wake up the threads 
****************************************************************************************/
void ThreadPool::Resume() throw()
{
    

    while (!m_task_queue.IsEmpty() && 
            m_task_queue.Front().second == static_cast<Priority_t>(CRITICAL))
    {
        taskPair_t pair;
        m_task_queue.Pop(pair, 1);
    }
    
    boost::mutex::scoped_lock lock(m_runLock);

    m_isRunning.store(true, boost::memory_order_seq_cst);
       
    m_runCond.notify_all();
}


/********************************************************************
*  Main Function the threads live in                             
********************************************************************/

void ThreadPool::EventLoop() throw()
{
    ++m_numThreads;
    try
    {
        while (true)
        {
             if (!m_isRunning.load(boost::memory_order_acquire))
            {
                GoToSleep();
            }
            
            /**************  set interupt point *****/
            boost::this_thread::interruption_point();

            /*****   get task from queue and excecute *******/
            try
            {
                ExecuteNextTask();
            }
            catch(const std::string &s)  /****  catch poison apple **/
            {
                --m_numThreads;
                return;
            }
            
                    
        }
    }
    catch(const boost::thread_interrupted& e)
    {
        --m_numThreads;
    }

    //--m_numThreads;
}

///////////////////////////////////////////////////////////////
////  Try to take task from queue and excecute it /////////////

void ThreadPool::ExecuteNextTask()
{
    taskPair_t pair;
    m_task_queue.Pop(pair);
    
    pair.first->Run();
    
}

/******************************************************************
 *    These functions are Special priority functions used during
 *    Pause and to remove ("kill") a thread;
 ******************************************************************/
void ThreadPool::PoisonApple(int t) throw(std::string)
{
    (void)t;
    throw(std::string("POISONAPPLE"));
}

void ThreadPool::PauseTrap(int a) throw(std::exception,boost::thread_interrupted)
{
    //-------------------------------------------------------*
    //  point of this trap is to force the threads that were 
    //  on the queue to loop back around and go to 
    //  sleep from the m_isRunning check 
    //--------------------------------------------------------*
    (void)a;
}

/***************************************************************
****   Go to sleep till awaken by notify *********************/

void ThreadPool::GoToSleep()
{
    boost::mutex::scoped_lock lock(m_runLock);
    m_runCond.wait(lock); //resume will notify

}

/*********************************************************************************/
//this overload will be used by the std::less compare func to sort queue by Priority
//thanks to Ron Matalon for the great idea
/*********************************************************************************/

bool operator<(const ThreadPool::taskPair_t& rhs_, const ThreadPool::taskPair_t& lhs_)
{
    return rhs_.second < lhs_.second;
}





/**********************************************************************/
/**********************************************************************

              TODO: try to integrate joiner thread int the code

void ThreadPool::JoinFinishedThreads()
{
   while(true)
   { 
        //boost::this_thread::interruption_point();

        boost::thread::id tid;
        m_doneThreads.Pop(tid);

        typename std::vector<threadPtr_t>::iterator iter;
        for (iter = m_threads.begin(); iter != m_threads.end(); ++iter)
        {
            if (iter->get()->get_id() == tid)
            {
                iter->get()->join();
                m_threads.erase(iter);
            }
        }
   }
}

*******************************************************************************/

} //namespace ilrd