#include "main.h"

SystemState current_state = STATE_INIT;
SystemEvent current_event = EVENT_NONE;

int main(void)
{
    RCC_Init();
    TIM2_Init();
    TIM3_Init();
    EXTI_Init();
    UART_Init();
    SPI_Init();
    Arm_Init();
    Button_Init();
    Display_Init();
    current_state = STATE_IDLE;
    while (1)
    {
        SCHEDULE_Run();
        current_state = FSM_Get_Next_State(current_state, current_event);
        FSM_Handle_State(current_state);
    }
}
