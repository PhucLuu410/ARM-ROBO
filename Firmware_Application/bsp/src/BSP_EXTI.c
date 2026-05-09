#include "BSP_EXTI.h"

void EXTI_Init(void)
{
    EXTI->IMR |= (1 << 11) | (1 << 12) | (1 << 0);
    EXTI->RTSR |= (1 << 11) | (1 << 12) | (1 << 0);
    EXTI->FTSR &= ~((1 << 11) | (1 << 12) | (1 << 0));

    AFIO->EXTICR[2] &= ~(0xF << 12);
    AFIO->EXTICR[3] &= ~(0xF << 0);
    AFIO->EXTICR[0] |= (0x1 << 0);

    GPIO_Config(GPIOA, 11, 0x0, 0x2);
    GPIO_Config(GPIOA, 12, 0x0, 0x2);
    GPIO_Config(GPIOB, 0, 0x0, 0x2);

    GPIOA->ODR &= ~((1 << 11) | (1 << 12));
    GPIOB->ODR &= ~(1 << 0);

    NVIC_EnableIRQ(USART1_IRQn);
    NVIC_EnableIRQ(TIM2_IRQn);
    NVIC_EnableIRQ(TIM3_IRQn);
    NVIC_EnableIRQ(EXTI15_10_IRQn);
    NVIC_EnableIRQ(EXTI0_IRQn);
    NVIC_EnableIRQ(ADC1_IRQn);
}