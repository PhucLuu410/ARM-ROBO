#include <APP_Control_Robot.h>

uint16_t *reg_data1 = (uint16_t *)0x20000002;
uint16_t *reg_data2 = (uint16_t *)0x20000004;
uint16_t *reg_data3 = (uint16_t *)0x20000006;
uint16_t *reg_data4 = (uint16_t *)0x20000008;

SystemFlag Control_Robot_Flag;

void Arm_Init(void)
{
    TIM2_PWM_CH1_Change_Duty(1500);
    TIM2_PWM_CH1_Change_Duty(1200);
}
void Arm_Start(void)
{
    MODBUS_Parse_Request();

    TIM2_PWM_CH1_Change_Duty(*reg_data1);
    TIM2_PWM_CH2_Change_Duty(*reg_data2);
    TIM2_PWM_CH3_Change_Duty(*reg_data3);
    TIM2_PWM_CH4_Change_Duty(*reg_data4);
}
void Arm_Stop(void)
{
    TIM2_PWM_CH1_Change_Duty(0);
    TIM2_PWM_CH2_Change_Duty(0);
    TIM2_PWM_CH3_Change_Duty(0);
    TIM2_PWM_CH4_Change_Duty(0);
}