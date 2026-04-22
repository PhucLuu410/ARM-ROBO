#ifndef __DSA_SCHEDULE_H
#define __DSA_SCHEDULE_H

#include <stdint.h>
#include <stdbool.h>
#include "BSP.h"

typedef struct
{
    void (*pTask)(void);
    uint32_t period;
    uint32_t last_run;
} Schedule_Task;

void SCHEDULE_Create_Task(void (*task_function)(void), uint32_t period);
void SCHEDULE_Run(void);

#endif