#include "BSP_EXTI.h"

void EXTI_Init(void)
{
    // EXTI->IMR |= (1 << 11) | (1 << 12);
    // EXTI->RTSR |= (1 << 11) | (1 << 12);

    // AFIO->EXTICR[2] &= ~(0xF << 12);
    // AFIO->EXTICR[3] &= ~(0xF << 0);

    GPIO_Config(GPIOA, 11, 0x0, 0x2);
    GPIO_Config(GPIOA, 12, 0x0, 0x2);

    NVIC_EnableIRQ(USART1_IRQn);
    NVIC_EnableIRQ(TIM2_IRQn);
    // NVIC_EnableIRQ(TIM3_IRQn);
    // NVIC_EnableIRQ(EXTI15_10_IRQn);
}