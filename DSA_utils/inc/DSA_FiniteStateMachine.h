#ifndef __FSM_H
#define __FSM_H

#include <stdint.h>
typedef struct
{
    uint8_t BUTTON_FLAG;
    uint8_t EMERGENCY_FLAG;
} SystemFlag;

typedef enum
{
    STATE_INIT,
    STATE_IDLE,
    STATE_RUNNING_MODE,
    STATE_ERROR

} SystemState;

typedef enum
{
    EVENT_NONE,
    EVENT_IDLE,
    EVENT_EMERGENCY_PRESS,
    EVENT_BUTTON_PRESS,
} SystemEvent;

SystemState FSM_Get_Next_State(SystemState current_state, SystemEvent event);
void FSM_Handle_State(SystemState state);

#endif