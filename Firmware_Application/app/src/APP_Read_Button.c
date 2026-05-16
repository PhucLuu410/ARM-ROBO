#include "APP_Read_Button.h"
#include "APP_Display.h"

extern SystemFlag System_GPIO_Flag;
extern SystemEvent current_event;
extern ModbusRequest request;
ModbusRequest save_data[4];
uint16_t save_data_index = 0;
uint8_t press = 0;
extern uint8_t GoToOffSetFlag;

void Button_Pressed(void)
{
    if (System_GPIO_Flag.BUTTON_FLAG == 1)
    {
        GoToOffSetFlag = 0;
        current_event = EVENT_BUTTON_PRESS;
        System_GPIO_Flag.BUTTON_FLAG = 0;
        press = (press + 1) % 3;
        switch (press)
        {
        case 0:
            Display_ShowStop();
            break;
        case 1:
            Display_ShowMode1();
            break;
        case 2:
            Display_ShowMode2();
            break;
        default:
            break;
        }
    }
}

void Emergency_Button_Pressed(void)
{
    if (System_GPIO_Flag.EMERGENCY_FLAG == 1)
    {
        System_GPIO_Flag.EMERGENCY_FLAG = 0;
        current_event = EVENT_EMERGENCY_PRESS;
        while (1)
        {
            GPIO_Config(GPIOA, 5, GPIO_MODE_OUTPUT_10M, GPIO_CNF_GP_PP);
            GPIO_WritePin(GPIOA, 5, 1);
            Display_ShowError();
        }
    }
}
static void Save_Button_Led(void)
{
    GPIO_WritePin(GPIOA, 4, 0);
    GPIO_Config(GPIOA, 4, GPIO_MODE_OUTPUT_10M, GPIO_CNF_GP_PP);
    GPIO_WritePin(GPIOA, 4, 1);
}

void Save_Button_State(void)
{
    if (System_GPIO_Flag.SAVE_FLAG == 1)
    {
        System_GPIO_Flag.SAVE_FLAG = 0;
        current_event = EVENT_SAVE_PRESS;
        Save_Button_Led();
        save_data[save_data_index] = request;
        save_data_index = (save_data_index + 1) % 4;
    }
}

void Button_Init(void)
{
    System_GPIO_Flag.BUTTON_FLAG = 0;
    System_GPIO_Flag.EMERGENCY_FLAG = 0;
    System_GPIO_Flag.SAVE_FLAG = 0;
    SCHEDULE_Create_Task(&Button_Pressed, 10);
    SCHEDULE_Create_Task(&Emergency_Button_Pressed, 1);
    SCHEDULE_Create_Task(&Save_Button_State, 7);
}
