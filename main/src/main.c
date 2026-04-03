#include "main.h"

uint32_t count = 0;
SystemFlag main_flag = {0};

int main(void)
{
    RCC_Init();
    TIM2_Init();
    TIM3_Init();
    EXTI_Init();
    UART_Init();
    while (1)
    {
        MODBUS_Parse_Request();
    }
}