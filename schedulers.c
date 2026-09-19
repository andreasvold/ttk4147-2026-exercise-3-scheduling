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
    int num_tasks = 0;

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

    do
    {
        // find next task to run
        int min_arrivaltime = 1000000;
        int min_taskindex = -1;

        for(int i = 0; i<taskCount; i++){
            if (tasks[i]->arrivalTime <= globalTime && tasks[i]->state != finished){
                if (tasks[i]->arrivalTime < min_arrivaltime){
                    min_arrivaltime = tasks[i]->arrivalTime;
                    min_taskindex = i;
                }
            }

        }
        // choose task to run from queue
        if(min_taskindex != -1){
            // Set the task state to running
            if (tasks[min_taskindex]->startTime == -1){
                tasks[min_taskindex]->startTime = globalTime;
                set_task_state(tasks[min_taskindex], running);
                do
            {
                pthread_mutex_lock(&timeMutex);
                pthread_cond_wait(&timeCond, &timeMutex);
                pthread_mutex_unlock(&timeMutex);

            } while (tasks[min_taskindex]->state != finished);
            }
        }
        else {
            // wait for the global clock/time condition to update.
            pthread_mutex_lock(&timeMutex);
            pthread_cond_wait(&timeCond, &timeMutex);
            pthread_mutex_unlock(&timeMutex);
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
