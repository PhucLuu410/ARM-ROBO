#ifndef UART_H
#define UART_H

#include "stm32f103xb.h"

void Uart_Init(void);
void Uart_SendData(uint8_t data);
uint8_t Uart_GetData(uint8_t *data);

#endif