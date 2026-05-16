#include <APP_Control_Robot.h>

uint16_t *reg_data1 = (uint16_t *)0x20000002;
uint16_t *reg_data2 = (uint16_t *)0x20000004;
uint16_t *reg_data3 = (uint16_t *)0x20000006;
uint16_t *reg_data4 = (uint16_t *)0x20000008;

extern ModbusRequest save_data[4];

uint8_t GoToOffSetFlag = 0;

void Arm_Start(void)
{
    TIM2->CR1 |= (1 << 0);
}

void Arm_Stop(void)
{
    TIM2->CR1 &= ~(1 << 0);
}

void Arm_Mode1(void)
{
    MODBUS_Parse_Request();
    TIM2_PWM_CH1_Change_Duty(*reg_data1);
    TIM2_PWM_CH2_Change_Duty(*reg_data2);
    TIM2_PWM_CH3_Change_Duty(*reg_data3);
    TIM2_PWM_CH4_Change_Duty(*reg_data4);
}

void Arm_Mode2(void)
{
    for (int i = 0; i < 4; i++)
    {
        if (save_data[i].servo_1 == 0 || save_data[i].servo_2 == 0 || save_data[i].servo_3 == 0 || save_data[i].servo_4 == 0)
            return;
    }
    // for (int i = 0; i < 4; i++)
    // {
    //     TIM2_PWM_CH1_Change_Duty(save_data[i].servo_1);
    //     TIM2_PWM_CH2_Change_Duty(save_data[i].servo_2);
    //     TIM2_PWM_CH3_Change_Duty(save_data[i].servo_3);
    //     TIM2_PWM_CH4_Change_Duty(save_data[i].servo_4);
    // }
}

void Arm_OffSet(void)
{
    TIM2_PWM_CH1_Change_Duty(1250);
    TIM2_PWM_CH2_Change_Duty(700);
    TIM2_PWM_CH3_Change_Duty(700);
    TIM2_PWM_CH4_Change_Duty(1750);
}
void Arm_Init(void)
{
    SCHEDULE_Create_Task(&Arm_Mode1, 3);
}
