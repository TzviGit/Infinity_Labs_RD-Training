/*  ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~
~ Version       0.0.0                                           ~
* Date          24/01/2022                                      *
~ Author        Nophar Hochenberg                               ~
* Reviewer                                                      *
~ Description   Thread pool - Test file                         ~
* Group         R&D 110                                         *
~ Company       ILRD Ramat Gan                                  ~
* ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ */
//gd+ test/thread_pool_test.cpp src/thread_pool.cpp src/task.cpp src/future.cpp -I include/ -lpthread -lboost_chrono -lboost_thread -lboost_system
// Standard libraries
#include <iostream> //std::cout, std::endl
#include <string> //std::string
#include <vector> //std::vector

// Third party libraries
#include <boost/shared_ptr.hpp> // boost::shared_ptr
#include <boost/thread.hpp> // boost::this_thread::sleep_for()
#include <boost/chrono.hpp> // boost::chrono
#include <boost/atomic.hpp> // boost::atomic

// Own libraries
#include "thread_pool.hpp" // API of this module
#include "task.hpp" // Task module
#include "future.hpp" // Future module

using namespace ilrd;

#define BOLD_CYAN_COLOR "\033[1;36m"
#define RED_COLOR "\033[0;31m"
#define GREEN_COLOR "\033[0;32m"
#define RESET_COLOR "\033[0m"

template<typename T>
bool IsFailure(T actual, T expected);
static void PrintTestHeader(std::string TestName);
static void TestTasksQueuePrioritization();
static void PopulateGlobalVector(int num);
static void ShutDownPool(ThreadPool &threadPool);
static void TestIncrementNumOfThreads();
static void TestIncrementNumOfThreadsWhenPaused();
static void TestDecrementNumOfThreads();
void SleepThread(size_t milliseconds);
static void ResetVector();
void JustSleep(void *empty);
static void TestDecrementNumOfThreadsWhenPaused();
static void TestThreadsResumeBeforeTheySucceedToPause();
static void TestShutDownWhenPaused();
static void TestDestructor();
static void TestExecutinOfTaskswithFuture();
bool IsPrimeNumber(int num);
static void TestSetNumOfThreadsAsCurrent();
static void TestDestructor2();
void InfiniteLoop(bool status);

static std::vector<size_t> s_vector;
static boost::atomic<size_t> threadsCounter(0);

int main()
{
    TestTasksQueuePrioritization();
    TestIncrementNumOfThreads();
    TestIncrementNumOfThreadsWhenPaused();
    TestDecrementNumOfThreads();
    TestDecrementNumOfThreadsWhenPaused();
    TestSetNumOfThreadsAsCurrent();
    TestThreadsResumeBeforeTheySucceedToPause();
    TestShutDownWhenPaused();
    TestExecutinOfTaskswithFuture();
    TestDestructor();
    TestDestructor2();

    // Test dtor whith infint loop tasks
    // increase num of threads in the middle of a task
    //edge case: decrease after decrease num of threads
    return 0;
}

static void TestTasksQueuePrioritization()
{
    PrintTestHeader("Test Prioritization of Tasks Queue >>");
    ThreadPool threadPool(1);
    threadPool.Pause();
    ThreadPool::taskPtr_t task1(new Task<void, int>(&PopulateGlobalVector, 1));
    ThreadPool::taskPtr_t task2(new Task<void, int>(&PopulateGlobalVector, 2));
    ThreadPool::taskPtr_t task3(new Task<void, int>(&PopulateGlobalVector, 3));
    ThreadPool::taskPtr_t task4(new Task<void, int>(&PopulateGlobalVector, 4));
    ThreadPool::taskPtr_t task5(new Task<void, int>(&PopulateGlobalVector, 5));
    ThreadPool::taskPtr_t task6(new Task<void, int>(&PopulateGlobalVector, 6));
    threadPool.AddTask(task5, ThreadPool::LOW);
    threadPool.AddTask(task1, ThreadPool::HIGH);
    threadPool.AddTask(task3);
    threadPool.AddTask(task4, ThreadPool::NORMAL);
    threadPool.AddTask(task6, ThreadPool::LOW);
    threadPool.AddTask(task2, ThreadPool::HIGH);
    threadPool.Resume();
    boost::this_thread::sleep_for(boost::chrono::milliseconds(1));

    if (false == IsFailure(s_vector.size(), (size_t)6))
    {
        if (1 == s_vector[0])
        {
            IsFailure(s_vector[0], (size_t)1);
            IsFailure(s_vector[1], (size_t)2);
        }
        else
        {
            IsFailure(s_vector[0], (size_t)2);
            IsFailure(s_vector[1], (size_t)1);
        }

        if (3 == s_vector[2])
        {
            IsFailure(s_vector[2], (size_t)3);
            IsFailure(s_vector[3], (size_t)4);
        }
        else
        {
            IsFailure(s_vector[2], (size_t)4);
            IsFailure(s_vector[3], (size_t)3);
        }

        if (5 == s_vector[4])
        {
            IsFailure(s_vector[4], (size_t)5);
            IsFailure(s_vector[5], (size_t)6);
        }
        else
        {
            IsFailure(s_vector[4], (size_t)6);
            IsFailure(s_vector[5], (size_t)5);
        }
    }

    ResetVector();
    ShutDownPool(threadPool);
}

static void ShutDownPool(ThreadPool &threadPool)
{
    bool shuttedDown = threadPool.ShutDown(1);
    while (false == shuttedDown)
    {
        shuttedDown = threadPool.ShutDown(1);
    }
}

static void PopulateGlobalVector(int num)
{
    s_vector.push_back(num);
}

template<typename T>
bool IsFailure(T actual, T expected)
{
    if (actual == expected)
    {
        std::cout << GREEN_COLOR << "Success" << RESET_COLOR << std::endl;
        return false;
    }
    else
    {
        std::cout << RED_COLOR << "Failure -  expected: "<< expected << " VS. actual: "<< actual << RESET_COLOR << std::endl;
        return true;
    }
}

static void ResetVector()
{
    while (false == s_vector.empty())
    {
        s_vector.pop_back();
    }
}

static void PrintTestHeader(std::string TestName)
{
    std::cout << BOLD_CYAN_COLOR << TestName << RESET_COLOR << std::endl;
}

static void TestIncrementNumOfThreads()
{
    PrintTestHeader("Test SetNumOfThread() --> Increment threads >>");
    ThreadPool threadPool(10);
    ThreadPool::taskPtr_t task(new Task<void, size_t>(&SleepThread, 500));
    for (size_t i = 0; i < 12; ++i)
    {
        threadPool.AddTask(task);
    }

    threadPool.SetNumOfThread(12);
    boost::this_thread::sleep_for(boost::chrono::milliseconds(200));

    IsFailure(threadsCounter.load(), (size_t)12);
    threadsCounter.store(0);
    ShutDownPool(threadPool);
}

void SleepThread(size_t milliseconds)//TODO: change name
{
    threadsCounter.boost::atomic<size_t>::fetch_add(1);
    boost::this_thread::sleep_for(boost::chrono::milliseconds(milliseconds));
}

static void TestSetNumOfThreadsAsCurrent()
{
    PrintTestHeader("Test SetNumOfThread() --> As current num of threads >>");
    ThreadPool threadPool(10);
    ThreadPool::taskPtr_t task(new Task<void, size_t>(&SleepThread, 500));
    for (size_t i = 0; i < 12; ++i)
    {
        threadPool.AddTask(task);
    }

    threadPool.SetNumOfThread(10);
    boost::this_thread::sleep_for(boost::chrono::milliseconds(200));

    IsFailure(threadsCounter.load(), (size_t)10);
    threadsCounter.store(0);
    ShutDownPool(threadPool);
}

static void TestIncrementNumOfThreadsWhenPaused()
{
    PrintTestHeader("Test SetNumOfThread() --> Increment threads when pool is paused >>");
    ThreadPool threadPool(10);
    ThreadPool::taskPtr_t task(new Task<void, size_t>(&SleepThread, 500));
    for (size_t i = 0; i < 10; ++i)
    {
        threadPool.AddTask(task);
    }
    boost::this_thread::sleep_for(boost::chrono::milliseconds(200));
    threadPool.Pause();
    for (size_t i = 0; i < 2; ++i)
    {
        threadPool.AddTask(task);
    }
    threadPool.SetNumOfThread(12);
    boost::this_thread::sleep_for(boost::chrono::milliseconds(200));
    IsFailure(threadsCounter.load(), (size_t)10);
    threadPool.Resume();
    boost::this_thread::sleep_for(boost::chrono::milliseconds(10));
    IsFailure(threadsCounter.load(), (size_t)12);

    threadsCounter.store(0);
    ShutDownPool(threadPool);
}

static void TestDecrementNumOfThreads()
{
    PrintTestHeader("Test SetNumOfThread() --> Decrement threads >>");
    ThreadPool threadPool(5);
    ThreadPool::taskPtr_t task1(new Task<void, void *>(&JustSleep, NULL));
    for (size_t i = 0; i < 5; ++i)
    {
        threadPool.AddTask(task1);
    }

    boost::this_thread::sleep_for(boost::chrono::milliseconds(200));
    threadPool.SetNumOfThread(3);

    ThreadPool::taskPtr_t task2(new Task<void, size_t>(&SleepThread, 2000));
    for (size_t i = 0; i < 5; ++i)
    {
        threadPool.AddTask(task2);
    }
    boost::this_thread::sleep_for(boost::chrono::milliseconds(1500));
    IsFailure(threadsCounter.load(), (size_t)3);
    threadsCounter.store(0);
    ShutDownPool(threadPool);
}

static void TestDecrementNumOfThreadsWhenPaused()
{
    PrintTestHeader("Test SetNumOfThread() --> Decrement threads when pool is paused >>");
    ThreadPool threadPool(5);
    ThreadPool::taskPtr_t task1(new Task<void, void *>(&JustSleep, NULL));
    for (size_t i = 0; i < 5; ++i)
    {
        threadPool.AddTask(task1);
    }

    boost::this_thread::sleep_for(boost::chrono::milliseconds(200));
    threadPool.Pause();
    //boost::this_thread::sleep_for(boost::chrono::milliseconds(800));
    threadPool.SetNumOfThread(3);
    boost::this_thread::sleep_for(boost::chrono::milliseconds(1000));
    threadPool.Resume();

    ThreadPool::taskPtr_t task2(new Task<void, size_t>(&SleepThread, 2000));
    for (size_t i = 0; i < 5; ++i)
    {
        threadPool.AddTask(task2);
    }
    boost::this_thread::sleep_for(boost::chrono::milliseconds(1500));
    IsFailure(threadsCounter.load(), (size_t)3);
    threadsCounter.store(0);
    ShutDownPool(threadPool);
}

void JustSleep(void *empty)
{
    boost::this_thread::sleep_for(boost::chrono::milliseconds(1000));
    (void)empty;
}

static void TestThreadsResumeBeforeTheySucceedToPause()
{
    PrintTestHeader("Test Threads Resume Before They Succeed To Pause >>");
    ThreadPool threadPool(1);
    ThreadPool::taskPtr_t task1(new Task<void, void *>(&JustSleep, NULL));
    for (size_t i = 0; i < 1; ++i)
    {
        threadPool.AddTask(task1);
    }
    boost::this_thread::sleep_for(boost::chrono::milliseconds(3));
    threadPool.Pause();
    boost::this_thread::sleep_for(boost::chrono::milliseconds(990));
    threadPool.Resume();
    ThreadPool::taskPtr_t task2(new Task<void, size_t>(&SleepThread, 0));
    for (size_t i = 0; i < 1; ++i)
    {
        threadPool.AddTask(task2);
    }

    boost::this_thread::sleep_for(boost::chrono::milliseconds(500));
    IsFailure(threadsCounter.load(), (size_t)1);
    threadsCounter.store(0);
    ShutDownPool(threadPool);
}

static void TestShutDownWhenPaused()
{
    PrintTestHeader("Test Shut down when paused >>");
    ThreadPool threadPool(100);

    ThreadPool::taskPtr_t task(new Task<void, size_t>(&SleepThread, 500));
    for (size_t i = 0; i < 200; ++i)
    {
        threadPool.AddTask(task);
    }

    boost::this_thread::sleep_for(boost::chrono::milliseconds(250));
    threadPool.Pause();
    boost::this_thread::sleep_for(boost::chrono::milliseconds(250));
    ShutDownPool(threadPool);
    IsFailure(threadsCounter.load(), (size_t)100);
    threadsCounter.store(0);
}

static void TestDestructor()
{
    PrintTestHeader("Test Destructor >>");
    ThreadPool threadPool(100);

    ThreadPool::taskPtr_t task(new Task<void, size_t>(&SleepThread, 1000));
    for (size_t i = 0; i < 100; ++i)
    {
        threadPool.AddTask(task);
    }
    boost::this_thread::sleep_for(boost::chrono::milliseconds(2000));
    IsFailure(threadsCounter.load(), (size_t)100);
    threadsCounter.store(0);
}

static void TestDestructor2()
{
    PrintTestHeader("Test Destructor2 >>");
    ThreadPool threadPool(100);

    ThreadPool::taskPtr_t task(new Task<void, size_t>(&SleepThread, 1000));
    for (size_t i = 0; i < 100; ++i)
    {
        threadPool.AddTask(task);
    }

    ThreadPool::taskPtr_t task2(new Task<void, bool>(&InfiniteLoop, true));
    threadPool.AddTask(task2);

    boost::this_thread::sleep_for(boost::chrono::milliseconds(2000));
    IsFailure(threadsCounter.load(), (size_t)100);
    threadsCounter.store(0);
    threadPool.ShutDown(1);
}

static void TestExecutinOfTaskswithFuture()
{
    PrintTestHeader("Test Executin of tasks with future >>");
    ThreadPool threadPool(4);

    std::vector<boost::shared_ptr<Future<bool> > > futureObjects;
    for (size_t i = 0; i < 4; ++i)
    {
        boost::shared_ptr<Future<bool> > futurePtr(new Future<bool>());
        futureObjects.push_back(futurePtr);
    }

    std::vector<boost::shared_ptr<Future<void> > > futureObjects2;
    for (size_t i = 0; i < 2; ++i)
    {
        boost::shared_ptr<Future<void> > futurePtr(new Future<void>());
        futureObjects2.push_back(futurePtr);
    }

    for (size_t i = 0; i < 2; ++i)
    {
        ThreadPool::taskPtr_t task1(new TaskWithFuture<bool, int>(&IsPrimeNumber, 2000051233, futureObjects[i]));
        threadPool.AddTask(task1, ThreadPool::HIGH);
    }

    for (size_t i = 2; i < 4; ++i)
    {
        ThreadPool::taskPtr_t task2(new TaskWithFuture<bool, int>(&IsPrimeNumber, 2000055450, futureObjects[i]));
        threadPool.AddTask(task2);
    }

    for (size_t i = 0; i < 2; ++i)
    {
        //ThreadPool::taskPtr_t task3(new TaskWithFuture<void, size_t>(&SleepThread, 5000, futureObjects2[i]));
        //threadPool.AddTask(task3, ThreadPool::LOW);
    }

    for (size_t i = 0; i < 2; ++i)
    {
        bool result = futureObjects[i]->Get();
        if (false == result || false == futureObjects[i]->IsValid())
        {
            IsFailure(result, true);
            IsFailure(futureObjects[i]->IsValid(), true);
            return;
        }
    }

    for (size_t i = 2; i < 4; ++i)
    {
        bool result = futureObjects[i]->Get();
        if (true == result || false == futureObjects[i]->IsValid())
        {
            IsFailure(result, false);
            IsFailure(futureObjects[i]->IsValid(), true);
            return;
        }
    }

    for (size_t i = 0; i < 2; ++i)
    {
        futureObjects2[i]->Wait();
        if (false == futureObjects2[i]->IsValid())
        {
            IsFailure(futureObjects2[i]->IsValid(), true);
            return;
        }
    }

    IsFailure(threadsCounter.load(), (size_t)2);
    threadsCounter.store(0);
    IsFailure(futureObjects[0]->Get(), true);
    ShutDownPool(threadPool);
}

bool IsPrimeNumber(int num)
{
    for (int i = 2; i < num; ++i)
    {
        if (0 == num % i)
        {
           return false;
        }
    }
    return true;
}

void InfiniteLoop(bool status)
{
    while (status) {}
}