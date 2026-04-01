#ifndef __TIM_H
#define __TIM_H

#include "stm32f103xb.h"
#include "stdbool.h"
#include "stdio.h"
#include "GPIO.h"

void TIM2_Init(void);
void TIM2_PWM_Change_Duty(uint16_t duty);

void TIM3_Init(void);
void TIM3_Start(void);
void TIM3_Stop(void);
void TIM3_ResetCount(void);

#endif