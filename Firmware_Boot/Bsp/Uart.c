#include "Uart.h"

static volatile uint8_t Index = 0;
uint8_t Data_Buffer[16] = {0};
uint8_t Start_Buffer[3] = {0};
static volatile uint8_t Update_Flag = 0;

void Uart_Init(void)
{
    USART1->BRR = 0x341;
    USART1->CR1 |= (1 << 5) | (1 << 3) | (1 << 2);
    USART1->CR1 |= (1 << 13);
    NVIC_EnableIRQ(USART1_IRQn);

    for (int i = 0; i < 16; i++)
    {
        Data_Buffer[i] = 0;
    }
    Index = 0;
}

void Uart_SendData(uint8_t data)
{
    while (!(USART1->SR & (1 << 7)))
        ;
    USART1->DR = data;
}

uint8_t Uart_GetData(uint8_t *data)
{
    for (uint8_t i = 0; i < 16; i++)
    {
        data[i] = Data_Buffer[i];
    }
    return 1;
}

void USART1_IRQHandler(void)
{
    while (!(USART1->SR & (1 << 5)))
        ;
    Data_Buffer[Index++ % 16] = USART1->DR;
}
