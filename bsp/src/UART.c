#include "UART.h"

void UART_Init(void)
{
    GPIO_Config(GPIOA, 9, 0x2, 0x2);  // Alternate function push-pull 2MHz Tx A9
    GPIO_Config(GPIOA, 10, 0x0, 0x2); // Input floating Rx A10

    USART1->CR1 |= (1 << 4) | (1 << 3) | (1 << 2); // TE Enable , RE Enable
    // USART1->CR1 |= (1 << 6);            // TC ISR
    USART1->CR1 |= (1 << 5);
    USART1->BRR |= (0x34 << 4) | 0x1; // 9600 baud rate

    USART1->CR1 |= (1 << 13); // Enable USART1
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