#ifndef __UART_H__
#define __UART_H__

#include "stdint.h"
#include "stm32f103xb.h"
#include "string.h"
#include "GPIO.h"
#include "utils.h"

void UART_Init(void);
void UART_SendData(uint8_t data);
void UART_SendString(const char *str);

#endif