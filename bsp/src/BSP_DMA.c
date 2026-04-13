#include "BSP_DMA.h"

uint32_t DMA_buffer[256] = {0};

void DMA_Init(void)
{
    DMA1_Channel5->CCR &= ~1; // disable trước

    DMA1_Channel5->CCR = 0;

    DMA1_Channel5->CCR &= ~(1 << 6); // DIR = 0 (Peripheral → Memory)
    DMA1_Channel5->CCR |= (1 << 5);  // Circular mode

    DMA1_Channel5->CCR &= ~(3 << 8);  // PSIZE = 8-bit
    DMA1_Channel5->CCR &= ~(3 << 10); // MSIZE = 8-bit

    DMA1_Channel5->CCR &= ~(1 << 6); // PINC = 0 (không increment)
    DMA1_Channel5->CCR |= (1 << 7);  // MINC = 1 (increment memory)

    DMA1_Channel5->CPAR = (uint32_t)&USART1->DR;
    DMA1_Channel5->CMAR = (uint32_t)&DMA_buffer;
    DMA1_Channel5->CNDTR = sizeof(DMA_buffer) / sizeof(DMA_buffer[0]);

    DMA1_Channel5->CCR |= 1; // enable
}
