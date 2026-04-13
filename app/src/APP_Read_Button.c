#include "APP_Read_Button.h"
#include <string.h>
extern SystemFlag System_GPIO_Flag;
uint8_t press = 0;
extern uint8_t count;
char string[30];
void Button_Pressed(void)
{
    if (System_GPIO_Flag.BUTTON_FLAG == 1)
    {
        System_GPIO_Flag.BUTTON_FLAG = 0;
        GPIO_Config(GPIOA, 4, GPIO_MODE_OUTPUT_10M, GPIO_CNF_GP_PP);
        GPIO_TogglePin(GPIOA, 4);
    }
}

void Emergency_Button_Pressed(void)
{
    if (System_GPIO_Flag.EMERGENCY_FLAG == 1)
    {
        System_GPIO_Flag.EMERGENCY_FLAG = 0;
        while (1)
        {
            GPIO_Config(GPIOA, 5, GPIO_MODE_OUTPUT_10M, GPIO_CNF_GP_PP);
            GPIO_WritePin(GPIOA, 5, 1);
        }
    }
}
void Button_Init(void)
{
    SCHEDULE_Create_Task(&Button_Pressed, 10);
    SCHEDULE_Create_Task(&Emergency_Button_Pressed, 1);
}
