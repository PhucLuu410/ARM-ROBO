#include "interupt.h"

volatile uint32_t test_timer = 0;
volatile uint32_t modbus_frame_ready = 0;
volatile uint32_t count = 0;
volatile uint32_t last_interrupt_time = 0;
SystemFlag main_flag = NO_FLAG;

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
        main_flag = MODBUS_FLAG;
        RingBuffer_Push(received_data);
        TIM3_ResetCount();
        TIM3_Start();
        GPIO_WritePin(GPIOC, 13, 0);
    }
}

void TIM2_IRQHandler(void)
{
    if (TIM2->SR & (1 << 0))
    {
        TIM2->SR &= ~(1 << 0);
        count = (count + 1 / 1000);
    }
    if (TIM2->SR & (1 << 1))
    {
        TIM2->SR &= ~(1 << 1);
    }
}

void TIM3_IRQHandler(void)
{
    if (TIM3->SR & (1 << 0))
    {
        TIM3->SR &= ~(1 << 0);
        TIM3_Stop();
        GPIO_WritePin(GPIOC, 13, 1);
        main_flag = NO_FLAG;
        MODBUS_Error_Clear_Frame();
    }
}

void HardFault_Handler(void)
{
    while (1)
    {
    }
}
