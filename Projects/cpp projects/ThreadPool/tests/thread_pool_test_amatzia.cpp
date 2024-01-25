/*****************************************************************************

Author:         Amatzia Safra
Company:		Infinity
Reviewer:       
Date:           
Group:          OL110
Description:    

*****************************************************************************/
#include <iostream>			// std::cout
#include <unistd.h>         // sleep
#include <cstdio>           // printf
#include <time.h>           // time_t, time()

#include "tests.hpp"		// tests' macros (TEST, FUNCTEST) and functions
							// (SummarizeTests).
#include "thread_pool.hpp"  

static int CtorAndAddTaskTest(void);
static int SetBiggerNumOfThreadsTest(void);
static int SetSmallerNumOfThreadsTest(void);
static int ShutDownTest(void);
static int PriorityAndPauseResumeTest(void);

using namespace ilrd;

int main()
{
    FUNCTEST(CtorAndAddTaskTest())
    FUNCTEST(SetBiggerNumOfThreadsTest())
    FUNCTEST(SetSmallerNumOfThreadsTest())
    FUNCTEST(ShutDownTest())
    FUNCTEST(PriorityAndPauseResumeTest())

    SummarizeTests();

    return 0;
}

static size_t g_count = 0;
static boost::mutex g_mutex;
static boost::condition_variable g_cv;

void FuncForCount(int i)
{
    ++g_count;

    boost::unique_lock<boost::mutex> lock(g_mutex);

    g_cv.wait(lock);

    --g_count;

    (void)i;
}

static int CtorAndAddTaskTest(void)
{
    int is_successful = 1;
    
    g_count = 0;
    size_t num_of_threads = 5;

    ThreadPool thread_pool(num_of_threads);

    for (size_t i = 0; i < num_of_threads * 2; i++)
    {
        thread_pool.AddTask(boost::shared_ptr<TaskInterface>
                                (new Task<void, int>(FuncForCount, 0)));
    }

    // A short sleeping so that all the threads will enter and increase g_count.
    boost::this_thread::sleep_for(boost::chrono::milliseconds(300));
    
    // Since there is a "wait" (by condition variable) which prevent the
    // threads from exiting, now "g_count" supposed to be exactly as the
    // "num_of_threads" we requested.
    TEST(g_count == num_of_threads);

    return is_successful;

    // Although all the threads are still waiting (because we hanv't "notify"ed
    // them), ThreadPool's Dtor cancel them all.
}

// See comments on "CtorAndAddTaskTest"
static int SetBiggerNumOfThreadsTest(void)
{
    int is_successful = 1;
    
    g_count = 0;
    size_t num_of_threads = 5;
    size_t new_num_of_threads = num_of_threads + 3;

    ThreadPool thread_pool(num_of_threads);

    for (size_t i = 0; i < num_of_threads * 2; i++)
    {
        thread_pool.AddTask(boost::shared_ptr<TaskInterface>
                                (new Task<void, int>(FuncForCount, 0)));
    }

    thread_pool.SetNumOfThread(new_num_of_threads);

    boost::this_thread::sleep_for(boost::chrono::milliseconds(500));
    
    TEST(g_count == new_num_of_threads);

    return is_successful;
}

// See comments on "CtorAndAddTaskTest"
static int SetSmallerNumOfThreadsTest(void)
{
    int is_successful = 1;
    
    g_count = 0;
    size_t num_of_threads = 5;
    size_t new_num_of_threads = num_of_threads - 3;

    ThreadPool thread_pool(num_of_threads);
    //usleep(1000);
    thread_pool.SetNumOfThread(new_num_of_threads);
    
    for (size_t i = 0; i < num_of_threads * 2; i++)
    {
        thread_pool.AddTask(boost::shared_ptr<TaskInterface>
                                (new Task<void, int>(FuncForCount, 0)));
    }
    
    boost::this_thread::sleep_for(boost::chrono::milliseconds(200));

    TEST(g_count == new_num_of_threads);

    return is_successful;
}

int EmptyFunc(int i)
{
    return i;
}

void SleepFunc(int time)
{
    ++g_count;

    sleep(time);
}

static int ShutDownTest()
{
    int is_successful = 1;
    g_count = 0;


    ThreadPool thread_pool(5);

    for (size_t i = 0; i < 5; i++)
    {
        thread_pool.AddTask(boost::shared_ptr<TaskInterface>
                                (new Task<void, int>(SleepFunc, 10)));
    }

    // In order at least some of the threads will arise and start to "sleep()".
    while (g_count < 3);

    // Since all the threads are "sleep()"ing,
    // they haven't returned (and were forced to being terminated),
    // hence ShutDown returns false.
    time_t before = time(NULL);
    TEST(thread_pool.ShutDown(1) == false)
    // Although there were 5 threads, and all of them were "sleep()"ing
    // (that is, "ShutDown()" waited for the five of them), the whole
    // waiting period should be one second only, rather than one second for
    // every thread.
    TEST(time(NULL) - before == 1)


/***********************************************/
    ThreadPool thread_pool2(5);

    for (size_t i = 0; i < 3; i++)
    {
        thread_pool2.AddTask(boost::shared_ptr<TaskInterface>
                                (new Task<int, int>(EmptyFunc, i)));
    }

    TEST(thread_pool2.ShutDown(1) == true)



/***********************************************/
    g_count = 0;

    ThreadPool thread_pool3(5);

    for (size_t i = 0; i < 3; i++)
    {
        thread_pool3.AddTask(boost::shared_ptr<TaskInterface>
                                (new Task<int, int>(EmptyFunc, i)));
    }

    thread_pool3.AddTask(boost::shared_ptr<TaskInterface>
                            (new Task<void, int>(SleepFunc, 10)));

    // Wait until the one "SleepFunc()" will arise.
    while (g_count < 1);

    TEST(thread_pool3.ShutDown(1) == false)

    return is_successful;
}

std::vector<int> g_vector;

void QueuePushFunc(int i)
{
    g_vector.push_back(i);
    ++g_count;
}

// If Pause and Resume were implemented incorrectly,
// a deadlock might occure (if "Resume() notified before all the "pausing tasks"
// have arrived to the condition variable, and they wait forever).
static int PriorityAndPauseResumeTest(void)
{
    int is_successful = 1;

    // With one thread only is much easier to detect the order of popping
    // tasks, because only one task is popped in a time, and just after the
    // previous were completed.
    ThreadPool thread_pool(1);

    g_count = 0;

    // Need a "Pause()" here in order that all the tasks will be really popped
    // at the expected order (if wasn't a "Pause()" here, 1 of 1st iteration
    // could be popped before 3 of 2nd iteration, as the latter still wouldn't
    // be pushed yet).
    thread_pool.Pause();
    //sleep(1);
    for (size_t i = 0; i < 3; i++)
    {
        thread_pool.AddTask(boost::shared_ptr<TaskInterface>
                                (new Task<void, int>(QueuePushFunc, 1)),
                            ThreadPool::LOW);
        thread_pool.AddTask(boost::shared_ptr<TaskInterface>
                                (new Task<void, int>(QueuePushFunc, 2)),
                            ThreadPool::NORMAL);
        thread_pool.AddTask(boost::shared_ptr<TaskInterface>
                                (new Task<void, int>(QueuePushFunc, 3)),
                            ThreadPool::HIGH);
    }
    thread_pool.Resume();
    
    int back = 0;

    // Waiting for all the tasks to be done (and all the numbers to be pushed).
    while (g_count < 9);
    
    for (size_t i = 0; i < 3; i++)
    {
        back = g_vector.back();
        TEST(back == 1)
        
        g_vector.pop_back();
    }

    for (size_t i = 0; i < 3; i++)
    {
        back = g_vector.back();
        TEST(back == 2)
        g_vector.pop_back();
    }

    for (size_t i = 0; i < 3; i++)
    {
        back = g_vector.back();
        TEST(back == 3)
        g_vector.pop_back();
    }

    return is_successful;
}