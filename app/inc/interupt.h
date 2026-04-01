#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

#include <stdint.h>
#include "stm32f103xb.h"
#include "bsp.h"
#include "utils.h"

void EXTI1_IRQHandler(void);
void USART1_IRQHandler(void);
void HardFault_Handler(void);
void TIM2_IRQHandler(void);
void TIM3_IRQHandler(void);

#endif