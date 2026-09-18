#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include "scheduling.h"
#include "schedulers.h"

void set_task_state(struct Task *task, enum taskState taskNewState)
{
    pthread_mutex_lock(&taskStateMutex);
    task->state = taskNewState;
    pthread_mutex_unlock(&taskStateMutex);
}
enum taskState read_task_state(struct Task *task){
    pthread_mutex_lock(&taskStateMutex);
    enum taskState state = task->state;
    pthread_mutex_unlock(&taskStateMutex);

    return state;
}

void wait_for_rescheduling(int quantum, struct Task *task)
{
    int startTime;
    int waitTime;

    pthread_mutex_lock(&timeMutex);
    startTime = globalTime;
    pthread_mutex_unlock(&timeMutex);

    do
    {
        pthread_mutex_lock(&timeMutex);
        pthread_cond_wait(&timeCond, &timeMutex);
        waitTime = globalTime - startTime;
        pthread_mutex_unlock(&timeMutex);
    } while (task->state != finished && waitTime < quantum);

    usleep(timeUnitUs / 100);
}

void round_robin(struct Task **tasks, int taskCount, int timeout, int quantum)
{
    int taskIndex = 0;

    do
    {
        // Skip finished tasks or those that have not arrived yet
        if (tasks[taskIndex]->state == finished || tasks[taskIndex]->arrivalTime > globalTime)
        {
            taskIndex = (taskIndex + 1) % taskCount;
            continue;
        }

        // Set the task state to running
        if (tasks[taskIndex]->startTime == -1)
            tasks[taskIndex]->startTime = globalTime;
        set_task_state(tasks[taskIndex], running);

        // Wait for the quantum interval
        wait_for_rescheduling(quantum, tasks[taskIndex]);

        //  Check if the task is finished
        if (tasks[taskIndex]->state == finished)
        {
        }
        else
        {
            set_task_state(tasks[taskIndex], preempted);
        }

        // Find the next task to run
        taskIndex = (taskIndex + 1) % taskCount;

    } while (globalTime < timeout);
}

// Implement your schedulers here!
void first_come_first_served(struct Task **tasks, int taskCount, int timeout)
{
    // gives the cpu to the task that has been in the ready queue the longest and lets it run till its finished. non-preemptive
    int taskIndex = 0;
    int task_running_index = -1;

    do
    {
        int task_lowest_arrivaltime = 10000000;
        if (task_running_index >= && tasks[task_running_index]->state == running){
            continue;
        }
        else{
            for(int i = 0; i< 4; i++){
                if(tasks[i]->arrivalTime < globalTime && tasks[i]->state != finished && tasks[i]->arrivaltime <= tasks[task_current_lowest_arrtime]->arrivalTime){
                    task_current_lowest_arrtime = i;
                }
            }
            if (task_current_lowest_arrtime != 10000000){
                task_running_index = task_current_lowest_arrtime;
                set_task_state(tasks[task_running_index], running);
            }

        }
    } while (globalTime < timeout);



}
void shortest_process_next(struct Task **tasks, int taskCount, int timeout)
{
    // Implement your solution here
}
void highest_response_ratio_next(struct Task **tasks, int taskCount, int timeout)
{
    // Implement your solution here
}
void shortest_remaining_time(struct Task **tasks, int taskCount, int timeout, int quantum)
{
    // Implement your solution here
}
void feedback(struct Task **tasks, int taskCount, int timeout, int quantum)
{
    // Implement your solution here
}
