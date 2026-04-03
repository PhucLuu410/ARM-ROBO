#include "main.h"

ModbusRequest receive_request;
SystemFlag main_flag = {0};
int ptr = 0;
volatile uint8_t c = 0;
uint16_t a = 0;
int main(void)
{
    SCB->VTOR = 0x08001000;
    RCC_Init();
    TIM2_Init();
    TIM3_Init();
    EXTI_Init();
    UART_Init();
    GPIO_Config(GPIOC, 13, 0x2, 0x0); // Output push-pull 2MHz
    GPIO_Config(GPIOA, 1, 0x0, 0x2);  // FF push-pull 2MHz
    while (1)
    {
        if (main_flag.MODBUS_FLAG == 1)
            MODBUS_Parse_Request();
    }
}