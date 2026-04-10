#include "APP_Read_Button.h"

extern SystemFlag System_GPIO_Flag;

void Button_Pressed(void)
{
    if (System_GPIO_Flag.BUTTON_FLAG == 1)
    {
        UART_SendString("Button Pressed\r\n");
        System_GPIO_Flag.BUTTON_FLAG = 0;
    }
}

void Emergency_Button_Pressed(void)
{
    if (System_GPIO_Flag.EMERGENCY_FLAG == 1)
    {
        UART_SendString("Emergency Button Pressed\r\n");
        System_GPIO_Flag.EMERGENCY_FLAG = 0;
    }
}