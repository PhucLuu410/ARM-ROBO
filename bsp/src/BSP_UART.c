#define BUFFER_SIZE 256
#define MODBUS_FRAME_SIZE 12

#include "BSP_UART.h"

extern SystemFlag Control_Robot_Flag;

volatile int front = 0;
volatile int rear = 0;
volatile uint8_t buffer[BUFFER_SIZE] = {0};

void UART_Init(void)
{
    GPIO_Config(GPIOA, 9, 0x2, 0x2);  // Alternate function push-pull 2MHz Tx A9
    GPIO_Config(GPIOA, 10, 0x0, 0x2); // Input floating Rx A10

    USART1->CR1 |= (1 << 4) | (1 << 3) | (1 << 2); // TE Enable , RE Enable

    USART1->CR1 |= (1 << 5);
    USART1->BRR |= (0x34 << 4) | 0x1; // 9600 baud rate
    USART1->CR1 |= (1 << 13);         // Enable USART1
}

void UART_SendData(uint8_t data)
{
    while (!(USART1->SR & (1 << 7)))
        ;
    USART1->DR = data;
}

void UART_SendString(const char *str)
{
    while (*str)
    {
        UART_SendData((uint8_t)(*str));
        str++;
    }
}

void USART1_IRQHandler(void)
{
    if (USART1->SR & (1 << 5)) // RXNE
    {
        uint8_t received_data = (uint8_t)(USART1->DR);
        RingBuffer_Push(received_data, &front, &rear, buffer, BUFFER_SIZE);
    }
    if (USART1->SR & (1 << 4)) // IDLE
    {
        volatile uint32_t tmp = USART1->SR;
        volatile uint32_t tmp2 = USART1->DR;
        (void)tmp;
        (void)tmp2;
        Control_Robot_Flag.MODBUS_FLAG = 1;
    }
}