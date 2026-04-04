#ifndef __APP_H
#define __APP_H

#include <APP_Change_PWM_Duty.h>
#include <APP_MODBUS.h>

// PWM
void PWM_Change_Duty_Cycle(uint16_t duty);

// MODBUS
void MODBUS_Parse_Request(void);
uint16_t MODBUS_CRC_Check(uint8_t *buf, int len);

// GPIO TEST
void GPIO_Test_Polling_Live(void);

#endif