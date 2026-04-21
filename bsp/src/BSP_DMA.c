#include "BSP_DMA.h"
#include "BSP_ADC.h"

void DMA_Init(void)
{
    DMA1_Channel1->CCR = 0;
    DMA1_Channel1->CCR |= (3 << 12);
    DMA1_Channel1->CCR |= (1 << 10);
    DMA1_Channel1->CCR |= (1 << 8);
    DMA1_Channel1->CCR |= (1 << 7);
    DMA1_Channel1->CCR |= (1 << 5);
    DMA1_Channel1->CNDTR = 1;
    DMA1_Channel1->CPAR = (uint32_t)&(ADC1->DR);
    DMA1_Channel1->CMAR = ADC_GetAddressValueBuffer();
    DMA1_Channel1->CCR |= (1 << 0);
}
