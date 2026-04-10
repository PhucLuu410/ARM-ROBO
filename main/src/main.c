#include "main.h"
uint32_t count = 0;
extern SystemFlag Control_Robot_Flag;
uint8_t check = 0;
int main(void)
{
    RCC_Init();
    TIM2_Init();
    TIM3_Init();
    EXTI_Init();
    UART_Init();
    Arm_Init();
    while (1)
    {
        Arm_Start();
    }
}
