#include "main.h"
extern SystemFlag main_flag;
uint8_t request_buffer[8];
ModbusRequest request;
int ptr = 0;
volatile uint8_t c = 0;

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
        if (main_flag == MODBUS_FLAG)
        {
            Get_MODBUS_Data(request_buffer);
            MODBUS_Parse_Request(request_buffer, &request);
            if (MODBUS_CRC_Check(&request) != request.crc)
            {
                MODBUS_Error_Clear_Frame();
            }
        }
    }
}