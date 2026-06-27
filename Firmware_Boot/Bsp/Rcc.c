#include "System.h"

void Rcc_Init(void)
{
    RCC->CR |= (1 << 0);
    while (!(RCC->CR & (1 << 1)))
        ;
    RCC->APB2ENR |= (1 << 0) | (1 << 2) | (1 << 4) | (1 << 9) | (1 << 14);

    GPIOA->CRL = 0;
    GPIOA->CRH = 0;
    GPIOA->CRL |= (1 << 16) | (1 << 17) | (1 << 18);
    GPIOA->CRH |= (0x0B << 4) | (8 << 8);
}
