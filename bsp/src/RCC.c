#include "RCC.h"

void RCC_Init(void)
{
    RCC->CR |= (1 << 16); // HSE ON
    while (!(RCC->CR & (1 << 17)))

        RCC->CFGR &= ~(3 << 0);
    RCC->CFGR |= (1 << 0);

    while (((RCC->CFGR >> 2) & 0x3) != 0x1)
        ;

    RCC->CFGR &= ~(0xF << 4);

    RCC->CFGR &= ~(0x7 << 8);

    RCC->CFGR &= ~(0x7 << 11);

    RCC->AHBENR |= (1 << 0); // DMA1
    RCC->AHBENR |= (1 << 6); // CRC

    RCC->APB2ENR |= (1 << 0);  // AFIO
    RCC->APB2ENR |= (1 << 2);  // GPIOA
    RCC->APB2ENR |= (1 << 3);  // GPIOB
    RCC->APB2ENR |= (1 << 4);  // GPIOC
    RCC->APB2ENR |= (1 << 9);  // ADC1
    RCC->APB2ENR |= (1 << 14); // USART1

    RCC->APB1ENR |= (1 << 0);  // TIM2
    RCC->APB1ENR |= (1 << 1);  // TIM3
    RCC->APB1ENR |= (1 << 28); // PWR
}