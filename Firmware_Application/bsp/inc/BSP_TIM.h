#ifndef __TIM_H
#define __TIM_H

#include <stdint.h>
#include <stdbool.h>

#include "stm32f103xb.h"
#include "BSP_GPIO.h"
#include "DSA.h"

void TIM2_Init(void);

void TIM2_PWM_CH1_Change_Duty(uint16_t duty);
void TIM2_PWM_CH2_Change_Duty(uint16_t duty);
void TIM2_PWM_CH3_Change_Duty(uint16_t duty);
void TIM2_PWM_CH4_Change_Duty(uint16_t duty);

void TIM2_Change_Period(uint16_t period);
void TIM2_Change_Frequency(uint16_t frequency);

void TIM3_Init(void);
void TIM3_Start(void);
void TIM3_Stop(void);
void TIM3_ResetCount(void);

#endif