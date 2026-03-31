#ifndef __TIM_H
#define __TIM_H

#include "stm32f103xb.h"
#include "stdbool.h"
#include "stdio.h"
#include "GPIO.h"

void TIM2_Init(void);
void Change_Duty(uint16_t duty);

#endif