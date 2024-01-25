#include <stdlib.h> /*malloc, free */
#include <assert.h> /*assert*/

#include "task.h"

extern const uuid_t BAD_UID;
struct task
{
    int (*action)(void *params);
    void *action_params;
    int (*kill)(void *kill_params);
    void *kill_params;
    size_t interval;
    time_t run_time;
    uuid_t uid;
};


task_t *TaskCreate(time_t run_time, size_t interval,
    int (*action)(void *action_params), void *action_params, int (*kill)(void *kill_params),
    void *kill_params)
{
    task_t *task = NULL;
    uuid_t uid = GetUID();
    if (IsEqualUID(BAD_UID, uid))
    {
        return NULL;
    }

    task = (task_t *)malloc(sizeof(*task)); /**MAYBE ADD UTIL FUNC HERE**/
    if (!task)
    {
        return NULL;
    }

    task->uid = uid;
    task->run_time = run_time;
    task->interval = interval;
    task->action = action;
    task->action_params = action_params;
    task->kill = kill;
    task->kill_params = kill_params;

    return task;

}

void TaskKillAndDestroy(task_t *task)
{
    assert(task);

    task->kill(task->kill_params);

    task->kill = NULL;
    task->kill_params = NULL;
    task->action = NULL;
    task->action_params = NULL;

    free(task);
    task = NULL;
}

uuid_t TaskGetUid(const task_t *task)
{
    assert(task);
    return task->uid;
}

void TaskSetRunTime(task_t *task, time_t run_time)
{
    assert(task);
    task->run_time = run_time;
}

time_t TaskGetRunTime(task_t *task)
{
    assert(task);
    return task->run_time;
}

size_t TaskGetInterval(const task_t *task)
{
    assert(task);
    return task->interval;
}

task_t *TaskClone(const task_t *task)
{
    task_t *clone = {0};
    assert(task);

     clone = TaskCreate(task->run_time, task->interval, task->action,
                    task->action_params, task->kill, task->kill_params);

        return clone;
}

int TaskExecute(task_t *task)
{
    assert(task);
    assert(task->action);
    return task->action(task->action_params);
}
