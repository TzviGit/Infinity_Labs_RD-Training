/********************************************************
 *
 * Author : Tzvi Moskowitz
 * Reviewed and approved by : ~
 *
 *  disccription - API for tasks
 * 22/09/2021
 * OLRD 110 Ramat Gan
 *
 **********************************************************/

#ifndef __TASK_OL_110_H__
#define __TASK_OL_110_H__

#include <time.h>
#include "uid.h"

typedef struct task task_t;



task_t *TaskCreate(time_t run_time, size_t interval,
    int (*action)(void *action_params), void *action_params,
    int (*kill)(void *kill_params), void *kill_param);

void TaskKillAndDestroy(task_t *task);

uuid_t TaskGetUid(const task_t *task);

void TaskSetRunTime(task_t *task, time_t run_time);

time_t TaskGetRunTime(task_t *task);

size_t TaskGetInterval(const task_t *task);

task_t *TaskClone(const task_t *task);


int TaskExecute(task_t *task);

#endif /* __TASK_OL_110_H__ */
