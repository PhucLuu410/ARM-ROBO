#include "interupt.h"

volatile uint32_t count = 0;
volatile uint32_t last_interrupt_time = 0;

void EXTI1_IRQHandler(void)
{
    if (EXTI->PR & (1 << 1))
    {
        uint32_t current_time = TIM2->CNT;

        if ((current_time - last_interrupt_time) > 1)
        {
            count++;
        }

        last_interrupt_time = current_time;
        EXTI->PR |= (1 << 1);
    }
}

void USART1_IRQHandler(void)
{
    if (USART1->SR & (1 << 5)) // RXNE
    {

        uint8_t received_data = (uint8_t)(USART1->DR);
        RingBuffer((char)received_data);
    }
}

void HardFault_Handler(void)
{
    while (1)
    {
    }
}
