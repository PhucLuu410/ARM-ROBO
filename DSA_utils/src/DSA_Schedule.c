#include "DSA_Schedule.h"

Schedule_Task schedule_tasks[10];
uint8_t task_count = 0;
extern volatile uint32_t count;
void SCHEDULE_Create_Task(void (*task_function)(void), uint32_t period)
{
    if (task_count < 10)
    {
        schedule_tasks[task_count].pTask = task_function;
        schedule_tasks[task_count].period = period;
        schedule_tasks[task_count].last_run = 0;
        task_count++;
    }
}
void SCHEDULE_Run(void)
{
    uint32_t current_time = count;
    for (uint8_t i = 0; i < task_count; i++)
    {
        if (current_time - schedule_tasks[i].last_run >= schedule_tasks[i].period)
        {
            schedule_tasks[i].last_run = current_time;
            schedule_tasks[i].pTask();
        }
    }
}