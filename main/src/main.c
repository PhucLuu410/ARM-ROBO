#include "main.h"

int main(void)
{
    SCB->VTOR = 0x08001000;
    RCC_Init();
    TIM2_Init();
    EXTI_Init();
    UART_Init();
    GPIO_Config(GPIOC, 13, 0x2, 0x0); // Output push-pull 2MHz
    GPIO_Config(GPIOA, 1, 0x0, 0x2);  // FF push-pull 2MHz
    char *duty = (char *)0x20004000;
    char str[20];
    while (1)
    {
        ReceiveData();
        Change_PWM_Duty_Cycle((duty[0] - 48) * 100 + (duty[1] - 48) * 10 + (duty[2] - 48) * 1);
    }
}