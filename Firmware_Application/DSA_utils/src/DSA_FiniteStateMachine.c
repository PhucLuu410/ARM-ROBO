#include "DSA_FiniteStateMachine.h"
#include "APP.h"

extern ModbusRequest save_data[4];

SystemState FSM_Get_Next_State(SystemState current_state, SystemEvent event)
{
    switch (current_state)
    {
    case STATE_IDLE:
        if (event == EVENT_BUTTON_PRESS)
            return STATE_RUNNING_MODE1;
        else if (event == EVENT_EMERGENCY_PRESS)
            return STATE_ERROR;
        break;
    case STATE_RUNNING_MODE1:
        if (event == EVENT_BUTTON_PRESS)
            return STATE_RUNNING_MODE2;
        else if (event == EVENT_EMERGENCY_PRESS)
            return STATE_ERROR;
        break;
    case STATE_RUNNING_MODE2:
        if (event == EVENT_BUTTON_PRESS)
            return STATE_IDLE;
        else if (event == EVENT_EMERGENCY_PRESS)
            return STATE_ERROR;
        break;
    case STATE_ERROR:
        if (event == EVENT_IDLE)
            return STATE_IDLE;
        break;
    default:
        return current_state;
    }
    return current_state;
}

void FSM_Handle_State(SystemState state)
{
    switch (state)
    {
    case STATE_IDLE:
        Arm_Start();
        Arm_OffSet();
        Arm_Stop();
        Display_ShowStop();
        break;
    case STATE_RUNNING_MODE1:
        Arm_Start();
        Arm_Mode1();
        Display_ShowMode1();
        break;
    case STATE_RUNNING_MODE2:
        Arm_Mode2();
        Display_ShowMode2();
        break;
    case STATE_ERROR:
        Arm_Stop();
        Display_ShowError();
        break;
    default:
        break;
    }
}