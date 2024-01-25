/*****************************************************************************

Author:     Amatzia Safra
Reviewer:   Shany
Date:       26.9.2021
Group:      OL110

*****************************************************************************/

#include <stdio.h> /* printf */

#include "scheduler.h"

#define FAIL(func) do                                                                       \
                   {                                                                        \
                       TestsFailed(1);                                                      \
                       printf("\n\33[1;31m%s\33[0m failed at line %d\n", #func, __LINE__);  \
                   }                                                                        \
                   while(0);

#define TEST(test) do                           \
                   {                            \
                       TestsRun(1);             \
                       if (!(test))             \
                       {                        \
                           is_successful = 0;   \
                           FAIL(test);          \
                       }                        \
                   }                            \
                   while(0);

#define FUNCTEST(func) do                                                           \
                       {                                                            \
                           if (func)                                                \
                           {                                                        \
                               printf("\n\33[32;1m%s succeeded.\33[0m", #func);     \
                           }                                                        \
                           else                                                     \
                           {                                                        \
                               printf("\n\33[1;31m%s failed.\33[0m", #func);        \
                           }                                                        \
                       }                                                            \
                       while(0);

typedef struct
{
    schedule_t *sched;
    uuid_t uid;
} sched_uid_t;

static int IntSchedStop(void *sched);

static int Action(void *param);
static int Action2(void *param);
static int Action3(void *param);
static int WhoFirst1(void *param);
static int WhoFirst2(void *param);
static int SizeAction(void *param);
static int RemoveTaskAction(void *sched_uid);

static int Kill(void *param);
static int EmptyKill(void *param);

static void CreateDestroyTest(void);
static int SizeIsEmptyTest1(void);
static int AddRemoveTaskTest1(void);
static int AddRemoveTaskTest2(void);
static int ClearTest(void);
static int RunTest(void);
static int RunStopTest1(void);
static int RunStopTest2(void);
static int RunStopTest3(void);
static int RunStopTest4(void);
static int RunSchedSizeTest(void);
static int RunSchedRemoveTest(void);

static size_t TestsRun(int add);
static size_t TestsFailed(int add);
static void SummarizeTests(void);


int main()
{
    CreateDestroyTest();
    FUNCTEST(SizeIsEmptyTest1())
    FUNCTEST(AddRemoveTaskTest1())
    FUNCTEST(AddRemoveTaskTest2())
    FUNCTEST(ClearTest())
    FUNCTEST(RunTest())
    FUNCTEST(RunStopTest1())
    FUNCTEST(RunStopTest2())
    FUNCTEST(RunStopTest3())
    FUNCTEST(RunStopTest4())
    FUNCTEST(RunSchedSizeTest())
    FUNCTEST(RunSchedRemoveTest())

    /*FUNCTEST(RunStopTest1())*/

    SummarizeTests();

    return 0;
}


static int IntSchedStop(void *sched)
{
    SchedStop((schedule_t *)sched);

    return 0;
}


static int Action(void *param)
{
    ++*(size_t *)param;

    return 0;
}

static int Action2(void *param)
{
    static int times = 3;

    *(time_t *)param = time(NULL);

    return !(--times > 0);
}

static int Action3(void *param)
{
    *(time_t *)param = time(NULL);

    return 0;
}

static int WhoFirst1(void *param)
{
    if (0 == *(int *)param)
    {
        *(int *)param = 1;
    }

    return 0;
}

static int WhoFirst2(void *param)
{
    if (0 == *(int *)param)
    {
        *(int *)param = 2;
    }

    return 0;
}

static int SizeAction(void *param)
{
    void **arr = (void **)param;

    *(size_t *)(arr[1]) = SchedSize((schedule_t *)arr[0]);

    return 0;
}

static int RemoveTaskAction(void *sched_uid)
{
    schedule_t *sched = ((sched_uid_t *)sched_uid)->sched;
    uuid_t uid = ((sched_uid_t *)sched_uid)->uid;

    SchedRemoveTask(sched, uid);

    return 0;
}


static int Kill(void *param)
{
    ++*(size_t *)param;

    return 0;
}

static int EmptyKill(void *param)
{
    (void)param;

    return 0;
}


static void CreateDestroyTest(void)
{
    schedule_t *sched = SchedCreate();

    SchedDestroy(sched);
}

static int SizeIsEmptyTest1(void)
{
    schedule_t *sched = SchedCreate();
    int is_successful = 1;

    TEST(SchedIsEmpty(sched));
    TEST(SchedSize(sched) == 0);

    SchedDestroy(sched);

    return is_successful;
}

static int AddRemoveTaskTest1(void)
{
    schedule_t *sched = SchedCreate();
    uuid_t uid1 = BAD_UID;
    size_t actions_done = 0;
    size_t killed = 0;
    int is_successful = 1;

    uid1 = SchedAddTask(sched, time(NULL) + 3, 1, Action, &actions_done, Kill, &killed);

    TEST(!SchedIsEmpty(sched))
    TEST(SchedSize(sched) == 1)
    TEST(0 == actions_done)
    TEST(0 == killed)

    TEST(SchedRemoveTask(sched, uid1) == 0)
    TEST(SchedIsEmpty(sched))
    TEST(SchedSize(sched) == 0)

    SchedDestroy(sched);
    TEST(0 == actions_done)
    TEST(1 == killed)

    return is_successful;
}

static int AddRemoveTaskTest2(void)
{
    schedule_t *sched = SchedCreate();
    uuid_t uid1 = BAD_UID, uid2 = BAD_UID, uid3 = BAD_UID, uid4 = BAD_UID;
    size_t actions_done = 0;
    size_t killed = 0;
    int is_successful = 1;

    uid1 = SchedAddTask(sched, time(NULL) + 3, 1, Action, &actions_done, Kill, &killed);

    TEST(!SchedIsEmpty(sched))
    TEST(SchedSize(sched) == 1)
    TEST(0 == killed)

    TEST(SchedRemoveTask(sched, uid1) == 0)
    TEST(SchedIsEmpty(sched))
    TEST(SchedSize(sched) == 0)
    TEST(1 == killed)


    uid2 = SchedAddTask(sched, time(NULL) + 3, 1, Action, &actions_done, Kill, &killed);
    uid3 = SchedAddTask(sched, time(NULL) + 3, 1, Action, &actions_done, Kill, &killed);
    TEST(!IsEqualUID(uid2, uid3))
    TEST(!SchedIsEmpty(sched))
    TEST(SchedSize(sched) == 2)

    TEST(SchedRemoveTask(sched, uid4) != 0)
    TEST(SchedSize(sched) == 2)
    TEST(1 == killed)

    uid4 = SchedAddTask(sched, time(NULL) + 1, 1, Action, &actions_done, Kill, &killed);

    SchedDestroy(sched);
    TEST(4 == killed)

    return is_successful;
}

static int ClearTest(void)
{
    schedule_t *sched = SchedCreate();
    size_t actions_done = 0;
    size_t killed = 0;
    int is_successful = 1;

    SchedAddTask(sched, time(NULL) + 3, 1, Action, &actions_done, Kill, &killed);
    SchedAddTask(sched, time(NULL) + 3, 1, Action, &actions_done, Kill, &killed);
    SchedAddTask(sched, time(NULL) + 3, 1, Action, &actions_done, Kill, &killed);
    SchedAddTask(sched, time(NULL) + 1, 1, Action, &actions_done, Kill, &killed);

    TEST(0 == actions_done)
    TEST(0 == killed)

    SchedClear(sched);
    TEST(SchedIsEmpty(sched));
    TEST(SchedSize(sched) == 0);
    TEST(0 == actions_done)
    TEST(4 == killed)

    SchedDestroy(sched);
    TEST(0 == actions_done)
    TEST(4 == killed)

    return is_successful;
}

/* "action" function self terminating & timing check + run empty scheduler */
static int RunTest(void)
{
    schedule_t *sched = SchedCreate();
    time_t begin_time = 0;
    time_t end_time = 0;
    time_t kill_time = 0;
    int is_successful = 1;

    SchedRun(sched);

    SchedAddTask(sched, time(NULL) + 1, 1, Action2, &end_time, Action3, &kill_time);
    TEST(!SchedIsEmpty(sched))
    TEST(SchedSize(sched) == 1)

    begin_time = time(NULL);
    SchedRun(sched);
    TEST(SchedIsEmpty(sched))
    TEST(SchedSize(sched) == 0)
    TEST(3 == end_time - begin_time)
    TEST(kill_time == end_time)

    SchedDestroy(sched);

    return is_successful;
}

/* stop by stop function + timing check + re-run after stopping */
static int RunStopTest1(void)
{
    schedule_t *sched = SchedCreate();
    time_t add_time = 0;
    time_t action_last_time = 0;
    size_t killed = 0;
    int is_successful = 1;

    add_time = time(NULL);
    SchedAddTask(sched, add_time + 1, 1, Action3, &action_last_time, Kill, &killed);
    SchedAddTask(sched, add_time + 4, 1, IntSchedStop, sched, Kill, &killed);
    TEST(SchedSize(sched) == 2)

    SchedRun(sched);
    TEST(3 == action_last_time - add_time)
    TEST(SchedSize(sched) == 2)
    TEST(0 == killed)

    SchedRun(sched);

    SchedDestroy(sched);
    TEST(2 == killed);

    return is_successful;
}

/* testing FIFO of scheduler */
static int RunStopTest2(void)
{
    schedule_t *sched = SchedCreate();
    int who_first = 0;
    int garbage_param = 0;
    int is_successful = 1;

    SchedAddTask(sched, time(NULL) + 1, 1, WhoFirst1, &who_first, EmptyKill, &garbage_param);
    SchedAddTask(sched, time(NULL) + 1, 1, WhoFirst2, &who_first, EmptyKill, &garbage_param);
    SchedAddTask(sched, time(NULL) + 2, 1, IntSchedStop, sched, EmptyKill, &garbage_param);
    TEST(!SchedIsEmpty(sched))
    TEST(SchedSize(sched) == 3)

    SchedRun(sched);
    TEST(1 == who_first)

    SchedDestroy(sched);

    return is_successful;
}

/* testing scheduler priority */
static int RunStopTest3(void)
{
    schedule_t *sched = SchedCreate();
    int who_first = 0;
    int garbage_param = 0;
    int is_successful = 1;

    SchedAddTask(sched, time(NULL) + 2, 1, WhoFirst1, &who_first, EmptyKill, &garbage_param);
    SchedAddTask(sched, time(NULL) + 1, 1, WhoFirst2, &who_first, EmptyKill, &garbage_param);
    SchedAddTask(sched, time(NULL) + 4, 1, IntSchedStop, sched, EmptyKill, &garbage_param);
    TEST(!SchedIsEmpty(sched));
    TEST(SchedSize(sched) == 3);

    SchedRun(sched);
    TEST(2 == who_first)

    SchedDestroy(sched);

    return is_successful;
}

/* when a task is removed before running */
static int RunStopTest4(void)
{
    schedule_t *sched = SchedCreate();
    uuid_t uid = BAD_UID;
    size_t count = 0;
    int garbage_param = 0;
    int is_successful = 1;

    SchedAddTask(sched, time(NULL) + 1, 1, Action, &count, EmptyKill, &garbage_param);
    uid = SchedAddTask(sched, time(NULL) + 1, 1, Action, &count, EmptyKill, &garbage_param);
    SchedAddTask(sched, time(NULL) + 2, 1, IntSchedStop, sched, EmptyKill, &garbage_param);
    SchedRemoveTask(sched, uid);

    SchedRun(sched);
    TEST(1 == count)

    SchedDestroy(sched);

    return is_successful;
}

static int RunSchedSizeTest(void)
{
    schedule_t *sched = SchedCreate();
    int garbage_param = 0;
    int is_successful = 1;
    size_t size = 0;
    void *arr[2];
    arr[0] = (void *)sched;
    arr[1] = (void *)&size;

    SchedAddTask(sched, time(NULL), 1, SizeAction, arr, EmptyKill, &garbage_param);
    SchedAddTask(sched, time(NULL) + 1, 1, IntSchedStop, sched, EmptyKill, &garbage_param);

    SchedRun(sched);
    TEST(2 == size)

    SchedDestroy(sched);

    return is_successful;
}

static int RunSchedRemoveTest(void)
{
    schedule_t *sched = SchedCreate();
    uuid_t uid = BAD_UID;
    int garbage_param = 0;
    int is_successful = 1;
    sched_uid_t sched_uid = {0};
    sched_uid.sched = sched;
    sched_uid.uid = uid;

    SchedAddTask(sched, time(NULL) + 1, 1, EmptyKill, &garbage_param, EmptyKill, &garbage_param);
    uid = SchedAddTask(sched, time(NULL) + 1, 1, RemoveTaskAction, &sched_uid, EmptyKill, &garbage_param);
    SchedAddTask(sched, time(NULL) + 2, 1, IntSchedStop, sched, EmptyKill, &garbage_param);
    sched_uid.uid = uid;
    TEST(SchedSize(sched) == 3)

    SchedRun(sched);
    TEST(SchedSize(sched) == 2)

    SchedDestroy(sched);

    return is_successful;
}


static size_t TestsRun(int add)
{
    static size_t tests = 0;

    tests += add;

    return tests;
}

static size_t TestsFailed(int add)
{
    static size_t failures = 0;

    failures += add;

    return failures;
}

static void SummarizeTests(void)
{
    size_t tests = TestsRun(0);
    size_t failures = TestsFailed(0);

    if (0 == failures)
    {
        printf("\n\n%ld tests done succeessfully.", tests);
    }
    else
    {
        printf("\n\n%ld tests, %ld failures", tests, failures);
    }

    printf("\n\n");
}
