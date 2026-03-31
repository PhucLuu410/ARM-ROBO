#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

#include "stm32f103xb.h"
#include "BSP.h"

void EXTI1_IRQHandler(void);
void USART1_IRQHandler(void);
void HardFault_Handler(void);

#endif