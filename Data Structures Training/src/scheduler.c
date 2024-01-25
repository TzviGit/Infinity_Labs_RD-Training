/******************************************************************************
* Author        Tzvi Moskowitz
* Reviewer      Shahar Refael Shoshany
* Description   Scheduler module
* Group         OL110
* Company       ILRD Ramat Gan
* ****************************************************************************/
#include <stdlib.h> /*malloc ,free */
#include <assert.h> /*assert**/

#include "scheduler.h" /*API of this file*/
#include "task.h"
#include "priority_q.h" /*API of Priority_q*/




struct scheduler
{
    int is_running;
    int remove_curr_task;
    pq_t *task_queue;
    task_t *task_running;
};

static int prioritizeByRunTime(const void *data1, const void *data2)
{
    task_t *task1 = (task_t *)data1;
    task_t *task2 = (task_t *)data2;

    assert(data1 && data2);

    /*****PRIORITIZE EARLIER TIMES*****/
    if (TaskGetRunTime(task1) > TaskGetRunTime(task2))
    {
        return -1;  /*MUST DO THIS WAY BECUASE SIZE_T CAN OVERFLOW SIGNED INT*/
    }
    else
    {
        return (TaskGetRunTime(task2) > TaskGetRunTime(task1));
    }
}

static int IsSameTaskUID(const void *data, const void *criteria)
{
    task_t *task = (task_t *)data;
    uuid_t *uid = (uuid_t *)criteria;

    assert(data);
    assert(criteria);

    return IsEqualUID(TaskGetUid(task), *uid);
}


static void SleepEnoughTime(time_t run_time);

schedule_t *SchedCreate(void)
{
    schedule_t *scheduler = (schedule_t *)malloc(sizeof(*scheduler));

    if (scheduler)
    {
        pq_t *priority_q = PQCreate(prioritizeByRunTime);
        if (!priority_q)
        {
            free(scheduler); scheduler = NULL;
            return NULL;
        }

        scheduler->task_queue = priority_q;
        scheduler->is_running = 0;
        scheduler->remove_curr_task = 0;
        scheduler->task_running = NULL;
    }

    return scheduler;

}

void SchedDestroy(schedule_t *sched)
{
    assert(sched);

    SchedClear(sched);
    PQDestroy(sched->task_queue);
    sched->task_queue = NULL;

    free(sched);
    sched = NULL;
}

size_t SchedSize(const schedule_t *sched)
{
    assert(sched);
    return PQSize(sched->task_queue) + !!sched->task_running;
}

int SchedIsEmpty(const schedule_t *sched)
{
    assert(sched);
    return PQIsEmpty(sched->task_queue) && !sched->task_running;
}

uuid_t SchedAddTask(schedule_t *sched, time_t run_time, size_t interval_in_sec,
    int (*action)(void *param), void *param, int (*kill)(void *kill_param),
    void *kill_param)
{
    task_t *new_task = TaskCreate(run_time, interval_in_sec, action, param, kill, kill_param);

    if (!new_task)
    {
        return BAD_UID;
    }

    assert(sched);
    assert(action);
    assert(kill);
    PQEnqueue(sched->task_queue, (void *)new_task);


    return TaskGetUid(new_task);
}

int SchedRemoveTask(schedule_t *sched, uuid_t uid)
{
    task_t *task = NULL;
    assert(sched);

    task = (task_t *)PQEraseIf(sched->task_queue, &uid , IsSameTaskUID);
    if (!task)
    {
        task = sched->task_running;

        if (!task || !IsEqualUID(uid, TaskGetUid(task)))
        {
            return 1;
        }

        sched->remove_curr_task = 1;;
    }
    else
    {
        TaskKillAndDestroy(task);
        task = NULL;
    }

    return 0;
}


void SchedClear(schedule_t *sched)
{
    assert(sched);

    sched->remove_curr_task = 1;

    while (!PQIsEmpty(sched->task_queue))
    {
        TaskKillAndDestroy((task_t *)PQDequeue(sched->task_queue));
    }

}

int SchedRun(schedule_t *sched)
{


    assert(sched);

    sched->is_running = 1;

    while (sched->is_running && !SchedIsEmpty(sched))
    {
        int put_back = 0;
        task_t *task = NULL;
        double curr_time = 0;

        curr_time = time(NULL);

        if ((time_t)-1 == curr_time)
       {
           sched->is_running = 0;
           return -1;
       }

        task = PQDequeue(sched->task_queue);

        if (TaskGetRunTime(task) > curr_time)
        {
            SleepEnoughTime(TaskGetRunTime(task));
        }

        sched->task_running = task;

        put_back = (0 == TaskExecute(task));


        if (!put_back || sched->remove_curr_task == 1)
        {
            TaskKillAndDestroy(task);
            task = NULL;
            sched->remove_curr_task = 0;
        }
        else
        {
           TaskSetRunTime(task, (TaskGetRunTime(task) + TaskGetInterval(task)));

            if (PQEnqueue(sched->task_queue, (void *)task))
            {
                TaskKillAndDestroy(task);
                sched->is_running = 0;

                return -1;
            }
        }

        sched->task_running = NULL;
    }

    return 0;
}

static void SleepEnoughTime(time_t run_time)
{
    int flag = 0;

    do
    {
        flag = sleep(difftime(run_time, time(NULL)));
    }
    while (0 != flag);
}

void SchedStop(schedule_t *sched)
{
    assert(sched);
    sched->is_running = 0;
}
