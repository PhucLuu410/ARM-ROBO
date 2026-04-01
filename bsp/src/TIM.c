#include "TIM.h"

void TIM2_Init(void)
{
    GPIO_Config(GPIOA, 0, 0x1, 0x2);   // Output pp
    TIM2->CR1 |= (1 << 7);             // Auto-reload preload enable
    TIM2->CCMR1 |= (6 << 4);           // PWM
    TIM2->CCMR1 |= (1 << 3);           // Corresponding preload
    TIM2->DIER |= (1 << 0) | (1 << 1); // Update interrupt enable
    TIM2->PSC = 7;                     // 8Mhz / 8 = 1Mhz
    TIM2->ARR = 999;                   // Count to 1000 -> 1ms 1 UEV
    TIM2->CCR1 = 500;                  // 50% duty cycle switch edge
    TIM2->CCER |= (1 << 0);            // CC1E = 1 → enable output CH1
    TIM2->CR1 |= (1 << 0);             // Counter enable
}

void TIM3_Init(void)
{
    TIM3->CR1 |= (1 << 7);  // Auto-reload preload enable
    TIM3->DIER |= (1 << 0); // Update interrupt enable
    TIM3->PSC = 7;          // 8Mhz / 8 = 1Mhz
    TIM3->ARR = 3999;       // Count to 1000 -> 1ms 1 UEV
}

void TIM3_Start(void)
{
    TIM3->CR1 |= (1 << 0); // Counter enable
}

void TIM3_Stop(void)
{
    TIM3->CR1 &= ~(1 << 0); // Counter disable
}

void TIM3_ResetCount(void)
{
    TIM3->CNT = 0;
}

void TIM2_PWM_Change_Duty(uint16_t duty)
{
    if (duty > 1000)
        duty = 1000;
    TIM2->CCR1 = duty;
}