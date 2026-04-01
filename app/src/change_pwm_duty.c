#include "change_pwm_duty.h"

void Change_PWM_Duty_Cycle(uint16_t duty)
{
    TIM2_PWM_Change_Duty(duty);
}