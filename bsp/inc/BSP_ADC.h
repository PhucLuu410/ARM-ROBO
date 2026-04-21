#ifndef __BSP_ADC_H
#define __BSP_ADC_H

#include <stdint.h>
#include <stdbool.h>
#include "stm32f103xb.h"
#include "DSA.h"
#include "BSP_GPIO.h"

void ADC_Init(void);
uint32_t ADC_GetAddressValueBuffer(void);

#endif