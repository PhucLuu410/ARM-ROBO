#ifndef GPIO_H
#define GPIO_H

#include "stm32f103xb.h"

// Mode
#define GPIO_MODE_INPUT 0
#define GPIO_MODE_OUTPUT_10M 1
#define GPIO_MODE_OUTPUT_2M 2
#define GPIO_MODE_OUTPUT_50M 3

// Config
#define GPIO_CNF_GP_PP 0 
#define GPIO_CNF_GP_OD 1 
#define GPIO_CNF_AF_PP 2
#define GPIO_CNF_AF_OD 3

void GPIO_Config(GPIO_TypeDef *GPIOx, uint8_t pin, uint8_t mode, uint8_t cnf);
void GPIO_WritePin(GPIO_TypeDef *GPIOx, uint8_t pin, uint8_t state);
void GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint8_t pin);

#endif