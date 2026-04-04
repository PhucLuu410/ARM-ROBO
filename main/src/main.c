#include "main.h"

uint32_t count = 0;
SystemFlag main_flag = {0};
uint8_t *reg_data1 = (uint8_t *)0x20000003;
uint8_t *reg_data2 = (uint8_t *)0x20000004;
int main(void)
{
    RCC_Init();
    TIM2_Init();
    TIM3_Init();
    EXTI_Init();
    UART_Init();
    TIM2_Change_Frequency(50);
    TIM2_PWM_Change_Duty(75);
    while (1)
    {
        MODBUS_Parse_Request();
        if ((((*reg_data1) << 8) | *reg_data2) == 0x0164)
        {
            TIM2_PWM_Change_Duty(100);
        }
        if ((((*reg_data1) << 8) | *reg_data2) == 0x0119)
        {
            TIM2_PWM_Change_Duty(25);
        }
        if ((((*reg_data1) << 8) | *reg_data2) == 0x014b)
        {
            TIM2_PWM_Change_Duty(75);
        }
    }
}