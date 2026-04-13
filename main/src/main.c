#include "main.h"
extern SystemFlag Control_Robot_Flag;
uint8_t check = 0;

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
    while (1)
    {
        SCHEDULE_Run();
        SPI_SendByte(0x55); // Send a test byte to the SPI device
    }
}
