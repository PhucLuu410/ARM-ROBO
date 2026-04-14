#include "BSP_TIM.h"

volatile uint32_t count = 0;

void TIM2_Init(void)
{
    GPIO_Config(GPIOA, 0, 0x3, 0x2); // Output pp
    GPIO_Config(GPIOA, 1, 0x3, 0x2); // Output pp
    GPIO_Config(GPIOA, 2, 0x3, 0x2); // Output p
    GPIO_Config(GPIOA, 3, 0x3, 0x2); // Output pp

    TIM2->CR1 |= (1 << 7);             // Auto-reload preload enable

    TIM2->CCMR1 |= (1 << 3);  // Corresponding preload CH1
    TIM2->CCMR1 |= (1 << 11); // Corresponding preload CH2
    TIM2->CCMR2 |= (1 << 3);  // Corresponding preload CH3
    TIM2->CCMR2 |= (1 << 11); // Corresponding preload CH4

    TIM2->CCMR1 |= (6 << 4);  // PWM CH1
    TIM2->CCMR1 |= (6 << 12); // PWM  CH2
    TIM2->CCMR2 |= (6 << 4);  // PWM CH3
    TIM2->CCMR2 |= (6 << 12); // PWM CH4

    TIM2->CCER |= (1 << 0);  // CC1E = 1 → enable output CH1
    TIM2->CCER |= (1 << 4);  // CC2E = 1 → enable output CH2
    TIM2->CCER |= (1 << 8);  // CC3E = 1 → enable output CH3
    TIM2->CCER |= (1 << 12); // CC4E = 1 → enable output CH4

    TIM2->DIER |= (1 << 0) | (1 << 1); // Update interrupt enable
    TIM2->PSC = 7;                     // 8Mhz / 8 = 1Mhz
    TIM2->ARR = 19999;                 // Count to 1000 -> 1ms 1 UEV
}

void TIM3_Init(void)
{
    TIM3->CR1 |= (1 << 7);  // Auto-reload preload enable
    TIM3->DIER |= (1 << 0); // Update interrupt enable
    TIM3->PSC = 7;          // 8Mhz / 8 = 1Mhz
    TIM3->ARR = 999;        // Count to 1000 -> 1ms 1 UEV
    TIM3->CR1 |= (1 << 0);  // Counter enable
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

void TIM2_PWM_CH1_Change_Duty(uint16_t duty)
{
    TIM2->CCR1 = duty;
}

void TIM2_PWM_CH2_Change_Duty(uint16_t duty)
{
    TIM2->CCR2 = duty;
}

void TIM2_PWM_CH3_Change_Duty(uint16_t duty)
{
    TIM2->CCR3 = duty;
}

void TIM2_PWM_CH4_Change_Duty(uint16_t duty)
{
    TIM2->CCR4 = duty;
}

void TIM2_Change_Period(uint16_t period)
{
    if (period > 65535)
        period = 65535;
    TIM2->ARR = period;
}

void TIM2_Change_Frequency(uint16_t frequency)
{
    if (frequency == 0)
        frequency = 1;
    uint32_t timer_clock = 8000000; // Assuming APB1 timer clock is 8 MHz
    uint32_t prescaler = (timer_clock / (frequency * 1000)) - 1;
    if (prescaler > 65535)
        prescaler = 65535;
    TIM2->PSC = prescaler;
}


void TIM2_IRQHandler(void)
{
    if (TIM2->SR & (1 << 0))
    {
        TIM2->SR &= ~(1 << 0);
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
        count = (count + 1) % 1000;
    }
}