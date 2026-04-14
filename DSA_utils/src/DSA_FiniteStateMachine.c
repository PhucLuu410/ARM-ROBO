#include "DSA_FiniteStateMachine.h"
#include "APP.h"
SystemState FSM_Get_Next_State(SystemState current_state, SystemEvent event)
{
    switch (current_state)
    {
    case STATE_IDLE:
        if (event == EVENT_BUTTON_PRESS)
            return STATE_RUNNING_MODE;
        else if (event == EVENT_EMERGENCY_PRESS)
            return STATE_ERROR;
        break;
    case STATE_RUNNING_MODE:
        if (event == EVENT_BUTTON_PRESS)
            return EVENT_IDLE;
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
        Arm_Stop();
        Display_ShowStop();
        break;
    case STATE_RUNNING_MODE:
        Arm_Start();
        Display_ShowMode1();
        break;
    case STATE_ERROR:
        Arm_Stop();
        Display_ShowError();
        break;
    default:
        break;
    }
}