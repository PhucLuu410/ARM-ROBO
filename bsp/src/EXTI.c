#include "EXTI.h"

void EXTI_Init(void)
{
    AFIO->EXTICR[0] &= ~(0xF << 4); // clear EXTI1

    EXTI->IMR |= (1 << 1);
    EXTI->RTSR |= (1 << 1);

    NVIC_EnableIRQ(EXTI1_IRQn);
    NVIC_EnableIRQ(USART1_IRQn);
}