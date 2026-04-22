#include "BSP_ADC.h"

static uint16_t adc_value = 0;

void ADC_Init(void)
{
    GPIO_Config(GPIOA, 8, 0, 0); // analog input

    ADC1->CR2 |= (1 << 1);
    ADC1->CR2 |= (7 << 17);
    ADC1->SMPR2 |= (7 << 24);
    ADC1->SQR3 |= (8 << 0);
    ADC1->SQR1 = 0;
    ADC1->CR1 |= (1 << 8);
    ADC1->CR2 |= (1 << 8);
    ADC1->CR2 |= (1 << 0);
    ADC1->CR2 |= (1 << 2);
    ADC1->CR2 |= (1 << 3);
    ADC1->CR2 |= (1 << 0);
    ADC1->CR2 |= (1 << 22);
}

uint32_t ADC_GetAddressValueBuffer(void)
{
    return (uint32_t)(&adc_value);
}