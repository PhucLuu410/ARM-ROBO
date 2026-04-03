#include "APP_Change_PWM_Duty.h"

void PWM_Change_Duty_Cycle(uint16_t duty)
{
    TIM2_PWM_Change_Duty(duty);
}