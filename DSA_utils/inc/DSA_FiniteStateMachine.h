#ifndef __FSM_H
#define __FSM_H

#include <stdint.h>
typedef struct
{
    uint8_t MODBUS_FLAG;
    uint8_t BUTTON_FLAG;
    uint8_t EMERGENCY_FLAG;
} SystemFlag;

typedef enum
{
    STATE_IDLE = 0,
    STATE_HOMING,
    STATE_RUNNING,
    STATE_ERROR
} SystemState;

typedef enum
{
    EVENT_NONE = 0,
    EVENT_MODBUS_RECEIVED,
    EVENT_EMERGENCY_TRIGGERED,
    EVENT_TIM2_TICK
} SystemEvent;

#endif